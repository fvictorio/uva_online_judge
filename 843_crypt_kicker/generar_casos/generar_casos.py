from copy import copy
from random import randrange, choice, shuffle, sample
from string import ascii_lowercase

MAX_DICTIONARY_LENGTH = 1000
MAX_WORD_LENGTH = 16
MAX_LINE_LENGTH = 80
MAX_NO_CASES = 50

def encrypt(s):
    enc_s = ""
    letters = list(ascii_lowercase)
    shuffle(letters)
    letters = ''.join(letters)

    for ch in s:
        if ch == ' ':
            enc_s += ' '
        else:
            idx = ascii_lowercase.index(ch)
            enc_s += letters[idx]

    return enc_s


if True:
    dictionary = set([])
    dictionary_length = randrange(1, MAX_DICTIONARY_LENGTH+1)

    for i in range(dictionary_length):
        while True:
            word_length = randrange(1, MAX_WORD_LENGTH+1)
            word = ''.join(choice(ascii_lowercase) for j in range(word_length))
            if not (word in dictionary):
                dictionary.add(word)
                break

    cases = []
    no_cases = randrange(1, MAX_NO_CASES+1)
    for i in range(no_cases):
        line = ""
        while True:
            word = sample(dictionary,1)[0]
            if (len(line) + len(word)) > MAX_LINE_LENGTH: break

            line += word
            line += ' '
        cases.append(line)

    encrypted_cases = [encrypt(case) for case in cases]

    if True:
        filename_suffix = randrange(1,10000)
        print(filename_suffix)
        f = open("input%i.txt" % filename_suffix, 'w')
        f.write("%d\n" % dictionary_length)
        for word in dictionary:
            f.write("%s\n" % word)
        for case in encrypted_cases:
            f.write("%s\n" % case)
        f.close()

        f = open("output%i.txt" % filename_suffix, 'w')
        for case in cases:
            f.write("%s\n" % case)
        f.close()
