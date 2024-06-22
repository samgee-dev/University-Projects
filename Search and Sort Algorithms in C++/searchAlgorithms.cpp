#include "searchAlgorithms.h"

int SearchAlgorithms::binarySearchRec(int array[], int length, int target) {
    // if (left > right) {
    //     return -1;
    // }

    // int middle = (left + right) / 2;

    // // if middle is target return middle

    // // less than middle
    // binarySearchRec(array, target, left, // right middle - 1);

    // // Otherwise or greater than middle
    // binarySearchRec(array, target, middle + 1, right)
    return 0;
}

int SearchAlgorithms::binarySearchIter(int array[], int length, int target) {
    int left = 0;
    int right = length - 1;

    while (left <= right) {
        int size = right - left;
        int middle = left + size / 2;

        if (array[middle] == target) {
            return middle;
        }

        if (array[middle] > target) {
            right = middle - 1;
            continue;
        } 
        else {
            left = middle + 1;
        }
    }

    return -1;
}

int SearchAlgorithms::jumpSearch(int array[], int length, int target) {
    int blockSize = std::sqrt(length);
    int start = 0;
    int next = start + blockSize;

    while (start < length) {
        if (target <= array[next - 1]) {
            for (int i = start; i < next; i++) {
                if (target == array[i]) {
                    return i;
                }
            }

            // Not found and cannot be elsewhere in array
            return -1;
        }

        start += blockSize;
        next += blockSize;

        if (next >= length) {
            next = length;
        }
    }

    return -1;
}

int SearchAlgorithms::exponentialSearch(int array[], int length, int target) {
    int bound = 1;

    while (bound < length && array[bound] < target) {
        if (target <= array[bound]) {
            return binarySearchwLR(array, length, target, bound/2, bound);
        }

        bound *= 2;
    }

    return binarySearchwLR(array, length, target, bound/2, length - 1);
}

int SearchAlgorithms::binarySearchwLR(int array[], int length, int target, int left, int right) {
    while (left <= right) {
        int size = right - left;
        int middle = left + size / 2;

        if (array[middle] == target) {
            return middle;
        }

        if (array[middle] > target) {
            right = middle - 1;
            continue;
        } 
        else {
            left = middle + 1;
        }
    }

    return -1;
}
