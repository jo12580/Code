#!/usr/bin/env python
# coding: utf-8

# ## 2.3 Python的语法

# ### 2.3.1 变量与类型

# In[ ]:


a = 123


# In[ ]:


a = 123  # 整数型
b = 123.456  # 浮点小数型
c = "Hello World!"  # 字符串型
d = True  # 布尔型
e = [1, 2, 3]  #列表型


# In[ ]:


a = 123
print(type(a))


# In[ ]:


a = True; b = False
print(a+b)


# In[ ]:


1.2e5   # 1.2x10的5次幂 120000
1.2e-5  # 1.2x10的-5次幂 0.000012


# ### 2.3.2 运算符

# In[ ]:


a = 3; b = 4

c = a + b               # 加法运算
print(c) 

d = a < b               # 比较（相比是大还是小）
print(d)

e = 3 < 4 and 4 < 5     # 逻辑与运算
print(e)


# In[ ]:


a = "Hello" + "World"
print(a)

b = [1, 2, 3] + [4, 5, 6]
print(b)


# ### 2.3.3 リスト

# In[ ]:


a = [1, 2, 3, 4, 5]     # 列表的创建

b = a[2]                # 获取第三个元素
print(b)

a.append(6)             # 向列表末尾添加元素る
print(a)

a[2] = 7                # 替换列表中的元素
print(a)


# ### 2.3.4 元组

# In[ ]:


a = (1, 2, 3, 4, 5)     # 元组的创建

b = a[2]                # 获取第三个元素
print(b)


# In[ ]:


print(a + (6, 7, 8, 9, 10))


# In[ ]:


a = [1, 2, 3]
a1, a2, a3 = a
print(a1, a2, a3)

b = (4, 5, 6)
b1, b2, b3 = b
print(b1, b2, b3)


# ### 2.3.5 字典

# In[ ]:


a = {"Apple":3, "Pineapple":4}      # 字典对象的创建

print(a["Apple"])       # 获取键名为"Apple"的值

a["Pinapple"] = 6       # 元素的替换
print(a["Pinapple"])

a["Melon"] = 3          # 元素的添加
print(a)


# ### 2.3.6 if语句

# In[ ]:


a = 7
if a < 12:
    print("Good morning!")
elif a < 17:
    print("Good afternoon!")
elif a < 21:
    print("Good evening!")
else:
    print("Good night!")


# ### 2.3.7 for语句

# In[ ]:


for a in [4, 7, 10]:    # 使用了列表的循环
    print(a)
    
for a in range(3):      # 使用了range函数的循环
    print(a)


# ### 2.3.8 while语句

# In[ ]:


a = 0
while a < 3:
    print(a)
    a += 1


# ### 2.3.9 闭包语法

# In[ ]:


a = [1, 2, 3, 4, 5, 6, 7]
b = [c*2 for c in a]    # 将a的元素乘以2再存放到新创建的列表对象中
print(b)


# In[ ]:


a = [1, 2, 3, 4, 5, 6, 7]
b = [c*2 for c in a if c < 5]
print(b)


# ### 2.3.10 函数

# In[ ]:


def add(a, b):          # 函数的定义
    c = a + b
    return c

print(add(3, 4))        # 函数的执行


# In[ ]:


def add(a, b=4):        # 为第二个参数设置缺省值
    c = a + b
    return c

print(add(3))           # 不指定第二个参数


# In[ ]:


def add(a, b ,c):
    d = a + b + c
    print(d)

e = (1, 2, 3)
add(*e)           # 一次性传递多个参数


# ### 2.3.11 变量的作用域

# In[ ]:


a = 123         # 全局变量

def showNum():
    b = 456     # 局部变量
    print(a, b)
    
showNum()


# In[ ]:


a = 123

def setLocal():
    a = 456     # a 被当作局部变量进行处理
    print("Local:", a)
    
setLocal()
print("Global:", a)


# In[ ]:


a = 123

def setGlobal():
    global a            # 也可以使用nonlocal关键字
    a = 456
    print("Global:", a)
    
setGlobal()
print("Global:", a)


# ### 2.3.12 类

# In[ ]:


class Calc:
    def __init__(self, a):
        self.a = a
   
    def add(self, b):
        print(self.a + b)
        
    def multiply(self, b):
        print(self.a * b)


# In[ ]:


calc = Calc(3)
calc.add(4)
calc.multiply(4)


# In[ ]:


class CalcPlus(Calc):     # 继承 Calc 类
    def subtract(self, b):
        print(self.a - b)
        
    def divide(self, b):
        print(self.a / b)


# In[ ]:


calc_plus = CalcPlus(3)
calc_plus.add(4)
calc_plus.multiply(4)
calc_plus.subtract(4)
calc_plus.divide(4)


# In[ ]:




