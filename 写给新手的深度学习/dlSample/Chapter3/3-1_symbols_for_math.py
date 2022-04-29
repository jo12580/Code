#!/usr/bin/env python
# coding: utf-8

# ## 3.1 关于数学符号

# ### 3.1.1 求和符号西格玛（$\Sigma$）

# In[ ]:


import numpy as np

a = np.array([1, 2, 3, 4, 5])
print(np.sum(a))


# In[ ]:


import numpy as np

a = np.array([2, 1, 2, 1, 2])
b = np.array([1, 2, 1, 2, 1])

print(np.sum(a*b))


# ### 3.1.2 纳皮尔数$e$

# In[ ]:


import numpy as np

def get_exp(x):
    return np.exp(x)
    
print(get_exp(1))


# ### 3.1.3 自然对数$\log$

# In[ ]:


import numpy as np

def get_log(x):
    return np.log(x)
    
print(get_log(1))


# In[ ]:




