#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10000

void bubbleSort(int *A,int n,const char *file){
	int compare=0;
	int assign=0;
	int aux,i;
	int moved = 1;
    FILE *pf;
    pf=fopen(file,"a+");
    if(pf==NULL){
        perror("File not found");
        exit(1);
    }
    fprintf(pf,"%d, ",n);
	while (moved == 1){
		moved = 0;
		for ( i = 0; i<n - 1; i++){
			compare++;
			if (A[i]>A[i + 1])
			{
				aux = A[i];
				A[i] = A[i + 1];
				A[i + 1] = aux;
				assign += 3;
				moved = 1;
			}
		}
	}
	fprintf(pf,"%d, ",assign);
	fprintf(pf,"%d, ",compare);
	fprintf(pf,"%d, ",assign+compare);
	fclose(pf);
}
void insertionSort(int *A,int n,const char *file){
    int compare=0;
	int assign=0;
	int i,k,j,x;
	FILE *pf;
    pf=fopen(file,"a+");
    if(pf==NULL){
        perror("File not found");
        exit(1);
    }
	for ( i = 1; i < n; i++){
		x = A[i];
		j = 0;
		assign++;
		while (A[j] < x){
			j++;
			compare++;
		}
		for (k = i - 1; k >= j; k--){
			A[k + 1] = A[k];
			assign++;
		}
		A[j] = x;
		assign++;
	}
	fprintf(pf,"%d, ",assign);
	fprintf(pf,"%d, ",compare);
	fprintf(pf,"%d, ",assign+compare);
	fclose(pf);
}

void selectionSort(int *A,int n,const char *file){
	int compare = 0, assign = 0, aux, min,j,i;
	FILE *pf;
    pf=fopen(file,"a+");
    if(pf==NULL){
        perror("File not found");
        exit(1);
    }
	for (i=0; i <n;i++){
		min = i;
		for (j =i+1;j<n;j++){
                compare++;
			if (A[j] < A[min]){
			   min=j;
			}
		}
		if(aux!=A[min]){
		aux = A[min];
		A[min] = A[i];
		A[i] = aux;
		assign+=3;
		}
	}

	fprintf(pf,"%d, ",assign);
	fprintf(pf,"%d, ",compare);
	fprintf(pf,"%d\n ",assign+compare);
	fclose(pf);


}
int main()
{
        int i,n;
    	int *A,*B,*C;
    	A = B = C = (int*)malloc(MAXSIZE*sizeof(int));
        const char *best="Best.csv";
        const char *worst="Worst.csv";
        const char *average="Average.csv";
        const char *null = "null.txt";
        A[0] = rand()%100;
        for(i = 1; i<=9; i++)
            A[i] = A[i-1] + rand()%100;
        B = A;
        C = A;

        bubbleSort(A,9,null);
        for(i = 0; i<=9; i++)
            printf("%d, ", A[i]);
        printf("\n");
        insertionSort(B,9,null);
        for(i = 0; i<=9; i++)
            printf("%d, ", B[i]);
            printf("\n");
        selectionSort(C,9,null);
        for(i = 0; i<=9; i++)
            printf("%d, ", C[i]);

        for(n=100;n<=10000;n+=100){
                A[0]=rand()%100;
            for(i=1;i<n;i++){
                A[i]=A[i-1]+rand()%100;
            }
            B = A;
            C = A;
            bubbleSort(A,n,best);
            insertionSort(B,n,best);
            selectionSort(C,n,best);
        }

        for(n=100;n<=10000;n+=100){
             A[0]=rand()%10000;
            for(i=1;i<n;i++){
                A[i]=A[i-1]-rand()%100;
            }
            B = A;
            C = A;
            bubbleSort(A,n,worst);
            insertionSort(B,n,worst);
            selectionSort(C,n,worst);
        }


    for(n=100;n<=10000;n+=100){
        for(i=0;i<n;i++){
                A[i]=rand()%n;
        }
        B = A;
        C = A;
            bubbleSort(A,n,average);
            insertionSort(B,n,average);
            selectionSort(C,n,average);
    }



    return 0;
}
