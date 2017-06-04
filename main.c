#include <string.h>
#include <malloc.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

	FILE *input = NULL, *output = NULL;
	char *line;
	char *word, *next_word;

	char eof;

	long int memory_size = strtol(argv[2], &word, 10);
	
	char *dir_name = malloc(256);
	strcpy(dir_name, argv[4]);
	strcat(dir_name, "index");
	output = fopen(dir_name, "w+");

	line = malloc( (size_t) (memory_size+1) );
	int files_counter;

	char file_counter_string[5];
	for (files_counter = 1; files_counter <= (strtol(argv[1], &word, 10)); files_counter++) {

		strcpy(dir_name, argv[3]);
		sprintf(file_counter_string, "%d", files_counter);
		strcat(dir_name, file_counter_string);
		input = fopen(dir_name, "r");

		if(input != NULL) {
			while( fread(line, 1, (size_t) memory_size, input) ) {
				eof = (char) (strlen(line) == ((size_t)memory_size) ? '0' : '1');
				word = strtok (line, " \n"); //  no memory is allocated, we're good at this point
				while (word != NULL ) {
					next_word = strtok (NULL, " \n");
					if(next_word == NULL && eof == '0'){
						// verify if the next word is null, then back it
						fseek(input, -strlen(word)-1, SEEK_CUR);
					} else {
						fwrite(word, strlen(word), 1, output);
						fwrite("\n", sizeof(char), 1, output);
					}
					word = next_word;
				}
				memset(line, '\0', (size_t)(memory_size+1)); // clear memory
			}
		}
	}
	memset(line, '\0', (size_t)(memory_size+1)); // clear memory to avoid errors

	if(input != NULL) fclose(input);
	if(output != NULL) fclose(output);

	free(line);
	free(dir_name);
	return 0;
}