from numpy import *
import operator
import matplotlib.pyplot as plt


# 创建数据函数
def creatDataSet():
    group = array([[1, 101], [5, 89], [108, 5], [115, 8]])  # #数据集的二维特征
    labels = ['爱情片', '爱情片', '动作片', '动作片']  # 数据集的标签
    return group, labels


# k-近邻算法
def classsify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0]  # 获取数据集的大小
    # 计算距离
    diffMat = tile(inX, (dataSetSize, 1)) - dataSet  # 返回的是数组类型，第一个A重复了reps次的一个数组，一行reps列
    sqDiffMat = diffMat ** 2
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances ** 0.5
    sortedDisIndicies = distances.argsort()
    classCount = {}
    for i in range(k):
        # 选择距离最小的k个点
        voteIlabel = labels[sortedDisIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1
    # 排序
    sortedClassCount = sorted(classCount.items(),
                              key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]


if __name__ == '__main__':
    group, labels = creatDataSet()  # 获取数据集的特征和标签
    print(group)
    # 绘制图点
    plt.scatter(group[0:2, 0], group[0:2, 1], label='Romance film')
    plt.scatter(group[2:, 0], group[2:, 1], label='Action film')
    plt.scatter(101, 20, label='unknown')
    plt.legend()
    print(classsify0([101, 20], group, labels, 3))
    plt.show()
