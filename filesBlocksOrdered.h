#ifndef INVERTED_INDEX_FILESBLOCKSORDERED_H
#define INVERTED_INDEX_FILESBLOCKSORDERED_H

#include "utils.h"

/*
 * Generate the File with the Blocks that will be used by Balanced Interleaving algorithm.
 * That function is describe on section 2.2 on doc.
 * */
void generateFilesBlocksOrdered(char **words, long words_size);

#endif //INVERTED_INDEX_FILESBLOCKSORDERED_H
