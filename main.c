#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree/btree.h"
#include "extio.h" 				/* import readLn (func for reading input) */

#include "dict.h"
#define MAX_LEN 15000

int menu() {
	int choose = 0;
	int MAX = 5;
	char menu[][40] = {"Add", "Search", "Delete", "Print(for test)" , "Save and exit"};

	printf("\n\t=======| DICTIONARY MENU|=======\n");
	for(int i = 0; i < MAX; i++)
        printf("\t%d. %s\n", i+1, menu[i]);
	printf("Enter your chose: ");
	while(scanf("%d", &choose) != 1) {
		printf("Input must be integer!\n");
		while (getchar() != '\n' );
		//wasting the buffer till the next new line
		printf("Enter your choose: ");
	}
	return choose;	
}
void display_Dict(BTA* rootBT);

int main()
{
	char *filetxt = "anhviet109K.txt";
	char *fileDict = "dict.db";
	char *fileSoundex = "soundex.db";
	BTA *rootDic;
	BTA *soundexT;
	int status, val;
	btinit();
	/* open file if file doesn't exist => create new file*/
	rootDic = btopn(fileDict, 0, FALSE);
	if(rootDic != NULL) {
		printf("Open file successful!\n");
	} else {
		printf("Creating data\n");
		rootDic = btcrt(fileDict, 0, FALSE);
		/* import, creating data from .txt; */
		createDict(filetxt, &rootDic);
	}
	soundexT = btopn(fileSoundex, 0, FALSE);
	if(soundexT != NULL) {
		printf("Open file successful!\n");
	} else {
		printf("(Creating data for soundex!)\n");
		soundexT = btcrt(fileSoundex, 0, FALSE);
		/* copy rootDic to soundexT for sug */
		createSoundexT(rootDic, &soundexT);
	}
	int choose = 0;
	char word[SIZE_WORD];
	char mean[SIZE_MEAN];
	//char mean_out[SIZE_MEAN];
	int i,k;
	char suggest[15][100]; 		/*  */
	printf("Type 6 for test\n");
	do {
		choose = menu();
		switch (choose) {
		case 6:
			/* test case : suggestion word */
			printf("Type a word for suggestion:\n"); readLn(stdin, word, SIZE_WORD);
			
			k = suggestion(soundexT,word,suggest);
			printf("k = %d\n",k );
			if(k!=0)
				for(i = 0; i < k; i++) printf("%s\n",suggest[i]);
			break;
		case 1:
			printf("Input data:\n");
			
			printf("- Word: ");  readLn(stdin, word, SIZE_WORD);
			for (int i =0 ; word[i] != '\0'; i++) {
				word[i] = tolower(word[i]);
			}
			printf("- Meaning: ");  fgets(mean, SIZE_MEAN, stdin);
			status = btins(rootDic, word, mean, sizeof(char)*SIZE_MEAN);
			btsel(rootDic, word, mean, sizeof(char)*SIZE_MEAN, &val);
			printf("%s|\n%s|\n",  word, mean);
			break;
		case 2:
			printf("Input data:\n");
			printf("- Word: ");  readLn(stdin, word, SIZE_WORD);
			for (int i =0 ; word[i] != '\0'; i++) {
				word[i] = tolower(word[i]);
			}
			status = bfndky(rootDic, word, &val);
			if (status != 0)
				printf("Can't Found!\n");
			else {
				btsel(rootDic, word, mean, sizeof(char)*SIZE_MEAN, &val);
				printf("%s|\n%s|\n",  word, mean);
			}
			break;

		case 3:
			printf("Input data:\n");
			printf("- Word: ");  readLn(stdin, word, SIZE_MEAN);
			for (int i =0 ; word[i] != '\0'; i++) {
				word[i] = tolower(word[i]);
			}
			status = btdel(rootDic, word);
			
			//just for test 
			if(status == 0)
			{
				btsel(rootDic, word, mean, sizeof(char)*SIZE_MEAN, &val);
				printf("Delete complete:\n%s|\n%s|\n",  word, mean);
			}
			else {
				printf("Not found !\n");
			}
			break;
		case 4:
			//btpos(rootDic, ZEND); this sh*t does not work
			//btsel(rootDic, "wrong", mean, sizeof(char)*SIZE_MEAN, &val);
			display_Dict(rootDic);
			
			//printf("Check last:\n%s\t %s\n",  word, mean);
			break;
		case 5:
			status = btcls(rootDic);
			if(status == 0) printf("Close rootDic successfully!\n");
			status = btcls(soundexT);
			if(status == 0) printf("Close soundexT successfully!\n");
		default:
			printf("Wrong choose! (1-5)\n");
			break;	
		}
	}  while (choose !=5);
	return 0;
}

void display_Dict(BTA* rootDic){
	/* must fix for dictionary output */
	int val; //rsize
	long j = 0;
	char word[SIZE_WORD];
	char mean[SIZE_MEAN];
	//btpos(rootDic, ZSTART); //ZSTART = 1;
	printf("\n ========Dictionary Data========\n");
	btsel(rootDic, "x-rays", mean, sizeof(char)*SIZE_MEAN,&val);
	while(btseln(rootDic, word, mean, sizeof(char)*SIZE_MEAN,&val) == 0) {
		printf("%s|\n%s|\n",  word, mean);
		j++;	
	}
	printf("n = %ld", j);
}
