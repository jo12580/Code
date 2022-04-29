#!/usr/bin/env python
# coding: utf-8

# ## 5.4 损失函数

# ### 5.4.1 平方和误差

# In[ ]:


import numpy as np

def square_sum(y, t):
    return 1.0/2.0 * np.sum(np.square(y - t))

err = square_sum(np.array([1,2,3]), np.array([2,3,4]))
print(err)


# ### 5.4.2 交叉熵误差

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

x = np.linspace(1.0e-03, 1)
y = - np.log(x)

plt.plot(x, y)
plt.show()


# In[ ]:


import numpy as np

def cross_entropy(y, t):  # 输出，正确答案
    return - np.sum(t * np.log(y + 1e-7))

err = cross_entropy(np.array([0.9,0.1,0.1]), np.array([1,0,0]))
print(err)


# In[ ]:




