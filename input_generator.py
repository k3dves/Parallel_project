from random import randint
import numpy as np
order=int(raw_input("number : "))-1
coff=np.random.random_integers(1, 1, (order, 3))
x=[0]
for i in range(order):
	x.append(randint(10,100))
x.append(0)
print(x)
print('\n\n\n')
answer=[]
k=0
for i in coff:
	s=0
	l=0
	for j in range(k,k+3):
		s=s+(i[l]*x[j])
		#print(l)
		l+=1	
	k+=1
	answer.append(s)	
	#print(i)

file=open('input.txt','w')
file.write(str(order))
file.write('\n')

for s in range(order):
	print>>file, coff[s]
	print>>file, answer[s]
	
