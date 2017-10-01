#include "../../lib/btree/btree.h"
#ifndef DICT_H
#define DICT_H

#define SIZE_WORD 150
#define SIZE_MEAN 15000
#define SIZE_LINE 256

/* Implement soundex suggestion */
typedef char str[SIZE_WORD];

struct soundex{
	str word[1000];
	int count;
};
/*  */
void createDict(char *filetxt, BTA **root);
void createSoundexT(BTA *Dict, BTA **soundexT);

int soundEx(char *SoundEx,
			char *WordString,
			int   LengthOption,
			int   CensusOption);

int add_dict(BTA *rootBT, char *word, char *mean);
int add_soundex(BTA *soundexT,char *word);
int add_word(BTA *rootBT, BTA *soundexT, char *word, char *mean);
/* add word = add_dict + add_soundex */
void print_mean(BTA *rootBT, char *word);
void find_word(BTA *rootBT, BTA* soundexT, char *word);

void print_soundex(struct soundex *se,char *word);
struct soundex *search_soundex(BTA *Sou,char *word);
void del_word(BTA *rootBT, BTA *soundexT, char *word);
#endif /* DICT_H */
