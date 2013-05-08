from random import choice, randrange, sample
from string import ascii_lowercase as letters

def random_word(n):
    return ''.join(choice(letters) for i in range(n))

def random_doublet(s):
    dif_letter_idx = randrange(len(s))
    while True:
        new_letter = choice(letters)
        if new_letter != s[dif_letter_idx]: break
    return s[:dif_letter_idx] + new_letter + s[dif_letter_idx+1:]

def generate_doublets_set(word_length, set_size, root_word = ""):
    if root_word == "":
        root_word = random_word(word_length)
    else:
        if word_length != len(root_word):
            word_length = len(root_word)
    #no puedo generar mas palabras distintas que 26^n
    set_size = min(set_size, 26**word_length)
    doublets_set = set([root_word])

    while (len(doublets_set) < set_size):
        doublets_set.add(random_doublet(sample(doublets_set,1)[0]))

    return doublets_set

def generate_case(filename, dict_length, word_length):
    s = generate_doublets_set(word_length, dict_length)

    f = open(filename, 'w')
    
    for word in s:
        f.write("%s\n" % word)

    f.close()

generate_case("input.txt", 2000, 6)
