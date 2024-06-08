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

char MACHINE_CODE_FILE[] = "data.txt";

// PROTOTYPES

//Allows user to enter a machine code instruction. 
//If not valid, will be handled by future functions.
void EnterInstruction(char file_name[]);                                                // Done by Nelson

//Reads a line and checks if it is 32-bit.
void Read32BitLine(char target[], FILE *ptr, const int line_number);                    // Done by Abdurrahman

//Deduces the operation being performed by the input machine instruction, returns index.
int DeduceOperationFromInstruction(char instruction[], const int line_number);          // Done by Abdurrahman

//Convert binary char string into an integer representing its decimal expansion.
int BinaryToDecimal(char* bit_string);                                                  // Done by Abdurrahman

void DecodeFormatR(const char *instruction, int line_number, int op_idx);               // Done by Jin
void DecodeFormatI(const char *instruction, int line_number, int op_idx);               // Done by Jin
void DecodeFormatD(const char *instruction, int line_number, int op_idx);               // Done by Jin

int main() {

    // Let user input instructions until STOP
    EnterInstruction(MACHINE_CODE_FILE);
    //printf("\n");

    //Read machine code instructions line by line from file.
    FILE *read_ptr;
    read_ptr = fopen(MACHINE_CODE_FILE, "r");
    if (read_ptr == NULL) {
        printf("ERROR: Could not open file: %s\n", MACHINE_CODE_FILE);
        exit(0);
    }

    char current_instruction[33]; //33 chars for 32 bits + null terminator '\0';

    int current_line = 1;

    while(fgets(current_instruction, 100, read_ptr) != NULL) {
        Read32BitLine(current_instruction, read_ptr, current_line);
        int operation_idx = DeduceOperationFromInstruction(current_instruction, current_line);
        //printf("%s\n", current_instruction);

        switch(LEGV8_OPERATIONS[operation_idx].format) {
            case 'R':
                DecodeFormatR(current_instruction, current_line, operation_idx);
                break;
            case 'I':
                DecodeFormatI(current_instruction, current_line, operation_idx);
                break;
            case 'D':
                DecodeFormatD(current_instruction, current_line, operation_idx);
                break;
        }

        ++current_line;
    }

    fclose(read_ptr);


    return 0;
} //END main()


//FUNCTIONS WRITTEN BY NELSON
void EnterInstruction(char file_name[]) {

    //variables
    char bit_instruction[CHAR_SIZE]; // bit string with character limit of 1000
    int choice;

    //declaring file
    FILE* fptr;
    fptr = fopen(file_name , "w"); // uses "w" to write


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
} //END EnterInstruction()


//FUNCTIONS WRITTEN BY ABDURRAHMAN
int BinaryToDecimal(char* bit_string) {
    int length = strlen(bit_string);
    int decimal_expansion = 0;
    for(int i = 0, p = length - 1; i < length && p >= 0; ++i, --p) {
        decimal_expansion += (int)(bit_string[i] - '0') * (1 << p); //digit * 2^x;
    }
    return decimal_expansion;
} //END BinaryToDecimal(char* bit_string)

void Read32BitLine(char target[], FILE *ptr, const int line_number) {
    int c; //Stores read file characters.
    int char_count = 0;

    //While the currently fetched character is not equal to the EOF character...
    while((c = fgetc(ptr)) != EOF) {
        //Ignore spaces;
        if(c == ' ')
            continue;

        //Check if we have reached the end of a line.
        if(c == '\n') {
            //If so, check if we counted 32 characters. If 32 chars weren't counted,
            //then it is not a valid instruction in our context. So print error and exit program.
            if(char_count != 32) {
                printf("LINE %d - ERROR: Non-32 bit instruction detected!", line_number);
                exit(0);
            }
            target[char_count] = '\0'; //Attach null character to prevent garbage values from printing.
            return;
        } else if(c != '0' && c != '1' && c != '\n') {
            printf("LINE %d - ERROR: Non-binary format detected!", line_number);
            exit(0);
        }
        target[char_count] = c;
        ++char_count;
    }
    return;
} //END Read32BitLine()

int DeduceOperationFromInstruction(char instruction[], const int line_number) {
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
} //END DeduceOperationFromInstruction

//FUNCTIONS WRITTEN BY JIN
void DecodeFormatR(const char *instruction, int line_number, int op_idx) {
    int opcode_size = R_FORMAT_CHUNK_SIZES[0];
    int rm_size = R_FORMAT_CHUNK_SIZES[1];
    int shamt_size = R_FORMAT_CHUNK_SIZES[2];
    int rn_size = R_FORMAT_CHUNK_SIZES[3];
    int rd_size = R_FORMAT_CHUNK_SIZES[4];
    int offset = 0;

    offset += opcode_size;

    // Get Rm substring
    char rm_bin[rm_size + 1];
    strncpy(rm_bin, instruction + offset, rm_size);
    rm_bin[rm_size] = '\0';
    offset += rm_size;

    offset += shamt_size;

    // Get Rn substring
    char rn_bin[rn_size + 1];
    strncpy(rn_bin, instruction + offset, rn_size);
    rn_bin[rn_size] = '\0';
    offset += rn_size;

    // Get Rd substring
    char rd_bin[rd_size + 1];
    strncpy(rd_bin, instruction + offset, rd_size);
    rd_bin[rd_size] = '\0';

    int rm_int = BinaryToDecimal(rm_bin);
    int rn_int = BinaryToDecimal(rn_bin);
    int rd_int = BinaryToDecimal(rd_bin);

    printf("%s X%d, X%d, X%d\n", LEGV8_OPERATIONS[op_idx].assembly, rd_int, rn_int, rm_int);
}

void DecodeFormatI(const char *instruction, int line_number, int op_idx) {
    int opcode_size = I_FORMAT_CHUNK_SIZES[0];  // 10
    int imm_size = I_FORMAT_CHUNK_SIZES[1];     // 12
    int rn_size = I_FORMAT_CHUNK_SIZES[2];      // 5
    int rd_size = I_FORMAT_CHUNK_SIZES[3];      // 5
    int offset = 0;

    offset += opcode_size;   // offset = 10

    // Get immediate value
    char imm_bin[imm_size + 1];
    strncpy(imm_bin, instruction + offset, imm_size);
    imm_bin[imm_size] = '\0';
    offset += imm_size;     // offset = 22

    // Get Rn substring
    char rn_bin[rn_size + 1];
    strncpy(rn_bin, instruction + offset, rn_size);
    rn_bin[rn_size] = '\0';
    offset += rn_size;     // offset = 27

    // Get Rd substring
    char rd_bin[rd_size + 1];
    strncpy(rd_bin, instruction + offset, rd_size);
    rd_bin[rd_size] = '\0';

    int imm_int = BinaryToDecimal(imm_bin);
    int rn_int = BinaryToDecimal(rn_bin);
    int rd_int = BinaryToDecimal(rd_bin);

    printf("%s X%d, X%d, #%d\n", LEGV8_OPERATIONS[op_idx].assembly, rd_int, rn_int, imm_int);
}

void DecodeFormatD(const char *instruction, int line_number, int op_idx) {
    int opcode_size = D_FORMAT_CHUNK_SIZES[0];
    int addr_size = D_FORMAT_CHUNK_SIZES[1];
    int op2_size = D_FORMAT_CHUNK_SIZES[2];
    int rn_size = D_FORMAT_CHUNK_SIZES[3];
    int rt_size = D_FORMAT_CHUNK_SIZES[4];
    int offset = 0;

    offset += opcode_size;

    char addr_bin[addr_size + 1];
    strncpy(addr_bin, instruction + offset, addr_size);
    addr_bin[addr_size] = '\0';
    offset += addr_size;

    offset += op2_size;

    char rn_bin[rn_size + 1];
    strncpy(rn_bin, instruction + offset, rn_size);
    rn_bin[rn_size] = '\0';
    offset += rn_size;

    char rt_bin[rt_size + 1];
    strncpy(rt_bin, instruction + offset, rt_size);
    rt_bin[rt_size] = '\0';

    int addr_int = BinaryToDecimal(addr_bin);
    int rn_int = BinaryToDecimal(rn_bin);
    int rt_int = BinaryToDecimal(rt_bin);

    printf("%s X%d, [X%d, #%d]\n", LEGV8_OPERATIONS[op_idx].assembly, rt_int, rn_int, addr_int);
}
