#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include <cmath>

class SearchAlgorithms {
public:
    int linearSearch(int array[], int length, int target);
    int binarySearchRec(int array[], int length, int target);
    int binarySearchIter(int array[], int length, int target);
    int jumpSearch(int array[], int length, int target);
    int exponentialSearch(int array[], int length, int target);
private:
    int binarySearchwLR(int array[], int length, int target, int left, int right);
};

#endif // SEARCH_ALGORITHMS_H