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


// global variable
const int CHAR_SIZE = 1000;

typedef struct OperationInfo {
    char *opcode;
    char *assembly;
    char format;
} OperationInfo;

//Look-up array of opcodes and corresponding information
const OperationInfo LEGV8_OPERATIONS[] = {
        {"10001011000", "ADD", 'R'},
        {"11001011000", "SUB", 'R'},
        {"1001000100", "ADDI", 'I'},
        {"1101000100", "SUBI", 'I'},
        {"11111000010", "LDUR", 'D'},
        {"11111000000", "STUR", 'D'}
};

const unsigned int NUMOPERATIONS = sizeof(LEGV8_OPERATIONS) / sizeof(LEGV8_OPERATIONS[0]);

const int R_FORMAT_CHUNK_SIZES[] = {11, 5, 6, 5, 5};
const int I_FORMAT_CHUNK_SIZES[] = {10, 12, 2, 5, 5};
const int D_FORMAT_CHUNK_SIZES[] = {11, 9, 2, 5, 5};

const char MACHINE_CODE_FILE[] = "data.txt";


// PROTOTYPES
void EnterInstruction(char file_name[]); // Done by Nelson

// Done by Abdurrahman
//Reads a line and checks if it is 32-bit.
void Read32BitLine(char target[], FILE *ptr, int line_number);
//Deduces the operation being performed by the input machine instruction, returns index.
int DeduceOperationFromInstruction(char instruction[], int line_number);
//Convert binary char string into an integer representing its decimal expansion.
int BinaryToDecimal(char* bit_string);








// beginning of main()
int main() {

    // Let user input instructions until STOP
    EnterInstruction(MACHINE_CODE_FILE);
    printf("\n");

    //Read machine code instructions line by line from file.
    FILE *read_ptr;
    read_ptr = fopen(MACHINE_CODE_FILE, "r");
    if (read_ptr == NULL) {
        printf("ERROR: Could not open file: %s\n", MACHINE_CODE_FILE);
        exit(0);
    }

    char current_instruction[33]; //33 chars for 32 bits + null terminator '\0';

    int line = 1;

    while(!feof(read_ptr)) {
        Read32BitLine(current_instruction, read_ptr, line);
        int operation_idx = DeduceOperationFromInstruction(current_instruction, line);
        printf("%s\n", current_instruction);

        /*
        switch(LEGV8_OPERATIONS[operation_idx].format) {
            case 'R':
                break;
            case 'I':
                break;
            case 'D':
                break;
        }
         */
        ++line;
    }

    fclose(read_ptr);


    return 0;
}
// end of main()


// beginning of EnterInstruction()
void EnterInstruction(char file_name[]) {

    //  variables
    char bit_instruction[CHAR_SIZE]; // bit string with character limit of 1000
    //memset (bit_instruction, '\0', 32 );
    int choice;

    // declaring file
    FILE* fptr;
    fptr = fopen(file_name , "w"); // uses "w" to write


   // file is opened and no errors are reached
   // grabs user instruction until user stops
    // file message if not opened successfully
    if (fptr == NULL) {
        printf("File did not open successfully");
        exit(0);
    }

    // do while loop to take in instructions
    do {

        // user_input
        printf("Please enter a 32-bit instruction: ");
        fflush(stdin);

        if(!feof(stdin)) {
            fgets(bit_instruction, CHAR_SIZE, stdin);
            fputs(bit_instruction, fptr);
        }





        // choice validation
        printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
        scanf("%d", &choice);
        while( choice != 0 && choice != 1) { // while loop until conditions are met
            printf("That is not a valid choice.\n");
            printf("Would you like to enter another bit instruction?(0 for no, 1 for yes): ");
            scanf("%d", &choice);
        }
    }while( choice != 0 ); // breaks out of loop if user input is 0



    // close file
    fclose(fptr);
} // end of EnterInstruction()

// beginning of BinaryToDecimal()
int BinaryToDecimal(char* bit_string) {
    int length = strlen(bit_string);
    int decimal_expansion = 0;
    for(int i = 0, p = length - 1; i < length && p >= 0; ++i, --p) {
        decimal_expansion += (int)(bit_string[i] - '0') * (1 << p); //digit * 2^x;
    }
    return decimal_expansion;
} // end of BinaryToDecimal


// beginning of Read32BitLine()
void Read32BitLine(char target[], FILE *ptr, int line_number) {
    int c;
    int char_count = 0;
    while((c = fgetc(ptr)) != EOF) {
        if(c == '\n') {
            if(char_count != 32) {
                printf("LINE %d - ERROR: Non-32 bit instruction detected!", line_number);
                exit(0);
            }
            target[char_count] = '\0';
            return;
        } else if(c != '0' && c != '1' && c != '\n') {
            printf("LINE %d - ERROR: Non-binary format detected!", line_number);
            exit(0);
        }
        target[char_count] = c;
        ++char_count;
    }
    return;
} // end of Read32BitLine()


// beginning DeduceOperationFromInstruction()
int DeduceOperationFromInstruction(char instruction[], int line_number) {
    int found;
    //Perform linear search for every defined opcode at the start of the instruction
    for(int i = 0; i < NUMOPERATIONS; ++i) {
        found = 1;
        //Find first matching opcode from our lookup table. This works due to the opcodes not being substrings of each other.
        for(int j = 0; j < strlen(LEGV8_OPERATIONS[i].opcode); ++j) {
            if(instruction[j] != LEGV8_OPERATIONS[i].opcode[j]) {
                found = 0;
                break;
            }
        }
        if(found)
            return i; //first match will return index to the opcode that matched in LEGv8_Operations.
    }
    //At this point, opcode not found, so not a valid instruction in our context.
    printf("LINE %d - ERROR: Did not find valid operation code!", line_number);
    exit(0);
} // end of DeduceOperationFromInstruction
