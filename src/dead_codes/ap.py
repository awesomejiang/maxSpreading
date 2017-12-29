from snap import *
import random
import time
import sys


#time
start = time.clock()

def get_wccs_G():
	ComponentDist = TIntPrV()
	GetWccSzCnt(G, ComponentDist)
	
	tmp = 0
	for comp in ComponentDist:
   		tmp += comp.GetVal2()
   	
   	return tmp


def get_wccs_num(n):
	T = ConvertGraph(PUNGraph, G)
	T.DelNode(n)
	ComponentDist = TIntPrV()
	GetWccSzCnt(T, ComponentDist)
	
	tmp = 0
	for comp in ComponentDist:
   		tmp += comp.GetVal2()

   	return tmp


#load data of graph
oG = LoadEdgeList(PNGraph, "/home/awesomejiang/Desktop/work/dataset/CA-HepTh.txt", 0, 1)
G = ConvertGraph(PUNGraph, oG)

#uG = LoadEdgeList(PUNGraph, "/home/awesomejiang/Desktop/soc-Epinions1.txt", 0, 1)
#convert it to an directed G
#G = ConvertGraph(PNGraph, uG)

#benchmark
#G = GenRndGnm(PNGraph, 20, 200)

f = open('output.dat','w')

wccs = get_wccs_G()

ArtNIdV = TIntV()
GetArtPoints(G, ArtNIdV)

print 'wccs number is:'+str(ArtNIdV.Len())

counter = 0
for NI in G.Nodes():
	n = NI.GetId()
	num = get_wccs_num(n)
	if num != wccs:
		counter += 1
		f.write(str(n)+'\t'+str(num-wccs)+'\n')
print counter

f.close()

#time
stop = time.clock()
print "Running time:", stop-start








