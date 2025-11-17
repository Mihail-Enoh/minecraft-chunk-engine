#include "chunk.h"
#include <stdlib.h>
#include <stdio.h>

const int C = 4;
const char N = -1, CC = 4;

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {
    char*** new_chunk = malloc(depth * sizeof(char**));
    for (int i = 0; i < depth; i++) {
        new_chunk[i] = malloc(height * sizeof(char*));
        for (int j = 0; j < height; j++) {
            new_chunk[i][j] = malloc(width * sizeof(char));
        }
    }
    for (int i = 0; i < depth; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                new_chunk[i][j][k] = chunk[k][j][depth-1-i];
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            free(chunk[i][j]);
        }
        free(chunk[i]);
    }
    free(chunk);
    return new_chunk;
}

void dfs_3d(char*** chunk, int width, int height, int depth, int x, int y, int z, char target, int* can_fall) {
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth) {
        return;
    }
    if (chunk[x][y][z] != target) {
        return;
    }
    if (*can_fall == 1) {
        if (y == 0) {
            *can_fall = 0;
        } else if (!(chunk[x][y-1][z] == 0 || chunk[x][y-1][z] == target || chunk[x][y-1][z] == target + C)) {
            *can_fall = 0;
        }
    }
    if (*can_fall == 0) {
        return;
    }
    chunk[x][y][z] = (char) (chunk[x][y][z] + C);
    if (y > 0 && chunk[x][y-1][z] == target)
    dfs_3d(chunk, width, height, depth, x, y - 1, z, target, can_fall);
    if (x < width -1 && chunk[x+1][y][z] == target)
    dfs_3d(chunk, width, height, depth, x + 1, y, z, target, can_fall);
    if (x > 0 && chunk[x-1][y][z] == target)
    dfs_3d(chunk, width, height, depth, x - 1, y, z, target, can_fall);
    if (y < height -1 && chunk[x][y+1][z] == target)
    dfs_3d(chunk, width, height, depth, x, y + 1, z, target, can_fall);
    if (z < depth -1 && chunk[x][y][z+1] == target)
    dfs_3d(chunk, width, height, depth, x, y, z + 1, target, can_fall);
    if (z > 0 && chunk[x][y][z-1] == target)
    dfs_3d(chunk, width, height, depth, x, y, z - 1, target, can_fall);
}

int can_fall_simple(char*** chunk, int width, int height, int depth, int x, int y, int z) {
    int can_fall = 1;
    if (y == 0) {
        return 0;
    }
    char target = chunk[x][y][z];
    dfs_3d(chunk, width, height, depth, x, y, z, target, &can_fall);
    return can_fall;
}

void chunk_fatal_fill_xyz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    chunk[x][y][z] = N;
    if (x > 0 && chunk[x-1][y][z] == block) {
        chunk_fatal_fill_xyz(chunk, width, height, depth, x-1, y, z, block);
    }
    if (x < width -1 && chunk[x+1][y][z] == block) {
        chunk_fatal_fill_xyz(chunk, width, height, depth, x+1, y, z, block);
    }
    if (y > 0 && chunk[x][y-1][z] > 0) {
        if (chunk[x][y-1][z] == block) {
            chunk_fatal_fill_xyz(chunk, width, height, depth, x, y-1, z, block);
        } else {
            char block2 = chunk[x][y-1][z];
            chunk_fatal_fill_xyz(chunk, width, height, depth, x, y-1, z, block2);
        }
    }
    if (y < height -1 && chunk[x][y+1][z] == block) {
        chunk_fatal_fill_xyz(chunk, width, height, depth, x, y+1, z, block);
    }
    if (z > 0 && chunk[x][y][z-1] == block) {
        chunk_fatal_fill_xyz(chunk, width, height, depth, x, y, z-1,  block);
    }
    if (z < depth -1 && chunk[x][y][z+1] == block) {
        chunk_fatal_fill_xyz(chunk, width, height, depth, x, y, z+1, block);
    }
}

char*** chunk_fatal_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char the_block = chunk[x][y][z];
    chunk_fatal_fill_xyz(chunk, width, height, depth, x, y, z, the_block);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == N) {
                    chunk[i][j][k] = block;
                }
            }
        }
    }
    return chunk;
}

int fatal_fall(char*** chunk, int width, int height, int depth, int x, int y, int z) {
    char*** new_chunk = malloc(width * sizeof(char**));
    for (int i = 0; i < width; i++) {
        new_chunk[i] = malloc(height * sizeof(char*));
        for (int j = 0; j < height; j++) {
            new_chunk[i][j] = calloc(depth, sizeof(char));
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                new_chunk[i][j][k] = (char) (chunk[i][j][k] % C);
            }
        }
    }
    chunk_fatal_fill(new_chunk, width, height, depth, x, y, z, CC);
    int can_fall = 1;
    if (y == 0) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                free(new_chunk[i][j]);
            }
            free(new_chunk[i]);
        }
        free(new_chunk);
        return 0;
    }
    char target = new_chunk[x][y][z];
    dfs_3d(new_chunk, width, height, depth, x, y, z, target, &can_fall);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            free(new_chunk[i][j]);
        }
        free(new_chunk[i]);
    }
    free(new_chunk);
    return can_fall;
}

void chunk_increment_fill_xyz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    chunk[x][y][z] = (char)(chunk[x][y][z] + C);
    if (x > 0 && chunk[x-1][y][z] == block) {
        chunk_increment_fill_xyz(chunk, width, height, depth, x-1, y, z, block);
    }
    if (x < width -1 && chunk[x+1][y][z] == block) {
        chunk_increment_fill_xyz(chunk, width, height, depth, x+1, y, z, block);
    }
    if (y > 0 && chunk[x][y-1][z] != 0) {
        if (chunk[x][y-1][z] == block) {
            chunk_increment_fill_xyz(chunk, width, height, depth, x, y-1, z, block);
        } else if (chunk[x][y-1][z] < C) {
            char block2 = chunk[x][y-1][z];
            chunk_increment_fill_xyz(chunk, width, height, depth, x, y-1, z, block2);
        }
    }
    if (y < height -1 && chunk[x][y+1][z] == block) {
        chunk_increment_fill_xyz(chunk, width, height, depth, x, y+1, z, block);
    }
    if (z > 0 && chunk[x][y][z-1] == block) {
        chunk_increment_fill_xyz(chunk, width, height, depth, x, y, z-1,  block);
    }
    if (z < depth -1 && chunk[x][y][z+1] == block) {
        chunk_increment_fill_xyz(chunk, width, height, depth, x, y, z+1, block);
    }
}

char*** chunk_fill_increment(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z) {
    char the_block = chunk[x][y][z];
    chunk_increment_fill_xyz(chunk, width, height, depth, x, y, z, the_block);
    return chunk;
}

void dfs_check_connection(char*** chunk, int width, int height, int depth, int*** visited, int x, int y, int z) {
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth) {
        return;
    }
    if (visited[x][y][z] || chunk[x][y][z] == 0) {
        return;
    }
    visited[x][y][z] = 1;
    dfs_check_connection(chunk, width, height, depth, visited, x + 1, y, z);
    dfs_check_connection(chunk, width, height, depth, visited, x - 1, y, z);
    dfs_check_connection(chunk, width, height, depth, visited, x, y + 1, z);
    dfs_check_connection(chunk, width, height, depth, visited, x, y - 1, z);
    dfs_check_connection(chunk, width, height, depth, visited, x, y, z + 1);
    dfs_check_connection(chunk, width, height, depth, visited, x, y, z - 1);
}

int all_connected_to_ground(char*** chunk, int width, int height, int depth) {
    int*** visited = (int***)malloc(width * sizeof(int**));
    for (int i = 0; i < width; i++) {
        visited[i] = (int**)malloc(height * sizeof(int*));
        for (int j = 0; j < height; j++) {
            visited[i][j] = (int*)calloc(depth, sizeof(int));
        }
    }
    for (int x = 0; x < width; x++) {
        for (int z = 0; z < depth; z++) {
            if (chunk[x][0][z] != 0) {
                dfs_check_connection(chunk, width, height, depth, visited, x, 0, z);
            }
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] != 0 && !visited[x][y][z]) {
                    for (int i = 0; i < width; i++) {
                        for (int j = 0; j < height; j++) {
                            free(visited[i][j]);
                        }
                        free(visited[i]);
                    }
                    free(visited);
                    return 0;
                }
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            free(visited[i][j]);
        }
        free(visited[i]);
    }
    free(visited);
    return 1;
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
    int new_temp_height = height;
    int blocks_found = 1;
    while (blocks_found > 0) {
        blocks_found = 0;
        for (int j = 0; j < new_temp_height; j++) {
            for (int i = 0; i < width; i++) {
                for (int k = 0; k < depth; k++) {
                    if (chunk[i][j][k] < C && chunk[i][j][k] != 0) {
                        char temp = chunk[i][j][k];
                        if (can_fall_simple(chunk, width, height, depth, i, j, k) == 1) {
                            blocks_found++;
                        } else {
                            chunk_fill(chunk, width, height, depth, i, j, k, temp);
                        }
                    }
                }
            }
        }
        for (int j = 0; j < new_temp_height; j++) {
            for (int i = 0; i < width; i++) {
                for (int k = 0; k < depth; k++) {
                    if (j < new_temp_height -1 && chunk[i][j+1][k] >= C) {
                        chunk[i][j][k] = (char) (chunk[i][j+1][k] % C);
                        chunk[i][j+1][k] = 0;
                    }
                }
            }
        }
    }
    for (int j = 0; j < new_temp_height; j++) {
        for (int i = 0; i < width; i++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] >= C) {
                    chunk[i][j][k] = (char) (chunk[i][j][k] % C);
                }
            }
        }
    }
    if (all_connected_to_ground(chunk, width, height, depth) == 0) {
        blocks_found = 1;
        while (blocks_found > 0) {
            blocks_found = 0;
            for (int j = 0; j < new_temp_height; j++) {
                for (int i = 0; i < width; i++) {
                    for (int k = 0; k < depth; k++) {
                        if (chunk[i][j][k] < C && chunk[i][j][k] != 0) {
                            if (fatal_fall(chunk, width, height, depth, i, j, k) == 1) {
                                blocks_found++;
                                chunk_fill_increment(chunk, width, height, depth, i, j, k);
                            }
                        }
                    }
                }
            }
            for (int j = 0; j < new_temp_height; j++) {
                for (int i = 0; i < width; i++) {
                    for (int k = 0; k < depth; k++) {
                        if (j < new_temp_height -1 && chunk[i][j+1][k] >= C) {
                            chunk[i][j][k] = (char) (chunk[i][j+1][k] % C);
                            chunk[i][j+1][k] = 0;
                        }
                    }
                }
            }
        }
        for (int j = 0; j < new_temp_height; j++) {
            for (int i = 0; i < width; i++) {
                for (int k = 0; k < depth; k++) {
                    if (chunk[i][j][k] >= C) {
                        chunk[i][j][k] = (char) (chunk[i][j][k] % C);
                    }
                }
            }
        }
    }
    for (int j = 0; j < new_temp_height;) {
        int is_empty_layer = 1;
        for (int i = 0; i < width && is_empty_layer; i++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] != 0) {
                    is_empty_layer = 0;
                    break;
                }
            }
        }
        if (is_empty_layer) {
            for (int y = j; y < new_temp_height - 1; y++) {
                for (int i = 0; i < width; i++) {
                    for (int k = 0; k < depth; k++) {
                        chunk[i][y][k] = chunk[i][y + 1][k];
                    }
                }
            }
            for (int i = 0; i < width; i++) {
                free(chunk[i][new_temp_height - 1]);
            }
            new_temp_height--;
        } else {
            j++;
        }
    }
    *new_height = new_temp_height;
    return chunk;
}


