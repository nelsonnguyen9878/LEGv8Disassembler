/*
 * Filename: LEGv8Disassembler.c
 * Date: 05/21/2024
 * CS205 Sp '24 Wiese
 * Programmers: Abdurrahman Alyajouri, Jin Pereyras , Nelson Nguyen
 *
 * Test Cases 1:
 *
 * Test Cases 2:
 *
 * Test Cases 3:
 *
 */

// LIBRARIES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// PROTOTYPES
void EnterInstruction();


const int LIST_SIZE = 1000;


// beginning of main()
int main() {
    EnterInstruction();




    return 0;
}
// end of main()

void EnterInstruction() {

    char user_input[LIST_SIZE];
    char bit_instruction[LIST_SIZE];




    FILE* fptr;
    fptr = fopen("instruction.txt" , "w");

    /*
    do{

        scanf("%s", bit_instruction[i])

    }(while)
     */

    int choice;
    if(fptr != NULL) {

        do {

            printf("Please enter a 32-bit instruction: ");
            fgets(user_input, LIST_SIZE, stdin);
            fputs(user_input, fptr);


            printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
            scanf("%d", &choice);
            while( choice != 0 && choice != 1) {
                printf("That is not a valid choice.");
                printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
                scanf("%d", &choice);
            }
        }while( choice != 0);
    }



    if (fptr == NULL) {
        printf("File did not open successfully");
    }

    fclose(fptr);


}
