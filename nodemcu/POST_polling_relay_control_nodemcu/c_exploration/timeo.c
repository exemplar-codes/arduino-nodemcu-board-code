#include <stdio.h>
// https://chat.openai.com/share/d9ea5770-9f2d-4b55-9d78-f0a244937b1a
#include <stdlib.h>
#include <unistd.h>

typedef void (*CallbackFunction)(void);

void setTimeout(unsigned int delayMs, CallbackFunction callback) {
    usleep(delayMs * 1000); // Convert delay to microseconds
    callback(); // Call the callback function after the delay
}

// Example callback function
void myCallback() {
    printf("Callback executed after delay.\n");
}

int main() {
    printf("Starting...\n");

    // Call setTimeout with delay and callback
    setTimeout(2000, myCallback); // Delay of 2000 milliseconds (2 seconds)

    printf("Continuing...\n");

    return 0;
}
#include <stdio.h>

void outerFunction() {
    printf("Inside outer function\n");

    void innerFunction() {
        printf("Inside inner function\n");
    }

    innerFunction(); // Call the nested function
}

int main() {
    int x = 2;
    printf("%s", (char) x);
    // outerFunction(); // Call the outer function

    return 0;
}
