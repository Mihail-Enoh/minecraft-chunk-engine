#include "chunk.h"
#include <stdlib.h>
#include <stdio.h>

const int RUN_MAX = 4096, OCTET = 32, P = 10 , last_8_bits = 0xFF, last_4_bits = 0x0F;
const int SIX = 6, FIVE = 5, EIGHT = 8, last_5_bits = 0x1F;

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {
    int *supercoding_vector = calloc((width*height*depth), sizeof(int));
    int cnt = 0;
    char current_block = 0;
    for (int j = 0; j < height; j++) {
        for (int k = 0; k < depth; k++) {
            for (int i = 0; i < width; i++) {
                if (i == j && i == k && i == 0) {
                    supercoding_vector[cnt]++;
                    current_block = chunk[i][j][k];
                } else {
                    if (chunk[i][j][k] == current_block && supercoding_vector[cnt] < RUN_MAX -1) {
                        supercoding_vector[cnt]++;
                    } else {
                        supercoding_vector[cnt] = supercoding_vector[cnt] * P + current_block;
                        cnt++;
                        current_block = chunk[i][j][k];
                        supercoding_vector[cnt]++;
                    }
                }
                if (i == width -1 && j == height -1 && k == depth -1) {
                    supercoding_vector[cnt] = supercoding_vector[cnt] * P + current_block;
                    cnt++;
                }
            }
        }
    }
    int encode_length = cnt;
    for (int i = 0; i < cnt; i++) {
        if (supercoding_vector[i] >= P * OCTET) {
            encode_length++;
        }
    }
    *length = encode_length;
    int j = 0;
    unsigned char *encoding_vector = calloc(encode_length, sizeof(unsigned char));
    for (int i = 0; i < cnt; i++) {
        int b = supercoding_vector[i] / P;
        int a = supercoding_vector[i] % P;
        if (b > OCTET-1) {
            encoding_vector[j] = (a << SIX) | (1 << FIVE) | ((b >> EIGHT) & last_4_bits);
            j++;
            encoding_vector[j] = b & last_8_bits;
            j++;
        } else {
            encoding_vector[j] = (a << SIX) | b;
            j++;
        }
    }
    free(supercoding_vector);
    return encoding_vector;
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
    char*** chunk = calloc(width, sizeof(char**));
    for (int i = 0; i < width; i++) {
        chunk[i] = calloc(height, sizeof(char*));
        for (int j = 0; j < height; j++) {
            chunk[i][j] = calloc(depth, sizeof(char));
        }
    }
    int code_length = 0, cnt_width = 0, cnt_height = 0, cnt_depth = 0;
    while (cnt_depth < depth  || cnt_height < height  || cnt_width < width) {
        unsigned char temp = code[code_length] >> SIX;
        char block = (char) temp;
        if (((code[code_length] >> FIVE) & 1) == 0) {
            int number = code[code_length] & last_5_bits;
            while (number > 0) {
                chunk[cnt_width][cnt_height][cnt_depth] = block;
                if (cnt_width == width-1) {
                    cnt_width = 0;
                    if (cnt_depth == depth -1) {
                        cnt_depth = 0;
                        if (cnt_height == height -1) {
                            cnt_depth = depth;
                            cnt_height = height;
                            cnt_width = width;
                        } else {
                            cnt_height++;
                        }
                    } else {
                        cnt_depth++;
                    }
                } else {
                    cnt_width++;
                }
            number--;
            }
        } else {
            int temp = code[code_length] & last_4_bits;
            int number = temp << EIGHT;
            code_length++;
            number |= code[code_length];
            while (number > 0) {
                chunk[cnt_width][cnt_height][cnt_depth] = block;
                if (cnt_width == width-1) {
                    cnt_width = 0;
                    if (cnt_depth == depth -1) {
                        cnt_depth = 0;
                        if (cnt_height == height -1) {
                            cnt_depth = depth;
                            cnt_height = height;
                            cnt_width = width;
                        } else {
                            cnt_height++;
                        }
                    } else {
                        cnt_depth++;
                    }
                } else {
                    cnt_width++;
                }
            number--;
            }
        }
        code_length++;
    }
    return chunk;
}
