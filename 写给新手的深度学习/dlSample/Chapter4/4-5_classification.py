#!/usr/bin/env python
# coding: utf-8

# ### 4.5.7 神经网络（分类）

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt

# x、y坐标
X = np.arange(-1.0, 1.0, 0.1)  # 元素数量是20个
Y = np.arange(-1.0, 1.0, 0.1)

# 权重
w_im = np.array([[1.0,2.0],
                 [2.0,3.0]])  # 中间层 2x2的矩阵
w_mo = np.array([[-1.0,1.0],
                 [1.0,-1.0]]) # 输出层 2x2的矩阵

# 偏置
b_im = np.array([0.3,-0.3]) # 中间层
b_mo = np.array([0.4,0.1])  # 输出层 

# 中间层
def middle_layer(x, w, b):
    u = np.dot(x, w) + b
    return 1/(1+np.exp(-u)) # sigmoid関数

# 输出层
def output_layer(x, w, b):
    u = np.dot(x, w) + b
    return np.exp(u)/np.sum(np.exp(u)) # SoftMax函数

# 保存分类结果的列表
x_1 = []
y_1 = []
x_2 = []
y_2 = []

# 在网格的各个方格中进行神经网络的运算
for i in range(20):
    for j in range(20):
        
        # 正向传播
        inp = np.array([X[i], Y[j]])
        mid = middle_layer(inp, w_im, b_im)
        out = output_layer(mid, w_mo, b_mo)
        
        # 对概率的大小进行比较并分类
        if out[0] > out[1]:
            x_1.append(X[i])
            y_1.append(Y[j])
        else:
            x_2.append(X[i])
            y_2.append(Y[j])

# 分布图的显示
plt.scatter(x_1, y_1, marker="+")
plt.scatter(x_2, y_2, marker="o")
plt.show()


# In[ ]:




