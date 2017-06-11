#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 21

int words_cmp(const void *a, const void *b) {
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	if(strlen(*ia) && strlen(*ib)){
		return strcmp(*ia, *ib);
	}else{
		return 0;
	}
}

int main(int argc, char * argv[]){

	FILE *input = NULL, *output = NULL;
	int i, word_count = 0, files_counter, ordered_files_count = 0, lowest_index = 0;

	size_t line_len = 0;

	long int memory_size = strtol(argv[2], NULL, 10);
	long words_size;
	words_size = (memory_size/(MAX_WORD_SIZE + sizeof(char *)));

	char **words = (char **) malloc( (size_t) words_size * sizeof(char *) );
	for(i = 0; i < words_size; i++) words[i] = (char *) calloc(1, MAX_WORD_SIZE);

	char dir_name[32], file_name_counter_string[10];

	for (files_counter = 1; files_counter <= (strtol(argv[1], NULL, 10)); files_counter++) {
		strcpy(dir_name, argv[3]);
		sprintf(file_name_counter_string, "%d", files_counter);
		strcat(dir_name, file_name_counter_string);
		input = fopen(dir_name, "r");
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

		if(word_count && files_counter == (strtol(argv[1], NULL, 10))){
			qsort(words, (size_t)words_size, sizeof(char *), words_cmp);

			ordered_files_count++;

			strcpy(dir_name, "./tmp/o");
			sprintf(file_name_counter_string, "%d", ordered_files_count);
			strcat(dir_name, file_name_counter_string);
			output = fopen(dir_name, "w+");

			for(i = 0; i < word_count; i++){
				fwrite(words[i], strlen(words[i]), 1, output);
				fwrite("\n", 1, 1, output);
				memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
			}
			fclose(output);
		}

		memset(file_name_counter_string, '\0', (size_t)(strlen(file_name_counter_string)+1));
		fclose(input);
	}
	for(i = 0; i < words_size; i++) memset(words[i], '\0', (size_t)MAX_WORD_SIZE);

	FILE *files[ordered_files_count];
	for(i = 0; i < ordered_files_count; i++) {
		strcpy(dir_name, "./tmp/o");
		sprintf(file_name_counter_string, "%d", i+1);
		strcat(dir_name, file_name_counter_string);
		files[i] = fopen(dir_name, "r");
		getline(&words[i], &line_len, files[i]);
	}

	output = fopen("./tmp/a", "w+");


	__ssize_t success;


	while(1){
		lowest_index = -1;
		for(i = 0; i < words_size; i++) {
			if(words[i] == NULL || !strlen(words[i])) continue;
			lowest_index = strcmp(words[i], words[lowest_index == -1 ? i : lowest_index]) > 0 ? lowest_index : i;
		}
		if(lowest_index == -1){
			break;
		}
		fwrite(words[lowest_index], strlen(words[lowest_index]), 1, output);
		success = getline(&words[lowest_index], &line_len, files[lowest_index]);
		if(success == -1){
			words[lowest_index] = NULL;
		}
	}

	fclose(output);
	for(i = 0; i < ordered_files_count; i++) fclose(files[i]);
	for(i = 0; i < words_size; i++) free(words[i]);
	free(words);

	return 0;
}