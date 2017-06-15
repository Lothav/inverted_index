#define _GNU_SOURCE

#include "balancedInterleaving.h"
void balancedInterleaving(char** words, long words_size){

	FILE *output;

	size_t line_len = 0;

	char *dir_name 		 = malloc(15),
			*file_name_aux  = malloc(15);

	unsigned int i, j;
	int lowest_index = 0;

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
				memcpy(aux_b, words[lowest_index == -1 ? i : (unsigned)lowest_index], strlen(words[lowest_index == -1 ? i : (unsigned)lowest_index]));

				for(j = 0; j < strlen(aux_a); j++)
					if((aux_a)[j] == ',')
						(aux_a)[j] = '\0';
				for(j = 0; j < strlen(aux_b); j++)
					if((aux_b)[j] == ',')
						(aux_b)[j] = '\0';

				lowest_index = strcmp(aux_a, aux_b) >= 0 && lowest_index != -1? (unsigned)lowest_index : i;
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

	free(dir_name);
	free(file_name_aux);
	free(aux_a);
	free(aux_b);
}
