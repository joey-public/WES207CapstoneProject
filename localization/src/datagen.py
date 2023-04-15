import numpy as np

def gen_source_loc()->np.array:
    pass

def gen_toas(N:int, x:np.array, s:np.array)->np.array:
    toas  = np.zeros(N)
    for i in range(N):
        r_loc = x[i,:]
        toas[i] = np.linalg.norm(r_loc-s)
    return toas / 3e8

N=4
x = np.array([[0, 0, 0], 
              [50, 0, 0],
              [0, 50, 0],
              [50, 50, 0]], )

s = np.array([10,10,-10])

print(gen_toas(N,x,s))
