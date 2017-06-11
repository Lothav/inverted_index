#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define FILE_BEFORE_SORT ".fbs"
#define MAX_WORD_SIZE 21


int main(int argc, char * argv[]){

	FILE *input = NULL, *output = NULL;
	long words_size;

	long int memory_size = strtol(argv[2], NULL, 10);
	
	char *dir_name = malloc(256);

	words_size = (memory_size/(MAX_WORD_SIZE + sizeof(char *)));
	int i, word_count;

	char **words = (char **) malloc( (size_t) words_size * sizeof(char *) );
	for(i = 0; i < words_size; i++) words[i] = (char *) calloc(1, MAX_WORD_SIZE);

	int files_counter;

	char file_name_counter_string[10];
	for (files_counter = 1; files_counter <= (strtol(argv[1], NULL, 10)); files_counter++) {

		strcpy(dir_name, argv[3]);
		sprintf(file_name_counter_string, "%d", files_counter);
		strcat(dir_name, file_name_counter_string);
		input = fopen(dir_name, "r");

		if(input != NULL) {
			word_count = 0;
			while(fread(words[word_count], 1, (size_t) (MAX_WORD_SIZE), input) && strlen(words[word_count])) {

				for( i = 1; i < MAX_WORD_SIZE; i++){
					if(words[word_count][i] == '\n' || words[word_count][i] == ' '){
						fseek(input, -(strlen(words[word_count])-i-1), SEEK_CUR);
						words[word_count][i] = '\0';
						break;
					}
				}
				printf("%d %s / ", word_count, words[word_count]);
				if( word_count < (words_size-1) ){
					word_count++;
				} else {
					word_count = 0;
					for(i = 0; i < words_size; i++)
						memset(words[i], '\0', (size_t)MAX_WORD_SIZE); // clear memory to avoid errors
				}
			}
		}
		printf("\n");
		for(i = 0; i < words_size; i++)
			memset(words[i], '\0', (size_t)MAX_WORD_SIZE); // clear memory to avoid errors
		memset(file_name_counter_string, '\0', (size_t)(strlen(file_name_counter_string)+1)); // clear memory to avoid errors
	}

	word_count = 0;


	while(word_count < words_size){
		printf("%s ", words[word_count]);
		word_count++;
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