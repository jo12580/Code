import torch
from torch.nn import functional as F

a = torch.full([4], 1 / 4.)  # torch.full() 依input形状创建全0张量
print(a)
print(a * torch.log2(a))  # torch.log2()   以 2 为底的对数的新张量
print(-(a * torch.log2(a)).sum())

b = torch.tensor([0.1, 0.1, 0.1, 0.7])
print(-(b * torch.log2(b)).sum())

c = torch.tensor([0.001, 0.001, 0.999])  # 生成张量
print(-(c * torch.log2(c)).sum())

x = torch.randn(1, 784)  # 随机返回的随机数
w = torch.randn(10, 784)

logits = x @ w.t()
pred = F.softmax(logits, dim=1)

pred_log = torch.log(pred)

F.cross_entropy(logits, torch.tensor([3]))  # cross_entropy（） 交叉熵

F.nll_loss(pred_log, torch.tensor([3]))  # nll_loss（）  负对数似然损失
