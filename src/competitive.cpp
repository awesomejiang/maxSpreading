# include"Snap.h"
//to avoid conflict, undef...
#undef max
#undef min 

# include<stdio.h>
# include<iostream>
# include<time.h>
# include<fstream>
# include"lt.h"

using namespace std;


// the usage 
void usage(char* program){
    cout<<"\nProgram Usage:"<<program<<endl;
    cout<<"     [ -k       2    ]    points chosen for every single run, default 2\n"<<endl;
    cout<<"     [ -o  data/lt_result.dat ]  the output file, default: data/compt.dat\n"<<endl;
    cout<<"     [ -h ]             display this information\n"<<endl;
    cout<<"\n"<<endl;
    exit(1);
}


// function to get system arguments from command line
void getMyArgs(int argc, char** argv, int* k, const char** filename){	
    char ch;
    while (( ch = getopt(argc, argv, "k:o:h")) != -1){
        switch (ch){
        case 'k':
            *k = atoi(optarg);break;
        case 'o':
            *filename = optarg;break;
        case 'h':
            usage(argv[0]);
        case '?':
            usage(argv[0]);
        }
    }    
}







//time add -pg and use  gprof *.cpp  to check if needed



int main(int argc, char* argv[]){

	time_t start,stop;
	start = time(NULL);

    //for parameters
    int k = 2;
    const char* filename ="data/pts_output.dat" ;
    getMyArgs(argc, argv, &k, &filename);
    
    //functions
	PNEANet set_init(PNEANet);
	vector<int> kk(PNEANet, int);
	void com(PNEANet);
	void B_IA(PNEANet, int);
	void test(PNEANet);
	
	//set random seed
	srand( (unsigned) time(NULL) );


	//load graph, change it to pneanet and set attr
	PNGraph oG = TSnap::LoadEdgeList<PNGraph>("/home/awesomejiang/Desktop/work/dataset/CA-GrQc.txt", 0, 1);
	PNEANet G = TSnap::ConvertGraph<PNEANet>(oG);

	/*
	PNGraph G = TSnap::LoadEdgeList<PNGraph>("/home/awesomejiang/Desktop/soc-Epinions1.txt", 0, 1);
	PNGraph G = TSnap::LoadEdgeList<PNGraph>("/home/awesomejiang/Desktop/facebook_combined.txt", 0, 1);

	uG = TSnap::LoadEdgeList<PUNGraph>("/home/awesomejiang/Desktop/facebook_combined.txt", 0, 1);
	//convert it to an directed G
	G = ConvertGraph(PNGraph, uG);

	//benchmark
	PNGraph G = TSnap::GenRndGnm(20, 200);
	*/



	//load the data of node.edge.edge_attr
	set_net(G);
	
/*	//file output
	ofstream f;
	f.open(filename); 
	f.close();
*/		
	test(G);
	B_IA(G,k);

	stop = time(NULL);
	cout<<"total running time:"<<stop-start<<endl;

	return 0;
	
}


//create a new S net with initial activated region
PNEANet set_init(PNEANet S){
	TStr attr = "float";


	// D. I. Y.:
	//test random set:
/*	for(int i=0;i<2000;){
		int tmp = rand();
		if(S->IsNode(tmp)){
			S->AddFltAttrDatN(tmp, 1, attr);
			i++;
		}
	}
*/	
	vector<int> kk(PNEANet, int);
	vector<int> pts;
	pts.push_back(14265);
	pts.push_back(9572);
	pts.push_back(15108);
	pts.push_back(17501);
	pts.push_back(4952);
	pts.push_back(24559);
	pts.push_back(24924);
	pts.push_back(23665);
	pts.push_back(13929);
	pts.push_back(12927);
	pts.push_back(122);
	pts.push_back(5052);
	pts.push_back(5346);
	pts.push_back(1172);
	pts.push_back(7350);
	pts.push_back(10055);
	pts.push_back(2710);
	pts.push_back(17075);
	pts.push_back(9017);
	pts.push_back(14746);
	pts.push_back(7307);
	pts.push_back(14181);
	pts.push_back(3765);
	pts.push_back(21994);
	pts.push_back(12212);
	pts.push_back(3345);
	pts.push_back(1149);
	pts.push_back(2042);
	pts.push_back(12842);
	pts.push_back(449);
	pts.push_back(13469);
	pts.push_back(3547);
	pts.push_back(23552);
	pts.push_back(16310);
	pts.push_back(9124);
	pts.push_back(9829);
	pts.push_back(5107);
	pts.push_back(10895);
	pts.push_back(21075);
	pts.push_back(5194);
	pts.push_back(16607);
	pts.push_back(6023);
	pts.push_back(7689);
	pts.push_back(17626);
	pts.push_back(16741);
	pts.push_back(3651);
	pts.push_back(4416);
	pts.push_back(13190);
	pts.push_back(9710);
	pts.push_back(5287);

	for(vector<int>::iterator iter=pts.begin();iter != pts.end();iter++)
		S->AddFltAttrDatN(*iter, 1, attr);
	

	queue<int> q;
	for(TNEANet::TNodeI NI = S->BegNI();NI < S->EndNI();NI++){
		int node = NI.GetId();
		if(1 == S->GetFltAttrDatN(node, attr)){
			q.push(node);
		}
	}

	int counter =0;
	while( q.size() && counter<200){
		counter++;
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
				}
			}
		}
	}
	modify_nodes(S);

	return S;
}
/*
//compare two strategies
void com(PNEANet G){

	TStr attr = "float";

	//set I
	PNEANet I = dup_net(G);
	set_init(I);
	
	//get 2 duplication
	PNEANet A = dup_net(I);
	PNEANet B = dup_net(G);
	


	cout<<"A's initial activated pts:"<<endl;
	for(TNEANet::TNodeI NI=I->BegNI();NI<I->EndNI();NI++){
		if( 1 == I->GetFltAttrDatN(NI, attr))
			cout<<NI.GetId()<<"\t";
	}
	cout<<endl;



int counter = 0;	
	int tog_node = -1;
	float tog_regB = -1;
	for(TNEANet::TNodeI NI=B->BegNI();NI<B->EndNI();NI++){
		int tmp_node = NI.GetId();
counter+=1;
cout<<counter;
		B->AddFltAttrDatN(tmp_node, 1, attr);
		spread_tog(A,B);
		float tmp_reg = acd_pts(B);
		
		if(tmp_reg > tog_regB){
			tog_node = tmp_node;
			tog_regB = tmp_reg;
		}
		
		set_attr(A, I);
		set_attr(B, G);
cout<<"\b\b\b\b";
	}

	cout<<"Spread normally: node:"<<tog_node<<"\t"<<"range:"<<tog_regB<<endl;



	int avoid_node = -1;
	float avoid_regB = -1;
int counter2 = 0;
	for(TNEANet::TNodeI NI=B->BegNI();NI<B->EndNI();NI++){
		int tmp_node = NI.GetId();
counter2+=1;
cout<<counter2;
		B->AddFltAttrDatN(tmp_node, 1, attr);
		spread_avoid(A,B);
		float tmp_reg = acd_pts(B);
		
		if(tmp_reg > avoid_regB){
			avoid_node = tmp_node;
			avoid_regB = tmp_reg;
		}
		
		set_attr(A, I);
		set_attr(B, G);
cout<<"\b\b\b\b";
	}

	cout<<"Spread avoidly: node:"<<avoid_node<<"\t"<<"range:"<<avoid_regB<<endl;

}
*/


//B spread with a initial A region, check the double acd pts
void B_IA(PNEANet G, int k){

	vector<int> kk(PNEANet, int);
	vector<int> kk_B(PNEANet, PNEANet, int);


	TStr attr = "float";

	//set I
	PNEANet I = dup_net(G);
	set_init(I);


	cout<<"A's initial activated pts:"<<endl;
	cout<<"Total#:"<<acd_pts(I)<<endl;

	
	//create 2 duplications
	PNEANet A = dup_net(I);
	PNEANet B = dup_net(G);
	
	//spread independently
	cout<<"Spread independently:"<<endl;
	//I's acd pts with no B
	cout<<"A spread with no B:"<<endl;
	spread_ind(A);
	cout<<"A'acd pts:"<<acd_pts(A)<<endl;
	set_attr(A, I);
	//B's acd pts with no I
	cout<<"B spread with no I:"<<endl;
	vector<int> pts = kk(G,k);
	cout<<"B'chosn pts:"<<endl;
	for(vector<int>::iterator iter = pts.begin();iter!=pts.end();iter++){
		B->AddFltAttrDatN(*iter, 1, attr);
		cout<<*iter<<"\t";
	}
	cout<<endl;
	spread_ind(B);
	cout<<"B'acd pts:"<<acd_pts(B)<<endl;
	set_attr(B, G);
	
	//spread together
	cout<<"Spread together:"<<endl;
	vector<int> ptsB = kk_B(I,G,k);
	cout<<"B'chosn pts:"<<endl;
	for(vector<int>::iterator iter = ptsB.begin();iter!=ptsB.end();iter++){
		B->AddFltAttrDatN(*iter, 1, attr);
		cout<<*iter<<"\t";
	}
	cout<<endl;
	cout<<"B spread with no I:"<<endl;
	spread_ind(B);
	cout<<"B'acd pts:"<<acd_pts(B)<<endl;
	
	set_attr(B, G);
	for(vector<int>::iterator iter = ptsB.begin();iter!=ptsB.end();iter++)
		B->AddFltAttrDatN(*iter, 1, attr);
	cout<<"B spread together with I:"<<endl;
	spread_tog(A, B);
	cout<<"A'acd pts:"<<acd_pts(A)<<endl;
	cout<<"B'acd pts:"<<acd_pts(B)<<endl;

	//anlysing part
	int counterA = 0;
	int counterB = 0;
	int counterM = 0;
	for(TNEANet::TNodeI NI=G->BegNI();NI<G->EndNI();NI++){
		int node = NI.GetId();
		if(A->GetFltAttrDatN(node, attr) > 0)
			counterA++;
		if(B->GetFltAttrDatN(node, attr) > 0)
			counterB++;
		if(A->GetFltAttrDatN(node, attr) > 0 && B->GetFltAttrDatN(node, attr) > 0)
			counterM++;
	}
	cout<<"Calculate influenced pts:"<<endl;
	cout<<"A:"<<counterA<<endl;
	cout<<"B:"<<counterB<<endl;
	cout<<"mix:"<<counterM<<endl;

	set_attr(A, I);
	set_attr(B, G);
	

}
//oringal KKalgo
vector<int> kk(PNEANet G, int k){
	TStr attr = "float";
	
	PNEANet A = dup_net(G);
	 
	int max_reg = -1;
	int node = -1;
	vector<int> pts;
	for(int i=0;i<k;i++){
		for(TNEANet::TNodeI NI=A->BegNI();NI<A->EndNI();NI++){
			for(vector<int>::iterator iter = pts.begin();iter!=pts.end();iter++)
				A->AddFltAttrDatN(*iter, 1, attr);
			if(0 == A->GetFltAttrDatN(NI, attr)){ 
				int tmp_node = NI.GetId();
				A->AddFltAttrDatN(tmp_node, 1, attr);
				spread_ind(A);
				int tmp_reg = acd_pts(A);
				if(tmp_reg > max_reg){
					max_reg = tmp_reg;
					node = tmp_node;
				}
			}
			set_attrN(A, G);
		} 
		pts.push_back(node);
	}

	return pts;
}



//implement KKalgo for B while A has initial pts
vector<int> kk_B(PNEANet I, PNEANet G, int k){
	TStr attr = "float";

	//create 2 duplications
	PNEANet A = dup_net(I);
	PNEANet B = dup_net(G);

	 
	int max_reg = -1;
	int node = -1;
	vector<int> pts;
	for(int i=0;i<k;i++){
		for(TNEANet::TNodeI NI=B->BegNI();NI<B->EndNI();NI++){
			for(vector<int>::iterator iter = pts.begin();iter!=pts.end();iter++)
				B->AddFltAttrDatN(*iter, 1, attr);
			if(0 == B->GetFltAttrDatN(NI, attr)){ 
				int tmp_node = NI.GetId();
				B->AddFltAttrDatN(tmp_node, 1, attr);
				spread_tog(A, B);
				int tmp_reg = acd_pts(B);
				if(tmp_reg > max_reg){
					max_reg = tmp_reg;
					node = tmp_node;
				}
			}
			set_attrN(A, I);
			set_attrN(B, G);
		} 
		pts.push_back(node);
	}
	
	return pts;
}






void test(PNEANet G){
	TStr attr = "float";
	
	PNEANet A = dup_net(G);
	PNEANet B = dup_net(G);
	
	set_init(A);

	int counterA = 0;
	int counterB = 0;
	int counterM = 0;
	for(TNEANet::TNodeI NI=G->BegNI();NI<G->EndNI();NI++){
		int node = NI.GetId();
		if(A->GetFltAttrDatN(node, attr) > 0)
			counterA++;
		if(B->GetFltAttrDatN(node, attr) > 0)
			counterB++;
		if(A->GetFltAttrDatN(node, attr) > 0 && B->GetFltAttrDatN(node, attr) > 0)
			counterM++;
	}
	cout<<"Calculate influenced pts:"<<endl;
	cout<<"A:"<<counterA<<endl;
	cout<<"B:"<<counterB<<endl;
	cout<<"mix:"<<counterM<<endl;
	
	spread_tog(A, B);

	counterA = 0;
	counterB = 0;
	counterM = 0;
	for(TNEANet::TNodeI NI=G->BegNI();NI<G->EndNI();NI++){
		int node = NI.GetId();
		if(A->GetFltAttrDatN(node, attr) > 0)
			counterA++;
		if(B->GetFltAttrDatN(node, attr) > 0)
			counterB++;
		if(A->GetFltAttrDatN(node, attr) > 0 && B->GetFltAttrDatN(node, attr) > 0)
			counterM++;
	}
	cout<<"Calculate influenced pts:"<<endl;
	cout<<"A:"<<counterA<<endl;
	cout<<"B:"<<counterB<<endl;
	cout<<"mix:"<<counterM<<endl;
	
}



