from snap import *

G = LoadEdgeList(PUNGraph, "/home/awesomejiang/Desktop/work/dataset/CA-GrQc.txt", 0, 1)

#f = open('art_pts.dat','w')


CmtyV = TCnComV()
modularity = CommunityGirvanNewman(G, CmtyV)
for Cmty in CmtyV:
	print "Community: "
	for NI in Cmty:
		print NI
print "The modularity of the network is %f" % modularity
#f.close()
