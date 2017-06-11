#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 21

int words_cmp(const void *a, const void *b) {
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}

int main(int argc, char * argv[]){

	FILE *input = NULL, *output = NULL;
	long words_size;

	long int memory_size = strtol(argv[2], NULL, 10);
	
	char *dir_name = malloc(32);

	words_size = (memory_size/(MAX_WORD_SIZE + sizeof(char *)));
	int i, word_count;

	char **words = (char **) malloc( (size_t) words_size * sizeof(char *) );
	for(i = 0; i < words_size; i++) words[i] = (char *) calloc(1, MAX_WORD_SIZE);

	int files_counter;

	char file_name_counter_string[10];
	word_count = 0;

	int ordered_files_count = 0;

	for (files_counter = 1; files_counter <= (strtol(argv[1], NULL, 10)); files_counter++) {

		strcpy(dir_name, argv[3]);
		sprintf(file_name_counter_string, "%d", files_counter);
		strcat(dir_name, file_name_counter_string);
		input = fopen(dir_name, "r");

		if(input != NULL) {
			while(fread(words[word_count], 1, (size_t) (MAX_WORD_SIZE), input) && strlen(words[word_count])) {
				for( i = 1; i < MAX_WORD_SIZE; i++){
					if(words[word_count][i] == '\n' || words[word_count][i] == ' '){
						fseek(input, -(strlen(words[word_count])-i-1), SEEK_CUR);
						words[word_count][i] = '\0';
						break;
					}
				}
				if( (word_count+1) != (words_size) ){
					word_count++;
				} else {
					word_count = 0;

					qsort(words, (size_t)words_size, sizeof(char *), words_cmp);

					ordered_files_count++;

					strcpy(dir_name, "./tmp/o");
					sprintf(file_name_counter_string, "%d", ordered_files_count);
					strcat(dir_name, file_name_counter_string);
					output = fopen(dir_name, "w+");

					for(i = 0; i < words_size; i++){
						fwrite(words[i], strlen(words[i]), 1, output);
						fwrite("\n", 1, 1, output);
						memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
					}
					fclose(output);
				}
			}
		}
		memset(file_name_counter_string, '\0', (size_t)(strlen(file_name_counter_string)+1)); // clear memory to avoid errors
	}

	fclose(input);
	memset(file_name_counter_string, '\0', (size_t)(strlen(file_name_counter_string)+1)); // clear memory to avoid errors

	for(i = 0; i < words_size; i++) free(words[i]);
	free(words);

	free(dir_name);
	return 0;
}

/*
 *



				word = strtok (line, " \n"); //  no memory is allocated, we're good at this point
				files_tmp_counter++;
				strcpy(dir_name, "./tmp/");
				sprintf(file_name_counter_string, "%d", files_tmp_counter);
				strcat(dir_name, file_name_counter_string);
				output = fopen(dir_name, "w+");
				while (word != NULL && word != "" ) {
					next_word = strtok (NULL, " \n");
					if(next_word == NULL && eof == '0'){
						// verify if the next word is null, then back it
						fseek(input, -strlen(word)-1, SEEK_CUR);
					} else {
						fwrite(word, strlen(word), 1, output);
						fwrite("\n", 1, 1, output);
					}
					word = next_word;
				}
*/