#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree/btree.h"
#include "extio.h" 				/* import readLn (func for reading input) */
#include "complete.h"
#include "dict.h"
#define MAX_LEN 15000

/* MAIN'S PROTOTYPE*/
int menu();

void display_Dict(BTA* rootBT);
/* about struct mean (for update later) == danh sach max 10 word + so luong hien co */

/* MAIN */
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
		printf("Open file %s successful!\n", fileDict);
	} else {
		printf("(Please wait) Creating data for Dictionary\n");
		rootDic = btcrt(fileDict, 0, FALSE);
		/* import, creating data from .txt; */
		createDict(filetxt, &rootDic);
	}
	soundexT = btopn(fileSoundex, 0, FALSE);
	if(soundexT != NULL) {
		printf("Open file %s successful!\n", fileSoundex);
	} else {
		printf("(Creating data for soundex!)\n");
		soundexT = btcrt(fileSoundex, 0, FALSE);
		/* copy rootDic to soundexT for sug */
		createSoundexT(rootDic, &soundexT);
	}
	
	char word[SIZE_WORD];
	char mean[SIZE_MEAN];
	char s[SIZE_LINE];

	char pri[]="Word: ";

	/* VARIABLE FOR RUNNING */
	
	int choose = 0;
	do {
		choose = menu();
		switch (choose) {
		case 1:
			/* find_word(rootDic, soundexT); */
			printf("Input data:(Use tab for compelete word)\n");
			/* printf("- Word: "); readLn(stdin, word, SIZE_WORD); */
			
			write(STDOUT_FILENO,pri,strlen(pri));
			tab_complete(rootDic, s);
			strcpy(word, s);
			find_word(rootDic, soundexT, word);
			
			break;
		case 2:
			printf("Input data: \n");
			printf("- Word: ");  readLn(stdin, word, SIZE_WORD);
			printf("- Mean: ");  readLn(stdin, mean, SIZE_MEAN);
			/*  */
			status = add_word(rootDic, soundexT, word, mean);
			if(status == 0) printf("This word already exists\n");
			else {
				printf("Adding successful\n");
				/* test */
				print_mean(rootDic, word);
			}
			break;
		case 3:
			printf("Input data:(Use tab for compelete word)\n");
			/* printf("- Word: "); readLn(stdin, word, SIZE_WORD); */
			write(STDOUT_FILENO,pri,strlen(pri));
			tab_complete(rootDic, s);
			strcpy(word, s);
			del_word(rootDic, soundexT, word);
			break;
		case 4:
			
			btsel(rootDic, word, mean, sizeof(char)*SIZE_MEAN, &val);
			printf("CHECK:%s|\n%s|\n",  word, mean);
			break;
		case 5:
			status = btcls(rootDic);
			if(status == 0) printf("Close rootDic successfully!\n");
			status = btcls(soundexT);
			if(status == 0) printf("Close soundexT successfully!\n");
			printf("GOODBYE!\n");
			break;
		default:
			printf("Invalid input! Type (1-5)\n");
			break;	
		}
	}  while (choose !=5);
	return 0;
}


/* FUNCTION DEF */
int menu() {
	/* need improve this limit MAX */
	int choose = 0;
	int MAX = 5;
	char menu[][40] = {"Search", "Add", "Delete", "Print lastest word" , "Save and exit"};

	printf("\n\t================================\n");
	printf(  "\t=======| DICTIONARY MENU|=======\n");
	printf(  "\t================================\n");
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


void display_Dict(BTA* rootDic){
	/* must fix for dictionary output */
	int val; //rse
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
