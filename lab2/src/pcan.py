import os
import operator
from numpy import *
import matplotlib.pyplot as plt
import cv2
# define PCA


def pca(data, threshold):
    data = float32(mat(data))
    rows, cols = data.shape  # 取大小
    data_mean = mean(data, 0)  # 对列求均值
    data_mean_all = tile(data_mean, (rows, 1))
    Z = data - data_mean_all
    T1 = Z*Z.T  # 使用矩阵计算，所以前面mat
    D, V = linalg.eig(T1)  # 特征值与特征向量
    total = sum(D)
    tmp = 0.0
    k = 0
    for item in D:
        if tmp/total < threshold:
            tmp += item
            k += 1
    V1 = V[:, 0:k]  # 取前k个特征向量
    V1 = Z.T*V1
    for i in range(k):  # 特征向量归一化
        L = linalg.norm(V1[:, i])
        V1[:, i] = V1[:, i]/L

    data_new = Z*V1  # 降维后的数据
    return data_new, data_mean, V1

# covert image to vector


def img2vector(filename):
    img = cv2.imread(filename, 0)  # read as 'gray'
    rows, cols = img.shape
    #rows, cols = (92, 112)
    # imgVector = zeros((1, rows*cols))  # create a none vectore:to raise speed
    imgVector = reshape(img, (1, rows*cols))  # change img from 2D to 1D
    return imgVector

# load dataSet


def loadDataSet(k, foldIndex):  # choose k(0-10) people as traintest for everyone
    # step 1:Getting data set
    print("--Getting data set---")
    # note to use '/'  not '\'
    dataSetDir = './lab2/data/orl_faces'
    # 显示文件夹内容
    # choose = random.permutation(10)+1  # 随机排序1-10 (0-9）+1
    choose = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    choose = choose[foldIndex+2:]+choose[0:foldIndex] + \
        choose[foldIndex:foldIndex+2]
    # print(choose)
    train_face = zeros((40*k, 112*92))
    train_face_number = zeros(40*k)
    test_face = zeros((40*(10-k), 112*92))
    test_face_number = zeros(40*(10-k))
    for i in range(40):  # 40 sample people
        people_num = i+1
        for j in range(10):  # everyone has 10 different face
            if j < k:
                filename = dataSetDir+'/s' + \
                    str(people_num)+'/'+str(choose[j])+'.pgm'
                img = img2vector(filename)
                train_face[i*k+j, :] = img
                train_face_number[i*k+j] = people_num
            else:
                filename = dataSetDir+'/s' + \
                    str(people_num)+'/'+str(choose[j])+'.pgm'
                img = img2vector(filename)
                test_face[i*(10-k)+(j-k), :] = img
                test_face_number[i*(10-k)+(j-k)] = people_num

    return train_face, train_face_number, test_face, test_face_number

# calculate the accuracy of the test_face


def facefind():
    true_num = 0
    for foldIndex in range(5):
        # Getting data set
        train_face, train_face_number, test_face, test_face_number = loadDataSet(
            8, foldIndex*2)
        # PCA training to train_face
        data_train_new, data_mean, V = pca(train_face, 0.93)
        num_train = data_train_new.shape[0]
        num_test = test_face.shape[0]
        temp_face = test_face - tile(data_mean, (num_test, 1))
        data_test_new = temp_face*V  # 得到测试脸在特征向量下的数据
        data_test_new = array(data_test_new)  # mat change to array
        data_train_new = array(data_train_new)
        for i in range(num_test):
            testFace = data_test_new[i, :]
            diffMat = data_train_new - tile(testFace, (num_train, 1))
            sqDiffMat = diffMat**2
            sqDistances = sqDiffMat.sum(axis=1)
            sortedDistIndicies = sqDistances.argsort()
            indexMin = sortedDistIndicies[0]
            if train_face_number[indexMin] == test_face_number[i]:
                true_num += 1

    accuracy = float(true_num)/(5*num_test)
    print('The classify accuracy is: %.2f%%' % (accuracy * 100))


facefind()
