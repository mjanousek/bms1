//
// Created by martin on 24.10.16.
//

#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"
static const char *OUT_FILE_NAME = "encoded_output";

int INPUT_BUFFER_SIZE = 150; // pocet
int CODE_WORD_BUFFER_SIZE = 256; // pocet

void encode_word(char *input_buffer, char *code_word) {
    /*TODO tady muze byt problem s pretecenim*/
    encode_data(input_buffer, sizeof(input_buffer), code_word);
}

void add_to_output(char *code_word, FILE *fout){
//    printf("%s \n", code_word);
    fputs(code_word, fout);
}

int main(int argc, char *argv[]) {

    char *file_name;
    FILE *fin;
    FILE *fout;
    char input_buffer[INPUT_BUFFER_SIZE];
    char code_word[INPUT_BUFFER_SIZE];

    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    file_name = argv[1];
    fin = fopen(file_name , "r");
    fout = fopen("encoded_output" , "w");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    printf("Jdeme cist\n");
    while (fgets(input_buffer, INPUT_BUFFER_SIZE, fin) != NULL)
    {
        printf("buffer: %s\n", input_buffer);
        encode_word(input_buffer, code_word);
        add_to_output(code_word, fout);
    }

//    int erasures[16];
//    int nerasures = 0;
//    /* ************** */
//
    /* Encode data into codeword, adding NPAR parity bytes */
//    encode_data(msg, sizeof(msg), codeword);
//
//    printf("Encoded data is: \"%s\"\n", codeword);
//
//#define ML (sizeof (msg) + NPAR)
//
//
//    /* Add one error and two erasures */
//    byte_err(0x35, 3, codeword);
//
//    byte_err(0x23, 17, codeword);
//    byte_err(0x34, 19, codeword);
//
//
//    printf("with some errors: \"%s\"\n", codeword);
//
//    /* We need to indicate the position of the erasures.  Eraseure
//       positions are indexed (1 based) from the end of the message... */
//
//    erasures[nerasures++] = ML-17;
//    erasures[nerasures++] = ML-19;
    fclose(fin);
//    fclose(fout);

    return EXIT_SUCCESS;
}

