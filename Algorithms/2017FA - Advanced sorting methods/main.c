#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 10000
int compHeap=0, compQuick=0;
int asigHeap=0, asigQuick=0;
const char *null = "null.txt";

int partition(int *a, int leftBound, int rightBound) {
    asigQuick++;
    int i, aux;
    for (i = leftBound; i < rightBound; i++) {
        compQuick++;
        if (a[i] <= a[rightBound]) {
            leftBound++;
            aux = a[i];
            a[i] = a[leftBound-1];
            a[leftBound-1] = aux;
            asigQuick += 3;
        }
    }
    aux = a[leftBound];
    a[leftBound] = a[rightBound];
    a[rightBound] = aux;
    asigQuick += 3;
    return leftBound;
}
void quickSort(int *a, int leftBound, int rightBound,const char *file) {
//    FILE *pf;
//    pf=fopen(file,"a+");
 //   if(pf==NULL){
//        perror("File not found");
//        exit(1);
//    }
     //if(file == null)
     //   ;
    //else{
//    fprintf(pf,"%d, ",rightBound);
    if (leftBound < rightBound) {
        int pivot = partition(a, leftBound, rightBound);
        quickSort(a, leftBound, pivot - 1,null);
        quickSort(a, pivot + 1, rightBound,null);
    }
  //  fprintf(pf,"%d, ",asigQuick);
 //	fprintf(pf,"%d, ",compQuick);
//	fprintf(pf,"\n");
//	fclose(pf);
   // }
}
// Quick select //
int quickPartition(int *a, int leftBound, int rightBound) {
    int i = rand() % (rightBound - leftBound) + leftBound;
    int aux = a[rightBound];
    a[rightBound] = a[i];
    a[i] = aux;
    return partition(a, leftBound, rightBound);
}
int quickSelect(int *a, int leftBound, int rightBound, int i) {
    if (leftBound == rightBound) {
        return a[leftBound];
    }
    int pivot = quickPartition(a, leftBound, rightBound);
    int k = pivot - leftBound + 1;
    if (i == k) {
        return a[pivot];
    } else if (i < k) {
        return quickSelect(a, leftBound, pivot - 1, i);
    } else {
        return quickSelect(a, pivot + 1, rightBound, i - k);
    }
}

// Heap sort //
void maxHeapify(int *a, int size, int i) {
    int pid;
    int lid = 2 * i + 1;
    int rid = 2 * i + 2;
    compHeap++;
    if (lid < size && a[lid] > a[i]) {
        pid = lid;
    } else {
        pid = i;
    }
   compHeap++;
    if (rid < size && a[rid] > a[pid]) {
        pid = rid;
    }
    if (pid != i) {
        int aux = a[pid];
        a[pid] = a[i];
        a[i] = aux;
        asigHeap += 3;
        maxHeapify(a, size, pid);
    }

}
void buildMaxHeap(int *a, int size) {
    int i;
    for (i = (size - 1) / 2; i >= 0; i--) {
        maxHeapify(a, size, i);
    }
}
void heapSort(int *a, int size, const char *file) {
 //   FILE *pf;
 //   pf=fopen(file,"a+");
 //   if(pf==NULL){
 //       perror("File not found");
 //       exit(1);
 //   }
   // if(file == null)
   //     ;
    //else{
 //   fprintf(pf,"%d, ",size);
    int i, size2 = size;
    buildMaxHeap(a, size);
    for (i = size - 1; i > 0; i--) {
        int aux = a[0];
        a[0] = a[i];
        a[i] = aux;
        asigHeap += 3;
        size2 = size2 - 1;
        maxHeapify(a, size2, 0);
    }
 //   fprintf(pf,"%d, ",asigHeap);
//	fprintf(pf,"%d, ",compHeap);
//	fprintf(pf,"\n");
//	fclose(pf);
   // }
}
// Heap sort //
int main(){
    int *a,*ua;
    int i;
   // int n;
    a = (int*)malloc(MAXSIZE*sizeof(int));
    ua = (int*)malloc(MAXSIZE*sizeof(int));
    //const char *heap = "Heap.csv";
    //const char *quick = "Quick.csv";
    //const char *heapWorst = "HeapWorst.csv";
    //const char *quickWorst = "QuickWorst.csv";

     //Correctness
    for(i=0;i<=9;i++)
        a[i] =rand()%100;
     for(i=0;i<=9;i++){
        ua[i] =a[i];
     }
     printf("Unordered a: ");
     for(i=0;i<=9;i++){
        printf("%d, ", a[i]);
     }
     printf("\n");
    // Heap sort corectness //
    heapSort(a,10,null);
    //printf("\nComp:%d, Asig:%d\n",compHeap, asigHeap);
    // Quick sort corectness //
    printf("\nHeapSort a: ");
    for(i=0;i<=9;i++){
        printf("%d, ", a[i]);
     }
     printf("\n");
     printf("\nUnordered a: ");
     for(i=0;i<=9;i++){
        printf("%d, ", ua[i]);
     }
    printf("\nQuick select before order: %d\n",quickSelect(ua,0,4,3));
    quickSort(ua,0,9,null);
    printf("\n");
    printf("QuickSort: ");
     for(i = 0; i<=9; i++)
            printf("%d, ", ua[i]);
   // printf("\nComp:%d, Asig:%d\n",compQuick, asigQuick);
    //Correctness
    printf("\nQuick select after order: %d\n",quickSelect(ua,0,4,3));
    /* Average case
    for(n=100;n<=10000;n+=100){
        for(i=0;i<n;i++){
                a[i]=rand()%n;
        }
            heapSort(a,n,heap);
            quickSort(ua,0,n,quick);
    }
    */
    /* Worst case
    for(n=100;n<=10000;n+=100){
        a[0] = rand()%n;
        ua[0] = rand()%n;
        for(i=0;i<n;i++){
                a[i]=a[i-1] + rand()%n;
                ua[i]=ua[i-1] + rand()%n;
        }
            heapSort(a,n,heapWorst);
           // quickSort(a,0,n,quickWorst);
    }
    */
    return 0;

}


