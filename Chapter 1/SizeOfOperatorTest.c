// Chapter 1: It's All Binary
// sizeof() operator test
// Demonstrates that the int type is 4 bytes (32 bits) long.
// Created by Jackson Eshbaugh on 17.06.2024.

#include <stdio.h>

int main(int argc, char** argv) {
    printf("The size of an int is %lu bytes, or %lu bits.\n", sizeof(int), sizeof(int) * 8);
    return 0;
}

