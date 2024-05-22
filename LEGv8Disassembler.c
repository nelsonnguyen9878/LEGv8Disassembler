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

    char user_input;
    char bit_instruction[LIST_SIZE];


    FILE* fptr;
    fptr = fopen("instruction.txt" , "w");

    do{
        printf("Please enter a 32-bit instruction: ");
        scanf("%s", bit_instruction[i])

    }(while)

    if(fptr != NULL) {
        do {
            scanf("%s", &user_input);
            fputs(user_input, fptr);
        } while(user_input !='n');
    }



    if (fptr == NULL) {
        printf("File did not open successfully");
    }

    fclose(fptr);


}
