#!/usr/bin/env python
# coding: utf-8

# ## 2.5 Matplotlib

# ### 2.5.1 模块的导入

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt


# ### 2.5.2 绘制图表

# In[ ]:


x = np.linspace(0, 2*np.pi)  #  从0到2π变化
y = np.sin(x)

plt.plot(x, y)
plt.show()


# ### 2.5.3 修饰图表

# In[ ]:


x = np.linspace(0, 2*np.pi)
y_sin = np.sin(x)
y_cos = np.cos(x)

# 轴的标签
plt.xlabel("x value")
plt.ylabel("y value")

# 图表的标题
plt.title("sin/cos")

# 对绘制过程中所使用凡例和线条的样式进行指定
plt.plot(x, y_sin, label="sin")
plt.plot(x, y_cos, label="cos", linestyle="dashed")
plt.legend() # 显示凡例

plt.show()


# ### 2.5.4 分布图的显示

# In[ ]:


x_1 = np.random.rand(100) - 1.0  # 将这个分组往左移1.0 
y_1 = np.random.rand(100)
x_2 = np.random.rand(100)
y_2 = np.random.rand(100)

plt.scatter(x_1, y_1, marker="+")  # 分布图的绘制
plt.scatter(x_2, y_2, marker="x")

plt.show()


# ### 2.5.5 图像的显示

# In[ ]:


img = np.array([[0, 1, 2, 3],
                [4, 5, 6, 7],
                [8, 9, 10,11],
                [12,13,14,15]])

plt.imshow(img, "gray")  # 使用灰阶值进行标识
plt.colorbar()   # 显示颜色条
plt.show()


# In[ ]:


img = plt.imread("flower.png")

plt.imshow(img)
plt.show()


# In[ ]:




