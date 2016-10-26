//
// Created by martin on 24.10.16.
//
//
//int main(int argc, char *argv[]){
//    /* Now decode -- encoded codeword size must be passed */
//    decode_data(codeword, ML);
//
//    /* check if syndrome is all zeros */
//    if (check_syndrome () != 0) {
//        correct_errors_erasures(encoded, readBytes, 0, NULL);
//        printf("Corrected codeword: \"%s\"\n", codeword);
//    }
//
//    exit(0);
//}
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

void create_output_file_name(char *output_file_name, char *input_file_name) {
    output_file_name = malloc(strlen(input_file_name)+1+3);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".ok");
}

int main(int argc, char *argv[]) {

    int bytes_read;
    char *file_name, *file_name_out = "";
    FILE *fin;
    FILE *fout;
    unsigned char decoded_code_word[DATA_BYTES + 1];
    unsigned char code_word[DATA_BYTES+NPAR];


    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    file_name = argv[1];
    fin = fopen(file_name , "r");
    fout = fopen("input.out.ok", "w");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    memset(decoded_code_word, 0, (DATA_BYTES + 1) * sizeof(char));
    memset(code_word, 0, (DATA_BYTES + NPAR + 1) * sizeof(char));
    while((bytes_read = fread(code_word, sizeof(unsigned char), DATA_BYTES + NPAR, fin)) > 0) {
        //TODO prokladani
        //zakodovani
        decode_data(code_word, bytes_read);
        //TODO correct errors
        if (check_syndrome() != 0) {
            correct_errors_erasures(code_word, bytes_read, 0, NULL);
            printf("Corrected codeword: \"%s\"\n", code_word);
        }
        memcpy(decoded_code_word, code_word, bytes_read - NPAR);
        decoded_code_word[bytes_read - NPAR] = '\0';
        //tisk do vystupu
        add_to_output(decoded_code_word, fout, bytes_read - NPAR + 1);

        //vycisteni pameti
        memset(decoded_code_word, 0, (DATA_BYTES) * sizeof(char));
        memset(code_word, 0, (DATA_BYTES + NPAR) * sizeof(char));

    }
    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}
