#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_ITEMS 10000
#define TIME_LIMIT 120.0


FILE *csv;
int n, capacity;
int items[MAX_ITEMS];

int bf_binLoad[MAX_ITEMS];
int bf_assignment[MAX_ITEMS];
int bf_bestBins;
int bf_bestAssignment[MAX_ITEMS];
int bf_timedOut;
clock_t bf_startTime;

void bf_solve(int itemIndex, int numBins) {
    double elapsed = (double)(clock() - bf_startTime) / CLOCKS_PER_SEC;
    
    if (elapsed > TIME_LIMIT) {
        bf_timedOut = 1;
        return;
    }

    if (numBins >= bf_bestBins) return;
    
    if (bf_timedOut) return;

    if (itemIndex == n) {
        bf_bestBins = numBins;
        memcpy(bf_bestAssignment, bf_assignment, sizeof(int) * n);
        return;
    }

    for (int b = 0; b < numBins; b++) { //case 1: iterate through bins to see where fit

        if (bf_binLoad[b] + items[itemIndex] <= capacity) { //fit it in if less than capacity

            bf_assignment[itemIndex] = b;
            bf_binLoad[b] += items[itemIndex];
            bf_solve(itemIndex + 1, numBins);

            bf_binLoad[b] -= items[itemIndex];
        }
    }

    bf_assignment[itemIndex] = numBins; // case 2: open new bin
    bf_binLoad[numBins] = items[itemIndex];
    bf_solve(itemIndex + 1, numBins + 1);

    bf_binLoad[numBins] = 0;
}

int ffd_binLoad[MAX_ITEMS];
int ffd_assignment[MAX_ITEMS];
int ffd_sortedItems[MAX_ITEMS];

void ffd_sort() {
    memcpy(ffd_sortedItems, items, sizeof(int) * n);
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (ffd_sortedItems[j] < ffd_sortedItems[j + 1]) {
                int tmp = ffd_sortedItems[j];
                ffd_sortedItems[j] = ffd_sortedItems[j + 1];
                ffd_sortedItems[j + 1] = tmp;
            }
}

int ffd_solve() {
    ffd_sort();
    memset(ffd_binLoad, 0, sizeof(ffd_binLoad));
    int numBins = 0;

    for (int i = 0; i < n; i++) {
        int placed = 0;
        for (int b = 0; b < numBins; b++) {
            if (ffd_binLoad[b] + ffd_sortedItems[i] <= capacity) {
                ffd_binLoad[b] += ffd_sortedItems[i];
                placed = 1;
                break;
            }
        }
        if (!placed) {
            ffd_binLoad[numBins] = ffd_sortedItems[i];
            numBins++;
        }
    }
    return numBins;
}

void generateItems(int seed) {
    srand(seed);
    for (int i = 0; i < n; i++)
        items[i] = (rand() % (capacity - 1)) + 1;
}

void runTest(int testN, int testCapacity, int seed) {
  
    n = testN;
    capacity = testCapacity;
    generateItems(seed);

    printf("\n");
    printf("n = %-3d, Capacity = %-3d  \n", n, capacity, seed);
    printf("==============================================\n");

    printf("Items: [ ");
    for (int i = 0; i < n; i++) printf("%d ", items[i]);
    printf("]\n\n");

    double bf_time = -1;
    int bf_result = -1;

    memset(bf_binLoad, 0, sizeof(bf_binLoad));
    bf_bestBins = n;
    bf_timedOut = 0;
    bf_startTime = clock();

    bf_solve(0, 0);

    bf_time = (double)(clock() - bf_startTime) / CLOCKS_PER_SEC;

    // if (bf_timedOut) {
    //     printf("  Brute Force : timed out (> %.0fs)\n", TIME_LIMIT);
    // } else {
    bf_result = bf_bestBins;
    printf("  Brute Force : %d bins | Time: %.6f sec\n", bf_result, bf_time);
    // }

    clock_t ffd_start = clock();
    int ffd_result = ffd_solve();
    double ffd_time = (double)(clock() - ffd_start) / CLOCKS_PER_SEC;

    printf("  FFD Greedy  : %d bins | Time: %.6f sec\n", ffd_result, ffd_time);


    if (bf_result != -1 && !bf_timedOut) { //results were obtained, under time
        if (bf_result == ffd_result)
            printf("  Result      : Both are %d bins\n", bf_result);
        else
            printf("  Result      : BF=%d, FFD=%d (FFD used %d extra)\n",
                   bf_result, ffd_result, ffd_result - bf_result);

        if (bf_time > 0)
            printf("  Comparison     : FFD was %.1fx faster\n", bf_time / ffd_time);
    }

    if (!bf_timedOut && bf_result != -1)
        fprintf(csv, "%d,%d,%.6f,%d,%.6f\n", n, bf_result, bf_time, ffd_result, ffd_time);
    else
        fprintf(csv, "%d,N/A,N/A,%d,%.6f\n", n, ffd_result, ffd_time);

    printf("========================================================\n");
}

int main() {

    printf("Bin Packing - Performance Comparison \n");

    capacity = 50;
    int seed = 42;

    int compareSizes[] = {5, 8, 10, 12, 14, 16};
    int numCompare = 6;

    csv = fopen("results.csv", "w");
    fprintf(csv, "n,bf_bins,bf_time,ffd_bins,ffd_time\n");

    for (int i = 0; i < numCompare; i++)
        runTest(compareSizes[i], capacity, seed);

    printf(" \n\n");  
    printf(" Larger Inputs:\n");
    int ffdSizes[] = {20, 21, 22, 23, 24, 25, 30, 35, 50, 100};
    int numFFD = 10;
    for (int i = 0; i < numFFD; i++)
        runTest(ffdSizes[i], capacity, seed);

    
    printf("  DONE \n");
    

    return 0;
}