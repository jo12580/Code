# python 的标准库手册推荐在任何情况下尽量使用time.clock().
# 只计算了程序运行CPU的时间，返回值是浮点数
import time

start = time.clock()


def move(n, a, b, c):
    if n == 1:
        print(a, '-->', c)
        with open("algorithm_1.txt", "a") as f:
            f.write(a)
            f.write('-->')
            f.write(c)
            f.write('\n')
    else:
        move(n - 1, a, c, b)  # 将前n-1个盘子从a移动到b上
        move(1, a, b, c)  # 将最底下的最后一个盘子从a移动到c上
        move(n - 1, b, a, c)  # 将b上的n-1个盘子移动到c上


move(99, 'A', 'B', 'C')

end = time.clock()
print('Running time: %s Seconds' % (end - start))
m = str(end - start)
with open("algorithm_1.txt", "a") as f:
    f.write(m)
