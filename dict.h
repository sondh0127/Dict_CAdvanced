#include "../../lib/btree/btree.h"
#ifndef DICT_H
#define DICT_H

#define SIZE_WORD 150
#define SIZE_MEAN 15000
#define SIZE_LINE 256


int autoComplete(BTA *soundexTree, char *word, char *result);

void createSoundexT(BTA *Dict, BTA **soundexT);
int suggestion(BTA *soundexTree, char *word, char s[][100]);

void createDict(char *filetxt, BTA **root);

#endif /* DICT_H */
