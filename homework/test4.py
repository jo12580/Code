# #      1.类  动物实例化
# class Animal:   #定义动物类
#
#     def __init__(self,name):  #init()定义构造函数，self作为第一个参数传入
#         self.name = name
#         print('动物名称实例化')
#     def wangzi(self):
#         print(self.name + '说他叫王子')
#     def photo(self):
#         print(self.name + '想拍好看照片')
#
# horse = Animal('binbin')    #创造实例化对象horse，init()接收参数
# print(horse.name)
# horse.wangzi()  #使用。来访问对象的属性
# horse.photo()

# #      2.子类父类调用
# class Person:   ##定义父类
#     def __init__(self,name):
#         self.name = name
#         print ('调用父类方法')
#     def eat(self):
#         print('调用父类方法')
#
# class Student(Person): #定义子类，调用子类构造方法，调用子类方法
#     def __init__(self):
#         print('调用子类构造方法')
#     def study(self):
#         print('调用子类方法')
# s = Student() #实例化子类
# s.study()     #调用子类方法
# s.eat()       #调用父类方法

# #3.numpy库  聚合计算
# import numpy as np
# arr = np.random.randn(5,4)
# #正态分布的数据
# print(arr.mean())
# #聚合计算
# print(np.mean(arr))
# #求和
# print(arr.sum())

# #     数组矩阵生成
# import numpy as np
# a=[1,2,3,4]
# #创建列表
# b=np.array((a))
# #列表转换为数组
# print(b)
# #输出数组
# array_zero = np.zeros([10,10])
# #创建10行10列的数值为浮点0的矩阵
# array_one = np.ones([10,10])
# #创建10行10列的数值为浮点1的矩阵
# print(array_zero)
# #输出矩阵
# print(array_one)

# #     matplotlib库  饼图绘制
# import matplotlib.pyplot as plt
# labels = 'Frogs', 'Hogs', 'Dogs', 'Logs'  # 给每块图取名字
# sizes = [15, 35, 40, 5]  # 每块图的大小
# explode = (0, 0.1, 0, 0)  # 第二块图分离，0,1为分离的距离
# plt.pie(sizes, explode=explode, labels=labels,
#         autopct='1%.1f%%', shadow=False, startangle=90)
# # %%为输出自身%，
# plt.axis('equal')
# plt.show()

# #       6.散点图绘制
# import numpy as np
# import matplotlib.pyplot as plt
#
# fig, ax = plt.subplots()
# # s散点图绘制
# ax.plot(10 * np.random.randn(100), 10 * np.random.randn(100), 'o')
# # 绘制区域
# ax.set_title('Simple Scatter')
# # Text(0.5, 1.0, 'Simple Scatter')
# plt.show()

#       7.立体图
import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d

a, b = np.mgrid[-2:2:20j, -2:2:20j]
# 测试数据
c = a * np.exp(-a ** 2 - b ** 2)
# 三维图形
ax = plt.subplot(111, projection='3d')
ax.set_title('Stereo Image');
ax.plot_surface(a, b, c, rstride=2, cstride=1, cmap=plt.cm.Spectral)
# 设置坐标轴标签
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()
