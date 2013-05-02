from hungarian import hungarian
from random import shuffle
from copy import copy

N = 13

def three_of_a_kind(tirada):
    return sum(tirada) if any(tirada.count(i) >= 3 for i in tirada) else 0
def four_of_a_kind(tirada):
    return sum(tirada) if any(tirada.count(i) >= 4 for i in tirada) else 0
def five_of_a_kind(tirada):
    return 50 if any(tirada.count(i) >= 5 for i in tirada) else 0
def short_straight(tirada):
    sorted_tirada = sorted(tirada)
    if [1,2,3,4] in (tirada[0:4], tirada[1:5]) or [2,3,4,5] in (tirada[0:4], tirada[1:5]) or\
       [3,4,5,6] in (tirada[0:4], tirada[1:5]): 
        return 25
    else:
        return 0
def long_straight(tirada):
    sorted_tirada = sorted(tirada)
    if tirada == [1,2,3,4,5] or tirada == [2,3,4,5,6]:
        return 35
    else:
        return 0
def full_house(tirada):
    return 40 if any(tirada.count(i) == 3 for i in tirada) and any(tirada.count(i) == 2 for i in tirada) else 0

def eval_tirada(tirada, category):
    if category == 1:
        return sum(t for t in tirada if t == 1)
    elif category == 2:
        return sum(t for t in tirada if t == 2)
    elif category == 3:
        return sum(t for t in tirada if t == 3)
    elif category == 4:
        return sum(t for t in tirada if t == 4)
    elif category == 5:
        return sum(t for t in tirada if t == 5)
    elif category == 6:
        return sum(t for t in tirada if t == 6)
    elif category == 7:
        return sum(tirada)
    elif category == 8:
        return three_of_a_kind(tirada)
    elif category == 9:
        return four_of_a_kind(tirada)
    elif category == 10:
        return five_of_a_kind(tirada)
    elif category == 11:
        return short_straight(tirada)
    elif category == 12:
        return long_straight(tirada)
    elif category == 13:
        return full_house(tirada)

def eval_score(tiradas, assignation):
    global N
    scores = [0 for i in range(N+1)] 
    for i in range(N):
        scores[i] = eval_tirada(tiradas[assignation[i]], i+1)
    scores[13] = 35 if sum(scores[j] for j in range(6)) >= 63 else 0
    return scores, sum(scores)

def eval_score_with_swap(tiradas, assignation, swap):
    new_assignation = copy(assignation)
    make_swap(new_assignation, swap)
    ts, s = eval_score(tiradas, new_assignation)
    return s

def make_swap (assignation, swap):
    a, b = swap
    assignation[a], assignation[b] = assignation[b], assignation[a]


file = open('input.txt', 'r')
lines = file.readlines()
file.close()

possible_swaps = []
for i in range(N-1):
    for j in range(i+1, N):
        possible_swaps.append( (i,j))

for caso in range(len(lines) / N):
    tiradas = []
    for i in range(N):
        line = lines[caso*N + i]
        tiradas.append([int(num) for num in line.split(' ')])
    best_score_overall = -1
    best_assignation_overall = []
    for i in range(30):
        assignation = range(N)
        shuffle(assignation)
        while True:
            hubo_swap = False
            table_scores, current_score = eval_score(tiradas, assignation)
            best_swap = possible_swaps[0]
            best_swap_score = eval_score_with_swap(tiradas, assignation, best_swap)
            for swap in possible_swaps[1:]:
                swap_score = eval_score_with_swap(tiradas, assignation, swap)
                if swap_score > current_score and swap_score > best_swap_score:
                    best_swap_score = swap_score
                    best_swap = swap
                    hubo_swap = True
            if hubo_swap:
                make_swap(assignation, best_swap)
            else:
                #print(assignation)
                #print(table_scores)
                print(current_score)
                if current_score > best_score_overall:
                    best_score_overall = current_score
                    best_assignation_overall = assignation
                    best_table_scores = table_scores
                break
