

#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 20

int n, capacity; // n is number of items typeshi
int items[MAX_ITEMS];
int binLoad[MAX_ITEMS];     // load of each bin
int assignment[MAX_ITEMS];  // which bin each item is currently assigned to
int bestBins;               // best no. of bins so far
int bestAssignment[MAX_ITEMS]; // best assignments


void solve(int itemIndex, int numBins) {

    if (numBins >= bestBins) return;

    // base case
    if (itemIndex == n) {
        bestBins = numBins;
        memcpy(bestAssignment, assignment, sizeof(int) * n); // dest, src, count
        return;
    }

    for (int b = 0; b < numBins; b++) { //case 1: fit currentt item to bin

        if (binLoad[b] + items[itemIndex] <= capacity) { //current load + item to fit is less then capacity, proceed

            assignment[itemIndex] = b;
            binLoad[b] += items[itemIndex];

            solve(itemIndex + 1, numBins); //ex. base case reach, pop

            // backtrack
            binLoad[b] -= items[itemIndex];
        }
    }

    assignment[itemIndex] = numBins;
    binLoad[numBins] = items[itemIndex];

    solve(itemIndex + 1, numBins + 1); //updated values

    // backtrack
    binLoad[numBins] = 0;
}

int main() {

    printf("Enter bin capacity: ");
    scanf("%d", &capacity);

    printf("Enter number of items: ");
    scanf("%d", &n);

    if (n > MAX_ITEMS) {
        printf(" maximum %d items supported only\n", MAX_ITEMS);
        return 1;
    }

    printf("Enter item sizes:\n");

    for (int i = 0; i < n; i++) {
        printf("  Item %d: ", i + 1);
        scanf("%d", &items[i]);

        if (items[i] > capacity) {
            printf(" item %d (size %d) exceeds bin capacity \n", i + 1, items[i]);
            return 1;
        }
    }

    bestBins = n; //start with worst case which is every item has own bin
    
    memset(binLoad, 0, sizeof(binLoad));
    memset(assignment, -1, sizeof(assignment));
    memset(bestAssignment, -1, sizeof(bestAssignment)); 

    solve(0, 0);

    printf("\n================================\n\n");


    printf("Minimum bins needed: %d\n\n", bestBins);

    for (int b = 0; b < bestBins; b++) {
        int load = 0;
        printf("Bin %d | Items: [ ", b + 1);
        for (int i = 0; i < n; i++) {
            if (bestAssignment[i] == b) {
                printf("%d ", items[i]);
                load += items[i];
            }
        }
        printf("] | Load: %d / %d\n", load, capacity);
    }

    printf("\n");
    return 0;
}