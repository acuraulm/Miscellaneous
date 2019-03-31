#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#define inf 1000000
#define DIM 600001

#define WHITE 0
#define GRAY 1
#define BLACK 2

int color[DIM] = {0};
int d[DIM] = {0};
int pi[DIM] = {0};

struct node{
	int key;
	node *next;
};
node *first[DIM], *last[DIM];

typedef int Queue[DIM];
Queue Q;

int startQ, endQ;

void generateNrOfLists(int nodes, int edges)
{
    int i=0,nodeIndex,nodeKey;
	for (i = 0; i <= nodes; i++)
		first[i] = last[i] = NULL;

	while (i<edges){
			do{
				nodeIndex = rand() % nodes + 1;
				nodeKey = rand() % nodes + 1;
			} while (nodeIndex == nodeKey);

			if (first[nodeIndex] == NULL){
                    first[nodeIndex] = (node *)malloc(sizeof(node *));
                    first[nodeIndex]->key = nodeKey;
                    first[nodeIndex]->next = NULL;
                    last[nodeIndex] = first[nodeIndex];
			}
			else {
				node *head,*tail;

				head = (node *)malloc(sizeof(node *));
				head->key = nodeKey;
				head->next = NULL;

				tail = (node*)malloc(sizeof(node*));
				tail = first[nodeIndex];

				int sameKey = 0;
				while (tail != NULL){
					if (tail->key == head->key)
						sameKey = 1;
					tail = tail->next;
				}
				if (!sameKey){
					last[nodeIndex]->next = head;
					last[nodeIndex] = head;
					i++;
				}
			}

	}
	}

void BFS(int startingNode,int nodes){

	for (int u = 0; u < nodes; ++u){
		if (u != startingNode)
		{
			color[u] = WHITE;
			d[u] = inf;
			pi[u] = NULL;
		}
	}

	color[startingNode] = GRAY; //gray
	d[startingNode] = 0;
	pi[startingNode] = NULL;

	endQ = -1;
	startQ = 0;

	if (endQ == DIM - 1){
		printf("Am ajuns la numarul maxim de elemente.\n");
	}
	else
		Q[++endQ] = startingNode;
	printf("\nBFS incepand din nodul %d: \n", startingNode);
	printf("\n%d", startingNode);

	while (endQ - startQ + 1 != 0){

		node *u;
		u = first[Q[startQ]];

		while (u!= NULL){

			int v = u->key;
			if (color[v] == WHITE){
				color[v] = GRAY;
				d[v] = d[u->key] + 1;

				printf(" %d", v);

				pi[v] = u->key;
				if (endQ == DIM - 1){
					printf("Am ajuns la maxim\n");
				}
				else
				{
					Q[++endQ] = v;
				}
			}
			if (u->next == NULL){
				color[u->key] = BLACK;
				u = u->next;
			}
			else
			u = u->next;

		}
		if (startQ > endQ)
			printf("Eroare \n");
		else
			startQ++;
	}
}

void printNrOfLists(int n)
{
	int i;
	node *p;
	for (i = 1; i <= n; i++)
	{
		printf("%d : ", i);
		p = first[i];
		while (p != NULL)
		{
			printf("%d ", p->key);
			p = p->next;
		}
		printf("\n");
	}
}

int main(){
    generateNrOfLists(8, 15);
	printNrOfLists(8);
	BFS(2, 5);
	return 0;
}
