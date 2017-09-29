#include <stdio.h>
#include <stdlib.h>
#include "../../lib/btree/btree.h"
#include "soundEx.h" 			/* library suggesting word */
#include <string.h>
#include <ctype.h>
#include "dict.h"
#define SIZE_WORD 150
#define SIZE_MEAN 15000
#define SIZE_LINE 256

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
	char mean[SIZE_MEAN];
	
	int rsize;
	int count = 0;
	char soundex[5];
	btpos(Dict, ZSTART); //ZSTART = 1;

	//int status = btsel(Dict,"",vi,100000*sizeof(char),&rsize);

	/* int soundEx(char *SoundEx,
	   char *WordString,
	   int   LengthOption,
	   int   CensusOption);
	*/

	
	while(btseln(Dict, word, mean, SIZE_MEAN, &rsize) ==0) {
		soundEx(soundex,word,4,1);
		btins(*soundexT,word,soundex,5*sizeof(char));
		count++;
	}
	printf("Word added into soundexTree: %d\n", count);
}



int suggestion(BTA *soundexTree, char *word, char s[][100])
{   /* must fix */
	int rsize, i=0;
	char en[40],soundex[5],code[5];
	soundEx(code,word,4,1);
	btsel(soundexTree,"",soundex,5*sizeof(char),&rsize);
	while(btseln(soundexTree,en,soundex,5*sizeof(char),&rsize)==0 && i<49){
		if(strcmp(code,soundex)==0) {
			printf("%s\n",en);
			strcpy(s[i++],en);
		}
		// xu li luu xau en chua tu co cung ma soundex
	}
	return i;
}


/* check s1 in s2 */
int check(char *s1, char *s2){
	int len1=strlen(s1);
	int len2=strlen(s2);
	int i;
	if(len1>len2) return 0;
	else {
		for(i=0;i<len1;i++){
			if(s1[i]!=s2[i])
				return 0;
		}
	}
	return 1;
}


/* find complete word, keep result in  *result */
/* return 1 if succcesful, 0 for fail */
int autoComplete(BTA *soundexTree, char *word, char *result){
	int len1=strlen(word);
	int rsize;
	char en[40], soundex[5];
	btsel(soundexTree,"",soundex,5*sizeof(char),&rsize);
	while(btseln(soundexTree,en,soundex,5*sizeof(char),&rsize)==0){
		if(check(word,en)==1){
			strcpy(result,en);
			return 1;
		}
	}
	return 0;
}
