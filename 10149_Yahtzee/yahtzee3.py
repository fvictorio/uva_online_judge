def dp (M, category, used_subsets, puntaje_bonus):
    if M[category].has_key(used_subsets):
        return M[category][used_subsets]

    for i in range(N):
        if used_subsets & (1 << i): continue # usado
        M[category][used_subsets | (1 << i)] =




N = 13

M = [{} for i in range(N)]

file = open('input.txt', 'r')
lines = file.readlines()
file.close()
tiradas = []
for i in range(N):
    line = lines[i]
    tiradas.append([int(num) for num in line.split(' ')])

for i in range(N):
    puntaje_tirada = eval_tirada(tiradas[i], 0)
    M[0][i] = puntaje_tirada + dp(M, 1, 1 << i, puntaje_tirada)
