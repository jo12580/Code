#!/usr/bin/env python
# coding: utf-8

# ## 3.4 正态分布

# In[ ]:


import numpy as np

a = np.array([1, 2, 3, 4, 5])

print("平均值:", np.average(a))
print("标准差:", np.std(a))


# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt
 
x = np.linspace(-4,4)
y = 1/np.sqrt(2*np.pi)*np.exp(-x*x/2)

plt.plot(x,y)
plt.tick_params(labelbottom=False, labelleft=False, labelright=False, labeltop=False, color="white")
plt.show()


# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt
 
# 随机数的生成（平均值50、标准差10、共计10000个）
x = np.random.normal(50, 10, 10000)

# 显示直方图
plt.hist(x, bins=50)  # 50是棒线的根数
plt.show()


# In[ ]:


print(np.average(x))
print(np.std(x))


# In[ ]:




