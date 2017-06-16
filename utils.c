#include "utils.h"

int words_cmp(const void *a, const void *b) {

	// Declare and alloc use variables.
	char *ia = malloc(MAX_WORD_SIZE);
	char *ib = malloc(MAX_WORD_SIZE);
	unsigned int i;

	// Copy strings values so we can modify their contents.
	memcpy(ia, *(char **)a, strlen(*(char **)a));
	memcpy(ib, *(char **)b, strlen(*(char **)b));

	int cmp;

	if(strlen(ia) && strlen(ib)){

		// Remove suffixs from words so we can compare then.
		for(i = 0; i < strlen(ia); i++)
			if((ia)[i] == ',')
				(ia)[i] = '\0';
		for(i = 0; i < strlen(ib); i++)
			if((ib)[i] == ',')
				(ib)[i] = '\0';

		// compare, free and return.
		cmp = strcmp(ia, ib);
		free(ia);
		free(ib);
		return cmp;
	} else {
		// return as if it's equal.
		free(ia);
		free(ib);
		return 0;
	}
}

void writeOnFile(char ** words, long words_size, int *ordered_files_count, int word_count ){

	// Declare and alloc use variables.
	FILE *output;
	int i;
	char *dir_name 		 = malloc(15),
		 *file_name_aux  = malloc(15);

	// Order words before write.
	qsort(words, (size_t)words_size, sizeof(char *), words_cmp);

	// Increment counter passed by pointer.
	(*ordered_files_count)++;

	// Open a temp file.
	strcpy(dir_name, "./tmp/a");
	sprintf(file_name_aux, "%d", *ordered_files_count);
	strcat(dir_name, file_name_aux);
	output = fopen(dir_name, "w+");

	for(i = 0; i < word_count; i++){
		// Write words on file.
		fwrite(words[i], strlen(words[i]), 1, output);
		fwrite("\n", 1, 1, output);
		memset(words[i], '\0', (size_t)MAX_WORD_SIZE);
	}
	// Close and free.
	fclose(output);
	free(dir_name);
	free(file_name_aux);
}


void moveFinalFileToFolder(char file_suffix, const char *argv){
	FILE *input, *output;

	// Open generated file.
	char dir_name[25];
	strcpy(dir_name, "./tmp/");
	strncat(dir_name, &file_suffix, 1);
	strcat(dir_name,  "1");
	input = fopen(dir_name, "r");

	// Open destination file.
	strcpy(dir_name, argv);
	strncat(dir_name, "index", 5);
	output = fopen(dir_name, "w+");

	// Copy file.
	copyFile(input, output);

	// Close files.
	fclose(input);
	fclose(output);
}

void copyFile(FILE *f1, FILE *f2){
	char buffer[MAX_WORD_SIZE];
	size_t n;

	// Copy line by line.
	while ((n = fread(buffer, 1, sizeof(buffer), f1)) > 0){
		fwrite(buffer, 1, n, f2);
	}
}

