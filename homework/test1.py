import torch

flag = False
if flag:
    a = [1, 2, 3, 4]
    b = torch.tensor(a)  # 使用torch.tensor()函数构建张量
    c = torch.tensor([1, 2, 3, 4])  # 使用torch.tensor()函数构建张量
    print("torch.tensor:",b,c)  # 输出张量

#  查看tensor类型
flag = False
# flag = True
if flag:
    a = torch.randn(2, 3)
    print(a.type())
    print(type(a))
    print(a.dtype)
    print(isinstance(a, torch.FloatTensor))

# 使用torch.tensor()创建tensor
flag = False
# flag = True
if flag:
    arr = np.ones((3, 3))
    print("ndarray的数据类型：", arr.dtype)

    t = torch.tensor(arr)
    print(t)

# 使用torch.from_numpy创建tensor
flag = False
# flag = True
if flag:
    arr = np.array([[1, 2, 3],
                    [4, 5, 6]])
    t = torch.from_numpy(arr)
    print("numpy array:\n", arr)
    print("tensor:\n", t)

    # print("\n修改arr")
    # arr[0,0] = 0
    # print("numpy array:\n", arr)
    # print("tensor：\n", t)

    print("\n修改tensor")
    t[0, 0] = 12
    print("numpy array:", arr)
    print("tensor:", t)
    print(t.shape)

# 使用torch.zeros()创建tensor
flag = False
# flag = True
if flag:
    out_t = torch.tensor([1])
    print(out_t)
    t = torch.zeros((3, 3), out=out_t)
    # print(out_t)
    print(t, '\n', out_t)
    print(id(t), id(out_t), id(t) == id(out_t))

# 使用torch.ones()创建tensor
flag = False
# flag = True
if flag:
    t = torch.ones((3, 3))
    print("torch.ones:\n", t)

# 使用torch.full()创建tensor
flag = False
# flag = True
if flag:
    t = torch.full((5, 3), 12, dtype=int)
    print("torch.full:\n", t)

# 使用torch.arange创建tensor
flag = False
# flag = True
if flag:
    t = torch.arange(0, 8, 2)
    print("torch.arange:\n", t)

# 使用torch.linspace创建tensor
flag = False
# flag = True
if flag:
    t = torch.linspace(2, 10, 5)
    print("torch.linspace:\n", t)

# 使用torch.eye创建tensor
flag = False
# flag = True
if flag:
    t = torch.eye(5)
    print("torch.eye:\n", t)

# 使用torch.normal创建tensor
flag = False
# flag = True
if flag:
    # mean:张量 std：张量
    # mean = torch.arange(1,5,dtype=torch.float)
    # std = torch.arange(1,5,dtype=torch.float)
    # t_normal = torch.normal(mean,std)
    # print("mean:{}\nstd:{}".format(mean,std))
    # print(t_normal)

    # mean:标量 std：标量
    # t_normal = torch.normal(0.,1.,size=(4,))
    # print(t_normal)

    # mean:张量 std：标量
    mean = torch.arange(1, 5, dtype=torch.float)
    std = 1
    t_normal = torch.normal(mean, std)
    print("mean:{}\nstd:{}".format(mean, std))
    print(t_normal)

# 使用torch.randn创建tensor
flag = False
# flag = True
if flag:
    t = torch.randn((3, 3))
    print("torch.randn:\n", t)
