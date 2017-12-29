from snap import *
import random
import time
import sys
import os

sys.setrecursionlimit(50000)
#time
start = time.clock()


filename = 'und_large'

#load data of graph
print 'Loading graph info...'
G = LoadEdgeList(PUNGraph, '/home/awesomejiang/Desktop/work/dataset_ap/'+filename+'.txt', 0, 1)
#check direct or undirect

Ectr = 0
for EI in G.Edges():
	src = EI.GetSrcNId()
	dst = EI.GetDstNId()
	Ectr += G.IsEdge(dst,src)

if Ectr == 0:
	for EI in G.Edges():
		G.AddEdge(EI.GetDstNId(), EI.GetSrcNId())

print 'Load sucessfully!'
	


#variables
stack = []
visit = {}
low = {}
counter = [1]
result = {}

if not os.path.exists('/home/awesomejiang/Desktop/work/dataset_ap/'+filename):
	os.mkdir('/home/awesomejiang/Desktop/work/dataset_ap/'+filename)
f = open('/home/awesomejiang/Desktop/work/dataset_ap/'+filename+'/ap.dat','w')

#initialize
for NI in G.Nodes():
	visit[NI.GetId()] = 0
	low[NI.GetId()] = 0
	result[NI.GetId()] = 0

#define functions
def find_min(v,tmp_v):
	min_l = visit[tmp_v]
	outdeg = G.GetNI(tmp_v).GetOutDeg()
	for n in range(outdeg):
		sub_v = G.GetNI(tmp_v).GetNbrNId(n)
		if sub_v != v :
			if low[sub_v] < min_l:
				min_l = low[sub_v]
			if visit[sub_v] < min_l:
				min_l = visit[sub_v]
	return min_l

def dfs_iter(v):
	outdeg = G.GetNI(v).GetOutDeg()

	times = 0
	for n in range(outdeg):
		tmp_v = G.GetNI(v).GetNbrNId(n)
		if visit[tmp_v] == 0:
			times =  1
			stack.append(tmp_v)
			counter[0] += 1
			visit[tmp_v] = counter[0]
			low[tmp_v] = counter[0]
			dfs_iter(tmp_v)
			low[tmp_v] = find_min(v,tmp_v)

			if low[tmp_v] >= visit[v]:
				result[v] += 1
	
	if times == 0:				
		stack.pop()		



def dfs(v):
	stack.append(v)
	visit[v] = counter[0]
	low[v] = counter[0]
	dfs_iter(v)
	

#main function
print 'Running dfs-like algo...'
for NI in G.Nodes():
	v = NI.GetId()
	if visit[v] == 0:
		dfs(v)
print 'Algo finished!'

#output
for NI in G.Nodes():
	NId = NI.GetId()
	num_NId = result[NId]
	if num_NId != 0:
		f.write(str(NId)+'\t'+str(num_NId)+'\n')

f.close()

#time
stop = time.clock()
print "Total running time:", stop-start








