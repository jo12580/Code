#!/usr/bin/env python
# coding: utf-8

# ### 6.2.6 数据的预处理

# #### 正规化

# In[ ]:


import numpy as np

def normalize(x): 
    x_max = np.max(x)
    x_min = np.min(x)
    return (x - x_min) / (x_max - x_min)

print(normalize(np.array([1,2,3,4])))


# #### 标准化

# In[ ]:


import numpy as np

def standardize(x): 
    ave = np.average(x)
    std = np.std(x)
    return (x - ave) / std

print(standardize(np.array([1,2,3,4])))


# In[ ]:




