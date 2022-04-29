#!/usr/bin/env python
# coding: utf-8

# ## 2.4 Numpy

# ### 2.3.1 Numpy的导入

# In[ ]:


import numpy as np


# ### 2.4.2 Numpy的数组

# In[ ]:


a = np.array([0, 1, 2, 3, 4, 5])
print(a)


# In[ ]:


b = np.array([[0, 1, 2], [3, 4, 5]])  # 传递列表的列表
print(b)


# In[ ]:


c = np.array([[[0, 1, 2], [3, 4, 5]], [[5, 4, 3], [2, 1, 0]]])
print(c)


# In[ ]:


print(np.shape(c))
print(np.size(c))


# In[ ]:


d = [[1,2],[3,4],[5,6]]  # (3, 2)
print(len(d))
print(len(np.array(d)))


# ### 2.4.3 生成数组的各种函数

# In[ ]:


print(np.zeros(10))
print(np.ones(10))
print(np.random.rand(10))


# In[ ]:


print(np.zeros((2, 3)))
print(np.ones((2, 3)))


# In[ ]:


print(np.arange(0, 1, 0.1))


# In[ ]:


print(np.arange(10))


# In[ ]:


print(np.linspace(0, 1, 11))


# In[ ]:


print(np.linspace(0, 1))


# ### 2.4.4 基于 reshape 的形状变换

# In[ ]:


a = np.array([0, 1, 2, 3, 4, 5, 6, 7])    # 数组的创建
b = a.reshape(2, 4)                       # 变换成形状为 (2, 4) 的二维数组
print(b)


# In[ ]:


c = b.reshape(2, 2, 2)  # 变换为(2, 2, 2)的三维数组
print(c)


# In[ ]:


d = c.reshape(4, 2)  # 变换为(4, 2)的二维数组
print(d)


# In[ ]:


e = d.reshape(-1)  # 变换为一维数组
print(e)


# In[ ]:


f = e.reshape(2, -1)
print(f)


# ### 2.4.5 数组的运算

# In[ ]:


a = np.array([0, 1, 2, 3, 4, 5]).reshape(2, 3)
print(a)


# In[ ]:


print(a + 3) # 对每个元素加3


# In[ ]:


print(a * 3) # 对每个元素乘3


# In[ ]:


b = np.array([5, 4, 3, 2, 1, 0]).reshape(2, 3)
print(b)


# In[ ]:


print(a + b)


# In[ ]:


print(a * b)


# ### 2.4.6 广播

# In[ ]:


a = np.array([[1, 1],
              [1, 1]])  # 二维数组
b = np.array([1, 2])  # 一维数组


# In[ ]:


print(a + b)


# In[ ]:


c = np.array([[1],
              [2]])  # 二维数组


# In[ ]:


print(a + c)


# ### 2.4.7 对元素的访问

# In[ ]:


a = np.array([0, 1, 2, 3, 4, 5])
print(a[2])


# In[ ]:


a[2] = 9
print(a)


# In[ ]:


b = np.array([[0, 1, 2],
              [3, 4, 5]])
print(b[1, 2])  # 与b[1][2]作用相同


# In[ ]:


b[1, 2] = 9
print(b)


# In[ ]:


c = np.array([[0, 1, 2],
              [3, 4, 5],
              [6, 7, 8]])
print(c[1])  # 只指定了一个索引值


# In[ ]:


c[1] = np.array([9, 10, 11])  # 使用数组替换元素
print(c)


# In[ ]:


d = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
print(d[d%2 == 0])  # 在[ ]中指定条件


# In[ ]:


e = np.zeros((3, 3))  # 二维数组，元素全部为0
f = np.array([8, 9])  # 用于替换的值

e[np.array([0, 2]), np.array([0, 1])] = f  # 将两个数组指定为索引
print(e)


# ### 2.4.8 切片

# In[ ]:


a = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
print(a[2:8])  # 切片


# In[ ]:


print(a[2:8:2])


# In[ ]:


print(a[:])


# In[ ]:


b = np.array([[0, 1, 2],
              [3, 4, 5],
              [6, 7, 8]])
print(b[0:2, 0:2])  # 指定每个维度的范围


# In[ ]:


b[0:2, 0:2] = np.array([[9, 9],
                        [9, 9]])  # 替换左上方的区域
print(b)


# In[ ]:


c = np.zeros(18).reshape(2, 3, 3)
print(c)


# In[ ]:


c[0, 0:2, 0:2] =  np.ones(4).reshape(2, 2)
print(c)


# ### 2.4.9 轴与transpose方法

# In[ ]:


a = np.array([[0, 1, 2],
              [3, 4, 5]])
print(a)


# In[ ]:


print(a.transpose(1, 0))  # 对轴进行调换


# In[ ]:


print(a.T)  # 转置


# In[ ]:


b = np.arange(12).reshape(2, 2, 3)
print(b)


# In[ ]:


print(b.transpose(1, 2, 0))


# ### 2.4.10 NumPy的函数

# In[ ]:


a = np.array([[0, 1],
              [2, 3]])
print(np.sum(a))


# In[ ]:


print(np.sum(a, axis=0))


# In[ ]:


print(np.sum(a, axis=1))


# In[ ]:


print(np.sum(a, axis=1, keepdims=True))


# In[ ]:


print(np.max(a))


# In[ ]:


print(np.max(a, axis=0))


# In[ ]:


print(np.argmax(a, axis=0))


# In[ ]:


print(np.where(a<2, 9, a))


# In[ ]:




