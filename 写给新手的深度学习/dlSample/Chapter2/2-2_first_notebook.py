#!/usr/bin/env python
# coding: utf-8

# ## 2.2 Anaconda、Jupyter Notebook的导入

# In[ ]:


print("Hello World")


# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-np.pi, np.pi)
plt.plot(x, np.cos(x))
plt.plot(x, np.sin(x))
plt.show()


# In[ ]:




