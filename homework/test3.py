import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms

batch_size = 200
learning_rate = 0.01
epochs = 10
# 为给定对象设置模块以获得更好的打印效果
train_loadar = torch.utils.data.DataLoader(
    datasets.MNIST('./data', train=True, download=True,
                   transform=transforms.Compose([
                       transforms.ToTensor(),
                       transforms.Normalize((0.1307,), (0.3801,))
                   ])),
    batch_size=batch_size, shuffle=True)
test_loader = torch.utils.data.DataLoader(
    datasets.MNIST('./data', train=False, transform=transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])),
    batch_size=batch_size, shuffle=True)
# randn（）产生随机数，zeros（），zeros（）张量返回一个充满标量值0的张量，其形状由变量定义
w1, b1 = torch.randn(200, 784, requires_grad=True), \
         torch.zeros(200, requires_grad=True)
w2, b2 = torch.zeros(200, 200, requires_grad=True), \
         torch.zeros(200, requires_grad=True)
w3, b3 = torch.randn(10, 200, requires_grad=True), \
         torch.zeros(10, requires_grad=True)

torch.nn.init.kaiming_normal_(w1)  # 填充输入“张量”
torch.nn.init.kaiming_normal_(w2)
torch.nn.init.kaiming_normal_(w3)


def forward(x):
    x = x @ w1.t() + b1
    x = F.relu(x)
    x = x @ w2.t() + b2
    x = F.relu(x)
    x = x @ w3.t() + b3
    x = F.relu(x)
    return x


optimizer = optim.SGD([w1, b1, w2, b2, w3, b3], lr=learning_rate)  # 随机梯度
criteon = nn.CrossEntropyLoss()

for epoch in range(epochs):
    for batch_idx, (data, target) in enumerate(train_loadar):
        data = data.view(-1, 28 * 28)

        logits = forward(data)
        loss = criteon(logits, target)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if batch_idx % 100 == 0:
            print('Train Epoch:{}/{}({:.0f}%)\tLoss:{:.6}'.format(
                epoch, batch_idx * len(data), len(train_loadar.dataset),
                       100. * batch_idx / len(train_loadar), loss.item()
            ))

test_loss = 0
correct = 0
for data, target in test_loader:
    data = data.view(-1, 28 * 28)
    logits = forward(data)
    test_loss += criteon(logits, target).item()

    pred = logits.data.max(1)[1]
    correct += pred.eq(target.data).sum()

test_loss /= len(test_loader.dataset)
print('\nTest set:Average loss:{:.4f},Accuracy:{}/{}({:.0f}%)\n'.format(
    test_loss, correct, len(test_loader.dataset),
    100. * correct / len(test_loader.dataset)
))
