import glob
import os

import cv2
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torchvision.transforms as transforms
from PIL import Image
from torch.nn import functional as F
import shutil

"""-------------------------------------------设置 ResNet18 架构---------------------------------------------------------"""


class ResBlk(nn.Module):  # 定义Resnet Block模块
    def __init__(self, ch_in, ch_out, stride=1):  # 进入网络前先得知道传入层数和传出层数的设定
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
        # extra module: [b, ch_in, h, w] => [b, ch_out, h, w]
        # 按元素添加:
        out = self.extra(x) + out  # 将x传入extra经过2块（block）输出后与原始值进行相加
        out = F.relu(out)  # 调用relu，这里使用F.调用
        return out


class ResNet18(nn.Module):  # 构建resnet18层

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
        # # [b, 256, h, w] => [b, 512, h, w]
        self.blk3 = ResBlk(128, 256, stride=1)
        # # [b, 512, h, w] => [b, 1024, h, w]
        self.blk4 = ResBlk(256, 256, stride=1)

        self.outlayer = nn.Linear(256 * 1 * 1, 10)  # 最后是全连接层

    def forward(self, x):  # 定义整个向前传播
        x = F.relu(self.conv1(x))  # 先经过第一层卷积

        # [b, 64, h, w] => [b, 1024, h, w]
        x = self.blk1(x)  # 然后通过4次resnet网络结构
        x = self.blk2(x)
        x = self.blk3(x)
        x = self.blk4(x)

        # print('after conv:', x.shape) #[b, 512, 2, 2]
        # F.adaptive_avg_pool2d功能尾巴变为1,1，[b, 512, h, w] => [b, 512, 1, 1]
        x = F.adaptive_avg_pool2d(x, [1, 1])
        # print('after pool:', x.shape)
        x = x.view(x.size(0), -1)  # 平铺一维值
        x = self.outlayer(x)  # 全连接层

        return x


"""-------------------------------------------------图像预处理--------------------------------------------------------"""
if os.path.isdir("./Object"):
    shutil.rmtree("./Object")
os.makedirs("Object")
filename = os.listdir("User-Defined/")  # 获取目录下所有图片名
print(filename)
os.listdir()  # 方法用于返回指定的文件夹包含的文件或文件夹的名字的列表。
base_dir = "User-Defined/"  # input
new_dir = "Object/"  # output
for img in filename:
    name = img
    print(img)
    img = cv2.imread(base_dir + str(img))
    print(img)
    Grayimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(Grayimg, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
    ret, thresh = cv2.threshold(Grayimg, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)
    cv2.imwrite('img.png', thresh)
    image = Image.open('./img.png')
    image_size = image.resize((350, 350), Image.ANTIALIAS)
    print(img)
    image_size.save(new_dir + name)
    os.remove('./img.png')

"""----------------------------------------------加载网络和参数--------------------------------------------------------"""
model = ResNet18()
model.load_state_dict(torch.load('./model.ckpt'))
model.eval()

file_list = glob.glob(os.path.join('Object/', '*'))
grid_rows = len(file_list) / 5 + 1

for i, file in enumerate(file_list):
    image = Image.open(file).resize((28, 28))
    gray_image = image.convert("1")
    transform = transforms.ToTensor()
    im_data = transform(gray_image)
    im_data = im_data.resize(1, 1, 28, 28)
    outputs = model(im_data)
    _, pred = torch.max(outputs, 1)
    plt.subplot(grid_rows, 5, i + 1)
    plt.imshow(gray_image)
    plt.subplots_adjust(left=None, bottom=None, right=None, top=None,
                        wspace=0, hspace=2)
    plt.title("{}".format(pred.item()), fontsize=12)
    plt.axis('off')
    print('[{}]预测数字为: [{}]'.format(file, pred.item()))
plt.show()
shutil.rmtree("./Object")
