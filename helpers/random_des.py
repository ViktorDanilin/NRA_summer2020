import random
n = int(input())
x=0
y=0
for i in range(n):
    x = random.uniform(0,90)
    y = random.uniform(0,180)
    x = round(x,4)
    y = round(y,4)
    print("{",x," , ",y,"},",sep="")