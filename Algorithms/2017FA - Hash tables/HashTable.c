#include <stdio.h>
#include <stdlib.h>
int N = 10007;
int c1 = 3;
int c2 = 5;
int c = 0;
int effort = 0;
int hashFunction(int,int);
int insertH(int*,int);
int findH(int*,int);
void fillH(int*,int,int*);
void outputTable(int);

int main()
{
    printf("Filling factor\t Max. Effort found\t Max. Effort not-found\n");
		outputTable(8000);
		outputTable(8500);
		outputTable(9000);
		outputTable(9500);
	return 0;
}
int hashFunction(int key, int i) {
	return (key + c1*i + c2*i*i) % N;
}
int insertH(int *hashTable, int key) {
	int i, aux;
	do {
		aux = hashFunction(key, i);
		if (hashTable[aux] == -1) {
			hashTable[aux] = key;
			return aux;
		}
		else i++;
	} while (i<N);
	return -1;
}
int findH(int *hashTable, int key) {
	int i = 0, aux;
	do {
		c++;
		aux = hashFunction(key, i);
		if (hashTable[aux] == key)
		{
			return 1;
		}
		else i++;
	} while ((hashTable[aux] != -1) && (i != N));
	return -1;
}
void fillH(int *hashTable, int fillFactor, int *u) {
	int i;
	for (i = 0; i<fillFactor; i++) {
		int randomNr = rand() % 10000;
		insertH(hashTable, randomNr);
		if (i<1500)
			u[i] = randomNr;
	}
}
void outputTable(int fillFactor) {
	int found = 0, notFound = 0, i, foundMax = 0, notFoundMax = 0, j, k;
	int hashTable[N];
	int a[1500];
	int u[1500];
	for (j = 0; j<5; j++) {
		for (k = 0; k<N; k++) {
			hashTable[k] = -1;
		}
		fillH(hashTable, fillFactor, u);
		for (i = 0; i<1500; i++) {
			c = 0;
			if ((findH(hashTable, u[i])) == 1) {
				if (c>foundMax) {
					foundMax = c;
				}
				found += c;
			}
		}
		for (i = 0; i<1500; i++)
			a[i] = rand() + 10000;
		for (i = 0; i<1500; i++) {
			c = 0;
			if ((findH(hashTable, a[i])) == -1) {
				if (c>notFoundMax) {
					notFoundMax = c;
				}
				notFound += c;
			}
		}
	}
	printf("%.2f \t\t\t %d  \t\t\t %.2f\n", (double)fillFactor/10000, notFoundMax, (notFound/5.0) / 1500);
	//printf("Filling factor \t Avg. Effort found \t Max. Effort found \t Avg. Effort not-found \t Max. Effort not-found\n %.2f \t\t\t %d \t\t\t %d \t\t\t %.2f \t\t\t %.2f\n", n, foundMax, notFoundMax, (double)g / 1500, ng / 1500);
}
