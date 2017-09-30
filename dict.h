#include "../../lib/btree/btree.h"
#ifndef DICT_H
#define DICT_H

#define SIZE_WORD 150
#define SIZE_MEAN 15000
#define SIZE_LINE 256

void createSoundexT(BTA *Dict, BTA **soundexT);
int suggestion(BTA *soundexTree, char *word, char s[][100]);

void createDict(char *filetxt, BTA **root);

int soundEx(char *SoundEx,
			char *WordString,
			int   LengthOption,
			int   CensusOption);

void find_word(BTA *rootBT, BTA* soundexT);

int tab_complete(BTA *btfile,char *key);    /* Hàm này kích hoạt terminal ở chế độ raw rồi xử lý kí tự*/

#endif /* DICT_H */
