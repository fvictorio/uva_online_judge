

def generate(n):
    if n <= 0:
        return ['']
    if n == 1:
        return ['()']

    expressions = []
    for k in range(n):
        for expr_l in generate(k):
            for expr_r in generate(n-k-1):
                expression = '(' + expr_l + ')' + expr_r
                expressions.append(expression)

    return expressions

def depth(expr):
    count = 0
    depth = 0
    for par in expr:
        if par == '(': count += 1
        if par == ')': count -= 1
        depth = max(depth, count)
    return depth

def c(n,d):
    return sum(1 for i in generate(n) if depth(i) <= d)

def fact(n):
    if n <= 0: return 1
    return n * fact(n-1)

def comb(n, k):
    return fact(n)/(fact(k)*fact(n-k));

def catalan(n, d=-1):
    if d == -1: d = n

    return comb(n+n,n)/(n+1)*(n-d+1)

def norm(a, b):
    accum = 0
    for (i,j) in zip(a,b):
        accum += abs(i-j)
    return accum

def correct(n):
    return [c(n,d) for d in range(1,n+1)]
