#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 10000
#define MAX_RAND 20000

int comp, asig;

void maxHeapify(int *a, int size, int i) {
    int largest;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    comp++;
    if (l < size && a[l] > a[i]) {
        largest = l;
    } else {
        largest = i;
    }
    comp++;
    if (r < size && a[r] > a[largest]) {
        largest = r;
    }
    if (largest != i) {
        int aux = a[largest];
        a[largest] = a[i];
        a[i] = aux;
        asig += 3;
        maxHeapify(a, size, largest);
    }
}

void bottomUp(int *a, int size) {
    int i;
    for (i = (size - 1) / 2; i >= 0; i--) {
        maxHeapify(a, size, i);
    }
}

void heapPush(int *b, int key, int *dim) {
    int i = *dim;
    comp++;
    while (i > 0 && b[(i - 1) / 2] < key) {
        comp++;
        b[i] = b[(i - 1) / 2];
        asig++;
        i = (i - 1) / 2;
    }
    b[i] = key;
    asig++;
    *dim = *dim + 1;
}

void topDown(int *a, int *b, int lng) {
    int dim = 0, i;
    for (i = 0; i < lng; i++) {
        heapPush(b, a[i], &dim);
    }
}

void buildMaxHeap(int *a, int lng) {
    int i;
    for (i = (lng - 1) / 2; i >= 0; i--) {
        maxHeapify(a, lng, i);
    }
}

void heapSort(int *a, int lng) {
    int i, size = lng;
    buildMaxHeap(a, lng);
    for (i = lng - 1; i > 0;  i--) {
        int aux = a[0];
        a[0] = a[i];
        a[i] = aux;
        asig += 3;
        size = size - 1;
        maxHeapify(a, size, 0);
    }
}

void printArray(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void averageCase(int *a, int dim) {
    int i;
    for (i = 0; i < dim; i++) {
        a[i] = rand();
    }
}

void worstCase(int *a, int dim) {
    int i;
    a[0] = 0;
    for (i = 0; i < dim; i++) {
        a[i] = a[i - 1] + rand() % MAX_RAND;
    }
}

void cpyArray(int *a, int *source, int dim) {
    int i;
    for (i = 0; i < dim; i++) {
        a[i] = source[i];
    }
}

int main() {
    int A1[10] = { 16,4,10,14,7,9,3,2,8,1 };
    int B[10] = { 4,1,3,2,16,9,10,14,8,7 };
    int Baux[10];
    int C[5] = { 7,4,3,1,2 };
    int i=0;
    bottomUp(A1,10);
    for(i=0;i<10;i++){
        printf("%d ",A1[i]);
    }
    topDown(B,Baux,10);
    printf("\n");
    for(i=0;i<10;i++){
        printf("%d ",Baux[i]);
    }
    heapSort(C,5);
    printf("\n");
    for(i=0;i<5;i++){
        printf("%d ",C[i]);
    }
/*
    FILE *f = fopen("Heap Average Case.csv", "w");
    FILE *g = fopen("Heap Worst Case.csv", "w");

    fprintf(f, "%s", "N, Bottom Up, Top Down\n");
    fprintf(g, "%s", "N, Bottom Up, Top Down\n");

    int a[MAX_SIZE], b[MAX_SIZE], aux[MAX_SIZE], n;
    comp = asig = 0;

    srand(time(NULL));

    // Average Case Generator
    for (n = 100; n <= 10000; n += 100) {
        int i;
        int cbu = 0, abu = 0;
        int ctd = 0, atd = 0;
        fprintf(f, "%d, ", n);
        for (i = 0; i < 5; i++) {
            averageCase(aux, n);
            comp = asig = 0;
            cpyArray(a, aux, n);

            bottomUp(a, n);
            cbu += comp;
            abu += asig;
            comp = asig = 0;
            cpyArray(a, aux, n);

            topDown(a, b, n);
            ctd += comp;
            atd += asig;
        }
        fprintf(f, "%.2lf, %.2lf\n", (cbu + abu) / 5.0, (ctd + atd) / 5.0);
    }

    // Worst Case Generator
    for (n = 100; n <= 10000; n += 100) {
        fprintf(g, "%d, ", n);
        worstCase(aux, n);
        comp = asig = 0;
        cpyArray(a, aux, n);
        bottomUp(a, n);
        fprintf(g, "%d, ", asig + comp);
        asig = comp = 0;
        cpyArray(a, aux, n);
        topDown(a, b, n);
        fprintf(g, "%d\n", asig + comp);
    }
*/
    return 0;
}



