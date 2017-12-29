from snap import *

G = LoadEdgeList(PNGraph, "/home/awesomejiang/Desktop/CA-GrQc.txt", 0, 1)


d = {}
for NI in G.Nodes():
	d[str( NI.GetId() )] = 0



f1 = open('../CA_pts/CA_n1000_k2.dat','r')

for line in f1:
	l = line.split('\t')[:-1]

	for e in l:
		d[e] += 1

f1.close()

f2 = open('fre_pts.dat','w')
for k in d:
	f2.write(str(k)+'\t'+str(d[k])+'\n')

f2.close()

