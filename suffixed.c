#include "suffixed.h"

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