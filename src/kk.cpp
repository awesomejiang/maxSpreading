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
    cout<<"     [ -o  data/kk_result.dat ]  the output file, default: data/lt_result.dat\n"<<endl;
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




//add -pg and use  gprof *  to check function time if needed



int main(int argc, char* argv[]){

	time_t start,stop;
	start = time(NULL);

    int k = 2;
    const char* filename ="data/kk_result.dat" ;
    getMyArgs(argc, argv, &k, &filename);

	//functions
	void kk(PNEANet, int);
	void kk2(PNEANet);
	void kk_art(PNEANet, int);
	
	//set random seed
	srand( (unsigned) time(NULL) );


	//load graph, change it to pneanet and set attr
	PUNGraph oG = TSnap::LoadEdgeList<PUNGraph>("/home/awesomejiang/Desktop/work/dataset/soc-Epinions1.txt", 0, 1);
	PNEANet G = TSnap::ConvertGraph<PNEANet>(oG);

//	PNEANet G = TSnap::GenRndGnm<PNEANet>(200, 1000);

	set_net(G);
	TStr attr = "float";

	//file output
//	ofstream f;
//	f.open(filename);

	

	//G is a POINTER
	//so duplicate a net or restore the status of net at the end of func(faster way)


//	cout<<"Running kk based on LTmodel..."<<endl;
//	kk(G,k);
//	cout<<"Running kk_art..."<<endl;
//	kk_art(G,k);
//	cout<<"Running kk2 function..."<<endl;
//	kk2(G);

	cout<<"check kk point finded by APmethod..."<<endl;
/*	G->AddFltAttrDatN(36679, 1, attr);
	G->AddFltAttrDatN(23345, 1, attr);
	G->AddFltAttrDatN(24478, 1, attr);
	G->AddFltAttrDatN(67569, 1, attr);
	G->AddFltAttrDatN(24697, 1, attr);
	G->AddFltAttrDatN(60216, 1, attr);
	G->AddFltAttrDatN(14017, 1, attr);
	G->AddFltAttrDatN(6435, 1, attr);
	G->AddFltAttrDatN(10732, 1, attr);
	G->AddFltAttrDatN(57246, 1, attr);
*/

for(int j =1;j<11;j++){
	ifstream f;
	f.open("E_AP.dat");
	PNEANet T = dup_net(G);
	for(int i = 0;i<10*j;i++){
		int tmp;
		f>>tmp;
		T->AddFltAttrDatN(tmp, 1, attr);
	}
	spread_ind(T);
	cout<<10*j<<"\t"<<acd_pts(T)<<endl;
	f.close();
}
	
/*
//tracing
PUNGraph T = TSnap::LoadEdgeList<PUNGraph>("/home/awesomejiang/Desktop/work/dataset/CA-GrQc.txt", 0, 1);
TCnComV CmtyV;
double modularity = TSnap::CommunityGirvanNewman(T, CmtyV);
  for (int c = 0; c < CmtyV.Len(); c++) {
    for (int i = 0; i < CmtyV[c].Len(); i++) {
      f<<CmtyV[c][i].Val<<"\t"<<c<<endl;
    }
  }
cout<<"The modularity of the network is"<<modularity<<endl;
	f.close();

//tracing ends
*/
	stop = time(NULL);
	cout<<"total running time:"<<stop-start<<endl;

	return 0;

}


void kk_art(PNEANet G, int k){
	TStr attr = "float";
	
	PNEANet A = dup_net(G);
	
	PUNGraph oG = TSnap::ConvertGraph<PUNGraph>(A);
	TIntV ArtNIdV;
	TSnap::GetArtPoints(oG, ArtNIdV);
	int len = ArtNIdV.Len();
	cout<<"Art pts:"<<len<<endl;
	int max_reg = -1;
	int node = -1;
	vector<int> pts;
	for(int i=0;i<k;i++){
		for(int j =0;j<len;j++){
			for(vector<int>::iterator iter = pts.begin();iter!=pts.end();iter++)
				A->AddFltAttrDatN(*iter, 1, attr);
			if(0 == A->GetFltAttrDatN(ArtNIdV[j], attr)){ 
				int tmp_node = ArtNIdV[j];
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
		cout<<node<<"\t"<<max_reg<<endl;
	}

	cout<<"greedy max acd pts:"<<max_reg<<endl;
}




void kk(PNEANet G, int k){
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
		cout<<node<<"\t"<<max_reg<<endl;
	}

	cout<<"greedy max acd pts:"<<max_reg<<endl;
}





//real optimal algo
void kk2(PNEANet G){
	TStr attr = "float";

	PNEANet A = dup_net(G);
	 
	
	int max_reg = -1;
	int node1 = -1;
	int node2 = -1;
int counter = 0;
	for(TNEANet::TNodeI NI1=A->BegNI();NI1<A->EndNI();NI1++){
		for(TNEANet::TNodeI NI2=A->BegNI();NI2<A->EndNI();NI2++){
			if(NI1 != NI2){
counter++;
cout<<counter;
				int tmp_node1 = NI1.GetId();
				int tmp_node2 = NI2.GetId();
				A->AddFltAttrDatN(tmp_node1, 1, attr);
				A->AddFltAttrDatN(tmp_node2, 1, attr);
				spread_ind(A);
				int tmp_reg = acd_pts(A);
				if(tmp_reg > max_reg){
					max_reg = tmp_reg;
					node1 = tmp_node1;
					node2 = tmp_node2;
				}				
				set_attrN(A, G);
				
cout<<"\b\b\b\b\b\b\b\b";
			}
		}
	}
	cout<<"real max acd pts(k=2):"<<"node1:"<<node1<<"\t"<<"node2:"<<node2<<"\t"<<"acd:"<<max_reg<<endl;
}













