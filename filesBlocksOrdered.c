
#include "filesBlocksOrdered.h"

void generateFilesBlocksOrdered(char **words, long words_size){

	FILE *input;
	// Open generated Suffixed file.
	input = fopen("./tmp/suffixed", "r");

	// Alloc some space to aux variables.
	char *dir_name 		 = malloc(15),
		 *file_name_aux  = malloc(15);

	// Aux variables
	int i, word_count = 0, ordered_files_count = 0;
	// Clear words Memory
	for(i = 0; i < words_size; i++) words[i][0] = '\0';

	// While can read words length > 0
	while(fread(words[word_count], 1, (size_t) (MAX_WORD_SIZE), input) && strlen(words[word_count])) {

		// Clear words cut in a half and back the cursor.
		for( i = 1; i < MAX_WORD_SIZE; i++){
			if(words[word_count][i] == '\n'){
				fseek(input, -(strlen(words[word_count])-i-1), SEEK_CUR);
				for( ; i < MAX_WORD_SIZE; i++)
					words[word_count][i] = '\0';
				break;
			}
		}
		if( (word_count+1) != (words_size) ){
			// Increment until words array reach full.
			word_count++;
		} else {
			word_count = 0;
			// Write on file ordered words.
			writeOnFile(words, words_size, &ordered_files_count, (int)words_size);
		}
	}

	// If remains words, write then on file.
	if(word_count) writeOnFile(words, words_size, &ordered_files_count, word_count);

	// Free aux allocs.
	free(file_name_aux);
	free(dir_name);

	// Close opened input.
	fclose(input);
}
