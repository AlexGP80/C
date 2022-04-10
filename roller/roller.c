#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enum { OUTPUT_LEN=1024 };

char * roll(char roll_str[]) {
    char* output = malloc(OUTPUT_LEN * sizeof(char));
    if (roll_str[0] == '\0') {
        sprintf(output, "%s", "ERROR: Roll Descriptor is empty.\n");
        return output;
        // return -1;
    }

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_sec ^ ts.tv_nsec); // Initializes the random number generator

    int chars_output = sprintf(output, "%s = [", roll_str);

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
                // return -1;
                sprintf(output, "%s", "ERROR: Duplicate 'd' in an element of "
                                      "the Roll Descriptor\n");
                return output;
            }
            d_processed = 1;
        } else if (chr == '+') {
            if (sign_processed) {
                // return -1;
                sprintf(output, "%s", "ERROR: Sign already present in current "
                                      "element of the Roll Descriptor\n");
                return output;
            }
            if (dice_faces) {
                for (int i=0; i<num_dice; ++i) {
                    char *plus = i>0? "+" : "";
                    int roll = (rand()%dice_faces) + 1;
                    chars_output += sprintf(output+chars_output, "%s(%d)", plus, roll);
                    result += mult * roll;
                }
            } else {
                int roll = num_dice;
                chars_output += sprintf(output+chars_output, "%d", roll);
                result += mult * roll;
            }
            chars_output += sprintf(output+chars_output, "]+[");

            sign_processed = 1;
            mult = 1;
            num_dice = 0;
            dice_faces = 0;
            d_processed = 0;
        } else if (chr == '-') {
            if (sign_processed) {
                // return -1;
                sprintf(output, "%s", "ERROR: Sign already present in current "
                                      "element of the Roll Descriptor\n");
                return output;
            }
            if (dice_faces) {
                for (int i=0; i<num_dice; ++i) {
                    char *plus = i>0? "+" : "";
                    int roll = (rand()%dice_faces) + 1;
                    chars_output += sprintf(output+chars_output, "%s(%d)", plus, roll);
                    result += mult * roll;
                }
            } else {
                int roll = num_dice;
                chars_output += sprintf(output+chars_output, "%d", roll);
                result += mult * roll;
            }
            chars_output += sprintf(output+chars_output, "]-[");

            sign_processed = 1;
            mult = -1;
            num_dice = 0;
            dice_faces = 0;
            d_processed = 0;
        }
    }

    if (dice_faces) {
        for (int i=0; i<num_dice; ++i) {
            char *plus = i>0? "+" : "";
            int roll = (rand()%dice_faces) + 1;
            chars_output += sprintf(output+chars_output, "%s(%d)", plus, roll);
            result += mult * roll;
        }
    } else {
        int roll = num_dice;
        chars_output += sprintf(output+chars_output, "%d", roll);
        result += mult * roll;
    }

    chars_output += sprintf(output+chars_output, "] = %d\n", result);

    // printf("\n\nresult: %d\n"
    //        "sign_processed: %d\n"
    //        "mult: %d\n"
    //        "num_dice: %d\n"
    //        "dice_faces: %d\n"
    //        "d_processed: %d\n\n",
    //        result, sign_processed, mult,
    //        num_dice, dice_faces, d_processed);

    return output;
}



int main(int argc, char **argv) {
    if (argc == 2) {
        char * output = roll(argv[1]);
        printf("\n%s\n\n", output);
        free(output);
        return 0;
    } else {
        puts("ERROR: Incorrect number of parameters. \nUsage: roller _roll_str_\n    where _roll_str_ is a valid Roll Descriptor (ex: 3d6+4-2d2)");
        return 1;
    }
}
