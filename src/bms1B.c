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
#include <math.h>
#include "ecc.h"
#include "constants.h"

void encode_word(unsigned char *input_buffer, unsigned char *code_word) {
    encode_data(input_buffer, DATA_BYTES, code_word);
}

void add_to_output(unsigned char *code_word, FILE *fout, int size){
    fwrite(code_word, sizeof(unsigned char), size, fout);
}

char * create_output_file_name(char *output_file_name, char *input_file_name) {
    output_file_name = malloc(strlen(input_file_name)+1+3);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".ok");
    return output_file_name;
}

int main(int argc, char *argv[]) {

    int bytes_read;
    char *file_name, *file_name_out = "";
    FILE *fin, *fout;
    unsigned char decoded_code_word[DATA_BYTES + 1];

    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    file_name = argv[1];
    fin = fopen(file_name , "rb");
    file_name_out = create_output_file_name(file_name_out, file_name);
    fout = fopen(file_name_out, "w");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    fseek(fin, 0L, SEEK_END);
    int sizeOfFile = ftell(fin);
    printf("size of file: %d\n", sizeOfFile);
    //seeking to go to the beginning)
    rewind(fin);
    int word_count = ceil(((float) sizeOfFile) / (DATA_BYTES+NPAR));
    int last_word_bytes = sizeOfFile % (DATA_BYTES+NPAR);
    unsigned char file_in_memory[word_count][DATA_BYTES+NPAR];
    unsigned char input_buffer[word_count];
    last_word_bytes = (last_word_bytes == 0) ? DATA_BYTES+NPAR : last_word_bytes;

    int i = 0;
    int word_lenght = word_count;
    printf("Number of words %d\n", word_lenght);
    while((bytes_read = fread(input_buffer, sizeof(unsigned char), word_lenght, fin)) > 0) {
//        printf("bytes read: %d\n", bytes_read);
        for (int j = 0; j < bytes_read; ++j) {
            file_in_memory[j][i] = input_buffer[j];
//            printf("%d, %d: %c\n", j, i, input_buffer[j]);
        }
        i++;
        if(i == last_word_bytes) {
            word_lenght--;
        }
    }


//    memset(decoded_code_word, 0, (DATA_BYTES + 1) * sizeof(char));
//    memset(code_word, 0, (DATA_BYTES + NPAR + 1) * sizeof(char));
    int bytes_count = DATA_BYTES + NPAR;
    for (i = 0; i < word_count; ++i) {
        if(i == word_count - 1){
            bytes_count = last_word_bytes;
            printf("Last word: %d\n", last_word_bytes);
        }
        //zakodovani
//        printf("ENCO: %s\n", file_in_memory[i]);
        decode_data(file_in_memory[i], bytes_count);
//        decoded_code_word[bytes_count - NPAR] = '\0';
        //TODO correct errors
        correct_errors_erasures(file_in_memory[i], bytes_count, 0, NULL);
//        printf("DECO: %s\n", file_in_memory[i]);
        memcpy(decoded_code_word, file_in_memory[i], bytes_count - NPAR);
//        printf("DECO: %s\n", decoded_code_word);
//        decoded_code_word[bytes_count - NPAR] = '\0';
        //tisk do vystupu
        add_to_output(decoded_code_word, fout, bytes_count - NPAR);

        //vycisteni pameti
        memset(decoded_code_word, 0, (DATA_BYTES) * sizeof(char));
    }
    free(file_name_out);

    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}
