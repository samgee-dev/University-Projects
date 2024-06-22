#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <memory>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

class SortingAlgorithms {
public:
    void bubbleSort(int array[], int length);
    void mergeSort(int* array, int length);
    void quickSort(int array[], int length);
    void countingSort(int array[], int length, int max);
    void bucketSort(int array[], int length, int numberOfBuckets);

private:
    void swap(int array[], int first, int second);
    void merge(int* left, int leftLength, int* right, int rightLength, int* result);
    void quickSort(int array[], int start, int end);
};

#endif // SORTING_ALGORITHMS_H