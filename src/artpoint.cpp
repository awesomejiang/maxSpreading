# include"Snap.h"
//to avoid conflict, undef...
#undef max
#undef min 

# include<stdio.h>
# include<iostream>
# include<time.h>
# include<fstream>
# include"lt.h"
# include<string>
# include<stdlib.h>

using namespace std;


// the usage 
void usage(char* program){
    cout<<"\nProgram Usage:"<<program<<endl;
    cout<<"     [ -k       2    ]    points chosen for every single run, default 2\n"<<endl;
    cout<<"     [ -i  data/kk_result.dat ]  the output file, default: und_small_sparse\n"<<endl;
    cout<<"     [ -h ]             display this information\n"<<endl;
    cout<<"\n"<<endl;
    exit(1);
}


// function to get system arguments from command line
void getMyArgs(int argc, char** argv, int* k, string* filename){	
    char ch;
    while (( ch = getopt(argc, argv, "k:i:h")) != -1){
        switch (ch){
        case 'k':
            *k = atoi(optarg);break;
        case 'i':
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

	time_t time1,time2,time3,time4;

    int k = 2;
    string filename = "soc-Epinions1" ;
    getMyArgs(argc, argv, &k, &filename);

	//functions
	void kk(PNEANet, int, string);
	void kk_art(PNEANet, int, string);
	void AP_spread(PNEANet, int, string);

	//set random seed
	srand( (unsigned) time(NULL) );


	//construct file path
	string tmp_str1 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str2 = filename;
	string tmp_str3 = ".txt";
	string result_str = tmp_str1;
	result_str += tmp_str2;
	result_str += tmp_str3;

	time1 = time(NULL);
	//load graph, change it to pneanet and set attr
	cout<<"Loading and initilizing net..."<<endl;
	
	PNEANet G= load(result_str.c_str());
	//bench mark
	//PNEANet G = TSnap::GenRndGnm<PNEANet>(200, 1000);

	set_net(G);
	TStr attr = "float";
	
	cout<<"Setting sucessfully!"<<endl;

	time2 = time(NULL);
	cout<<"Cost time:"<<time2-time1<<endl;
	
	cout<<"Running normal kk algo..."<<endl;
	kk(G, k, filename);
	time3 = time(NULL);
	cout<<"Cost time:"<<time3-time2<<endl;
	
	cout<<"Running kk algo with ap set..."<<endl;
	kk_art(G, k, filename);
	time4 = time(NULL);
	cout<<"Cost time:"<<time4-time3<<endl;
//	cout<<"check kk point finded by APmethod..."<<endl;
//	AP_spread(G, k, filename);

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

	return 0;

}

void kk(PNEANet G, int k, string filename){
	TStr attr = "float";
	
	PNEANet A = dup_net(G);


	//construct output file path
	string tmp_str4 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str5 = filename;
	string tmp_str6 = "/kk.dat";
	string output_str = tmp_str4;
	output_str += tmp_str5;
	output_str += tmp_str6;

	ofstream output;
	output.open(output_str.c_str());

	int max_reg = -1;
	int node = -1;
	vector<int> pts;
	for(int i=0;i<k;i++){
		for(TNEANet::TNodeI NI=A->BegNI();NI<A->EndNI();NI++){
			for(vector<int>::iterator iter_pts = pts.begin();iter_pts!=pts.end();iter_pts++)
				A->AddFltAttrDatN(*iter_pts, 1, attr);
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
	cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		cout<<i+1<<"\t"<<node<<"\t"<<max_reg;
		output<<node<<"\t"<<max_reg<<endl;
	}
	
	output.close();

	cout<<"\n"<<"greedy max acd pts:"<<max_reg<<endl;
}






void kk_art(PNEANet G, int k, string filename){
	TStr attr = "float";
	
	PNEANet A = dup_net(G);

	//construct input file path
	string tmp_str1 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str2 = filename;
	string tmp_str3 = ".txt";
	string input_str = tmp_str1;
	input_str += tmp_str2;
	input_str += tmp_str3;

	ifstream input;
	input.open(input_str.c_str());

	//load artpoints
	vector<int> ap;	
	string line ;
	string t = "\t";
	while(getline(input, line)){
		int pos = line.find(t);
		ap.push_back( atoi( line.substr(0, pos).c_str() ) );
	}
	
	input.close();

	//construct output file path
	string tmp_str4 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str5 = filename;
	string tmp_str6 = "/kk_art.dat";
	string output_str = tmp_str4;
	output_str += tmp_str5;
	output_str += tmp_str6;

	ofstream output;
	output.open(output_str.c_str());


	int max_reg = -1;
	int node = -1;
	vector<int> pts;
	for(int i=0;i<k;i++){
		for(vector<int>::iterator iter_ap = ap.begin();iter_ap!=ap.end();iter_ap++){
			for(vector<int>::iterator iter_pts = pts.begin();iter_pts!=pts.end();iter_pts++)
				A->AddFltAttrDatN(*iter_pts, 1, attr);
			if(0 == A->GetFltAttrDatN(*iter_ap, attr)){ 
				int tmp_node = *iter_ap;
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
		cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		cout<<i+1<<"\t"<<node<<"\t"<<max_reg;
		output<<node<<"\t"<<max_reg<<endl;
	}
	output.close();

	cout<<"\n"<<"greedy max acd pts:"<<max_reg<<endl;
}




void AP_spread(PNEANet G, int k, string filename){
	TStr attr = "float";

	//construct input file path
	string tmp_str1 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str2 = filename;
	string tmp_str3 = ".txt";
	string input_str = tmp_str1;
	input_str += tmp_str2;
	input_str += tmp_str3;

	ifstream input;
	input.open(input_str.c_str());

	
	//load artpoints
	vector<int> ap;	
	string line ;
	string t = "\t";
	while(getline(input, line)){
		int pos = line.find(t);
		ap.push_back( atoi( line.substr(0, pos).c_str() ) );
	}
	
	input.close();



	//construct output file path
	string tmp_str4 = "/home/awesomejiang/Desktop/work/dataset_ap/";
	string tmp_str5 = filename;
	string tmp_str6 = "/AP_spread";
	string output_str = tmp_str4;
	output_str += tmp_str5;
	output_str += tmp_str6;

	ofstream output;
	output.open(output_str.c_str());


	for(int j = 0;j<k;j++){
		PNEANet T = dup_net(G);
		for(int i = 0;i<j;i++){
			T->AddFltAttrDatN(ap[i], 1, attr);
		}
		spread_ind(T);
		output<<j<<"\t"<<acd_pts(T)<<endl;
	}
	
	output.close();
}
	





