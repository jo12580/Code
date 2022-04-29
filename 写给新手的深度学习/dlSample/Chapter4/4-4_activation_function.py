#!/usr/bin/env python
# coding: utf-8

# ## 4.4 激励函数

# ### 4.4.1 阶跃函数

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pyplot as plt

def step_function(x):
    return np.where(x<=0, 0, 1)

x = np.linspace(-5, 5)
y = step_function(x)

plt.plot(x, y)
plt.show()


# ### 4.4.2 Sigmoid函数

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

def sigmoid_function(x):
    return 1/(1+np.exp(-x))

x = np.linspace(-5, 5)
y = sigmoid_function(x)

plt.plot(x, y)
plt.show()


# ### 4.4.3 tanh

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

def tanh_function(x):
    return np.tanh(x)

x = np.linspace(-5, 5)
y = tanh_function(x)

plt.plot(x, y)
plt.show()


# ### 4.4.4 ReLU

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

def relu_function(x):
    return np.where(x <= 0, 0, x)

x = np.linspace(-5, 5)
y = relu_function(x)

plt.plot(x, y)
plt.show()


# ### 4.4.5 Leaky ReLU

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

def leaky_relu_function(x):
    return np.where(x <= 0, 0.01*x, x)

x = np.linspace(-5, 5)
y = leaky_relu_function(x)

plt.plot(x, y)
plt.show()


# ### 4.4.6 恒等函数

# In[ ]:


get_ipython().run_line_magic('matplotlib', 'inline')

import numpy as np
import matplotlib.pylab as plt

x = np.linspace(-5, 5)
y = x

plt.plot(x, y)
plt.show()


# ### 4.4.7 SoftMax函数

# In[ ]:


import numpy as np

def softmax_function(x):
    return np.exp(x)/np.sum(np.exp(x)) # SoftMax函数

y = softmax_function(np.array([1,2,3]))
print(y)


# In[ ]:




