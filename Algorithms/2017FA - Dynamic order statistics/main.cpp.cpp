#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
struct nodeT
{
    int key;
    int rank;
    nodeT *leftN,*rightN,*parinte;
};

nodeT *BUILD_TREE(int left, int right, int &dim)
{
    nodeT *p;
    int n=sizeof(nodeT);
    int dim1=0, dim2=0;
    if (left>right)
    {
        dim=0;
        return NULL;
    }
    else
    {
        p=(nodeT*) malloc(n);
        p->key=(left+right)/2;
        p->leftN = NULL;
        p->rightN = NULL;
        p->rank=right-left+1;
        if(left<right)
        {
            p->leftN=BUILD_TREE(left,(left+right)/2-1,dim1);
            if(p->leftN != NULL)
            {
                p->leftN->parinte = p;
            }
            p->rightN=BUILD_TREE((left+right)/2+1,right,dim2);
            if(p->rightN != NULL)
            {
                p->rightN->parinte = p;
            }
        }
    }
    dim=p->rank;
    return p;
}

nodeT* OS_SELECT(nodeT *root, int i)
{
    int r;
    if (root->leftN!=NULL)
        r=root->leftN->rank+1;
    else r=1;
    if (i==r)
        return root;
    if (i<r)
        return OS_SELECT(root->leftN,i);
    else
        return OS_SELECT(root->rightN,i-r);
}
void prettyPrint(nodeT *d)
{
    if(d!=NULL)
    {
        prettyPrint(d->leftN);
		cout<<endl;
        for(int i=0; i<(d->rank); i++)
            cout<<" ";
		cout<<d->key<<","<<d->rank;
        prettyPrint(d->rightN);
    }
}
void print_tree_select(int n, int m)
{
    int h;
    nodeT *root;
    root=BUILD_TREE(1,n,h);
    root->parinte=NULL;
	prettyPrint(root);
	cout<<"\n";
		cout<<"Selected node "<<OS_SELECT(root,m)->key<<" with size: "<<OS_SELECT(root,m)->rank;
}
int main()
{
    int n,m;
    cout<<"Nr of elements:";cin>>n;
    cout<<"First select:";cin>>m;
    print_tree_select(n,m);

}
