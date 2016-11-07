/**
* Author: xjanou14, Martin Janousek
* 1. projekt do predmetu BMS
 *
* File: bms1B.c
* Description: Program dekoduje vstupni soubor zakodovany programem bms1A
* za pomoci knihovny rscode a prokladani.
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
#include <math.h>
#include "ecc.h"
#include "constants.h"

void add_to_output(unsigned char *code_word, FILE *fout, int size){
    fwrite(code_word, sizeof(unsigned char), size, fout);
}

/**
 * Pomocna metoda pro vytvoreni jmena vystupniho souboru
 */
char * create_output_file_name(char *output_file_name, char *input_file_name) {
    output_file_name = malloc(strlen(input_file_name)+1+3);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".ok");
    return output_file_name;
}

/**
 * main
 */
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

    /**
     * Otevreni vstupniho a vystupniho souboru, vytvoreni jmena pro vystupni soubor
     */
    file_name = argv[1];
    fin = fopen(file_name , "rb");
    file_name_out = create_output_file_name(file_name_out, file_name);
    fout = fopen(file_name_out, "w");

    if(fin == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }


    // zjisteni velikosti souboru a navrat na zacatek souboru.
    fseek(fin, 0L, SEEK_END);
    int sizeOfFile = ftell(fin);
    rewind(fin);

    /*
     * Vypocitani poctu slov a velikost posledniho slova
     */
    int word_count = ceil(((float) sizeOfFile) / (DATA_BYTES+NPAR));
    int last_word_bytes = sizeOfFile % (DATA_BYTES+NPAR);
    
    /*
     * inicializace 2D pole pro ulozeni vsech zakodovanych slov 
     */
    unsigned char file_in_memory[word_count][DATA_BYTES+NPAR];
    unsigned char input_buffer[word_count];
    //pocet bytu slova
    last_word_bytes = (last_word_bytes == 0) ? DATA_BYTES+NPAR : last_word_bytes;

    int i = 0;
    int words_lenght = word_count;
    
    /*
     * Nacitani vstupniho souboru a ukladani do pameti. Byty jsou ukladany
     * postupne do vsech slov (provadi se deinterleaving). Probiha nejprve
     * nacteni vsech 1. bytu slov, pote 2. bytu, atd
     */
    while((bytes_read = fread(input_buffer, sizeof(unsigned char), words_lenght, fin)) > 0) {
        for (int j = 0; j < bytes_read; ++j) {
            file_in_memory[j][i] = input_buffer[j];
        }
        i++;
        /* 
         * pokud je index roven poctu bytu v poslednim slove, pak dale bude prolozeno jiz pouze
         * words_lenght - 1
         */
        if(i == last_word_bytes) {
            words_lenght--;
        }
    }

    int bytes_count = DATA_BYTES + NPAR;
    for (i = 0; i < word_count; ++i) {
        if(i == word_count - 1){
            bytes_count = last_word_bytes;
        }

        // dekodovani slova
        decode_data(file_in_memory[i], bytes_count);
        // oprava chyb
        correct_errors_erasures(file_in_memory[i], bytes_count, 0, NULL);
        // kopirovani dekodovanych a opravenych slov do pameti
        memcpy(decoded_code_word, file_in_memory[i], bytes_count - NPAR);
        //tisk do vystupu
        add_to_output(decoded_code_word, fout, bytes_count - NPAR);

        //vycisteni pameti
        memset(decoded_code_word, 0, (DATA_BYTES) * sizeof(char));
    }

    //uvolneni pameti
    free(file_name_out);

    //uzavreni otevrenych souboru
    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}
