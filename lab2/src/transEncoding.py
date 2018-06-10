import os
import sys
import codecs
import chardet
#from subFunc_tools import *


def convert(file, in_enc="GBK", out_enc="UTF-8"):
    in_enc = in_enc.upper()
    out_enc = out_enc.upper()
    if(in_enc == 'UTF-8'):
        return
    try:
        f = codecs.open(file, 'r', in_enc)
        new_content = f.read()
        codecs.open(file, 'w', out_enc).write(new_content)
    except Exception as e:
        print(file)
        print("Fail", e)


if __name__ == "__main__":
    path = r'./easy_ham/'
    list_files = os.listdir(path)
    for fileName in list_files:
        filePath = path + fileName
        with open(filePath, "rb") as f:
            data = f.read()
            codeType = chardet.detect(data)['encoding']
            convert(filePath, codeType, 'UTF-8')
    path = r'./spam/'
    list_files = os.listdir(path)
    for fileName in list_files:
        filePath = path + fileName
        with open(filePath, "rb") as f:
            data = f.read()
            codeType = chardet.detect(data)['encoding']
            convert(filePath, codeType, 'UTF-8')
