n=4038# # of points in G

f1 = open('points1_2.dat','r')

d = {}
for i in range(n+1):
	d[str(i)] = 0
		
for line in f1:
	l = line.split('\t')[1:2]

	for e in l:
		d[e] += 1

f1.close()

f2 = open('fre_pts_first.dat','w')
for k in d:
	f2.write(str(k)+'\t'+str(d[k])+'\n')
	
f2.close()
	
