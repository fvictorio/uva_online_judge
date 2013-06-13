from random import randrange as rr

def get_all_subsets(s, n):
    if n == 0:
        return [[]]
    subsets = []
    for i in range(len(s) - n + 1):
        for ss in get_all_subsets(s[i+1:], n-1):
            subsets.append([s[i]] + ss)
    return subsets

def solve(v):
    n = len(v)/2
    sum_v = sum(v)
    d = {}
    for (i,k) in [(abs(sum_v - 2*sum(s)), sum(s)) for s in get_all_subsets(v,n)]:
        d.update({i:k})
    return d[min(d)]

def generate_case(n):
    v = [rr(1,451) for i in range(n)]
    sol = solve(v)
    return(v,sol)

def generate_cases(T=10, n=10, input_file="input.txt", output_file="output.txt"):
    input_handle = open(input_file, 'w')
    output_handle = open(output_file, 'w')

    input_handle.write("%i\n\n" % T)

    for i in range(T):
        (v,s) = generate_case(n)
        a = s
        b = sum(v) - a
        if (a > b): a,b=b,a
        input_handle.write("%i\n" % n)
        input_handle.writelines(str(k)+"\n" for k in v)
        input_handle.write("\n")
        output_handle.write("%i %i\n" % (a,b))
        if (i + 1 < T): output_handle.write("\n")

    input_handle.close()
    output_handle.close()

generate_cases(n=15)
