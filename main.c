
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char * argv[]){

	int D, M;

	scanf("%d %d", &D, &M);

	DIR *dir;
	FILE *input, *output;
	char *line;
	char *word, *next_word;

	char eof;

	char *dir_name = malloc(256);
	strcpy(dir_name, argv[2]);
	strcat(dir_name, "index");
	output = fopen(dir_name, "w+");

	struct dirent *ent;
	if ((dir = opendir (argv[1])) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (!strcmp (ent->d_name, ".") || !strcmp (ent->d_name, "..")) continue;

			strcpy(dir_name, argv[1]);
			strcat(dir_name, ent->d_name);

			input = fopen(dir_name, "r");
			if(input != NULL) {
				line = malloc( (size_t) M+1 );
				memset(line, '\0', (size_t)M+1); // clear memory

				while( fread(line, 1, (size_t) M, input) ) {
					eof = (char) (strlen(line) == M ? '0' : '1');
					word = strtok (line, " \n"); //  no memory is allocated, we're good at this point
					while (word != NULL ) {
						next_word = strtok (NULL, " \n");
						if(next_word == NULL && eof == '0'){
							// verify if the next word is null, then back it
							fseek(input, -strlen(word)-1, SEEK_CUR);
						} else {
							fwrite(word, strlen(word), 1, output);
							fwrite("\n", sizeof(char), 1, output);
							printf("%s-", word);
						}
						word = next_word;
					}
					printf("///");
					memset(line, '\0', (size_t)M+1); // clear memory
				}
				free(line);
			}
		}
		closedir (dir);
	} else {
		perror ("");
	}

	free(dir_name);
	return 0;
}