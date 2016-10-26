//
// Created by martin on 24.10.16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecc.h"
#include "constants.h"

void encode_word(unsigned char *input_buffer, unsigned char *code_word) {
    encode_data(input_buffer, DATA_BYTES, code_word);
}

void add_to_output(unsigned char *code_word, FILE *fout, int size){
    fwrite(code_word, sizeof(unsigned char), size, fout);
}
//
//void create_output_file_name(char *output_file_name, char *input_file_name) {
//    output_file_name = malloc(strlen(input_file_name)+1+4);
//    strcpy(output_file_name, input_file_name);
//    strcat(output_file_name, ".out");
//}

int main(int argc, char *argv[]) {

    int bytes_read;
    char *file_name/*, *file_name_out = ""*/;
    FILE *fin;
    FILE *fout;
    unsigned char input_buffer[DATA_BYTES];
    unsigned char code_word[DATA_BYTES+NPAR];


    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    file_name = argv[1];
//    create_output_file_name(file_name_out, file_name);
    fin = fopen(file_name , "r");
    fout = fopen("input.out", "w");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES));
    memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
    while((bytes_read = fread(input_buffer, sizeof(unsigned char), DATA_BYTES, fin)) > 0){
        //zakodovani
        encode_word(input_buffer, code_word);
        //TODO prokladani

        //tisk do vystupu
        add_to_output(code_word, fout, bytes_read + NPAR);

        //clear memory
        memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES));
        memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
    }

    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}

