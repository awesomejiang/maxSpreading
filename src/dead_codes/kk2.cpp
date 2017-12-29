//what if we donnot choose the temp largest but to choose the exact largest?
//specifically, for k=2

# include"Snap.h"
//to avoid conflict, undef...
#undef max
#undef min 
# include<vector>
# include<queue>
# include<stdio.h>
# include<algorithm>
# include<iostream>
# include<time.h>
# include<fstream>

using namespace std;


// the usage 
void usage(char* program){
    cout<<"\nProgram Usage:"<<program<<endl;
    cout<<"     [ -n      10    ]    repeating times, default 10\n"<<endl;
    cout<<"     [ -k       2    ]    points chosen for every single run, default 2\n"<<endl;
    cout<<"     [ -o  data/pts_output.dat ]  the output file, default: data/pts_output.dat\n"<<endl;
    cout<<"     [ -h ]             display this information\n"<<endl;
    cout<<"\n"<<endl;
    exit(1);
}


// function to get system arguments from command line
void getMyArgs(int argc, char** argv, int* n,int* k, const char** filename){	
    char ch;
    while (( ch = getopt(argc, argv, "n:k:o:h")) != -1){
        switch (ch){
        case 'n':
            *n = atoi(optarg);break;
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




//time add -pg and use gprof kk.cpp to check if needed



int main(int argc, char* argv[]){

	PNGraph G = TSnap::LoadEdgeList<PNGraph>("/home/awesomejiang/Desktop/CA-GrQc.txt", 0, 1);
	
	//set random seed
	srand( (unsigned) time(NULL) );
	

	int start = clock();

	//functions
	vector<int> chs_pts(PNGraph);
	vector<int> expand(PNGraph, vector<int>);
	
    //for parameters
    int n  = 10;
    int k = 2;
    const char* filename ="data/pts_output.dat" ;
    getMyArgs(argc, argv, &n, &k, &filename);


	ofstream f;
	f.open(filename);
	int reg = 0;
	//main loop
	for(int i = 0 ; i< n; i++){
		vector<int> tmp = chs_pts(G);
		f<<tmp[0]<<"\t"<<tmp[1]<<endl;
		reg += expand(G, tmp).size();
	}
	f.close();
	cout<<"avr acd reg is:"<<reg/n<<endl;
	int stop = clock();
	cout<<"total running time:"<<(stop-start)/CLOCKS_PER_SEC<<endl;
	
	return 0;
	
}




//traverse the graph to get optimal points, return point's id
vector<int> chs_pts(PNGraph G){


	int start = clock();

	vector<int> expand(PNGraph, vector<int>);
	
	
	int maxreg = 0;
	int r1=-1;
	int r2=-1;
	

	for(TNGraph::TNodeI NI1 = G->BegNI(); NI1 < G->EndNI(); NI1++){
		int node1 = NI1.GetId();
		for(TNGraph::TNodeI NI2 = G->BegNI(); NI2 < G->EndNI(); NI2++){
			int node2 = NI2.GetId();
			if(node1 != node2){
				vector<int> tmp;
				tmp.push_back(node1);
				tmp.push_back(node2);		
				int reg = expand(G, tmp).size();
				if(reg > maxreg){
				maxreg = reg;
				r1 = node1;
				r2 = node2;
				}
			}	
		}
	}
	vector<int> result;
	if(r1 != -1 && r2 != -1){
		result.push_back(r1);
		result.push_back(r2);

	}
	
	int stop = clock();
	cout<<"running time:"<<(stop-start)/CLOCKS_PER_SEC<<endl;
	
	return result;
}



//expand the region using given points
vector<int> expand(PNGraph G, vector<int> chsn_pts){


	// put chosn pts into q and incre initially
	queue<int> q;
	int len_chsn_pts = chsn_pts.size();
	for( int i = 0; i < len_chsn_pts; i++ )
		q.push(chsn_pts[i]);
	vector<int> acd;
	
	
	//iter process, BFS
	while( q.size() ){
		int temp = q.front();
		q.pop();

		if( find(acd.begin(),acd.end(), temp) == acd.end() )
			acd.push_back(temp);


		int outdeg = G->GetNI(temp).GetOutDeg();
		for( int i = 0; i < outdeg; i++){
			int outN = G->GetNI(temp).GetOutNId(i);

			if( find(acd.begin(),acd.end(), outN) == acd.end() &&
				rand() <= RAND_MAX/G->GetNI(outN).GetInDeg() )
				q.push(outN);
			
		}
	}

	return acd;
}








