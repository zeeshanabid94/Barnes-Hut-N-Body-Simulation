import sys
import random

f = open("input.txt", 'w')

f.write("# input format\n")
f.write("# mass x y vx vy\n")

for i in range(0, 1000):
    mass = str(random.uniform(1, 10000000000000000))
    x = str(random.uniform(1, 10000))
    y = str(random.uniform(1, 10000))
    vx = str(random.uniform(1, 10))
    vy = str(random.uniform(1, 10))
    f.write(mass)
    f.write(' ')
    f.write(x)
    f.write(' ')
    f.write(y)
    f.write(' ')
    f.write(vx)
    f.write(' ')
    f.write(vy)
    f.write('\n')
