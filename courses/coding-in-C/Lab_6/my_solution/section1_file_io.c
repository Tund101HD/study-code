#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
void demo_fgetc(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading (fgetc)\n", filename);
        printf("-> Make sure the file exists in the current working directory\n");
        printf("-> Or provide an absolute/relative path to the file\n");
        return;
    }

    printf("\n=== fgetc() Demo ===\n");
    printf("Reading first 50 characters:\n");

    int ch;
    int count = 0;
    while ((ch = fgetc(file)) != EOF && count < 50) {
        putchar(ch);
        count++;
    }
    printf("\n");

    fclose(file);
}

/*
 * Demonstrates reading file line by line using fgets()
 */
void demo_fgets(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading (fgets)\n", filename);
        return;
    }

    printf("\n=== fgets() Demo ===\n");
    printf("Reading first 3 lines:\n");

    char buffer[BUFFER_SIZE];
    int line_count = 0;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL && line_count < 3) {
        printf("Line %d: %s", line_count + 1, buffer);
        line_count++;
    }

    fclose(file);
}

/*
 * Demonstrates formatted reading using fscanf()
 */
void demo_fscanf(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading (fscanf)\n", filename);
        return;
    }

    printf("\n=== fscanf() Demo ===\n");
    printf("Reading structured data (time, probability):\n");

    float time;
    double probability;
    int count = 0;

    while (fscanf(file, "%f %lf", &time, &probability) == 2 && count < 5) {
        printf("Sample %d: time=%.2f s, probability=%.4f\n",
               count + 1, time, probability);
        count++;
    }

    fclose(file);
}

/*
 * Demonstrates writing characters using fputc()
 */
void demo_fputc(void) {
    const char *filename = "output_fputc.txt";
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing (fputc)\n", filename);
        return;
    }

    printf("\n=== fputc() Demo ===\n");
    printf("Writing characters to '%s'\n", filename);

    const char *message = "Hello from fputc!\n";
    for (size_t i = 0; i < strlen(message); i++) {
        fputc(message[i], file);
    }

    fclose(file);
    printf("Successfully wrote to file\n");
}

/*
 * Demonstrates writing strings using fputs()
 */
void demo_fputs(void) {
    const char *filename = "output_fputs.txt";
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing (fputs)\n", filename);
        return;
    }

    printf("\n=== fputs() Demo ===\n");
    printf("Writing lines to '%s'\n", filename);

    fputs("Line 1: This is fputs() demo\n", file);
    fputs("Line 2: It writes complete strings\n", file);
    fputs("Line 3: Including newlines\n", file);

    fclose(file);
    printf("Successfully wrote to file\n");
}

/*
 * Demonstrates formatted writing using fprintf()
 */
void demo_fprintf(void) {
    const char *filename = "output_fprintf.txt";
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing (fprintf)\n", filename);
        return;
    }

    printf("\n=== fprintf() Demo ===\n");
    printf("Writing formatted data to '%s'\n", filename);

    fprintf(file, "Sensor Data Output\n");
    fprintf(file, "==================\n\n");

    for (int i = 0; i < 5; i++) {
        fprintf(file, "Measurement %d: time=%.2f s, value=%.4f\n",
                i + 1, i * 0.5, i * 0.1 + 0.5);
    }

    fclose(file);
    printf("Successfully wrote formatted data to file\n");
}

int main(void) {
    printf("Lab 6 - Section I: File I/O Operations\n");
    printf("=======================================\n");
    const char *input_file = "sensor1.txt";
    demo_fgetc(input_file);
    demo_fgets(input_file);
    demo_fscanf(input_file);
    demo_fputc();
    demo_fputs();
    demo_fprintf();

    return 0;
}
