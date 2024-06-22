#include "sortingAlgorithms.h"


void SortingAlgorithms::bubbleSort(int array[], int length) {
    if (length < 0) {
        throw "invalid length";
    }

    for (int i = 0; i < length; i++) {
        bool isSorted = true;
        for (int j = 0; j < length - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(array, j, j + 1);
                isSorted = false;
            }
        }

        if (isSorted) {
            return;
        }
    }
}

void SortingAlgorithms::mergeSort(int* array, int length) {
    if (length < 2) {
        return;
    }

    int middle = length / 2;

    int* left = new int[middle];
    for (int i = 0; i < middle; i++) {
        left[i] = array[i];
    }

    int* right = new int[length - middle];
    for (int i = middle; i < length; i++) {
        right[i - middle] = array[i];
    }

    mergeSort(left, middle);
    mergeSort(right, length - middle);

    merge(left, middle, right, length - middle, array);


    delete[] left;
    delete[] right;

}

void SortingAlgorithms::quickSort(int array[], int length) {
    quickSort(array, 0, length - 1);

}

void SortingAlgorithms::countingSort(int array[], int length, int max) {
    int* counts = new int[max + 1]();

    for (int i = 0; i < length; i++) {
        counts[array[i]]++;
    }

    int arrayIndex = 0;
    for (int i = 0; i < (max + 1); i++) {
        for (int j = 0; j < counts[i]; j++) {
            array[arrayIndex++] = i;
        }
    }

    delete[] counts;
}

void SortingAlgorithms::bucketSort(int array[], int length, int numberOfBuckets) {
    // // Create bucket
    // list<list<int>> buckets;
    // for (int i = 0; i < numberOfBuckets; i++) {
    //     buckets.add(list<>())
    // }

    // // Put all elements in the correct bucket
    // for (int i = 0; i < length; i++) {
    //     buckets.get(array[i] / numberOfBuckets).add(array[i]);
    // }

    // // Sort elements in each bucket and add back to array
    // int i = 0;
    // for (auto bucket : buckets) {
    //     sort(bucket);
    //     for (auto item : bucket) {
    //         array[i++] = item;
    //     }
    // }
}

void SortingAlgorithms::swap(int array[], int first, int second) {
    int temp = array[first];
    array[first] = array[second];
    array[second] = temp;
}

void SortingAlgorithms::merge(int *left, int leftLength, int *right, int rightLength, int *result) {
    int i = 0, j = 0, k = 0;

    while (i < leftLength && j < rightLength) {
        if (left[i] <= right[j]) {
            result[k++] = left[i++];
        }
        else {
            result[k++] = right[j++];
        }
    }

    while (i < leftLength) {
        result[k++] = left[i++];
    }

    while (j < rightLength) {
        result[k++] = right[j++];
    }
}

void SortingAlgorithms::quickSort(int array[], int start, int end) {
    int length = end - start + 1;
    if (length <= 1) {
        return;
    }

    int pivot = end;
    int pivotElement = array[pivot];
    int boundary = start - 1;
    int currentIndex = start;

    while (currentIndex <= pivot) {
        if (pivotElement >= array[currentIndex]) {
            boundary++;
            swap(array, boundary, currentIndex);
        }

        currentIndex++;
    }

    quickSort(array, start, boundary - 1);
    quickSort(array, boundary + 1, end);
}
