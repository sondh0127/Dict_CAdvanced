#include <stdio.h>
#include <stdlib.h>
#include "btree/btree.h"
#include "soundEx.h" 			/* library suggesting word */
#include <string.h>
#include <ctype.h>
#include "dict.h"
#include <time.h>
#define SIZE_WORD 150
#define SIZE_MEAN 15000
#define SIZE_LINE 256

/* Two function help to generating dictionary */
/*  begin of random str */
#define SIZE_RANDOM 10
char random_c()
{
	srand(time(NULL));
	char ch;
	int l = rand()%26;
	ch='a' + l;
	return ch;
}

char *random_s()
{
	int length = rand()%SIZE_RANDOM + 1;
	char *s = (char *)malloc(SIZE_RANDOM *sizeof(char));
	for(int i = 0;i < length; i++)
		s[i] = random_c();
	s[length]='\0';
	return s;
}
/*  end of random str */



/* Create new dictionary database function and other support */
void separateWordMean(char *word, char *mean, char *line)
{								/* format input for mean and word */
	mean[0] = '\0';
	/* strchr find char in str return position if found (char*) */ 
	char *p = strchr(line, '/');
	if (p != NULL) {
		strcpy(mean, p);
		*p = '\0';
		
		strcpy(word, line+1);
		// remove last space
		if (strlen(word) != 0)
			if (word[strlen(word) - 1] == ' ')
				word[strlen(word) - 1] = '\0';
	} else {
		//remove last enter
		strcpy(word, line+1);
		//remove last enter
		if (strlen(word) != 0)
			if (word[strlen(word) - 1] == '\n')
				word[strlen(word) - 1] = '\0';
	}

}

void createDict(char *filetxt, BTA **root)
{   /* create Dictionary from txt file */
	FILE *fp;
	
	if((fp = fopen(filetxt, "r")) == NULL) {
		printf("Can't open file %s\n", filetxt);
		return;
	}
	
	char word[SIZE_WORD];
	char mean[SIZE_MEAN];
	char line[SIZE_LINE];
	
	//fseek(fp, 0, SEEK_SET);
	int count=0;
	/* xu li phan dau file txt */
	while(1){
		fgets(line,SIZE_LINE,fp);
		//puts(line);
		/*1. split line into word and mean */
		/* strstr: find str2 trong str1 tra ve str1 neu tim thay */
		if(strstr(line, "@a") !=  NULL) {
			separateWordMean(word,mean,line);
			break;
		}
	}
	/*2 . after that, append more line of mean, and repeat*/
	while(!feof(fp)){
		//if(count == 3) break;
		fgets(line,SIZE_LINE,fp);
		while(line[0] != '@') {
			if(feof(fp)) break;
			strcat(mean, line);
			fgets(line,SIZE_LINE,fp);
		}
		int result = btins(*root, word, mean, SIZE_MEAN);
		if (result == 0) {
			count++;
			printf("word: %s|\nmean: %s|\n",word, mean);
		}
		if(line[0] == '@') {
			separateWordMean(word,mean,line);	
		}		
	}
	printf("Word added into dictionary: %d\n", count);
	fclose(fp);
}

void createSoundexT(BTA *Dict, BTA **soundexT)
{   /* copy data from Dic(BTA) to new tree for suggestion case */
	char word[SIZE_WORD];
	int rsize;
	int count = 0;
	btpos(Dict, ZSTART); //ZSTART = 1;
	
	//int status = btsel(Dict,"",vi,100000*sizeof(char),&rsize);
	while(bnxtky(Dict, word, &rsize) ==0) {
		add_soundex(*soundexT, word);
		count++;
	}
	printf("Word added into soundexTree: %d\n", count);
}
/* End of create */

/* must update dict add and create Dict too*/
int add_dict(BTA *rootBT, char *word, char *mean) {
	for (int i =0 ; word[i] != '\0'; i++) {
		word[i] = tolower(word[i]);
	}
	int status;
	status = btins(rootBT, word, mean, sizeof(char)*SIZE_MEAN);
	if(status == 0) return 1;
	else
		return 0;
}


int add_soundex(BTA *soundexT,char *word)
{
	struct soundex *se;
	char code_sdx[10];
	int rsize;
	int check = 0; //check duplicate word in soundex 
	se=(struct soundex*)malloc(sizeof(struct soundex));
	soundEx(code_sdx,word,4,4);
	
	if(btsel(soundexT, code_sdx, (char *)se, sizeof(struct soundex), &rsize) == 0) {
		for (int i = 0; i < se->count; i++) {
			if(strcmp(se->word[i], word) == 0) check = 1;
		}
		if(check == 1) return 0;
		else {
			strcpy(se->word[se->count],word);
			se->count++;
			btupd(soundexT, code_sdx, (char *)se, sizeof(struct soundex));
		}
	
	} else {
		strcpy(se->word[0],word);
		se->count = 1;
		btins(soundexT, code_sdx,(char *)se,sizeof(struct soundex));
	}
	free(se);
	return 1;
}

int add_word(BTA *rootBT, BTA *soundexT, char *word, char *mean)
{
	int status1, status2;
	status1 = add_soundex(soundexT, word);
	status2 = add_dict(rootBT, word, mean);
	if(status1 == 1 && status2 == 1)
		return 1;
	else return 0;
}

struct soundex *search_soundex(BTA *Sou,char *word)
{
	struct soundex *se;
	int rsize;
	char s[10];
	soundEx(s,word,4,4);
	se=(struct soundex*)malloc(sizeof(struct soundex));
	if(btsel(Sou,s,(char *)se,sizeof(struct soundex),&rsize)==0)
		return se;
	else
    {
		free(se);
		return NULL;
    }
}

void find_word(BTA *rootBT, BTA* soundexT, char *word)
{
	struct soundex *se;
	int status, rsize; 
	status = bfndky(rootBT, word, &rsize);
	if (status != 0) {
		printf("Can't Found!\n");
		printf("\t- Do you want to show the similar word with \"%s\" ",word);
		char c1=getchar();
		while(c1!='y'&& c1!='n') {
			printf("\n\t(Type 'y' or 'n')\n");
			c1=getchar();
		}
		if(c1 == 'y') {
			char s[SIZE_WORD];
			soundEx(s,word,4,4);
			printf("\n=>soundEx code of word %s is : %s.\n",word,s);
			se = search_soundex(soundexT, word);
			print_soundex(se, word);
		}
	} else print_mean(rootBT, word);
	printf("-------------------------------------------------\n");
}

void print_mean(BTA *rootBT, char *word)
{
	/* int status; */
	int rsize; 
	char mean[SIZE_MEAN];
	if(rootBT!=NULL) {
		btsel(rootBT, word, mean, sizeof(char)*SIZE_MEAN, &rsize);
		printf("%s|\n%s|\n",  word, mean);
	}
}

void print_soundex(struct soundex *se,char *word)
{
	if(se==NULL)
		printf("\n=>No word is pronounced similar with %s.\n",word);
	else
    {
		printf("\n=>Word list are pronounced similar with %s :\n",word);
		for(int i = 0; i < se->count; i++){
			printf("\t%d. %s\n",i+1, se->word[i]);
		}
    }
	return;
}

void del_word(BTA *rootBT, BTA *soundexT, char *word)
{
	int status;
	/* int rsize; */
	struct soundex *se;
	se=(struct soundex*)malloc(sizeof(struct soundex));
	status = btdel(rootBT, word);
	if (status != 0) printf("%s does not exists in dictionary!\n", word);
	else {
		char code_sdx[10];
		soundEx(code_sdx,word,4,4);
		se = search_soundex(soundexT, word);
		for(int i = 0; i < se->count; i++){
			if(strcmp(se->word[i],word)==0) {
				se->count--;
				for (int j= i; i < se->count; j++) {
					strcpy(se->word[j],se->word[j+1]);
				}
			}
		}
		btupd(soundexT,code_sdx,(char *)se,sizeof(struct soundex));
		printf("\nDelete completed!\n");
		free(se);
		/* check exist */
		print_mean(rootBT, word);
	}

}
