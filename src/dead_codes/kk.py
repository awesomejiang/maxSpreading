from snap import *
import random
import time
import sys


#time
start = time.clock()

#denote attr as 'float'
attr = 'float'

#set attrN as G's
def set_attrN(A,G):
	for NI in G.Nodes():
		node = NI.GetId()
		tmp = G.GetFltAttrDatN(node, attr)
		A.AddFltAttrDatN(NI, tmp, attr)



#set attrE as G's
def set_attrE(A, G):

	for EI in G.Edges():
		edge = EI.GetId()
		tmp = G.GetFltAttrDatE(edge, attr)
		A.AddFltAttrDatE(edge, tmp, attr);


#set attr as G's
def set_attr(A, G):
	
	set_attrN(A, G)
	
	set_attrE(A, G)
	
#generate a duplication of net(cuz TNEANet's stupid construction function wont copy the attr data T T)	
def copy_net(A, G):

	A.AddFltAttrE(attr)
	A.AddFltAttrN(attr)
	
	set_attr(A, G)


#claculate number of activated points
def acd_pts(S):

	counter = 0
	for NI in S.Nodes():
		counter += S.GetFltAttrDatN(NI, attr)

	return counter



#set node threshold 
#and modify edge weight by threshold s.t. if node value >1, the node is deemed activated
def set_net(G):
	
	#threshold(threshold must >0)
	G.AddFltAttrN(attr, 0.5)
	
	#modify weights s.t. if N's threshold >=1, N is activated.
	G.AddFltAttrE(attr, 0)
	for NI in G.Nodes():
		dstN = NI.GetId()
		indeg = NI.GetInDeg()
		for i in range(indeg):
			srcN = NI.GetInNId(i)
			EI = G.GetEI(srcN, dstN)
			G.AddFltAttrDatE(EI, 1/(indeg*G.GetFltAttrDatN(dstN, attr)), attr)
			
	#Clean nodes' thresholds
	for NI in G.Nodes():
		G.AddFltAttrDatN(NI, 0, attr)


#independent spread: classify the points to A/B/A+B/empty points
def modify_nodes(S):

	for NI in S.Nodes():
		n = NI.GetId()
		v = S.GetFltAttrDatN(n, attr)
	
		if v >= 1:
			S.AddFltAttrDatN(n, 1, attr)
		else:
			S.AddFltAttrDatN(n, 0, attr)


#one tech spreads from initial region independently
def spread_ind(S):


	q = []
	for NI in S.Nodes():
		node = NI.GetId()
		if 1 == S.GetFltAttrDatN(node, attr):
			q.append(node)



	while len(q):
		srcN = q[0]
		NI = S.GetNI(srcN)
		del q[0]
		outdeg = NI.GetOutDeg()
		for i in range(outdeg):
			dstN = NI.GetOutNId(i)
			init_v = S.GetFltAttrDatN(dstN, attr)
			if init_v < 1:
				EI = S.GetEI(srcN, dstN)
				final_value = init_v+S.GetFltAttrDatE(EI, attr)
				S.AddFltAttrDatN(dstN, final_value, attr)
				if final_value >= 1:
					q.append(dstN)
					
	modify_nodes(S)


"""
#expand function
def expand(G, chsn_pts):

	queue = chsn_pts[:]
	acd = []

	while queue :
		temp = queue[0]
		queue.pop(0)
		if temp not in acd:
			acd.append(temp)
		
		tempNI = G.GetNI(temp)
		outdeg = tempNI.GetOutDeg()
		for i in range( outdeg ):
			outN = tempNI.GetOutNId(i)
		
			#set activating probability here
			if outN not in acd and random.random() <= 0.5/G.GetNI(outN).GetInDeg():
				queue.append(outN)

	return acd


#traverse the graph to get optimal points, return new acd region
def chs_pt(G, chsn_pts):
	maxreg = len(chsn_pts)
	new_pt = -1

	for NI in G.Nodes():
		tmp_pts= chsn_pts+[NI.GetId()]
		out = expand(G, tmp_pts)
		reg = len(out)
		if reg > maxreg:
			maxreg = reg
			new_pt = NI.GetId()

	if new_pt != -1 :
		return new_pt
	else:
		print "error: no new region is acdtivated."
		return None

#main prog: add k points to get a biggist acd region

chsn_pts = []
for k in range(2):
	chsn_pts.append( chs_pt(G,chsn_pts) )
	print chsn_pts


print "activated region is: ", len( expand(G, chsn_pts) )
"""



def kk(G, k):

	A = ConvertGraph(PNEANet, G)
	copy_net(A,G)

	max_reg = -1
	node = -1
	pts = []
	for i in range(k):
		for NI in A.Nodes():
			for item in pts:
				A.AddFltAttrDatN(item, 1, attr)
			if 0 == A.GetFltAttrDatN(NI, attr):
				tmp_node = NI.GetId()
				A.AddFltAttrDatN(tmp_node, 1, attr)
				spread_ind(A)
				tmp_reg = acd_pts(A)
				print tmp_reg
				if tmp_reg > max_reg:
					max_reg = tmp_reg
					node = tmp_node

			set_attrN(A, G)

		pts.append(node)
		print node+'\t'+max_reg


	print 'greedy max acd pts:'+max_reg


#load data of graph
#G = LoadEdgeList(PNGraph, "/home/awesomejiang/Desktop/soc-Epinions1.txt", 0, 1)
oG = LoadEdgeList(PNGraph, "/home/awesomejiang/Desktop/CA-GrQc.txt", 0, 1)
G = ConvertGraph(PNEANet, oG)

#uG = LoadEdgeList(PUNGraph, "/home/awesomejiang/Desktop/soc-Epinions1.txt", 0, 1)
#convert it to an directed G
#G = ConvertGraph(PNGraph, uG)

#benchmark
#G = GenRndGnm(PNGraph, 20, 200)

set_net(G)

kk(G, 1)

#time
stop = time.clock()
print "Running time:", stop-start








