from itertools import permutations

xs = [2, 3, 5, 7, 9]

for x in permutations(xs):
     if (x[0] + x[1] * (x[2]**2) + (x[3]**3) - x[4] == 399):
             print(x)
