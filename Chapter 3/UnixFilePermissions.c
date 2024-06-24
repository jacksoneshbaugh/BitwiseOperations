// Chapter 3
// A simple Unix file permissions checker.
// Created by Jackson Eshbaugh on 18.06.2024.

#include <stdio.h>
#include <stdlib.h>

// File Permission Globals:
int READ = 4;
int WRITE = 2;
int EXECUTE = 1;

int main(int argc, char** argv) {

    // Allow for multiple executions, using infinite loop with quit flag.
    while(1) {

        printf("Welcome to Unix File Permission Checker!\nCreated by Jackson Eshbaugh.\n");

        char permissionsBuffer[4]; // Allocate 3 chars (3 numbers) for the permissions.
        printf("Enter a three-digit file permission: ");
        fgets(permissionsBuffer, sizeof(permissionsBuffer), stdin); // Only take in 3 chars (avoid an overflow)
        fflush(stdin);
        printf("\n");

        // As opposed to atoi(), use strtol(), and cast to int. strtol() is safer.
        int permissions = (int) strtol(permissionsBuffer, NULL, 10);

        // Check input for correctness:
        // * Must be >= 100 (3 characters in length)
        if(permissions < 100) {
            // Invalid permission provided.
            printf("Please try again. Provide a three-digit permission (a number >= 100).");
            continue;
        }

        // Use modular arithmetic to pull each digit off of the number.
        int everyonePermission = permissions % 10;

        // Remove the counted digit using integer division.
        permissions /= 10;

        // Do the same two more times.
        int ownerGroupPermission = permissions % 10;
        permissions /= 10;
        int ownerPermission = permissions % 10;

        // Now, we have each permission individually.
        // Next, we'll ask the user for a list of permissions to test with these given ones.
        // This will be done as a list of permissions, separated by spaces. Each permission contains
        // two characters. The first could be 'o', 'g', or 'e' (for owner, group, and everyone). The
        // next could be either 'r', 'w', or 'e' (for read, write, or execute).
        // The maximum number of arguments that could be passed is 3 * 3, or 9. In total characters,
        // that's 18 for non-whitespace, plus 8 whitespaces (the 9th doesn't need whitespace; it's at the
        // end of the string). Hence, we'll allocate a buffer with space for 27 characters (one extra
        // character for the null byte ('\0')), and the requested permissions to compare into this.

        char comparisonBuffer[27];
        printf("Enter each permission code you'd like to test, separated by a space.\n");
        printf("A permission code is a two character sequence. The first character specifies the person/people, and the second specifies the permission.\n");
        printf("First, use either \"o\",\"g\", or \"e\" (for the file's OWNER, the file owner's GROUP, and EVERYONE, respectively).\n");
        printf("Then, use either \"r\", \"w\", or \"e\" (for READ, WRITE, or EXECUTE).\n");
        printf("Now, enter your permission codes, separated by a space: ");

        fgets(comparisonBuffer, sizeof(comparisonBuffer), stdin); // Fetch the input to the comparison buffer.
        fflush(stdin);
        printf("\n");

        // Now, we'll loop through each character in the string and perform each test.
        // The loop should terminate when we reach the null char in the string.

        // Sample prompt: or ow oe\0
        // Each group of three chars contains a selector, a permission, and a separator. A
        // separator is either a space or '\0'.

        for (int i = 0; i < sizeof(comparisonBuffer); i += 3) {
            // Parse a group of three chars in this loop.

            switch(comparisonBuffer[i]) {
                // Owner, owner's group, or everyone
                case 'o':
                    switch(comparisonBuffer[i + 1]) {
                        // Read, write, or execute
                        case 'r':
                            if(ownerPermission & READ) {
                                printf("Owner can read the file.\n");
                                break;
                            }

                            printf("Owner can't read the file.\n");
                            break;

                        case 'w':
                            if(ownerPermission & WRITE) {
                                printf("Owner can write to the file.\n");
                                break;
                            }

                            printf("Owner can't write to the file.\n");
                            break;

                        case 'e':
                            if(ownerPermission & EXECUTE) {
                                printf("Owner can execute the file.\n");
                                break;
                            }

                            printf("Owner can't execute the file.\n");
                            break;
                    }
                    break;

                case 'g':
                    switch(comparisonBuffer[i + 1]) {
                        // Read, write, or execute
                        case 'r':
                            if(ownerGroupPermission & READ) {
                                printf("Owner's group can read the file.\n");
                                break;
                            }

                            printf("Owner's group can't read the file.\n");
                            break;

                        case 'w':
                            if(ownerGroupPermission & WRITE) {
                                printf("Owner's group can write to the file.\n");
                                break;
                            }

                            printf("Owner's group can't write to the file.\n");
                            break;

                        case 'e':
                            if(ownerGroupPermission & EXECUTE) {
                                printf("Owner's group can execute the file.\n");
                                break;
                            }

                            printf("Owner's group can't execute the file.\n");
                            break;
                    }
                    break;

                case 'e':
                    switch(comparisonBuffer[i + 1]) {
                        // Read, write, or execute
                        case 'r':
                            if(everyonePermission & READ) {
                                printf("Everyone can read the file.\n");
                                break;
                            }

                            printf("Everyone can't read the file.\n");
                            break;

                        case 'w':
                            if(everyonePermission & WRITE) {
                                printf("Everyone can write to the file.\n");
                                break;
                            }

                            printf("Everyone can't write to the file.\n");
                            break;

                        case 'e':
                            if(everyonePermission & EXECUTE) {
                                printf("Everyone can execute the file.\n");
                                break;
                            }

                            printf("Everyone can't execute the file.\n");
                            break;
                    }
                    break;
            }

            // Check if we need to loop again (if the third character == ' ')
            if(comparisonBuffer[i + 2] == ' ') {
                continue;
            } else {
                printf("Checked all permissions.\n");
                break;
            }

        }

        printf("Enter 'q' to quit, or enter any other character to check a new file's permissions: ");
        char entry = fgetc(stdin);
        fflush(stdin);
        printf("\n");

        if(entry == 'q') {
            printf("Goodbye!\n");
            exit(0);
        }

    }

}
