// Prints a decimal number in binary.
// Created by Jackson Eshbaugh on 20.06.2024.

#import <stdio.h>
#import <stdlib.h>
#import <string.h>

void printBinary(unsigned int decimal) {

    // To determine size (in bits), we multiply the maximum size of an unsigned int by 8 (to convert from bytes to bits).
    int size = sizeof(decimal) * 8;

    // This counts the leading zeros of the number, then subtracts them from the size.
    // Hence, we start at the first bit set to 1 (unless decimal == 0)
    size -= __builtin_clz(decimal);

    if(size == 0) size = 1; // Handle zero case, we need one space to display "0."

    int curr = 0;
    char binary[size + 1];
    for(unsigned int mask = 1 << (size - 1); mask != 0; mask >>= 1) {
        if((decimal & mask) != 0) {
            binary[curr] = '1';
        } else {
            binary[curr] = '0';
        }
        curr++;
    }

    binary[curr] = '\0';

    printf("%s", binary);

}

int main(int argc, char** argv) {

    printf("# Decimal to Binary #\n");
    printf("by Jackson Eshbaugh\n");
    char inputBuffer[33];

    while(1) {
        printf("Enter a number in Base 10 (Decimal), or enter q to quit: ");
        if(fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            fflush(stdin);
            printf("\n");

            if (strcmp(inputBuffer, "q\n") == 0) {
                printf("Goodbye!\n");
                exit(0);
            }

            int input = (int) strtol(inputBuffer, NULL, 10);

            printf("In binary, %i is: ", input);
            printBinary(input);
            printf("\n");
        } else {
            printf("Error reading the input. Try again.\n");
        }
    }

}