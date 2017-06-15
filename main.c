#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 33

int words_cmp(const void *a, const void *b) {
	char *ia = malloc(MAX_WORD_SIZE);
	char *ib = malloc(MAX_WORD_SIZE);
	int i;

	memcpy(ia, *(char **)a, strlen(*(char **)a));
	memcpy(ib, *(char **)b, strlen(*(char **)b));

	int cmp;

	if(strlen(ia) && strlen(ib)){
		for(i = 0; i < strlen(ia); i++)
			if((ia)[i] == ',')
				(ia)[i] = '\0';
		for(i = 0; i < strlen(ib); i++)
			if((ib)[i] == ',')
				(ib)[i] = '\0';

		cmp = strcmp(ia, ib);
		free(ia);
		free(ib);
		return cmp;
	} else {
		free(ia);
		free(ib);
		return 0;
	}
}

void generateSuffixedFile(char **words, char * argv[], long words_size){

	FILE *input, *output;

	output = fopen("./tmp/suffixed", "w+");

	int word_pos = 0, word_amount = 0, i, word_count = 0, files_counter;
	char *dir_name 		 = malloc(strlen(argv[3]) + 15),
		 *file_name_aux  = malloc(15),
		 *word_comp_suff = malloc(MAX_WORD_SIZE);

	for (files_counter = 1; files_counter <= (strtol(argv[1], NULL, 10)); files_counter++) {

		strcpy(dir_name, argv[3]);
		sprintf(file_name_aux, "%d", files_counter);
		strcat(dir_name, file_name_aux);

		input = fopen(dir_name, "r");

		memset(word_comp_suff, '\0', (size_t) strlen(word_comp_suff));
		while(fread(word_comp_suff, 1, (size_t) (MAX_WORD_SIZE), input)){

			for (i = 1; i < MAX_WORD_SIZE; i++)
				if (word_comp_suff[i] == '\n' || word_comp_suff[i] == ' ') {
					for (; i < MAX_WORD_SIZE; i++)
						word_comp_suff[i] = 0;
					break;
				}
			fseek(input, 0, SEEK_SET);

			while (fread(words[word_count], 1, (size_t) (MAX_WORD_SIZE), input) && strlen(words[word_count])) {
				for (i = 1; i < MAX_WORD_SIZE; i++) {
					if (words[word_count][i] == '\n' || words[word_count][i] == ' ') {
						fseek(input, -(strlen(words[word_count]) - i - 1), SEEK_CUR);
						words[word_count][i] = '\0';
						break;
					}
				}
				if ((word_count + 1) != (words_size)) {
					word_count++;
				} else {
					word_count = 0;
					for(i = 0; i < words_size; i++){
						word_amount += strcmp(words[i], word_comp_suff) == 0;
						memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
					}
				}
			}
			if(strlen(words[0])){
				for(i = 0; i < words_size; i++){
					word_amount += strcmp(words[i], word_comp_suff) == 0;
					memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
				}
			}

			fwrite(word_comp_suff, 1, strlen(word_comp_suff), output);
			fwrite(",", 1, 1, output);

			memset(file_name_aux, '\0', (size_t)(strlen(file_name_aux)));
			sprintf(file_name_aux, "%d", files_counter);
			fwrite(file_name_aux, 1, strlen(file_name_aux), output);
			fwrite(",", 1, 1, output);

			memset(file_name_aux, '\0', (size_t)(strlen(file_name_aux)));
			sprintf(file_name_aux, "%d", word_amount);
			fwrite(file_name_aux, 1, strlen(file_name_aux), output);
			fwrite(",", 1, 1, output);

			memset(file_name_aux, '\0', (size_t)(strlen(file_name_aux)));
			sprintf(file_name_aux, "%d", word_pos);
			fwrite(file_name_aux, 1, strlen(file_name_aux), output);

			fwrite("\n", 1, 1, output);

			word_amount = 0;
			word_pos += strlen(word_comp_suff)+1;
			fseek(input, word_pos, SEEK_SET);
			memset(word_comp_suff, '\0', (size_t) strlen(word_comp_suff));
		}

		word_pos = 0;
		memset(dir_name, '\0', strlen(dir_name));
		memset(file_name_aux, '\0', (size_t)(strlen(file_name_aux)));
		fclose(input);
	}

	fclose(output);
	free(file_name_aux);
	free(word_comp_suff);
	free(dir_name);

}

void writeOnFile(char ** words, long words_size, int *ordered_files_count, int word_count ){

	FILE *output;
	int i;
	char *dir_name 		 = malloc(15),
		 *file_name_aux  = malloc(15);

	qsort(words, (size_t)words_size, sizeof(char *), words_cmp);

	(*ordered_files_count)++;

	strcpy(dir_name, "./tmp/a");
	sprintf(file_name_aux, "%d", *ordered_files_count);
	strcat(dir_name, file_name_aux);
	output = fopen(dir_name, "w+");

	for(i = 0; i < word_count; i++){
		fwrite(words[i], strlen(words[i]), 1, output);
		fwrite("\n", 1, 1, output);
		memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
	}
	fclose(output);
	free(dir_name);
	free(file_name_aux);
}

void generateFilesBlocksOrdered(char **words, long words_size){

	FILE *input;

	input = fopen("./tmp/suffixed", "r");

	char *dir_name 		 = malloc(15),
		 *file_name_aux  = malloc(15);

	int i, word_count = 0, ordered_files_count = 0;
	for(i = 0; i < words_size; i++) words[i][0] = '\0';

	while(fread(words[word_count], 1, (size_t) (MAX_WORD_SIZE), input) && strlen(words[word_count])) {
		for( i = 1; i < MAX_WORD_SIZE; i++){
			if(words[word_count][i] == '\n'){
				fseek(input, -(strlen(words[word_count])-i-1), SEEK_CUR);
				for( ; i < MAX_WORD_SIZE; i++)
					words[word_count][i] = '\0';
				break;
			}
		}
		if( (word_count+1) != (words_size) ){
			word_count++;
		} else {
			word_count = 0;
			writeOnFile(words, words_size, &ordered_files_count, (int)words_size);
		}
	}

	if(word_count) writeOnFile(words, words_size, &ordered_files_count, word_count);

	free(file_name_aux);
	free(dir_name);

	memset(file_name_aux, '\0', (size_t)(strlen(file_name_aux)));
	fclose(input);
}

int main(int argc, char * argv[]){

	FILE  *output = NULL;
	int i, j, lowest_index = 0;

	size_t line_len = 0;

	long int memory_size = strtol(argv[2], NULL, 10);
	long words_size;
	words_size = (memory_size/(MAX_WORD_SIZE + sizeof(char *)));

	char **words = (char **) malloc( (size_t) words_size * sizeof(char *) );
	for(i = 0; i < words_size; i++) words[i] = (char *) calloc(1, MAX_WORD_SIZE);

	char dir_name[32], *file_name_aux = malloc(10);

	char *word_comp_suff = malloc(MAX_WORD_SIZE);

	generateSuffixedFile(words, argv, words_size);

	generateFilesBlocksOrdered(words, words_size);

	for(i = 0; i < words_size; i++) words[i] = NULL;

	FILE *files[words_size];
	int file_index = 0;
	int iterations = 1;
	long loop = words_size;
	char file_suffix = 'b';
	char tmp_file_suffix;

	char *aux_a = malloc(MAX_WORD_SIZE);;
	char *aux_b = malloc(MAX_WORD_SIZE);;

	while(1){
		for( ; file_index < loop; file_index++) {
			strcpy(dir_name, "./tmp/");
			tmp_file_suffix = file_suffix-1;
			strncat(dir_name, &tmp_file_suffix, 1);
			sprintf(file_name_aux, "%d", file_index+1);
			strcat(dir_name, file_name_aux);
			files[file_index % words_size] = fopen(dir_name, "r");
			if(files[file_index % words_size] == NULL){
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
			if(file_index < loop)
				getline(&words[file_index % words_size], &line_len, files[file_index% words_size]);
		}

		if(files[1] == NULL && iterations == 1){
			break;
		}

		memset(dir_name, '\0', strlen(dir_name));
		strcpy(dir_name, "./tmp/");
		strncat(dir_name, &file_suffix, 1);
		sprintf(file_name_aux, "%d", iterations++);
		strcat(dir_name, file_name_aux);

		output = fopen(dir_name, "w+");
		__ssize_t success;
		while(1){
			lowest_index = -1;

			for(i = 0; i < words_size; i++) {
				if(words[i] == NULL || !strlen(words[i])) continue;

				memcpy(aux_a, words[i], strlen(words[i]));
				memcpy(aux_b, words[lowest_index == -1 ? i : lowest_index], strlen(words[lowest_index == -1 ? i : lowest_index]));

				for(j = 0; j < strlen(aux_a); j++)
					if((aux_a)[j] == ',')
						(aux_a)[j] = '\0';
				for(j = 0; j < strlen(aux_b); j++)
					if((aux_b)[j] == ',')
						(aux_b)[j] = '\0';

				lowest_index = strcmp(aux_a, aux_b) >= 0 && lowest_index != -1? lowest_index : i;
				memset(aux_a, '\0', strlen(aux_a));
				memset(aux_b, '\0', strlen(aux_b));
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
	free(word_comp_suff);
	free(file_name_aux);
	free(aux_a);
	free(aux_b);

	return 0;
}