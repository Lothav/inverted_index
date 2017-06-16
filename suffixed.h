#ifndef INVERTED_INDEX_SUFFIXED_H
#define INVERTED_INDEX_SUFFIXED_H

#include "utils.h"

/**
 * First function called that will generate the Suffixed file.
 * That function is describe on section 2.1 on doc.
 * */
void generateSuffixedFile(char **words, char * argv[], long words_size);

#endif //INVERTED_INDEX_SUFFIXED_H
