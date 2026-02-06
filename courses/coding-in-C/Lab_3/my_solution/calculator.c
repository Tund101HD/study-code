#include <stdlib.h>
#include <stdio.h>

void println(char* input);

int main() {
    println("Welcome to calculator.");
    println("");

    float erste_zahl = 0;
    float zweite_zahl = 0;
    char operation = '\0';
    float result = 0;

    printf("Enter the first number: ");
    scanf("%f", &erste_zahl);

    printf("Enter the second number: ");
    scanf("%f", &zweite_zahl);

    printf("Choose an arithmetic operation (+, -, *, /): ");
    scanf(" %c", &operation);

    switch (operation) {
        case '+':
            result = erste_zahl + zweite_zahl;
            break;
        case '-':
            result = erste_zahl - zweite_zahl;
            break;
        case '*':
            result = erste_zahl * zweite_zahl;
            break;
        case '/':
            if (zweite_zahl == 0) {
                println("Error: Division by zero is not allowed.");
                return 1;
            }
            result = erste_zahl / zweite_zahl;
            break;
        default:
            println("Error: Invalid operation selected.");
            return 1;
    }

    printf("The result of %.2f %c %.2f = %.2f\n", erste_zahl, operation, zweite_zahl, result);

    return 0;
}

void println(char* input)
{
    printf("%s\n", input);
}
