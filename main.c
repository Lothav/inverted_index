
#include "suffixed.h"
#include "filesBlocksOrdered.h"
#include "balancedInterleaving.h"

int main(int argc, char * argv[]){
	(void)argc; // prevent get errors from non use

	// Get max memory bytes from arguments.
	long memory_size = strtol(argv[2], NULL, 10);
	// Calc max words that we'll be able to pull to memory.
	long words_size  = (memory_size/(MAX_WORD_SIZE + sizeof(char *)));;

	/*
	 * Main code Array.
	 * Will be used in every file operation respecting memory limit.
	 * */
	int i;
	char **words = (char **) malloc( (size_t) words_size * sizeof(char *) );
	for(i = 0; i < words_size; i++)
		words[i] = (char *) calloc(1, MAX_WORD_SIZE);

	// Call Main functions.
	// Their descriptions can be found on theirs respective headers.
	generateSuffixedFile(words, argv, words_size);
	generateFilesBlocksOrdered(words, words_size);
	balancedInterleaving(words, words_size);

	for(i = 0; i < words_size; i++) free(words[i]);
	free(words);

	return 0;
}