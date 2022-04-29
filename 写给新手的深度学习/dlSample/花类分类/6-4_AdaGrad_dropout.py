#!/usr/bin/env python
# coding: utf-8

# ## 6.4 深度学习的实现

# ### AdaGrad + Dropout

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets

# -- 鸢尾花数据的读入 --
iris_data = datasets.load_iris()
input_data = iris_data.data
correct = iris_data.target
n_data = len(correct)  # 样本数量

# -- 对输入数据进行标准化处理 --
ave_input = np.average(input_data, axis=0)
std_input = np.std(input_data, axis=0)
input_data = (input_data - ave_input) / std_input

# -- 将正确答案转换为独热编码格式 --
correct_data = np.zeros((n_data, 3))
for i in range(n_data):
    correct_data[i, correct[i]] = 1.0

# -- 训练数据与测试数据 --
index = np.arange(n_data)
index_train = index[index%2 == 0]
index_test = index[index%2 != 0]

input_train = input_data[index_train, :]  # 训练 输入
correct_train = correct_data[index_train, :]  # 训练 正确答案
input_test = input_data[index_test, :]  # 测试 输入
correct_test = correct_data[index_test, :]  # 测试 正确答案

n_train = input_train.shape[0]  # 训练数据的样本数
n_test = input_test.shape[0]  # 测试数据的样本数

# -- 各种设置值 --
n_in = 4  # 输入层的神经元数量
n_mid = 50  # 中间层的神经元数量
n_out = 3  # 输出层的神经元数量

wb_width = 0.1  # 权重和偏置的分散度
eta = 0.01  # 学习系数
epoch = 1000
batch_size = 8
interval = 100  # 显示进度的时间间隔


# -- 各个网络层的祖先类 --
class BaseLayer:
    def __init__(self, n_upper, n):
        self.w = wb_width * np.random.randn(n_upper, n)  # 权重（矩阵）
        self.b = wb_width * np.random.randn(n)  # 偏置（向量）

        self.h_w = np.zeros(( n_upper, n)) + 1e-8
        self.h_b = np.zeros(n) + 1e-8
        
    def update(self, eta):      
        self.h_w += self.grad_w * self.grad_w
        self.w -= eta / np.sqrt(self.h_w) * self.grad_w
        
        self.h_b += self.grad_b * self.grad_b
        self.b -= eta / np.sqrt(self.h_b) * self.grad_b

# -- 中间层 --
class MiddleLayer(BaseLayer):
    def forward(self, x):
        self.x = x
        self.u = np.dot(x, self.w) + self.b
        self.y = np.where(self.u <= 0, 0, self.u)  # ReLU
    
    def backward(self, grad_y):
        delta = grad_y * np.where(self.u <= 0, 0, 1)  # ReLU的微分

        self.grad_w = np.dot(self.x.T, delta)
        self.grad_b = np.sum(delta, axis=0)
        
        self.grad_x = np.dot(delta, self.w.T) 

# -- 输出层 --
class OutputLayer(BaseLayer):     
    def forward(self, x):
        self.x = x
        u = np.dot(x, self.w) + self.b
        self.y = np.exp(u)/np.sum(np.exp(u), axis=1, keepdims=True)  # SoftMax函数

    def backward(self, t):
        delta = self.y - t
        
        self.grad_w = np.dot(self.x.T, delta)
        self.grad_b = np.sum(delta, axis=0)
        
        self.grad_x = np.dot(delta, self.w.T) 
        
# -- Dropout --
class Dropout:
    def __init__(self, dropout_ratio):
        self.dropout_ratio = dropout_ratio  # 将神经元设为无效状态的概率

    def forward(self, x, is_train):  # is_train: 训练时为True
        if is_train:
            rand = np.random.rand(*x.shape)  # 与输入数据形状相同的随机数矩阵
            self.dropout = np.where(rand > self.dropout_ratio, 1, 0)  # 1:有效 0:无效
            self.y = x * self.dropout  # 对神经元进行随机的无效化处理
        else:
            self.y = (1-self.dropout_ratio)*x  # 测试过程中减少输出
        
    def backward(self, grad_y):
        self.grad_x = grad_y * self.dropout  # 不对无效的神经元进行反向传播处理


# -- 各网络层的初始化 --
middle_layer_1 = MiddleLayer(n_in, n_mid)
dropout_1 = Dropout(0.5)
middle_layer_2 = MiddleLayer(n_mid, n_mid)
dropout_2 = Dropout(0.5)
output_layer = OutputLayer(n_mid, n_out)

# -- 正向传播 --
def forward_propagation(x, is_train):
    middle_layer_1.forward(x)
    dropout_1.forward(middle_layer_1.y, is_train)
    middle_layer_2.forward(dropout_1.y)
    dropout_2.forward(middle_layer_2.y, is_train)
    output_layer.forward(dropout_2.y)

# -- 反向传播 --
def backpropagation(t):
    output_layer.backward(t)
    dropout_2.backward(output_layer.grad_x)
    middle_layer_2.backward(dropout_2.grad_x)
    dropout_1.backward(middle_layer_2.grad_x)
    middle_layer_1.backward(dropout_1.grad_x)

# -- 权重和偏置的更新 --
def uppdate_wb():
    middle_layer_1.update(eta)
    middle_layer_2.update(eta)
    output_layer.update(eta)

# -- 计算误差 --
def get_error(t, batch_size):
    return -np.sum(t * np.log(output_layer.y + 1e-7)) / batch_size  # 误差的统计和测算


# -- 用于记录误差 --
train_error_x = []
train_error_y = []
test_error_x = []
test_error_y = []

# -- 记录学习的过程和经过 --
n_batch = n_train // batch_size  # 每一轮epoch的批次尺寸
for i in range(epoch):

    # -- 误差的统计和测算 --  
    forward_propagation(input_train, False)
    error_train = get_error(correct_train, n_train)
    forward_propagation(input_test, False)
    error_test = get_error(correct_test, n_test)
    
    # -- 誤差の記録 -- 
    test_error_x.append(i)
    test_error_y.append(error_test) 
    train_error_x.append(i)
    train_error_y.append(error_train) 
    
    # -- 误差的记录 -- 
    if i%interval == 0:
        print("Epoch:" + str(i) + "/" + str(epoch),
              "Error_train:" + str(error_train),
              "Error_test:" + str(error_test))

    # -- 学习 -- 
    index_random = np.arange(n_train)
    np.random.shuffle(index_random)  # 将索引值随机的打乱排序
    for j in range(n_batch):
        
        #  取出最小批次
        mb_index = index_random[j*batch_size : (j+1)*batch_size]
        x = input_train[mb_index, :]
        t = correct_train[mb_index, :]
        
        # 正向传播和反向传播
        forward_propagation(x, True)
        backpropagation(t)
        
        # 权重和偏置的更新
        uppdate_wb() 

        
# -- 使用图表显示误差记录 -- 
plt.plot(train_error_x, train_error_y, label="Train")
plt.plot(test_error_x, test_error_y, label="Test")
plt.legend()

plt.xlabel("Epochs")
plt.ylabel("Error")

plt.show()

# -- 测算正确率 -- 
forward_propagation(input_train, False)
count_train = np.sum(np.argmax(output_layer.y, axis=1) == np.argmax(correct_train, axis=1))

forward_propagation(input_test, False)
count_test = np.sum(np.argmax(output_layer.y, axis=1) == np.argmax(correct_test, axis=1))

print("Accuracy Train:", str(count_train/n_train*100) + "%",
      "Accuracy Test:", str(count_test/n_test*100) + "%")


# In[ ]:


# Sepal length、Sepal width、Petal length、Petal width
samples = np.array([[5.0, 3.5, 1.0, 0.5],  # Iris1
                    [5.5, 2.5, 4.0, 1.0],  # Iris2
                    [7.0, 3.0, 6.0, 2.0],  # Iris3
                    [6.6, 2.5, 1.5, 0.2]   # Iris4
                   ])

# 标准化
ave_input = np.average(samples, axis=0)
std_input = np.std(samples, axis=0)
samples = (samples - ave_input) / std_input

# 判定
forward_propagation(samples, False)  # False表示是非训练数据
print(output_layer.y)


# In[ ]:




