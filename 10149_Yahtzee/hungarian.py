from copy import copy, deepcopy
from pprint import pprint

def minimum_uncovered_element (M, marked_rows, marked_cols):
    n = len(M)
    initialized = False

    for i in range(n):
        if i in marked_rows: continue
        for j in range(n):
            if j in marked_cols: continue
            if not initialized:
                minn = M[i][j]
                initialized = True
            minn = min(minn, M[i][j])

    return minn

def zeros_below (M, i, j):
    cant = 0
    for k in range(i+1, len(M)):
        if M[k][j] == 0:
            cant += 1
    return cant

def assign_zero_in_row(M, i, cols_assigned):
    n = len(M)
    best_zero = -1
    best_zb = n
    for j in range(n):
        if j in cols_assigned: continue
        if M[i][j] == 0:
            zb = zeros_below(M, i, j)
            if zb < best_zb:
                best_zero = j
                best_zb = zb
    return best_zero
        

def cover_zeros(M):
    n = len(M)

    assigned = set([])
    cols_assigned = set([])
    rows_assigned = set([])
    rows_unassigned = set(range(n))

    for i in range(n):
        zero_to_assign = assign_zero_in_row(M, i, cols_assigned)
        if zero_to_assign != -1:
            assigned.add( (i, zero_to_assign) )
            cols_assigned.add(zero_to_assign)
            rows_assigned.add(i)
            rows_unassigned.remove(i)

    marked_rows = copy(rows_unassigned)    
    marked_cols = set([])

    while True:
        something_marked = False

        for mr in marked_rows:
            for j, elem_mr in enumerate(M[mr]):
                if elem_mr == 0 and not (j in marked_cols):
                    marked_cols.add(j)
                    something_marked = True

        for mc in marked_cols:
            for i in range(n):
                if (i, mc) in assigned and not (i in marked_rows):
                    marked_rows.add(i)
                    something_marked = True

        if not something_marked: break

    crossed_rows = set([i for i in range(n) if not (i in marked_rows)])
    crossed_cols = copy(marked_cols)

    return (crossed_rows, crossed_cols, assigned)


def minimum (v):
    minn = v[0]
    for i in range(1, len(v)):
        minn = min(minn, v[i])
    return minn

def minimum_column (M, j):
    minn = M[0][j]
    for i in range(1, len(M)):
        minn = min(minn, M[i][j])
    return minn

def hungarian (matrix):
    M = deepcopy(matrix)
    n = len(M)
    
    # 1) Substract the minimum of each row from that row
    
    for i in range(n):
        mr = minimum(M[i])
        for j in range(n):
            M[i][j] -= mr

    # 2) Idem for each column

    for j in range(n):
        mc = minimum_column(M, j)
        for i in range(n):
            M[i][j] -= mc

    while True:

        # 3) Cover the zeros with the minimum lines
        crossed_rows, crossed_cols, assigned = cover_zeros(M)

        if len(crossed_rows) + len(crossed_cols) == n:
            return assigned

        mue = minimum_uncovered_element(M, crossed_rows, crossed_cols)
        for i in crossed_rows:
            for j in range(n):
                M[i][j] += mue
        for j in crossed_cols:
            for i in range(n):
                M[i][j] += mue
        min_matrix = min(min(row) for row in M)
        for i in range(n):
            for j in range(n):
                M[i][j] -= min_matrix
