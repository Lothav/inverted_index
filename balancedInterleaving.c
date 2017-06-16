#define _GNU_SOURCE

#include "balancedInterleaving.h"
char balancedInterleaving(char** words, long words_size){

	// Declare and alloc some util aux variables.
	FILE *output;
	size_t line_len = 0;
	unsigned int i, j;
	FILE *files[words_size];
	int file_index = 0, iterations = 1, lowest_index = 0;
	long loop = words_size;
	char file_suffix = 'b',
	 	 tmp_file_suffix,
	 	 *aux_a 		 = malloc(MAX_WORD_SIZE),
		 *aux_b 		 = malloc(MAX_WORD_SIZE),
		 *dir_name 		 = malloc(15),
		 *file_name_aux  = malloc(15);


	// Clear words memory to avoid errors.
	for(i = 0; i < words_size; i++) words[i] = NULL;

	// While true
	while(1){

		// Fill the words array
		for( ; file_index < loop; file_index++) {
			// Open tmp file with the block
			strcpy(dir_name, "./tmp/");
			tmp_file_suffix = file_suffix-1;
			strncat(dir_name, &tmp_file_suffix, 1);
			sprintf(file_name_aux, "%d", file_index+1);
			strcat(dir_name, file_name_aux);
			files[file_index % words_size] = fopen(dir_name, "r");
			if(files[file_index % words_size] == NULL){
				// When Array is filled, stop looping
				if((file_index % words_size) == 0){
					file_index = -1;
					loop = words_size;
					file_suffix++;
					iterations = 1;
					continue;
				} else {
					loop = -1;
					break;
				}
			}
			// Fill array
			if(file_index < loop)
				getline(&words[file_index % words_size], &line_len, files[file_index% words_size]);
		}

		// If file[1] is null and we got just 1 iteration, means that we have the final file
		if(files[1] == NULL && iterations == 1){
			break;
		}


		// Open file to write in
		memset(dir_name, '\0', strlen(dir_name));
		strcpy(dir_name, "./tmp/");
		strncat(dir_name, &file_suffix, 1);
		sprintf(file_name_aux, "%d", iterations++);
		strcat(dir_name, file_name_aux);
		output = fopen(dir_name, "w+");

		__ssize_t success;
		while(1){
			lowest_index = -1;

			// For each word
			for(i = 0; i < words_size; i++) {
				if(words[i] == NULL || !strlen(words[i])) continue;

				// Copy words to a clear memory so we can modify it
				memcpy(aux_a, words[i], strlen(words[i]));
				memcpy(aux_b, words[lowest_index == -1 ? i : (unsigned)lowest_index],
					   strlen(words[lowest_index == -1 ? i : (unsigned)lowest_index]));

				// Clear the word after ',' character
				for(j = 0; j < strlen(aux_a); j++)
					if((aux_a)[j] == ',')
						(aux_a)[j] = '\0';
				for(j = 0; j < strlen(aux_b); j++)
					if((aux_b)[j] == ',')
						(aux_b)[j] = '\0';

				// Compare the two words and find the lowest one.
				lowest_index = strcmp(aux_a, aux_b) >= 0 && lowest_index != -1? (unsigned)lowest_index : i;
				memset(aux_a, '\0', strlen(aux_a));
				memset(aux_b, '\0', strlen(aux_b));
			}
			// There's no more files to iterate
			if(lowest_index == -1 || !files[lowest_index]){
				break;
			}
			// Write on file the word with the lowest alphabetic order.
			fwrite(words[lowest_index], strlen(words[lowest_index]), 1, output);
			// Get the next word from that file that the writed was came from.
			success = getline(&words[lowest_index], &line_len, files[lowest_index]);
			if(success == -1){
				words[lowest_index] = NULL;
			}
		}

		// Clear words memory.
		for(i = 0; i < words_size; i++) words[i] = NULL;
		// Close files opened.
		if(output)fclose(output);
		for(i = 0; i < words_size; i++)
			if(files[i]) {
				fclose(files[i]);
				files[i] = NULL;
			}

		// Increment aux's to continue the loop.
		if(loop != -1){
			loop += words_size;
		} else {
			file_index = 0;
			loop = words_size;
			file_suffix++;
			iterations = 1;
		}
	}

	// Free leaked memory.
	free(dir_name);
	free(file_name_aux);
	free(aux_a);
	free(aux_b);

	return tmp_file_suffix;
}
