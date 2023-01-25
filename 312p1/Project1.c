#include <stdio.h>
#include <stdint.h>
#include "Project1.h"


void print_num(uint32_t num) {
    //binary number 2153791520
    //if binary executable has been entered and number entered, turn number into binary
    #ifdef BINARY
        for (int i = 31; i >= 0; i--) {
            printf("%d", (num >> i) & 1);
        }
        printf("\n");
    #endif

    //hexadecimal number 2271560481
    //if hexadecimal executable has been entered and number entered, turn number into hexadecimal
    #ifdef HEXADECIMAL
        printf("0x%x\n", num);
    #endif
}

//for either executable ran, the binary or hexadecimal outputs; this function converts to the complements
uint32_t complement(uint32_t num) {
    return ~num;
}

//main function
int main() {
    uint32_t num;
    int option;
    //prompt user to input number
    printf("Input number\n");
    scanf("%u", &num);

    while (1) {
        //Display options to user
        printf("Select option\n");
        printf(" 1 - display number\n");
        printf(" 2 - shift left circular one digit\n");
        printf(" 3 - shift right circular one digit\n");
        printf(" 4 - complement\n");
        printf(" 5 - exit\n");
        scanf("%d", &option);

        //by choice of user, run necessary functions, definitions, or break loop
        switch (option) {
            case 1:
                print_num(num);
                break;
            case 2:
                num = SLC(num);
                break;
            case 3:
                num = SRC(num);
                break;
            case 4:
                num = complement(num);
                break;
            case 5:
                return 0;
            default:
                printf("Invalid option\n");
                break;
        }
    }
}