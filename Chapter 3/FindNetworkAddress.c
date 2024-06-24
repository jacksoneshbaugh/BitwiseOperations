// Using a device's IP address and the subnet mask, determines the
// network address.
// Created by Jackson Eshbaugh on 23.06.2024.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findNetworkAddress(int subnetMaskByte, int deviceIpByte) {
    // Using an AND operation only gives a 1 when both corresponding values are 1.
    // This is our wanted behavior, we want to zero out any host bit that isn't
    // given to the subnet. When a bit is given to the subnet, that bit is set to 1 in the mask.
    return subnetMaskByte & deviceIpByte;
}

int main(int argc, char **argv) {

    char iPBuffer[16];
    char subnetBuffer[16];

    while(1) {
        printf("Enter a device's local IPv4 address or q to quit: ");
        if(fgets(iPBuffer, sizeof(iPBuffer), stdin) != NULL) {
            fflush(stdin);

            if (strcmp(iPBuffer, "q\n") == 0) {
                printf("Goodbye!\n");
                exit(0);
            }

            printf("Enter a subnet mask: ");
            if(fgets(subnetBuffer, sizeof(subnetBuffer), stdin) != NULL) {
                fflush(stdin);

                // Now that we have a local IP and subnet mask, we need to
                // parse each byte as its own variable.

                char *iPList = strtok(iPBuffer, ".");
                int iP[4];

                int i = 0;
                while(iPList != NULL) {
                    iP[i] = (int) strtol(iPList, NULL, 10);
                    iPList = strtok(NULL, ".");
                    i++;
                }

                char *subnetList = strtok(subnetBuffer, ".");
                int subnet[4];

                i = 0;
                while(subnetList != NULL) {
                    subnet[i] = (int) strtol(subnetList, NULL, 10);
                    subnetList = strtok(NULL, ".");
                    i++;
                }

                printf("The network address is ");

                for(int j = 0; j < 4; ++j) {
                    printf("%i.", findNetworkAddress(subnet[j], iP[j]));
                }
                printf("\n");


            } else {
                printf("Error reading the input. Try again.\n");
                continue;
            }

        } else {
            printf("Error reading the input. Try again.\n");
        }
    }

}