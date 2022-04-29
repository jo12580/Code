#!/usr/bin/env python
# coding: utf-8

# ### 4.5.5 神经网络（回归）

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt

# x、y坐标
X = np.arange(-1.0, 1.0, 0.2)  # 元素数量为10个
Y = np.arange(-1.0, 1.0, 0.2)

# 容纳输出的10x10的网格
Z = np.zeros((10,10))

# 权重
w_im = np.array([[4.0,4.0],
                 [4.0,4.0]])  # 中间层 2×2的矩阵
w_mo = np.array([[1.0],
                 [-1.0]])     # 输出层 2×1的矩阵

# 偏置
b_im = np.array([3.0,-3.0]) # 中间层
b_mo = np.array([0.1])      # 输出层 

# 中间层
def middle_layer(x, w, b):
    u = np.dot(x, w) + b
    return 1/(1+np.exp(-u)) # sigmoid函数

# 输出层
def output_layer(x, w, b):
    u = np.dot(x, w) + b
    return u  # 恒等函数

# 在网格的各个方格中进行神经网络的运算
for i in range(10):
    for j in range(10):
        
        # 正向传播
        inp = np.array([X[i], Y[j]])        # 输入层
        mid = middle_layer(inp, w_im, b_im) # 中间层
        out = output_layer(mid, w_mo, b_mo) # 输出层
        
        # 在网格收纳NN的输出
        Z[j][i] = out[0]

# 网格的表示
plt.imshow(Z, "gray", vmin = 0.0, vmax = 1.0)
plt.colorbar()  
plt.show()


# In[ ]:




