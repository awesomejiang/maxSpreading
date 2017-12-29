#ifndef LT_H
#define LT_H

# include"Snap.h"
//to avoid conflict, undef...
#undef max
#undef min 
# include<vector>
# include<deque>
# include<queue>
# include<stdio.h>
# include<algorithm>
# include<iostream>
# include<fstream>
# include<string>

using namespace std;

//load edge info to a PNEANet
PNEANet load(string file){
	PNEANet G = TSnap::LoadEdgeList<PNEANet>(file.c_str(), 0, 1);
	int Ectr = 0;
	for(TNEANet::TEdgeI EI=G->BegEI();EI<G->EndEI();EI++){
		int src = EI.GetSrcNId();
		int dst = EI.GetDstNId();
		Ectr += G->IsEdge(dst,src);
	}

	if( Ectr == 0 ){
		for(TNEANet::TEdgeI EI=G->BegEI();EI<G->EndEI();EI++){
			G->AddEdge(EI.GetDstNId(), EI.GetSrcNId());
		}
	}

	return G;
}





//set attrN as G's
void set_attrN(PNEANet A, PNEANet G){
	TStr attr = "float";
	
	for(TNEANet::TNodeI NI=A->BegNI();NI<A->EndNI();NI++){
		int node = NI.GetId();
		float tmp = G->GetFltAttrDatN(node, attr);
		A->AddFltAttrDatN(NI, tmp, attr);
	}
	
}

//set attrE as G's
void set_attrE(PNEANet A, PNEANet G){
	TStr attr = "float";
	
	for(TNEANet::TEdgeI EI=A->BegEI();EI<A->EndEI();EI++){
		int edge = EI.GetId();
		float tmp = G->GetFltAttrDatE(edge, attr);
		A->AddFltAttrDatE(edge, tmp, attr);
	}
	
}

//set attr as G's
void set_attr(PNEANet A, PNEANet G){
	
	set_attrN(A, G);
	
	set_attrE(A, G);
	
}

//generate a duplication of net(cuz TNEANet's stupid construction function wont copy the attr data T T)
PNEANet dup_net(PNEANet G){

	TStr attr = "float";

	PNEANet A = TSnap::ConvertGraph<PNEANet>(G);
	A->AddFltAttrE(attr);
	A->AddFltAttrN(attr);
	
	set_attr(A, G);
	
	return A;

}

//claculate number of activated points
float acd_pts(PNEANet S){

	TStr attr = "float";
	float counter = 0;
	for(TNEANet::TNodeI NI=S->BegNI();NI<S->EndNI();NI++)
		counter += S->GetFltAttrDatN(NI, attr);

	return counter;
}	



//set node threshold 
//and modify edge weight by threshold s.t. if node value >1, the node is deemed activated
void set_net(PNEANet G){
	
	TStr attr = "float";
	
	//threshold(threshold must >0)
	G->AddFltAttrN(attr, 0.5);
	
	//modify weights s.t. if N's threshold >=1, N is activated.
	G->AddFltAttrE(attr, 0);
	for(TNEANet::TNodeI NI=G->BegNI();NI<G->EndNI();NI++){
		int dstN = NI.GetId();
		int indeg = NI.GetInDeg();
		for (int i=0;i<indeg;i++){
			int srcN = NI.GetInNId(i);
			TNEANet::TEdgeI EI = G->GetEI(srcN, dstN);
			G->AddFltAttrDatE(EI, 1/(indeg*G->GetFltAttrDatN(dstN, attr)), attr);
		}
	}
	
	//Clean nodes' thresholds
	for(TNEANet::TNodeI NI=G->BegNI();NI<G->EndNI();NI++)
		G->AddFltAttrDatN(NI, 0, attr);

}


//one tech spreads from initial region independently
void spread_ind(PNEANet S){

	void modify_nodes(PNEANet);
	
	TStr attr = "float";

	queue<int> q;
	for(TNEANet::TNodeI NI = S->BegNI();NI < S->EndNI();NI++){
		int node = NI.GetId();
		if(1 == S->GetFltAttrDatN(node, attr))
			q.push(node);
	}


	while( q.size() ){
		int srcN = q.front();
		TNEANet::TNodeI NI = S->GetNI(srcN);
		q.pop();
		int outdeg = NI.GetOutDeg();
		for (int i=0;i<outdeg;i++){
			int dstN = NI.GetOutNId(i);
			float init_v = S->GetFltAttrDatN(dstN, attr);
			if(init_v < 1){
				TNEANet::TEdgeI EI = S->GetEI(srcN, dstN);
				float final_value = init_v+S->GetFltAttrDatE(EI, attr);
				S->AddFltAttrDatN(dstN, final_value, attr);
				if(final_value >= 1)
					q.push(dstN);
			}
		}
	}
	modify_nodes(S);
}

//one tech spreads from initial region independently with output
void spread_ind_out(PNEANet S){

	void modify_nodes(PNEANet);
	
	TStr attr = "float";

	queue<int> q;
	for(TNEANet::TNodeI NI = S->BegNI();NI < S->EndNI();NI++){
		int node = NI.GetId();
		if(1 == S->GetFltAttrDatN(node, attr)){
			q.push(node);
			cout<<node<<endl;
		}
	}


	while( q.size() ){
		int srcN = q.front();
		TNEANet::TNodeI NI = S->GetNI(srcN);
		q.pop();
		int outdeg = NI.GetOutDeg();
		for (int i=0;i<outdeg;i++){
			int dstN = NI.GetOutNId(i);
			float init_v = S->GetFltAttrDatN(dstN, attr);
			if(init_v < 1){
				TNEANet::TEdgeI EI = S->GetEI(srcN, dstN);
				float final_value = init_v+S->GetFltAttrDatE(EI, attr);
				S->AddFltAttrDatN(dstN, final_value, attr);
				if(final_value >= 1){
					q.push(dstN);
					cout<<dstN<<endl;
				}
			}
		}
	}
	modify_nodes(S);
}
//change edge weight after spreading for one step
void adjust_weight(int n, PNEANet A, PNEANet B){

	TStr attr = "float";
	
	float v_A = A->GetFltAttrDatN(n, attr);
	float v_B = B->GetFltAttrDatN(n, attr);
	
	if(v_A >= 1 && v_B >= 1){
		int outd = A->GetNI(n).GetOutDeg();
		for(int i=0;i<outd;i++){
			int outN = A->GetNI(n).GetOutNId(i);
			int e = A->GetEI(n,outN).GetId();
			float tmpA = A->GetFltAttrDatE(e, attr);
			A->AddFltAttrDatE(e, v_A/(v_A+v_B)*tmpA, attr);
			float tmpB = B->GetFltAttrDatE(e, attr);
			B->AddFltAttrDatE(e, v_B/(v_A+v_B)*tmpB, attr);
		}
	}
	if(v_A >= 1 && v_B < 1){
		int outd = A->GetNI(n).GetOutDeg();
		for(int i=0;i<outd;i++){
			int outN = A->GetNI(n).GetOutNId(i);
			int e = A->GetEI(n,outN).GetId();
			B->AddFltAttrDatE(e, 0, attr);
		}
	}
	if(v_A < 1 && v_B >= 1){
		int outd = A->GetNI(n).GetOutDeg();
		for(int i=0;i<outd;i++){
			int outN = A->GetNI(n).GetOutNId(i);
			int e = A->GetEI(n,outN).GetId();
			A->AddFltAttrDatE(e, 0, attr);
		}
	}
}

//two techs spread from initial region together every node can be set to be AB-mixed state
void spread_tog(PNEANet A, PNEANet B){

	void modify_nodes(PNEANet, PNEANet);

	TStr attr = "float";
	
	//initiate work deque
	deque<int> dq;
	for(TNEANet::TNodeI NI = A->BegNI();NI < A->EndNI();NI++){
		int node = NI.GetId();
		if(A->GetFltAttrDatN(node, attr) == 1.0 || B->GetFltAttrDatN(node, attr) == 1.0)
			dq.push_back(node);
	}

	while( dq.size() ){
	
		//transfer dq to dq_tmp
		deque<int> dq_tmp;
		while(dq.size()){
			int tmp = dq.front();
			dq.pop_front();
			dq_tmp.push_back(tmp);
		}

		//change edge weight 
		for(deque<int>::iterator iter = dq_tmp.begin();iter!=dq_tmp.end();iter++)
			adjust_weight(*iter, A, B);
		
		//for ele in q_tmp, check unactivated neibrs, and put the new ones in to q
		while(dq_tmp.size()){
			int srcN = dq_tmp.front();
			dq_tmp.pop_front();
			TNEANet::TNodeI NI = A->GetNI(srcN);
			int outdeg = NI.GetOutDeg();
			for (int i=0;i<outdeg;i++){
				int dstN = NI.GetOutNId(i);
				float init_v_A = A->GetFltAttrDatN(dstN, attr);
				float init_v_B = B->GetFltAttrDatN(dstN, attr);
				if(init_v_A < 1 && init_v_B < 1){
					int e = A->GetEI(srcN, dstN).GetId();
					init_v_A += A->GetFltAttrDatE(e, attr);
					A->AddFltAttrDatN(dstN, init_v_A, attr);
					init_v_B += B->GetFltAttrDatE(e, attr);
					B->AddFltAttrDatN(dstN, init_v_B, attr);
					if(A->GetFltAttrDatN(dstN, attr)>=1 || B->GetFltAttrDatN(dstN, attr)>=1)
						dq.push_back(dstN);
				}
			}
		}
	}
	
	modify_nodes(A, B);
}


//two techs spread from initial region together. B will avoid A attr pts while spreading.
void spread_avoid(PNEANet A, PNEANet B){

	void modify_nodes(PNEANet, PNEANet);
	
	TStr attr = "float";
	
	//initiate work deque
	deque<int> dq;
	for(TNEANet::TNodeI NI = A->BegNI();NI < A->EndNI();NI++){
		int node = NI.GetId();
		if(A->GetFltAttrDatN(node, attr) == 1.0 || B->GetFltAttrDatN(node, attr) == 1.0)
			dq.push_back(node);
	}

	while( dq.size() ){
	
		//transfer dq to dq_tmp
		deque<int> dq_tmp;
		while(dq.size()){
			int tmp = dq.front();
			dq.pop_front();
			dq_tmp.push_back(tmp);
		}

		//change edge weight 
		for(deque<int>::iterator iter = dq_tmp.begin();iter!=dq_tmp.end();iter++)
			adjust_weight(*iter, A, B);
			
		//for ele in q_tmp, check unactivated neibrs, and put the new ones in to q
		while(dq_tmp.size()){
			int srcN = dq_tmp.front();
			dq_tmp.pop_front();
			TNEANet::TNodeI NI = A->GetNI(srcN);
			int outdeg = NI.GetOutDeg();
			for (int i=0;i<outdeg;i++){
				int dstN = NI.GetOutNId(i);
				float init_v_A = A->GetFltAttrDatN(dstN, attr);
				float init_v_B = B->GetFltAttrDatN(dstN, attr);
				if(init_v_A < 1 && init_v_B < 1){
					int edge = A->GetEI(srcN, dstN).GetId();
					if(init_v_A == 0 || init_v_B != 0){
						init_v_B += B->GetFltAttrDatE(edge, attr);
						B->AddFltAttrDatN(dstN, init_v_B, attr);
					}
					init_v_A += A->GetFltAttrDatE(edge, attr);
					A->AddFltAttrDatN(dstN, init_v_A, attr);
					if(A->GetFltAttrDatN(dstN, attr)>=1 || B->GetFltAttrDatN(dstN, attr)>=1)
						dq.push_back(dstN);
				}
			}
		}
	}
	

	modify_nodes(A, B);
}




//independent spread: classify the points to A/B/A+B/empty points
void modify_nodes(PNEANet S){

	TStr attr = "float";

	for(TNEANet::TNodeI NI = S->BegNI();NI < S->EndNI();NI++){
		int n = NI.GetId();
		float v = S->GetFltAttrDatN(n, attr);
	
		if(v >= 1)
			S->AddFltAttrDatN(n, 1, attr);

		else
			S->AddFltAttrDatN(n, 0, attr);

	}
}


//competitive model: classify the points to A/B/A+B/empty points
void modify_nodes(PNEANet A, PNEANet B){

	TStr attr = "float";

	for(TNEANet::TNodeI NI = A->BegNI();NI < A->EndNI();NI++){
		int n = NI.GetId();
		float v_A = A->GetFltAttrDatN(n, attr);
		float v_B = B->GetFltAttrDatN(n, attr);
	
		if(v_A < 1 && v_B < 1){
			A->AddFltAttrDatN(n, 0, attr);
			B->AddFltAttrDatN(n, 0, attr);
		}
		else{
			if(v_A >= 1 && v_B < 1){
				A->AddFltAttrDatN(n, 1, attr);
				B->AddFltAttrDatN(n, 0, attr);
			}
			else{
				if(v_A < 1 && v_B >= 1){
					A->AddFltAttrDatN(n, 0, attr);
					B->AddFltAttrDatN(n, 1, attr);
				}
				else{
					A->AddFltAttrDatN(n, v_A/(v_A+v_B), attr);
					B->AddFltAttrDatN(n, v_B/(v_A+v_B), attr);
				}
			}
		}
	}
}


#endif



