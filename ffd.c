

#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100

int n, capacity;
int items[MAX_ITEMS];
int originalIndex[MAX_ITEMS]; 
int binLoad[MAX_ITEMS];    
int assignment[MAX_ITEMS];    



void sortDescending() {

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            if (items[j] < items[j + 1]) {
   
                int tmp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = tmp;
  
                tmp = originalIndex[j];
                originalIndex[j] = originalIndex[j + 1];
                originalIndex[j + 1] = tmp;
            }
        }
    }
}

int main() {

    printf("bin capacity: ");
    scanf("%d", &capacity);

    printf("Enter number of items: ");
    scanf("%d", &n);

    if (n > MAX_ITEMS) {
        printf("yo only maximum %d items supported.\n", MAX_ITEMS);
        return 1;
    }

    printf("Enter item sizes:\n");
    for (int i = 0; i < n; i++) {
        printf("  Item %d: ", i + 1);
        scanf("%d", &items[i]);
        originalIndex[i] = i + 1; 

        if (items[i] > capacity) {
            printf("  Error: item %d (size %d) exceeds bin capacity!\n", i + 1, items[i]);
            return 1;
        }
    }


    sortDescending();

    printf("\nSorted == [ ");
    for (int i = 0; i < n; i++) printf("%d ", items[i]);
    printf("]\n");


    memset(binLoad, 0, sizeof(binLoad));
    memset(assignment, -1, sizeof(assignment));

    int numBins = 0;

    for (int i = 0; i < n; i++) { //loop the items

        int placed = 0; //flag the item

        for (int b = 0; b < numBins; b++) { //loop the bins

            if (binLoad[b] + items[i] <= capacity) {

                assignment[i] = b;
                binLoad[b] += items[i];
                placed = 1;
                break; 
            }
        }

        if (!placed) { // case for opening a new bin
            assignment[i] = numBins;
            binLoad[numBins] = items[i];
            numBins++;
        }
    }

    printf("================================\n");
    printf("Bins used: %d\n\n", numBins);

    for (int b = 0; b < numBins; b++) {
        int load = 0;
        printf("Bin %d | Items: [ ", b + 1);
        for (int i = 0; i < n; i++) {
            if (assignment[i] == b) {
                printf("%d ", items[i]);
                load += items[i];
            }
        }
        printf("] | Load: %d / %d\n", load, capacity);
    }

    printf("\n");
    return 0;
}