#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <time.h>
#include <fstream>
using namespace std;
struct nodeM{
	int key,childrenNr;
	nodeM **childs;
};
nodeM *root;
nodeM *vecM[10];
struct nodeB{
	int key;
	nodeB *child,*brother;
};
nodeB *vecB[10];
int R[10]={0,2,7,5,2,7,7,-1,5,2};

void T1(){
    int childNo[10];
    for(int i=0; i<10; i++){
        if(R[i] != -1)
            childNo[R[i]]++;
    }
	for(int i=0; i<10; i++){
        vecM[i] = (nodeM*)malloc(sizeof(nodeM));
        vecM[i]->key = i;
        vecM[i]->childrenNr = 0;
        vecM[i]->childs = (nodeM**)malloc(sizeof(nodeM*)*(childNo[i]));
    }
	for (int i=1;i<10;i++){
		if (R[i]==-1)
			root=vecM[i];
		else{
			int p=R[i];
			vecM[p]->childrenNr+=1;
			int aux=vecM[p]->childrenNr;
			vecM[p]->childs[aux]=vecM[i];
		}
	}
}
void multiwayPrint(nodeM *M,int l){
    if(M!=NULL){
        cout<<endl;
        for(int i=0; i<l; i++)
            cout<<"  ";
        cout<<M->key;
        l++;
        if(M->childrenNr != 0)
            for(int i=1; i<=M->childrenNr; i++)
                multiwayPrint(M->childs[i],l);
    }
}
void createNullBinary(){
    for(int i=0; i<10; i++){
            vecB[i] = (nodeB*)malloc(sizeof(nodeB));
            vecB[i]->key = i;
            vecB[i]->child = NULL;
            vecB[i]->brother = NULL;
    }
}
void T2(nodeM *vecM){
	if (vecM!=NULL){
		for (int i=1;i<=vecM->childrenNr;i++){
			if (i==1){
				int aux=vecM->key;
				vecB[aux]->child=vecB[vecM->childs[1]->key];
			}else{
				vecB[vecM->childs[i-1]->key]->brother=vecB[vecM->childs[i]->key];
			}
			T2(vecM->childs[i]);
		}
	}
}

void binaryPrint(int key, int l){
    if(vecB!=NULL){
        l++;
        cout<<endl;
        for(int i=0; i<l; i++)
            cout<<"  ";
        cout<<key;
        if(vecB[key]->child!=NULL)
            binaryPrint(vecB[key]->child->key,l+1);
        if(vecB[key]->brother!=NULL)
            binaryPrint(vecB[key]->brother->key,l);
    }
}

int main()
{
    createNullBinary();
    cout<<"Parent vector: ";
    for(int i=1; i<10; i++)
            cout<<R[i]<<" ";

    cout<<endl<<"Multiway tree: ";
	T1();
	multiwayPrint(root,0);

	cout<<endl<<"Binary tree: ";
	T2(root);
	binaryPrint(vecB[7]->key,0);

	return 0;
}
