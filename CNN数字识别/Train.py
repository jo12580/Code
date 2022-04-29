import os
import struct
import time

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.utils.data as data
import torch.utils.data as data
import torchvision
import torchvision.transforms as transforms
from matplotlib import pyplot as plt
from sklearn import decomposition
from sklearn.metrics import ConfusionMatrixDisplay
from sklearn.metrics import confusion_matrix

start = time.perf_counter()  # 启动时间

# Device configuration  判断能否使用cuda加速
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')

""""------------------------------------定义文件名--------------------------------------------------"""
# 训练集文件
train_images_idx3_ubyte_file = './MNIST/raw/t10k-images-idx3-ubyte'
# 训练集标签文件
train_labels_idx1_ubyte_file = './MNIST/raw/t10k-labels-idx1-ubyte'

# 测试集文件
test_images_idx3_ubyte_file = './MNIST/raw/train-images-idx3-ubyte'
# 测试集标签文件
test_labels_idx1_ubyte_file = './MNIST/raw/train-labels-idx1-ubyte'

""" -------------------------------------Hyper parameters（定义函数）----------------------------------"""
num_epochs = 5  # 循环次数
num_classes = 10  # 分类
batch_size = 100  # 一次训练所选取的样本数
learning_rate = 0.001  # 学习率

"""---------------------------------------下载训练数据(MNIST dataset)-------------------------------------------------"""
train_dataset = torchvision.datasets.MNIST(root='./',
                                           train=True,
                                           transform=transforms.ToTensor(),
                                           download=True)

test_dataset = torchvision.datasets.MNIST(root='./',
                                          train=False,
                                          transform=transforms.ToTensor())

# Data loader
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           shuffle=True)

test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          shuffle=False)

"""-------------------------------------------设置 ResNet18 架构---------------------------------------------------------"""


class ResBlk(nn.Module):  # 定义Resnet Block模块
    def __init__(self, ch_in, ch_out, stride=1):  # 进入网络前须知道传入层数和传出层数的设定
        super(ResBlk, self).__init__()  # 初始化

        # resbok添加stride支持
        # 根据resnet网络结构构建2个（block）块结构 第一层卷积 卷积核大小3*3,步长为1，边缘加1
        self.conv1 = nn.Conv2d(ch_in, ch_out, kernel_size=3, stride=stride, padding=1)
        # 将第一层卷积处理的信息通过BatchNorm2d
        self.bn1 = nn.BatchNorm2d(ch_out)
        # 第二块卷积接收第一块的输出，above too
        self.conv2 = nn.Conv2d(ch_out, ch_out, kernel_size=3, stride=1, padding=1)
        self.bn2 = nn.BatchNorm2d(ch_out)

        # 确保输入维度等于输出维度
        self.extra = nn.Sequential()  # 先建一个空的extra
        if ch_out != ch_in:
            # [b, ch_in, h, w] => [b, ch_out, h, w]
            self.extra = nn.Sequential(
                nn.Conv2d(ch_in, ch_out, kernel_size=1, stride=stride),
                nn.BatchNorm2d(ch_out)
            )

    def forward(self, x):  # 定义局部向前传播函数
        out = F.relu(self.bn1(self.conv1(x)))  # 对第一块卷积后的数据再经过relu操作
        out = self.bn2(self.conv2(out))  # 第二块卷积后的数据输出
        # short cut.
        # extra module: [b, ch_in, h, w] => [b, ch_out, h, w]
        # element-wise add:
        out = self.extra(x) + out  # 将x传入extra经过2块（block）输出后与原始值进行相加
        out = F.relu(out)  # 调用relu，这里使用F.调用

        return out


class ResNet18(nn.Module):  # 构建resnet18层,带有权重的18层，包括卷积层和全连接层，不包括池化层和BN层
    def __init__(self):
        super(ResNet18, self).__init__()

        self.conv1 = nn.Sequential(  # 首先定义一个卷积层
            nn.Conv2d(1, 32, kernel_size=3, stride=3, padding=0),
            nn.BatchNorm2d(32)
        )
        # followed 4 blocks 调用4次resnet网络结构，输出都是输入的2倍
        # [b, 64, h, w] => [b, 128, h ,w]
        self.blk1 = ResBlk(32, 64, stride=1)
        # [b, 128, h, w] => [b, 256, h, w]
        self.blk2 = ResBlk(64, 128, stride=1)
        # [b, 256, h, w] => [b, 512, h, w]
        self.blk3 = ResBlk(128, 256, stride=1)
        # [b, 512, h, w] => [b, 1024, h, w]
        self.blk4 = ResBlk(256, 256, stride=1)

        self.outlayer = nn.Linear(256 * 1 * 1, 10)  # 最后是全连接层

    def forward(self, x):  # 定义整个向前传播
        x = F.relu(self.conv1(x))  # 先经过第一层卷积

        # [b, 64, h, w] => [b, 1024, h, w]
        x = self.blk1(x)  # 然后通过4次resnet网络结构
        x = self.blk2(x)
        x = self.blk3(x)
        x = self.blk4(x)

        # F.adaptive_avg_pool2d功能尾巴变为1,1，[b, 512, h, w] => [b, 512, 1, 1]
        x = F.adaptive_avg_pool2d(x, [1, 1])
        x = x.view(x.size(0), -1)  # 平铺一维值
        x = self.outlayer(x)  # 全连接层

        return x


"""-------------------------------------------------读取模型数据-----------------------------------------------------"""


def decode_idx3_ubyte(idx3_ubyte_file):
    # 读取二进制数据
    bin_data = open(idx3_ubyte_file, 'rb').read()

    # 解析文件头信息，依次为魔数、图片数量、每张图片高、每张图片宽
    offset = 0
    fmt_header = '>iiii'  # 因为数据结构中前4行的数据类型都是32位整型，所以采用i格式，但需要读取前4行数据，所以需要4个i。
    magic_number, num_images, num_rows, num_cols = struct.unpack_from(fmt_header, bin_data, offset)
    print('魔数:%d, 图片数量: %d张, 图片大小: %d*%d' % (magic_number, num_images, num_rows, num_cols))

    # 解析数据集
    image_size = num_rows * num_cols
    offset += struct.calcsize(fmt_header)
    print(offset)
    fmt_image = '>' + str(
        image_size) + 'B'  # 图像数据像素值的类型为unsigned char型，对应的format格式为B。这里还要加上图像大小784，是为了读取784个B格式数据
    print(fmt_image, offset, struct.calcsize(fmt_image))
    images = np.empty((num_images, num_rows, num_cols))
    for i in range(num_images):
        if (i + 1) % 10000 == 0:
            print('已解析 %d' % (i + 1) + '张')
            print(offset)
        images[i] = np.array(struct.unpack_from(fmt_image, bin_data, offset)).reshape((num_rows, num_cols))
        offset += struct.calcsize(fmt_image)
    return images


def decode_idx1_ubyte(idx1_ubyte_file):
    # 读取二进制数据
    bin_data = open(idx1_ubyte_file, 'rb').read()

    # 解析文件头信息，依次为魔数和标签数
    offset = 0
    fmt_header = '>ii'
    magic_number, num_images = struct.unpack_from(fmt_header, bin_data, offset)
    print('魔数:%d, 图片数量: %d张' % (magic_number, num_images))

    # 解析数据集
    offset += struct.calcsize(fmt_header)
    fmt_image = '>B'
    labels = np.empty(num_images)
    for i in range(num_images):
        if (i + 1) % 10000 == 0:
            print('已解析 %d' % (i + 1) + '张')
        labels[i] = struct.unpack_from(fmt_image, bin_data, offset)[0]
        offset += struct.calcsize(fmt_image)
    return labels


def load_train_images(idx_ubyte_file=train_images_idx3_ubyte_file):
    return decode_idx3_ubyte(idx_ubyte_file)


def load_train_labels(idx_ubyte_file=train_labels_idx1_ubyte_file):
    return decode_idx1_ubyte(idx_ubyte_file)


def load_test_images(idx_ubyte_file=test_images_idx3_ubyte_file):
    return decode_idx3_ubyte(idx_ubyte_file)


def load_test_labels(idx_ubyte_file=test_labels_idx1_ubyte_file):
    return decode_idx1_ubyte(idx_ubyte_file)


"""----------------------------------------训练集与测试集数据----------------------------------------------------------"""
mean = train_dataset.data.float().mean() / 255  # 计算训练集的均值
std = train_dataset.data.float().std() / 255  # 计算训练集的标准差
print(f'训练集的均值: {mean}')  # 输出计算的均值
print(f'训练集的标准差: {std}')

# 打印出训练/测试集中的示例数量，以确保所有内容都已正确加载
print(f'训练数量: {len(train_dataset)}')
print(f'测试数量: {len(test_dataset)}')

"""----------------------------------------Train the model----------------------------------------------------------"""
model = ResNet18().to(device)

#  损失率与优化
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
total_step = len(train_loader)
for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_loader):
        start1 = time.perf_counter()  # 启动时间
        images = images.to(device)
        labels = labels.to(device)

        # 前向传输
        outputs = model(images)
        loss = criterion(outputs, labels)

        # Backwad and optimize
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if (i + 1) % 100 == 0:
            print('Epoch[{}/{}], Step[{}/{}], Loss:{:.4f}'
                  .format(epoch + 1, num_epochs, i + 1, total_step, loss.item()))
            end1 = time.perf_counter()
            print('耗时: %s s' % (end1 - start1))
            start1 = 0
            end1 = 0
# Save the model checkpoint
torch.save(model.state_dict(), 'model.ckpt')  # 生成模型包

""""----------------------------------------- Test the model--------------------------------------------------------"""
if __name__ == '__main__':
    train_images = load_train_images()
    train_labels = load_train_labels()
    test_images = load_test_images()
    test_labels = load_test_labels()

    for i in range(1):
        print(train_labels[i])
        plt.imshow(train_images[i], cmap='gray')
        plt.pause(0.000001)
        plt.show()

model.eval()
with torch.no_grad():
    correct = 0
    total = 0
    for images, labels in test_loader:
        images = images.to(device)
        labels = labels.to(device)
        outputs = model(images)
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()

    print('测试10000张图片正确率为:{}%'.format(100 * correct / total))

if not os.path.isdir("./User-Defined"):
    os.makedirs("User-Defined")
end = time.perf_counter()
print('总耗时: %s s' % (end - start))

"""------------------------------------------------模型预测----------------------------------------------------------"""
train_iterator = data.DataLoader(train_dataset,
                                 shuffle=True,
                                 batch_size=batch_size)
test_iterator = data.DataLoader(test_dataset,
                                batch_size=batch_size)


# ------绘制混淆矩阵
def get_predictions(model, iterator, device):
    model.eval()

    images = []
    labels = []
    probs = []

    with torch.no_grad():
        for (x, y) in iterator:
            x = x.to(device)

            y_pred = model(x)

            y_prob = F.softmax(y_pred, dim=-1)

            images.append(x.cpu())
            labels.append(y.cpu())
            probs.append(y_prob.cpu())
    images = torch.cat(images, dim=0)
    labels = torch.cat(labels, dim=0)
    probs = torch.cat(probs, dim=0)

    return images, labels, probs


images, labels1, probs = get_predictions(model, test_iterator, device)
pred_labels = torch.argmax(probs, 1)


def plot_confusion_matrix(labels, pred_labels):
    fig = plt.figure(figsize=(10, 10));
    ax = fig.add_subplot(1, 2, 1);
    cm = confusion_matrix(labels, pred_labels);
    cm = ConfusionMatrixDisplay(cm, display_labels=range(10));
    cm.plot(values_format='d', cmap='Blues', ax=ax)


plot_confusion_matrix(labels1, pred_labels)  # 显示混淆矩阵
plt.show()  # 绘图显示


# ----------计算PCA
def get_representations(model, iterator, device):
    model.eval()

    outputs = []
    intermediates = []
    labels = []

    with torch.no_grad():
        for (x, y) in iterator:
            x = x.to(device)
            y_pred = model(x)
            outputs.append(y_pred.cpu())
            intermediates.append(y_pred.cpu())
            labels.append(y)
    outputs = torch.cat(outputs, dim=0)
    intermediates = torch.cat(intermediates, dim=0)
    labels = torch.cat(labels, dim=0)
    return outputs, intermediates, labels


outputs, intermediates, labels = get_representations(model, train_iterator, device)


def get_pca(data, n_components=2):
    pca = decomposition.PCA()
    pca.n_components = n_components
    pca_data = pca.fit_transform(data)
    return pca_data


def plot_representations(data, labels, n_images=None):
    if n_images is not None:
        data = data[:n_images]
        labels = labels[:n_images]
    fig = plt.figure(figsize=(10, 10))
    ax = fig.add_subplot(111)
    scatter = ax.scatter(data[:, 0], data[:, 1], c=labels, cmap='tab10')
    handles, labels = scatter.legend_elements()
    legend = ax.legend(handles=handles, labels=labels)


# 然后绘制PCA表示
output_pca_data = get_pca(outputs)
plot_representations(output_pca_data, labels)

# 在第二组卷积和子采样层之后绘制中间值的PCA表示。
intermediate_pca_data = get_pca(intermediates)
plot_representations(intermediate_pca_data, labels)
