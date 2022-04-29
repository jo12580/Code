#!/usr/bin/env python
# coding: utf-8

# ### 7.2.3 im2col的实现 -简单的im2col-

# In[ ]:


import numpy as np

def im2col(image, flt_h, flt_w, out_h, out_w): # 输入图像，过滤器的高度和宽度，输出图像的高度和宽度
   
    img_h, img_w = image.shape # 输入图像的高度和宽度

    cols = np.zeros((flt_h*flt_w, out_h*out_w)) # 生成的矩阵的尺寸

    for h in range(out_h):
        h_lim = h + flt_h
        for w in range(out_w):
            w_lim = w + flt_w
            cols[:, h*out_w+w] = img[h:h_lim, w:w_lim].reshape(-1)

    return cols

img = np.array([[1, 2, 3, 4],
                [5, 6, 7, 8],
                [9, 10,11,12],
                [13,14,15,16]])
cols = im2col(img, 2, 2, 3, 3)
print(cols)


# In[ ]:


import numpy as np

def im2col(image, flt_h, flt_w, out_h, out_w):
   
    img_h, img_w = image.shape
    cols = np.zeros((flt_h, flt_w, out_h, out_w))

    for h in range(flt_h):
        h_lim = h + out_h
        for w in range(flt_w):
            w_lim = w + out_w
            cols[h, w, :, :] = img[h:h_lim, w:w_lim]

    cols = cols.reshape(flt_h*flt_w, out_h*out_w)

    return cols

img = np.array([[1, 2, 3, 4],
                [5, 6, 7, 8],
                [9, 10,11,12],
                [13,14,15,16]])
cols = im2col(img, 2, 2, 3, 3)
print(cols)


# ### 7.2.4 im2col的实现 -im2col的实用化-

# In[ ]:


def im2col(images, flt_h, flt_w, out_h, out_w):
   
    n_bt, n_ch, img_h, img_w = images.shape # 批次尺寸，通道数，输入图像的高度和宽度
    
    cols = np.zeros((n_bt, n_ch, flt_h, flt_w, out_h, out_w))

    for h in range(flt_h):
        h_lim = h + out_h
        for w in range(flt_w):
            w_lim = w + out_w
            cols[:, :, h, w, :, :] = images[:, :, h:h_lim, w:w_lim]

    cols = cols.transpose(1, 2, 3, 0, 4, 5).reshape(n_ch*flt_h*flt_w, n_bt*out_h*out_w)
    return cols

img = np.array([[[[1, 2, 3, 4],
                  [5, 6, 7, 8],
                  [9, 10,11,12],
                  [13,14,15,16]]]])
cols = im2col(img, 2, 2, 3, 3)
print(cols)


# In[ ]:


def im2col(images, flt_h, flt_w, out_h, out_w, stride, pad):
   
    n_bt, n_ch, img_h, img_w = images.shape
    
    img_pad = np.pad(images, [(0,0), (0,0), (pad, pad), (pad, pad)], "constant")
    cols = np.zeros((n_bt, n_ch, flt_h, flt_w, out_h, out_w))

    for h in range(flt_h):
        h_lim = h + stride*out_h
        for w in range(flt_w):
            w_lim = w + stride*out_w
            cols[:, :, h, w, :, :] = img_pad[:, :, h:h_lim:stride, w:w_lim:stride]

    cols = cols.transpose(1, 2, 3, 0, 4, 5).reshape(n_ch*flt_h*flt_w, n_bt*out_h*out_w)
    return cols

img = np.array([[[[1, 2, 3, 4],
                  [5, 6, 7, 8],
                  [9, 10,11,12],
                  [13,14,15,16]]]])
cols = im2col(img, 2, 2, 3, 3, 1, 0)
print(cols)


# ### 7.2.6 col2im的实现

# In[ ]:


def col2im(cols, img_shape, flt_h, flt_w, out_h, out_w, stride, pad):
 
    n_bt, n_ch, img_h, img_w = img_shape
    
    cols = cols.reshape(n_ch, flt_h, flt_w, n_bt, out_h, out_w, ).transpose(3, 0, 1, 2, 4, 5)
    images = np.zeros((n_bt, n_ch, img_h+2*pad+stride-1, img_w+2*pad+stride-1))
    
    for h in range(flt_h):
        h_lim = h + stride*out_h
        for w in range(flt_w):
            w_lim = w + stride*out_w
            images[:, :, h:h_lim:stride, w:w_lim:stride] += cols[:, :, h, w, :, :]

    return images[:, :, pad:img_h+pad, pad:img_w+pad]

cols = np.ones((4, 4))
img_shape = (1, 1, 3, 3)
images = col2im(cols, img_shape, 2, 2, 2, 2, 1, 0)
print(images)


# In[ ]:




