//
// Created by martin on 24.10.16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecc.h"
#include "constants.h"

void encode_word(unsigned char *input_buffer, unsigned char *code_word, int size) {
    encode_data(input_buffer, size, code_word);
}

void print_to_output(unsigned char **interlaced_result, FILE *fout, int words_count, int last_word_byte){
    int i, j;
    for (i = 0; i < DATA_BYTES + NPAR; ++i) {
        for (j = 0; j < words_count; ++j) {
            if(j < words_count - 1 || i < last_word_byte){
                fputc(interlaced_result[j][i], fout);
//                printf("%d%d\n", j, i);
            }
        }
    }
}

unsigned char** interleaving(unsigned char **interlaced_result, unsigned char *code_word, int bytes_read, int words_count){
    if(words_count == 0){
        interlaced_result = malloc(sizeof(unsigned char*));
    } else {
        interlaced_result = realloc(interlaced_result, sizeof(unsigned char*) * (words_count + 1));
    }
    interlaced_result[words_count] = malloc(bytes_read * sizeof(unsigned char*));
    memcpy(interlaced_result[words_count], code_word, bytes_read);
    printf("INTE: %s", interlaced_result[words_count]);
    return interlaced_result;
}

void add_to_output(unsigned char *code_word, FILE *fout, int size){
    fwrite(code_word, sizeof(unsigned char), size, fout);
}

char * create_output_file_name(char *output_file_name, char *input_file_name) {
    output_file_name = malloc(strlen(input_file_name)+1+4);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".out");
    return output_file_name;
}

int main(int argc, char *argv[]) {

    int bytes_read, words_count = 0, last_word_bytes = 0;
    char *file_name, *file_name_out = "";
    FILE *fin;
    FILE *fout;
    unsigned char input_buffer[DATA_BYTES + 1];
    unsigned char code_word[DATA_BYTES+NPAR];

    unsigned char **interlaced_result;

    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    file_name = argv[1];
    file_name_out = create_output_file_name(file_name_out, file_name);
    fin = fopen(file_name , "r");
    fout = fopen(file_name_out, "w");


    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES + 1));
    memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
    while((bytes_read = fread(input_buffer, sizeof(unsigned char), DATA_BYTES, fin)) > 0){
        printf("READ: \"%s\"\n", input_buffer);
        //zakodovani
        encode_word(input_buffer, code_word, bytes_read);

        printf("ENCO: \"%s\"\n", code_word);

        //TODO prokladani
        interlaced_result = interleaving(interlaced_result, code_word, bytes_read + NPAR, words_count);

        //clear memory
        memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES));
        memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
        ++words_count;
        last_word_bytes = bytes_read;
    }

    //tisk do vystupu
    printf("-%d-", last_word_bytes + NPAR);
    print_to_output(interlaced_result, fout, words_count, last_word_bytes + NPAR);

    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}

