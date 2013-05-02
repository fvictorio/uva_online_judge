from hungarian import hungarian

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

file = open('input.txt', 'r')
lines = file.readlines()
file.close()

for caso in range(len(lines) / 13):
    tiradas = []
    for i in range(13):
        line = lines[caso*13 + i]
        tiradas.append([int(num) for num in line.split(' ')])
    M = [[0 for col in range(14)] for row in range(14)]
    for i, tirada in enumerate(tiradas):
        M[i][0] = sum(t for t in tirada if t == 1)
        M[i][1] = sum(t for t in tirada if t == 2)
        M[i][2] = sum(t for t in tirada if t == 3)
        M[i][3] = sum(t for t in tirada if t == 4)
        M[i][4] = sum(t for t in tirada if t == 5)
        M[i][5] = sum(t for t in tirada if t == 6)
        M[i][6] = sum(tirada)
        M[i][7] = three_of_a_kind(tirada)
        M[i][8] = four_of_a_kind(tirada)
        M[i][9] = five_of_a_kind(tirada)
        M[i][10] = short_straight(tirada)
        M[i][11] = long_straight(tirada)
        M[i][12] = full_house(tirada)
        M[i][13] = 35 if sum(M[i][j] for j in range(6)) >= 63 else 0
    max_matrix = max(max(row) for row in M)
    M_hungared = [[max_matrix-elem for elem in row] for row in M]
    assigned = hungarian(M_hungared)
    result = []
    for r,c in sorted(assigned):
        result.append(M[r][c])
    result = result[:13]
    result.append(sum(result))
