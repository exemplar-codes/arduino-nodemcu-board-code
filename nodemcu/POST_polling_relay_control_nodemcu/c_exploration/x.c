#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// https://chat.openai.com/share/d9ea5770-9f2d-4b55-9d78-f0a244937b1a
void cycleThroughURLs(void (*CallbackFunction)(const char *), const char *baseURL, const char *port, int initialIP, int numVariations) {
    if (baseURL == NULL) baseURL = "http://192.168.0.";
    if (port == NULL) port = ":3000";
    if (initialIP == 0) initialIP = 100;
    if (numVariations == 0) numVariations = 11;

    for (int i = 0; i < numVariations; i++) {
        char generatedURL[100]; // Adjust the size as needed
        sprintf(generatedURL, "%s%d%s", baseURL, initialIP + i, port);

        if (CallbackFunction != NULL) {
            CallbackFunction(generatedURL);
        } else {
            printf("%s\n", generatedURL);
        }
    }
}

char* serverUrl[100];

bool checkIfURLIsLive(const char *url) {
}

void setFoundServerURL(const char *url) {
    printf("Checking URL: %s\n", url);
    // add http code check
    bool foundServer = 0;
    if(foundServer)
        printf("Found server URL: %s\n", serverUrl);
}


void findServerInNetwork() {
    cycleThroughURLs(setFoundServerURL, NULL, NULL, 0, 0);
}

// serverUrl


int main() {
    // Call the function without arguments to use defaults
    findServerInNetwork();

    return 0;
}
