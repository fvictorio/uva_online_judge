

def sol(curr_depth, max_depth, length):

    # todos los que siguen son )
    if curr_depth == length:
        return 1
    
    # si o si tengo que cerrar )
    if curr_depth == max_depth:
        return sol(curr_depth-1, max_depth, length-1)

    # si o si tengo que abrir (
    if curr_depth == 0:
        return sol(1, max_depth, length-1)

    # puedo abrir o cerrar
    return sol(curr_depth-1,max_depth,length-1) + sol(curr_depth+1,max_depth,length-1)

def f(n, d):
    return sol(0,d,n) - sol(0,d-1,n)
