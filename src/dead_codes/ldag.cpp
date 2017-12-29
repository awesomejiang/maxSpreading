#include "Snap.h"
//to avoid conflict, undef...
#undef max
#undef min 
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;



int main(){
	int start = clock();
	void set_Eweight(PNEANet&);
	vector<float> max_inf(PNEANet, PNEANet, int);
	PNEANet ldag(PNEANet, int, float);

	//load graph, change it to pneanet and set attr
	PNGraph oG = TSnap::LoadEdgeList<PNGraph>("/home/awesomejiang/Desktop/CA-GrQc.txt", 0, 1);
	PNEANet G = TSnap::ConvertGraph<PNEANet>(oG);
	set_Eweight(G);

	//test
//	PNEANet L = ldag(G, G->BegNI().GetId(), 0.5);
//	cout<<"V:"<< L->GetNodes()<<"\t";
//	cout<<"E:"<< L->GetEdges()<<endl;
	

	int stop = clock();
	cout<<"running time:"<<(stop-start)/CLOCKS_PER_SEC<<endl;
	return 0 ;
	
}



//weight
void set_Eweight(PNEANet &G){
	
	TStr attr = "float";
	G->AddFltAttrE(attr, 0);

	for(TNEANet::TEdgeI EI=G->BegEI();EI<G->EndEI();EI++){
		int w = G->GetNI(EI.GetDstNId()).GetInDeg();
		G->AddFltAttrDatE(EI, w, attr);
	}
}

//return (max value, that NId)
vector<float> max_inf(PNEANet G, PNEANet S, int v){
	
	TStr attr = "float";
	vector<float> result(2,0);
	for(TNEANet::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++){
		int N = NI.GetId();
//cout<<N<<endl;
		if(  !S->IsNode(N) ){
//cout<<"go"<<endl;
			float inf_N = G->GetFltAttrDatN(N, attr);
			if(inf_N > result[0]){
				result[0] = inf_N;
				result[1] = N;
			}
		}
	}
	return result;
}


PNEANet ldag(PNEANet G, int v, float theta){


	//initiate inf(attrN)
	TStr attr = "float";
	G->AddFltAttrN(attr, 0);
	G->AddFltAttrDatN(v, 1, attr);


	PNEANet S = TNEANet::New();


	while( max_inf(G, S, v)[0] > theta ){
		int x = int(max_inf(G, S, v)[1]);
//cout<<"max1:"<<max_inf(G, S, v, inf)[0]<<"\t"<<max_inf(G, S, v, inf)[1]<<endl;
			
		//add node
		S->AddNode(x);
		
		//add edges
		int outd = G->GetNI(x).GetOutDeg();
		for(int i = 0 ; i< outd; i++){
			int endN = G->GetNI(x).GetOutNId(i);
			if( S->IsNode(endN) && endN != x )
				S->AddEdge(x,endN);
		}

		//update inf
		int ind = G->GetNI(x).GetInDeg();
		for(int i = 0; i<ind; i++){
			int tmp = G->GetNI(x).GetInNId(i);
			float inf_tmp = G->GetFltAttrDatN(tmp, attr);
			TNEANet::TEdgeI EI = G->GetEI(tmp, x);
			inf_tmp += G->GetFltAttrDatE(EI, attr)*G->GetFltAttrDatN(x, attr);
			G->AddFltAttrDatN(tmp, inf_tmp, attr);
		}
//cout<<"max2:"<<max_inf(G, S, v, inf)[0]<<"\t"<<max_inf(G, S, v, inf)[1]<<endl;
	}

	return S;
}

























