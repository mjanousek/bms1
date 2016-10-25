//
// Created by martin on 24.10.16.
//

int main(int argc, char *argv[]){
    /* Now decode -- encoded codeword size must be passed */
    decode_data(codeword, ML);

    /* check if syndrome is all zeros */
    if (check_syndrome () != 0) {
        correct_errors_erasures (codeword,
                                 ML,
                                 nerasures,
                                 erasures);

        printf("Corrected codeword: \"%s\"\n", codeword);
    }

    exit(0);
}
