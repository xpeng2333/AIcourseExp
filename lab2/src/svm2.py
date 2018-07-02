import re
from math import *
from svm1 import *
import os
import pickle
# 切割文本,统计词频
processedEmail = list()
EmailTagList = list()
wordDictSpam = dict()
wordDictHam = dict()
wordSet = set()
with open("lab2/data/easyham_and_spam_corpus_raw_and_processed_and_y.pickle", "rb") as emilFile:
    processedEmail = pickle.load(emilFile)
    EmailTagList = processedEmail[2]
    processedEmail = processedEmail[1]

totalNum = len(EmailTagList)
hamNum = EmailTagList.count(0)
spamNum = EmailTagList.count(1)


def splitText(bigString):

    wordlist = {}
    rtnList = []
    wordFreqList = {}
    # 分词
    listofTokens = re.split(r'\W*', bigString)
    length = len(listofTokens)
    for token in listofTokens:
        if token not in wordlist:
            wordlist[token] = 1
            rtnList.append(token)
        else:
            wordlist[token] += 1
        wordFreqList[token] = float(wordlist[token])/length
    return rtnList, wordFreqList

# 统计单词反文档频率


def docFre(word):
    fre = 0
    print("hdsjkfhsk")
    # for i in range(1, 26):
    for email in processedEmail:
        # if word in re.split(r'\W*', open('spam/%d.txt' % i).read()):
        if word in re.split(r'\W*', email):
            fre += 1
    return float(fre)/totalNum


# 特征词提取，这里面使用TF-IDF方法


def extractFeature(textType):
    docList = []
    classList = []
    fullText = []
    wordTFIDF = {}
    # 每个类测试邮件一共有25封
    if textType == "spam":
        # for i in range(1, 26):
        for email in processedEmail[hamNum:]:
            # wordlist, wordFreqList = splitText(
            #    open(textType+'/%d.txt' % i).read())
            wordlist, wordFreqList = splitText(email)
            print("hfdjkshfusk")
            fullText.append(wordlist)
            for word in wordFreqList:
                wordIDF = docFre(word)
                wordTFIDFValue = wordIDF*wordFreqList[word]
                if word not in wordTFIDF:
                    wordTFIDF[word] = wordTFIDFValue
                else:
                    wordTFIDF[word] += wordTFIDFValue
    elif textType == "ham":
        for email in processedEmail[:hamNum]:
            # wordlist, wordFreqList = splitText(
            #    open(textType+'/%d.txt' % i).read())
            wordlist, wordFreqList = splitText(email)
            fullText.append(wordlist)
            for word in wordFreqList:
                wordIDF = docFre(word)
                wordTFIDFValue = wordIDF*wordFreqList[word]
                if word not in wordTFIDF:
                    wordTFIDF[word] = wordTFIDFValue
                else:
                    wordTFIDF[word] += wordTFIDFValue
    else:
        print("error!")
    sortedWordTFIDF = sorted(wordTFIDF.items(),
                             key=lambda asd: asd[1], reverse=True)
   # 选取前100个词为分类词
    keywords = [word[0] for word in sortedWordTFIDF[:100]]
    return keywords

# 对一个邮件词集构建特征向量（使用0,1表示存在与否）


def extaxtDocFeatureVec(text, keyword):
    vec = []
    for i, word in enumerate(keyword):
        if word in text:
            vec.append(1)
        else:
            vec.append(0)
    return vec

# 抽取所有邮件的特征向量


def extactFeatureVec():
    hamWordsVec = extractFeature('ham')
    spamWordsVec = extractFeature('spam')
    wordVecs = []
    classList = []
    for email in processedEmail[:hamNum]:
        wordlistHam, wordFreqList = splitText(email)
        vecHam = extaxtDocFeatureVec(wordlistHam, hamWordsVec)
        wordVecs.append(vecHam)
        classList.append(1)
    for email in processedEmail[hamNum:]:
        wordlistSpam, wordFreqList = splitText(email)
        vecSpam = extaxtDocFeatureVec(wordlistSpam, spamWordsVec)
        wordVecs.append(vecSpam)
        classList.append(-1)
    '''
    for i in range(1, 26):
        wordlistHam, wordFreqList = splitText(open('ham/%d.txt' % i).read())
        wordlistSpam, wordFreqList = splitText(open('spam/%d.txt' % i).read())
        vecHam = extaxtDocFeatureVec(wordlistHam, hamWordsVec)
        vecSpam = extaxtDocFeatureVec(wordlistSpam, spamWordsVec)
        wordVecs.append(vecHam)
        classList.append(1)
        wordVecs.append(vecSpam)
        classList.append(-1)
    '''
# print wordVecs
# print classList

    return wordVecs, classList

# 使用SVM训练数据并使用交叉测试测试正确率


def textSpam(k1=1.3):
    dataArr, labelArr = extactFeatureVec()
    print("fhdjsjh")
    trainDataArr = dataArr[:int(0.8*hamNum)]+dataArr[hamNum:int(0.8*spamNum)]
    trainLabelArr = labelArr[:int(0.8*hamNum)] + \
        labelArr[hamNum:int(0.8*spamNum)]
    b, alphas = smoP(trainDataArr, trainLabelArr, 200, 0.0001,
                     10, ('rbf', k1))  # C=200 important
    datMat = mat(trainDataArr)
    labelMat = mat(trainLabelArr).transpose()
    svInd = nonzero(alphas.A > 0)[0]
    sVs = datMat[svInd]  # get matrix of only support vectors
    labelSV = labelMat[svInd]
    print("there are %d Support Vectors" % shape(sVs)[0])
    testDataMat = mat(dataArr[int(0.8*hamNum):hamNum] +
                      dataArr[int(0.8*spamNum):])
    testLabel = labelArr[int(0.8*hamNum):hamNum]+labelArr[int(0.8*spamNum):]
# testLabel[2]=testLabel[2]*-1
    m, n = shape(testDataMat)
    errorCount = 0
    for i in range(m):
        kernelEval = kernelTrans(sVs, testDataMat[i, :], ('rbf', k1))
        predict = kernelEval.T * multiply(labelSV, alphas[svInd]) + b
        if sign(predict) != sign(testLabel[i]):
            errorCount += 1
    print("the training error rate is: %f" % (float(errorCount)/m))


if __name__ == '__main__':
    textSpam()
