#include <stdio.h>
#include <string.h>


void cleanStdin(void) {
	int c;
	do { c = getchar(); } while (c != '\n' && c != EOF);
}

void removeLastEnter(char *s) {
	if (strlen(s) != 0)
		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0';
}

char* readLn(FILE * fin, char *s, int max_len) {
	char* p;
	p = fgets(s, max_len * sizeof(char), fin);
	
	if (p != NULL) { // already read sth
		// Re-read line if it contain only enter character
		removeLastEnter(s);
		if (strlen(s) == 0) {
			p = readLn(fin, s, max_len);
		}
	}

	return p;
}
