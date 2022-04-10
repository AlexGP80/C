#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>



int roll(char roll_str[]) {
    if (roll_str[0] == '\0') {
        // return "ERROR: El descriptor de la tirada está vacío.\n";
        return -1;
    }

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_sec ^ ts.tv_nsec); // Initializes the random number generator


    int result = 0;
    int sign_processed = 0;
    int mult = 1;
    int num_dice = 0;
    int dice_faces = 0;
    int d_processed = 0;
    for (size_t pos=0; roll_str[pos]!='\0'; ++pos) {

        char chr = roll_str[pos];

        // printf("\n\nPOS: %zu\n"
        //        "CHR: '%c'\n"
        //        "ASCII: %d\n"
        //        "result: %d\n"
        //        "sign_processed: %d\n"
        //        "mult: %d\n"
        //        "num_dice: %d\n"
        //        "dice_faces: %d\n"
        //        "d_processed: %d\n\n",
        //        pos, chr, chr, result, sign_processed, mult,
        //        num_dice, dice_faces, d_processed);

        if (chr >= 48 && chr <= 57) {
            sign_processed = 0;
            if (!d_processed) {
                num_dice = num_dice*10 + (chr - 48);
            } else {
                dice_faces = dice_faces*10 + (chr - 48);
            }
        } else if (chr == 'd') {
            if (d_processed) {
                return -1;
            }
            d_processed = 1;
        } else if (chr == '+') {
            if (sign_processed) {
                return -1;
            }

            if (dice_faces) {
                for (int i=0; i<num_dice; ++i) {
                    int roll = mult * ((rand()%dice_faces)+1);
                    printf("(%d)", roll);
                    result += roll;
                }
            } else {
                int roll = mult * (num_dice);
                printf("(%d)", roll);
                result += roll;
            }

            sign_processed = 1;
            mult = 1;
            num_dice = 0;
            dice_faces = 0;
            d_processed = 0;
        } else if (chr == '-') {
            if (sign_processed) {
                return -1;
            }

            if (dice_faces) {
                for (int i=0; i<num_dice; ++i) {
                    int roll = mult * ((rand()%dice_faces)+1);
                    printf("(%d)", roll);
                    result += roll;
                }
            } else {
                int roll = mult * (num_dice);
                printf("(%d)", roll);
                result += roll;
            }

            sign_processed = 1;
            mult = -1;
            num_dice = 0;
            dice_faces = 0;
            d_processed = 0;
        }
    }

    if (dice_faces) {
        for (int i=0; i<num_dice; ++i) {
            int roll = mult * ((rand()%dice_faces)+1);
            printf("(%d)", roll);
            result += roll;
        }
    } else {
        int roll = mult * (num_dice);
        printf("(%d)", roll);
        result += roll;
    }

    // printf("\n\nresult: %d\n"
    //        "sign_processed: %d\n"
    //        "mult: %d\n"
    //        "num_dice: %d\n"
    //        "dice_faces: %d\n"
    //        "d_processed: %d\n\n",
    //        result, sign_processed, mult,
    //        num_dice, dice_faces, d_processed);

    return result;
}



int main(int argc, char **argv) {
    if (argc == 2) {
        printf("\n%d\n\n", roll(argv[1]));
        return 0;
    } else {
        puts("ERROR: Incorrect number of parameters. \nUsage: roller _roll_str_\n    where _roll_str_ is a valid Roll Descriptor (ex: 3d6+4-2d2)");
        return 1;
    }
}
