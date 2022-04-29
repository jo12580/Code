#!/usr/bin/env python
# coding: utf-8

# ## 3.2 线性代数

# ### 3.2.1 标量

# In[ ]:


a = 1
b = 1.2
c = -0.25
d = 1.2e5


# ### 3.2.2 向量

# In[ ]:


import numpy as np

a = np.array([1, 2, 3])  # 作为横向量使用
b = np.array([-2.3, 0.25, -1.2, 1.8, 0.41])


# ### 3.2.3 矩阵

# In[ ]:


import numpy as np

a = np.array([[1, 2, 3],
              [4, 5, 6]])  # 2x3的矩阵
b = np.array([[0.21, 0.14],
              [-1.3, 0.81],
              [0.12, -2.1]])  # 3x2的矩阵


# ### 3.2.4 张量

# In[ ]:


import numpy as np

a = np.array([[[0, 1, 2, 3],
               [2, 3, 4, 5],
               [4, 5, 6, 7]],
              
              [[1, 2, 3, 4],
               [3, 4, 5, 6],
               [5, 6, 7, 8]]])  # (2, 3, 4)的3阶张量


# In[ ]:


# 1阶张量（向量）
b = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24])

# 2阶张量（矩阵）
b = b.reshape(4, 6)
print(b)


# In[ ]:


# 3阶的张量
b = b.reshape(2, 3, 4)
print(b)


# In[ ]:


# 4阶的张量
b = b.reshape(2, 2, 3, 2)
print(b)


# In[ ]:


c = np.array([[[1,2,3,4],
               [2,0,0,0],
               [3,0,0,0]],
             
              [[2,0,0,0],
               [0,0,0,0],
               [0,0,0,0]]])  # (2, 3, 4)的3阶的张量


# In[ ]:


c = c.transpose(0, 2, 1)  # (2, 4, 3)
print(c) 


# In[ ]:


c = c.transpose(2, 0, 1)  # (3, 2, 4)
print(c) 


# In[ ]:


c = c.transpose(1, 0, 2)  # (2, 3, 4)
print(c) 


# ### 3.2.5 标量与矩阵的乘积

# In[ ]:


import numpy as np

c = 2
a = np.array([[0, 1, 2],
              [3, 4, 5],
              [6, 7, 8]]) 

print(c*a)


# ### 3.2.6 元素项的乘积

# In[ ]:


import numpy as np

a = np.array([[0, 1, 2],
              [3, 4, 5],
              [6, 7, 8]]) 
b = np.array([[0, 1, 2],
              [2, 0, 1],
              [1, 2, 0]]) 

print(a*b)


# In[ ]:


print(a+b)  # 加法运算


# In[ ]:


print(a-b)  # 减法运算


# In[ ]:


print(a/(b+1))  # 除法（加1是为了防止除以零）


# In[ ]:


print(a%(b+1))  # 求余（加1是为了防止除以零）


# ### 3.2.7 矩阵乘法

# In[ ]:


import numpy as np

a = np.array([[0, 1, 2],
              [1, 2, 3]]) 
b = np.array([[2, 1],
              [2, 1],
              [2, 1]]) 

print(np.dot(a, b))


# In[ ]:


import numpy as np

a = np.array([1, 2, 3])  # 作为行数为1的矩阵进行处理
b = np.array([[1, 2],
              [1, 2],
              [1, 2]])

print(np.dot(a, b))


# ### 3.2.8 矩阵的转置

# In[ ]:


import numpy as np

a = np.array([[1, 2, 3],
              [4, 5, 6]])
print(a.T)  # 转置


# In[ ]:


import numpy as np

a = np.array([[0, 1, 2],
              [1, 2, 3]])  # 2x3
b = np.array([[0, 1, 2],
              [1, 2, 3]])  # 2x3 这样的话是无法进行矩阵乘法计算的

# print(np.dot(a, b))  # 这个代码会导致程序执行报错
print(np.dot(a, b.T))  # 通过转置操作，使矩阵可以相乘


# In[ ]:




