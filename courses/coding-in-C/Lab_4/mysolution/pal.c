#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_MAX_LENGTH 100
char* getInput();

int main() {
    printf("Enter a string to check if it's a palindrome: ");
    char* input = getInput();
    if (input == NULL) {
        return 1;
    }
    input[strcspn(input, "\n")] = 0;

    int length = strlen(input);
    int isPalindrome = 1;

    for (int i = 0; i < length / 2; i++) {
        if (input[i] != input[length - 1 - i]) {
            isPalindrome = 0;
            break;
        }
    }

    if (isPalindrome) {
        printf("\"%s\" is a palindrome.\n", input);
    } else {
        printf("\"%s\" is not a palindrome.\n", input);
    }

    free(input);
    return 0;
}

char* getInput() {
    char* ret = malloc(INPUT_MAX_LENGTH * sizeof(char));
    if (ret == NULL) {
        return NULL;
    }
    if (fgets(ret, INPUT_MAX_LENGTH, stdin) == NULL) {
        free(ret);
        return NULL;
    }
    return ret;
}
