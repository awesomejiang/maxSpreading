from snap import *

G = LoadEdgeList(PNEANet, "/home/awesomejiang/Desktop/work/dataset/p2p-Gnutella09.txt", 0, 1)

f = open('../PG9.dat','w')
#output Nid,outdeg, indeg, the sum outdeg/indeg of its neibrs  
for NI in G.Nodes():
	outd = NI.GetOutDeg()
	ind = NI.GetInDeg()
	cc = GetNodeClustCf(G,NI.GetId())
	nr_outd = 0
	nr_ind = 0
	for i in range(outd):
		outN = NI.GetOutNId(i)
		nr_outd += G.GetNI(outN).GetOutDeg()
		nr_ind += G.GetNI(outN).GetInDeg()
	f.write(str(NI.GetId())+'	'+str(outd)+'	'+str(ind)+'	'+str(cc)+'	'+str(nr_outd)+'	'+str(nr_ind)+'\n')

f.close()
