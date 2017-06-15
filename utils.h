#ifndef INVERTED_INDEX_UTILS_H
#define INVERTED_INDEX_UTILS_H

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 33

int words_cmp(const void *a, const void *b);
void writeOnFile(char ** words, long words_size, int *ordered_files_count, int word_count );


#endif //INVERTED_INDEX_UTILS_H
