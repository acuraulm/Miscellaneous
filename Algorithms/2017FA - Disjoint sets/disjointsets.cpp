#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

struct node{
    int key;
    int rank;
    node *p;
};
struct edge{
    int weight;
    node* head;
	node* tail;
};
node* MAKE_SET(int x){
    node *p;
    p=(node*)malloc(sizeof(node));
    p->key=x;
    p->p=p;
    p->rank=0;
    return p;
}
node* FIND_SET(node *x){
    if(x!=x->p)
        x->p = FIND_SET(x->p);
    return x->p;
}
void  LINK(node* x, node* y) {
	if (x->rank > y->rank) {
		y->p = x;
	}else if (y->rank > x->rank) {
		x->p = y;
	}else{
		y->p = x;
		x->rank = x->rank + 1;
	}
}
void UNION(node *x, node *y){
    LINK(FIND_SET(x),FIND_SET(y));
}
int partition(edge a[], int l, int r){
	int i = l - 1;
	for (int j = l; j < r - 1; j++){
		if (a[j].weight <= a[r - 1].weight){
			i++;
			swap(a[i], a[j]);
		}
	}
	swap(a[i + 1], a[r - 1]);
	return i + 1;
}

void quickSort(edge a[], int l, int r){
	if (l < r - 1) {
		int q = partition(a, l, r);
		quickSort(a, l, q);
		quickSort(a, q + 1, r);
	}
	return;
}
edge* Kruskal(node* vertices[], edge edges[], int n,int m) {
	edge MST[10000];
	int MSTsize = 0;
	for (int i = 0; i < n; i++) {
		vertices[i]->p = vertices[i];
	}
	quickSort(edges, 0, m);
	for (int i = 0; i < m; i++) {
		if (FIND_SET(edges[i].head) != FIND_SET(edges[i].tail)) {
			MST[MSTsize].head = edges[i].head;
			MST[MSTsize].tail = edges[i].tail;
			MST[MSTsize].weight = edges[i].weight;
			UNION(edges[i].head,edges[i].tail);
			MSTsize++;
		}
	}
	return MST;
}
int main()
{
	int n=14;
	node **x=(node**)malloc(sizeof(node*)*n);
	 for(int i=0;i<n;i++){
        x[i]=(node*)malloc(sizeof(node));
    }
	 for(int i=0;i<n;i++){
        x[i]=MAKE_SET(i);
    }

    cout<<"Created unconnected vertices"<<endl;
    for (int i=0;i<n;i++)
		cout << "(" << x[i]->key << ", " << x[i]->p->key<<", "<<x[i]->rank << ") "<<endl;
	UNION(x[0],x[1]);
	UNION(x[2],x[3]);
	UNION(x[1],x[2]);
	UNION(x[4],x[5]);
	UNION(x[5],x[6]);
	cout<<"(key,parent,rank) After UNION"<<endl;;
	for (int i=0;i<n;i++)
		cout << "(" << x[i]->key << ", " << x[i]->p->key<<", "<<x[i]->rank << ") "<<endl;
	cout<<"FIND_SET "<<x[3]->key<<" -> "<<FIND_SET(x[3])->key<<endl;
	cout<<"FIND_SET "<<x[8]->key<<" -> "<<FIND_SET(x[8])->key<<endl;
	cout<<"FIND_SET "<<x[5]->key<<" -> "<<FIND_SET(x[5])->key<<endl;
	cout<<"FIND_SET "<<x[6]->key<<" -> "<<FIND_SET(x[6])->key<<endl;

	edge edges[15];
	edges[0].head = x[0];
	edges[0].tail = x[1];
	edges[0].weight = 4;
	edges[1].head = x[1];
	edges[1].tail = x[2];
	edges[1].weight = 8;
	edges[2].head = x[2];
	edges[2].tail = x[3];
	edges[2].weight = 7;
	edges[3].head = x[3];
	edges[3].tail = x[4];
	edges[3].weight = 9;
	edges[4].head = x[4];
	edges[4].tail = x[5];
	edges[4].weight = 10;
	edges[5].head = x[5];
	edges[5].tail = x[6];
	edges[5].weight = 2;
	edges[6].head = x[6];
	edges[6].tail = x[7];
	edges[6].weight = 1;
	edges[7].head = x[0];
	edges[7].tail = x[7];
	edges[7].weight = 8;
	edges[8].head = x[1];
	edges[8].tail = x[7];
	edges[8].weight = 11;
	edges[9].head = x[2];
	edges[9].tail = x[8];
	edges[9].weight = 2;
	edges[10].head = x[8];
	edges[10].tail = x[7];
	edges[10].weight = 7;
	edges[11].head = x[8];
	edges[11].tail = x[6];
	edges[11].weight = 6;
	edges[12].head = x[2];
	edges[12].tail = x[5];
	edges[12].weight = 4;
	edges[13].head = x[3];
	edges[13].tail = x[5];
	edges[13].weight = 14;
	int m = 14;
	n = 9;
	cout << "\n(head, tail, weight)\n";
	for (int i = 0; i < m; i++) {
		cout << "(" << edges[i].head->key << ", " << edges[i].tail->key << ", " << edges[i].weight << ") "<<endl;
	}
	edge* MST = Kruskal(x, edges, n, m);
	cout << "\nMinimum spanning tree\n";
	for (int i = 0; i < n -1 ; i++) {
		cout << "(" << MST[i].head->key << ", " << MST[i].tail->key << ", " << MST[i].weight << ")"<<endl;
	}

	return 0;
}

