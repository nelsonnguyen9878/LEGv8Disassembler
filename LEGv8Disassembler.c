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

// global variable
const int CHAR_SIZE = 1000;


// beginning of main()
int main() {
    EnterInstruction();




    return 0;
}
// end of main()


// beginning of EnterInstruction()
void EnterInstruction() {

    //  varaibles
    char bit_instruction[CHAR_SIZE]; // bit string with character limit of 1000
    int choice;

    // declaring file 
    FILE* fptr;
    fptr = fopen("instruction.txt" , "w"); // uses "w" to write 


   // file is opened and no errors are reached
   // grabs user instruction until user stops
    if(fptr != NULL) {
        // do while loop to take in instructions
        do {

            // user_input
            printf("Please enter a 32-bit instruction: ");
            fflush(stdin);
            fgets(bit_instruction, CHAR_SIZE, stdin);
            fputs(bit_instruction, fptr);

            // choice validation
            printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
            scanf("%d", &choice);
            while( choice != 0 && choice != 1) { // while loop until conditions are met
                printf("That is not a valid choice.\n");
                printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
                scanf("%d", &choice);
            }

        }while( choice != 0); // breaks out of loop if user input is 0
    }
    
    // file message if not opened succesfully
    if (fptr == NULL) {
        printf("File did not open successfully");
    }

    // close file
    fclose(fptr);
}
// end of EnterInstruction()
