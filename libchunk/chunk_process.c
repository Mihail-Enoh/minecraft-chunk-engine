#include "chunk.h"

const char M = 4;

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {
    for (int i = 0 ; i < width ; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == target_block) {
                    if (i > 0 && chunk[i-1][j][k] != target_block) {
                        chunk[i-1][j][k] = M;
                    }
                    if (i < width - 1 && chunk[i+1][j][k] != target_block) {
                        chunk[i+1][j][k] = M;
                    }
                    if (j > 0 && chunk[i][j-1][k] != target_block) {
                        chunk[i][j-1][k] = M;
                    }
                    if (j < height - 1 && chunk[i][j+1][k] != target_block) {
                        chunk[i][j+1][k] = M;
                    }
                    if (k > 0 && chunk[i][j][k-1] != target_block) {
                        chunk[i][j][k-1] = M;
                    }
                    if (k < depth - 1 && chunk[i][j][k+1] != target_block) {
                        chunk[i][j][k+1] = M;
                    }
                    if (i > 0 && j > 0 && chunk[i-1][j-1][k] != target_block) {
                        chunk[i-1][j-1][k] = M;
                    }
                    if (i > 0 && k > 0 && chunk[i-1][j][k-1] != target_block) {
                        chunk[i-1][j][k-1] = M;
                    }
                    if (j > 0 && k > 0 && chunk[i][j-1][k-1] != target_block) {
                        chunk[i][j-1][k-1] = M;
                    }
                    if (i < width -1 && j < height -1 && chunk[i+1][j+1][k] != target_block) {
                        chunk[i+1][j+1][k] = M;
                    }
                    if (i < width -1 && k < depth -1 && chunk[i+1][j][k+1] != target_block) {
                        chunk[i+1][j][k+1] = M;
                    }
                    if (j < height -1 && k < depth -1 && chunk[i][j+1][k+1] != target_block) {
                        chunk[i][j+1][k+1] = M;
                    }
                    if (i > 0 && j < height -1 && chunk[i-1][j+1][k] != target_block) {
                        chunk[i-1][j+1][k] = M;
                    }
                    if (i > 0 && k < depth -1 && chunk[i-1][j][k+1] != target_block) {
                        chunk[i-1][j][k+1] = M;
                    }
                    if (j > 0 && k < depth -1 && chunk[i][j-1][k+1] != target_block) {
                        chunk[i][j-1][k+1] = M;
                    }
                    if (i < width -1 && j > 0 && chunk[i+1][j-1][k] != target_block) {
                        chunk[i+1][j-1][k] = M;
                    }
                    if (i < width -1 && k > 0 && chunk[i+1][j][k-1] != target_block) {
                        chunk[i+1][j][k-1] = M;
                    }
                    if (j < height -1 && k > 0 && chunk[i][j+1][k-1] != target_block) {
                        chunk[i][j+1][k-1] = M;
                    }
                }
            }
        }
    }
    for (int i = 0 ; i < width ; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == M) {
                    chunk[i][j][k] = shell_block;
                }
            }
        }
    }
    return chunk;
}

void chunk_fill_xOz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    chunk[x][y][z] = M;
    if (x > 0 && chunk[x-1][y][z] == block) {
        chunk_fill_xOz(chunk, width, height, depth, x-1, y, z, block);
    }
    if (x < width -1 && chunk[x+1][y][z] == block) {
        chunk_fill_xOz(chunk, width, height, depth, x+1, y, z, block);
    }
    if (z > 0 && chunk[x][y][z-1] == block) {
        chunk_fill_xOz(chunk, width, height, depth, x, y, z-1,  block);
    }
    if (z < depth -1 && chunk[x][y][z+1] == block) {
        chunk_fill_xOz(chunk, width, height, depth, x, y, z+1, block);
    }
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char the_block = chunk[x][y][z];
    chunk_fill_xOz(chunk, width, height, depth, x, y, z, the_block);
    for (int i = 0; i < width; i++) {
        for (int k = 0; k < depth; k++) {
            if (chunk[i][y][k] == M) {
                chunk[i][y][k] = block;
            }
        }
    }
    return chunk;
}

void chunk_fill_xyz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    chunk[x][y][z] = M;
    if (x > 0 && chunk[x-1][y][z] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x-1, y, z, block);
    }
    if (x < width -1 && chunk[x+1][y][z] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x+1, y, z, block);
    }
    if (y > 0 && chunk[x][y-1][z] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x, y-1, z,  block);
    }
    if (y < height -1 && chunk[x][y+1][z] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x, y+1, z, block);
    }
    if (z > 0 && chunk[x][y][z-1] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x, y, z-1,  block);
    }
    if (z < depth -1 && chunk[x][y][z+1] == block) {
        chunk_fill_xyz(chunk, width, height, depth, x, y, z+1, block);
    }
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char the_block = chunk[x][y][z];
    chunk_fill_xyz(chunk, width, height, depth, x, y, z, the_block);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == M) {
                    chunk[i][j][k] = block;
                }
            }
        }
    }
    return chunk;
}
