#include "chunk.h"

char ***chunk_place_block(
    char ***chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    if (x < width && y < height && z < depth && x >= 0 && y >= 0 && z >= 0) {
        chunk[x][y][z] = block;
    }
    return chunk;
}

char ***chunk_fill_cuboid(
    char ***chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
    if (x1 < x0) {
        int aux = x1;
        x1 = x0;
        x0 = aux;
    }
    if (y1 < y0) {
        int aux = y1;
        y1 = y0;
        y0 = aux;
    }
    if (z1 < z0) {
        int aux = z1;
        z1 = z0;
        z0 = aux;
    }
    for (int i = x0; i <=x1 ; i++) {
        for (int j = y0; j <= y1; j++) {
            for (int k = z0; k <= z1; k++) {
                chunk_place_block(chunk , width , height , depth , i , j , k , block);
            }
        }
    }
    return chunk;
}

char ***chunk_fill_sphere(
    char ***chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
    for (int i = 0 ; i < width; i++) {
        for (int j = 0 ; j < height; j++) {
            for (int k = 0 ; k < depth ; k++) {
                int distance = (x-i)*(x-i)+(y-j)*(y-j)+(z-k)*(z-k);
                if (radius * radius > distance) {
                chunk_place_block(chunk , width , height , depth , i , j , k , block);
                }
            }
        }
    }
    return chunk;
}
