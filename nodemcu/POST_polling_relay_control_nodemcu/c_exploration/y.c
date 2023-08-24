#include <stdio.h>
#include <stdlib.h>

// https://chat.openai.com/share/d9ea5770-9f2d-4b55-9d78-f0a244937b1a
// Function pointer type for mapping and filtering functions
typedef int (*Callback)(int);
typedef int (*Callback)(int);

// Map function: applies the provided function to each element of the array
int* map(int arr[], int length, Callback mapper) {
    int* result = malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        result[i] = mapper(arr[i]);
    }
    return result;
}

// Filter function: filters the array based on the provided predicate function
int* filter(int arr[], int length, Callback predicate, int* resultLength) {
    int* result = malloc(length * sizeof(int));
    *resultLength = 0;
    for (int i = 0; i < length; i++) {
        if (predicate(arr[i])) {
            result[(*resultLength)++] = arr[i];
        }
    }
    return result;
}

// Mapping function example: square the number
int square(int num) {
    return num * num;
}

// Filtering function example: check if the number is even
int isEven(int num) {
    return num % 2 == 0;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int length = sizeof(arr) / sizeof(arr[0]);

    int* squared = map(arr, length, square);
    int squaredLength = length;

    int* evenNumbers = filter(arr, length, isEven, &squaredLength);

    printf("Original array: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Squared array: ");
    for (int i = 0; i < squaredLength; i++) {
        printf("%d ", squared[i]);
    }
    printf("\n");

    printf("Even numbers array: ");
    for (int i = 0; i < squaredLength; i++) {
        printf("%d ", evenNumbers[i]);
    }
    printf("\n");

    free(squared);
    free(evenNumbers);

    return 0;
}
