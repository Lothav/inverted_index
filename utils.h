#ifndef INVERTED_INDEX_UTILS_H
#define INVERTED_INDEX_UTILS_H

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 33

/**
 * Utils File.
 * Have some util functions that will use by the main functions.
 * */

/**
 * Function that will use by qsort() to compare strings.
 * */
int words_cmp(const void *a, const void *b);

/**
 * Write words on file.
 * */
void writeOnFile(char ** words, long words_size, int *ordered_files_count, int word_count );

/**
 * Copy File to other directory.
 * */
void copyFile(FILE *f1, FILE *f2);

/**
 * Move generated file to right directory.
 * */
void moveFinalFileToFolder(char file_suffix, const char * argv);

#endif //INVERTED_INDEX_UTILS_H
