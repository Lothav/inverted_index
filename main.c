
#include "suffixed.h"
#include "filesBlocksOrdered.h"
#include "balancedInterleaving.h"

/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

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
		words[i] = (char *) malloc(MAX_WORD_SIZE);

	// Call Main functions.
	// Their descriptions can be found on theirs respective headers.
	generateSuffixedFile(words, argv, words_size);
	generateFilesBlocksOrdered(words, words_size);
	balancedInterleaving(words, words_size);

	for(i = 0; i < words_size; i++) free(words[i]);
	free(words);

	return 0;
}