#include "utils.h"

int words_cmp(const void *a, const void *b) {
	char *ia = malloc(MAX_WORD_SIZE);
	char *ib = malloc(MAX_WORD_SIZE);
	unsigned int i;

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

