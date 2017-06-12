#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

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

				strcpy(dir_name, "./tmp/a");
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

			strcpy(dir_name, "./tmp/a");
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
	for(i = 0; i < words_size; i++) words[i] = NULL;

	FILE *files[words_size];
	int file_index = 0;
	int iterations = 1;
	long loop = words_size;
	char file_suffix = 'b';
	char tmp_file_suffix;
	while(1){
		for( ; file_index < loop; file_index++) {
			strcpy(dir_name, "./tmp/");
			tmp_file_suffix = file_suffix-1;
			strncat(dir_name, &tmp_file_suffix, 1);
			sprintf(file_name_counter_string, "%d", file_index+1);
			strcat(dir_name, file_name_counter_string);
			files[file_index % words_size] = fopen(dir_name, "r");
			if(files[file_index % words_size] == NULL){
				loop = -1;
				break;
			}
			if(file_index < loop)
				getline(&words[file_index % words_size], &line_len, files[file_index% words_size]);
		}

		if(files[1] == NULL){
			break;
		}

		memset(dir_name, '\0', strlen(dir_name));
		strcpy(dir_name, "./tmp/");
		strncat(dir_name, &file_suffix, 1);
		sprintf(file_name_counter_string, "%d", iterations++);
		strcat(dir_name, file_name_counter_string);

		output = fopen(dir_name, "w+");
		__ssize_t success;
		while(1){
			lowest_index = -1;
			for(i = 0; i < words_size; i++) {
				if(words[i] == NULL || !strlen(words[i])) continue;
				lowest_index = strcmp(words[i], words[lowest_index == -1 ? i : lowest_index]) > 0 ? lowest_index : i;
			}
			if(lowest_index == -1 || !files[lowest_index]){
				break;
			}
			fwrite(words[lowest_index], strlen(words[lowest_index]), 1, output);
			success = getline(&words[lowest_index], &line_len, files[lowest_index]);
			if(success == -1){
				words[lowest_index] = NULL;
			}
		}

		for(i = 0; i < words_size; i++) words[i] = NULL;
		if(output)fclose(output);
		for(i = 0; i < words_size; i++)
			if(files[i]) {
				fclose(files[i]);
				files[i] = NULL;
			}
		if(loop != -1){
			loop += words_size;
		} else {
			file_index = 0;
			loop = words_size;
			file_suffix++;
			iterations = 1;
		}
	}


	for(i = 0; i < words_size; i++) free(words[i]);
	free(words);

	return 0;
}