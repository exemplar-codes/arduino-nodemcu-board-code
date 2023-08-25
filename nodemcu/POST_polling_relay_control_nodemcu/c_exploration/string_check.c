#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool* parseBooleanArray(const char *json_string, int *array_length) {
    // Ensure the JSON array starts with '[' and ends with ']'
    if (json_string[0] != '[' || json_string[strlen(json_string) - 1] != ']') {
        printf("Invalid JSON array format.\n");
        return NULL;
    }

    // Count the number of commas to determine the array length
    *array_length = 1; // At least one element
    for (const char *c = json_string; *c; ++c) {
        if (*c == ',') {
            ++(*array_length);
        }
    }

    bool *bool_array = malloc(*array_length * sizeof(bool));
    if (bool_array == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    const char *current = json_string + 1; // Skip '['
    for (int i = 0; i < *array_length; ++i) {
        if (strncmp(current, "true", 4) == 0) {
            bool_array[i] = true;
            current += 4;
        } else if (strncmp(current, "false", 5) == 0) {
            bool_array[i] = false;
            current += 5;
        } else {
            printf("Invalid boolean value.\n");
            free(bool_array);
            return NULL;
        }

        // Skip comma or whitespace
        while (*current && (*current == ',' || *current == ' ' || *current == '\t' || *current == '\n')) {
            ++current;
        }
    }

    return bool_array;
}

int main() {
    const char *json_string = "[false, true, false]";
    int array_length = 0;
    bool *bool_array = parseBooleanArray(json_string, &array_length);

    if (bool_array != NULL) {
        printf("Parsed boolean array:\n");
        for (int i = 0; i < array_length; ++i) {
            printf("%s\n", bool_array[i] ? "true" : "false");
        }

        free(bool_array);
    }

    return 0;
}
