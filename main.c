
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char * argv[]){

	int D, M;

	//scanf("%d %d", &D, &M);

	DIR *dir;
	FILE *input;
	char *dir_name = malloc(sizeof(255));

	struct dirent *ent;
	if ((dir = opendir (argv[1])) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			strcpy(dir_name, argv[1]);
			if (!strcmp (ent->d_name, ".") || !strcmp (ent->d_name, "..")) continue;

			strcat(dir_name, ent->d_name);
			input = fopen(dir_name, "r");
			printf ("%s\n", ent->d_name);
		}
		closedir (dir);
	} else {
		perror ("");
	}

	free(dir_name);
	return 0;
}