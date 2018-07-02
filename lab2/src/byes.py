import pickle
import numpy as np
import math
#import nltk

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

for item in processedEmail[:hamNum]:
    tmp = item.split()
    for word in tmp:
        wordDictHam[word] = wordDictHam.get(word, 0)+1
for item in processedEmail[hamNum:]:
    tmp = item.split()
    for word in tmp:
        wordDictSpam[word] = wordDictSpam.get(word, 0)+1

for word in wordDictHam:
    if wordDictHam[word] > 50:
        wordSet.add(word)
for word in wordDictSpam:
    if wordDictSpam[word] > 50:
        wordSet.add(word)

count = 0
index = 0
for email in processedEmail:
    Pham = 1.0
    Pspam = 1.0
    tmp1 = 1.0
    tmp2 = 1.0
    tmp = email.split()
    tmpwordSet = set()
    for item in tmp:
        if item in wordSet:
            tmpwordSet.add(item)
    for word in tmpwordSet:
        if word in wordDictHam:
            #tmp1 += math.log(float(wordDictHam[word])/hamNum)
            tmp1 *= float(wordDictHam[word])/hamNum
            #tmp1 *= 1.0/hamNum
        else:
            #tmp1 += math.log(1.0/hamNum)
            tmp1 *= 0.1/hamNum
        if word in wordDictSpam:
            #tmp2 += math.log(float(wordDictSpam[word])/spamNum)
            tmp2 *= float(wordDictSpam[word])/spamNum
            #tmp2 *= 1.0/spamNum
        else:
            #tmp2 += math.log(1.0/spamNum)
            tmp2 *= 0.1/spamNum
        tmp1 += 0.000000000000001
        tmp2 += 0.000000000000001
    Pham = tmp1*hamNum/(tmp1*hamNum+tmp2*spamNum)
    Pspam = tmp2*spamNum/(tmp1*hamNum+tmp2*spamNum)
    if Pham < Pspam:
        pred = 1
    else:
        pred = 0
    if pred == EmailTagList[index]:
        count += 1
    index += 1
print('The classify accuracy is: %.2f%%' % (float(count)/totalNum * 100))
