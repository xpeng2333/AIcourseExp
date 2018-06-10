import scipy.io
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd
import numpy as np
import pickle
import os
import re
from nltk.stem.snowball import SnowballStemmer
from bs4 import BeautifulSoup
import nltk
from nltk.corpus import stopwords
nltk.download("stopwords")
# import snips as snp  # my snippets
# snp.prettyplot(matplotlib)  # my aesthetic preferences for plotting
from os import listdir
from os.path import isfile, join

spampath = join(os.getcwd(), "spam")
spamfiles = [join(spampath, fname) for fname in listdir(spampath)]

hampath = join(os.getcwd(), "easy_ham")
hamfiles = [join(hampath, fname) for fname in listdir(hampath)]


def get_body(fpath):
    with open(fpath, "r") as myfile:
        try:
            lines = myfile.readlines()
            idx = lines.index("\n")  # only grabs first instance
            return "".join(lines[idx:])
        except:
            print("Couldn't decode file %s" % (fpath,))


def word_salad(body):
    '''Produce a word salad from email body.'''
    # Parse HTML extract content only (but count tags)
    soup = BeautifulSoup(body, 'html.parser')

    # Pull out only the non-markup tex
    body = soup.get_text()

    # Count the number of HTML elements and specific link elements
    nhtml = len(soup.find_all())
    nlinks = len(soup.find_all("a"))
    # Sub in special strings for "counting"
    body = body + nhtml*" htmltag " + nlinks*" linktag "

    # lowercase everything
    body = body.lower()

    # Replace all URLs with special strings
    regx = re.compile(r"(http|https)://[^\s]*")
    body, nhttps = regx.subn(repl=" httpaddr ", string=body)

    # Replace all email addresses with special strings
    regx = re.compile(r"\b[^\s]+@[^\s]+[.][^\s]+\b")
    body, nemails = regx.subn(repl=" emailaddr ", string=body)

    # Replace all numbers with special strings
    regx = re.compile(r"\b[\d.]+\b")
    body = regx.sub(repl=" number ", string=body)

    # Replace all $, ! and ? with special strings
    regx = re.compile(r"[$]")
    body = regx.sub(repl=" dollar ", string=body)
    regx = re.compile(r"[!]")
    body = regx.sub(repl=" exclammark ", string=body)
    regx = re.compile(r"[?]")
    body = regx.sub(repl=" questmark ", string=body)

    # Remove all other punctuation (replace with white space)
    regx = re.compile(r"([^\w\s]+)|([_-]+)")
    body = regx.sub(repl=" ", string=body)

    # Replace all newlines and blanklines with special strings
    regx = re.compile(r"\n")
    body = regx.sub(repl=" newline ", string=body)
    regx = re.compile(r"\n\n")
    body = regx.sub(repl=" blankline ", string=body)

    # Make all white space a single space
    regx = re.compile(r"\s+")
    body = regx.sub(repl=" ", string=body)

    # Remove any trailing or leading white space
    body = body.strip(" ")

    # Remove all useless stopwords
    bodywords = body.split(" ")
    keepwords = [
        word for word in bodywords if word not in stopwords.words('english')]

    # Stem all words
    stemmer = SnowballStemmer("english")
    stemwords = [stemmer.stem(wd) for wd in keepwords]
    body = " ".join(stemwords)

    return body


# Reserve in memory, faster than append
emails_raw = ["email"]*len(hamfiles + spamfiles)
# Reserve in memory, faster than append
emails_processed = ["email"]*len(hamfiles + spamfiles)
y = [0]*len(hamfiles) + [1]*len(spamfiles)  # Ground truth vector

for idx, fpath in enumerate(hamfiles + spamfiles):
    body = get_body(fpath)  # Extract only the email body text
    emails_raw[idx] = body
    processed = word_salad(body)  # All preprocessing
    emails_processed[idx] = processed

# Pickle these objects for easier access later
with open("easyham_and_spam_corpus_raw_and_processed_and_y.pickle", "wb") as myfile:
    pickle.dump([emails_raw, emails_processed, y], myfile)
