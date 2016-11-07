/**
* Author: xjanou14, Martin Janousek
* 1. projekt do predmetu BMS
*
* File: bms1A.c
* Description: Program zakodovava vstupni soubor pomoci knihovny RSCode
* a prokladani.
*
* Prevzaty kod:
*      * Pro projekt byla pouzita knihovna rscode, ktera se
*        sklada z nasledujicich souboru: berlekamp.c, crcgen.c, rs.c,
*        ecc.h, galois.c
*      * Z knihovny byl pouzity take Makefile, ktery byl pouze upraven
*      pro potreby projektu
*
* Knihovna rscode je licencovana pod GNU GENERAL PUBLIC LICENSE,
* (zdroj: http://rscode.sourceforge.net)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecc.h"
#include "constants.h"

/**
 * @param: input buffer - nezakodovane slovo
 * @param: code_word - zakodovane slovo
 * @param: velikost nezakodovaneho slova
 *
 * Metoda zakoduje input_buffer pomoci knihovny rscode
 */
void encode_word(
    unsigned char *input_buffer,
    unsigned char *code_word,
    int size
) {
    encode_data(input_buffer, size, code_word);
}

/**
 * @param: encoded_word_matrix - 2D pole zakodovanych slov
 * @param: fout - vystupni soubor
 * @param: words_count - pocet zakodovanych slov
 * @param: last_word_byte - pocet bytu zakodovaneho slova posledniho slova
 *
 * Ulozi do vystupniho souboru vsechny zakodovane slova v prolozenem poradi
 * bytu jednotlivych slov. (Nejdrive 1. byty vsech slov, pak 2. byty vsech slov, ...)
 */
void interlace_and_print_to_output(
    unsigned char **encoded_word_matrix,
    FILE *fout,
    int words_count,
    int last_word_byte
){
    int i, j;
    for (i = 0; i < DATA_BYTES + NPAR; ++i) {
        for (j = 0; j < words_count; ++j) {
            if(j < words_count - 1 || i < last_word_byte){
                fputc(encoded_word_matrix[j][i], fout);
            }
        }
    }
}

/**
 * Metoda uvolni pamet obsazenou 2D polem zakodovanych slov
 */
void free_memmory(unsigned char **encoded_word_matrix, int words_count){
    for (int i = 0; i < words_count; ++i) {
        free(encoded_word_matrix[i]);
    }
    free(encoded_word_matrix);
}


/**
 * @param: words_count - pocet slov v encoded_word_matrix
 * @param: bytes_read - pocet bytu slova code_word
 * @param: code_word - zakodovane slovo
 * @param: encoded_word_matrix - matice zakodovanych slov
 * 
 * Metoda ulozi slovo do 2D pole zakodovanych slov
 */
unsigned char** save_to_matrix(
    unsigned char **encoded_word_matrix,
    unsigned char *code_word,
    int bytes_read,
    int words_count
){
    if(words_count == 0){
        encoded_word_matrix = malloc(sizeof(unsigned char*));
    } else {
        encoded_word_matrix = realloc(encoded_word_matrix, sizeof(unsigned char*) * (words_count + 1));
    }
    encoded_word_matrix[words_count] = malloc(bytes_read * sizeof(unsigned char*));
    memcpy(encoded_word_matrix[words_count], code_word, bytes_read);
    
    return encoded_word_matrix;
}

/**
 * Pomocna metoda pro vytvoreni jmena vystupniho souboru
 */
char * create_output_file_name(char *output_file_name, char *input_file_name) {
    output_file_name = malloc(strlen(input_file_name)+1+4);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".out");
    return output_file_name;
}

/**
 * main
 */
int main(int argc, char *argv[]) {

    int bytes_read, words_count = 0, last_word_bytes = 0;
    char *file_name, *file_name_out = "";
    FILE *fin;
    FILE *fout;
    unsigned char input_buffer[DATA_BYTES + 1];
    unsigned char code_word[DATA_BYTES+NPAR];

    unsigned char **encoded_word_matrix;

    /* Initialization the ECC library */
    initialize_ecc ();

    if(argc < 2){
        fprintf(stderr, "Bad number of arguments\n");
        return EXIT_FAILURE;
    }

    /**
     * Otevreni vstupniho a vystupniho souboru, vytvoreni jmena pro vystupni soubor
     */
    file_name = argv[1];
    file_name_out = create_output_file_name(file_name_out, file_name);
    fin = fopen(file_name , "r");
    fout = fopen(file_name_out, "wb");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }
    
    /**
     * Vycisteni pameti nacteneho slova a zakodovaneho slova
     */
    memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES + 1));
    memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
    while((bytes_read = fread(input_buffer, sizeof(unsigned char), DATA_BYTES, fin)) > 0){

        // zakodovani nacteneho slova
        encode_word(input_buffer, code_word, bytes_read);

        // ulozeni kodovaneho slova do pameti
        encoded_word_matrix = save_to_matrix(encoded_word_matrix, code_word, bytes_read + NPAR, words_count);

        // vycisteni pameti nacteneho slova a zakodovaneho slova
        memset(input_buffer, 0, sizeof(unsigned char) * (DATA_BYTES));
        memset(code_word, 0, sizeof(unsigned char) * (DATA_BYTES + NPAR));
        ++words_count;
        // pocet bytu je dulezity az po ukonceni tohoto whilu
        last_word_bytes = bytes_read;
    }

    //tisk do vystupu
    interlace_and_print_to_output(encoded_word_matrix, fout, words_count, last_word_bytes + NPAR);

    // uvolneni alokovane pameti
    free_memmory(encoded_word_matrix, words_count);
    free(file_name_out);

    // uzavreni otevrenych souboru
    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}

