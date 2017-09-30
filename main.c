#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree/btree.h"
#include "extio.h" 				/* import readLn (func for reading input) */
#include "complete.h"
 
#include "dict.h"
#define MAX_LEN 15000
int menu();

/* int add_word(BTA *rootBT, BTA *soundexT, char *word, char *mean); */
/* add word = add_dict + add_soundex (miss add soundex)*/

/* MAIN'S PROTOTYPE*/
int add_dict(BTA *rootBT, char *word, char *mean);
void search_mean(BTA *rootBT, char *word);
void print_mean(BTA *rootBT, char *word);
void del_word(BTA *rootBT, char *word);
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
	
	char word[SIZE_WORD];
	char mean[SIZE_MEAN];
	char s[SIZE_WORD];
	/* int i,k; */
	/* char suggest[15][100]; 		/\*  *\/ */
	int choose = 0;
	printf("Type 6 for test\n");
	do {
		choose = menu();
		switch (choose) {
		case 6:
			/* test case : suggestion word */
			printf("Type a word for autocomplete:\n"); readLn(stdin, word, SIZE_WORD);
			
			break;
		case 1:
			printf("Input data: \n");
			printf("- Word: ");  readLn(stdin, word, SIZE_WORD);
			printf("- Mean: ");  readLn(stdin, mean, SIZE_MEAN);
			/*  */
			status = add_dict(rootDic, word, mean);
			if(status != 0) printf("This word already exists\n");
			else {
				printf("Adding successful\n");
				/* test */
				print_mean(rootDic, word);
			}
			break;
		case 2:
			/* find_word(rootDic, soundexT); */
			printf("Input data:\n");
			printf("- Word: "); //readLn(stdin, word, SIZE_WORD);
			tab_complete(rootDic, s);
			strcpy(word, s);
			search_mean(rootDic, word);
			
			break;

		case 3:
			printf("Input data:\n");
			printf("- Word: "); //readLn(stdin, word, SIZE_WORD);
			tab_complete(rootDic, s);
			strcpy(word, s);
			del_word(rootDic, word);
			
			break;
		case 4:
			//btpos(rootDic, ZEND); this sh*t does not work
			
			//display_Dict(rootDic);
			btsel(rootDic, word, mean, sizeof(char)*SIZE_MEAN, &val);
			printf("CHECK:%s|\n%s|\n",  word, mean);
			break;
		case 5:
			status = btcls(rootDic);
			if(status == 0) printf("Close rootDic successfully!\n");
			status = btcls(soundexT);
			if(status == 0) printf("Close soundexT successfully!\n");
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
	char menu[][40] = {"Add", "Search", "Delete", "Print(for test)" , "Save and exit"};

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

void print_mean(BTA *rootBT, char *word)
{
	int status, rsize; 
	char mean[SIZE_MEAN];
	if(rootBT!=NULL) {
		btsel(rootBT, word, mean, sizeof(char)*SIZE_MEAN, &rsize);
		printf("%s|\n%s|\n",  word, mean);
	}
}

int add_dict(BTA *rootBT, char *word, char *mean) {
	for (int i =0 ; word[i] != '\0'; i++) {
		word[i] = tolower(word[i]);
	}
	int status, rsize; // return size	
	status = btins(rootBT, word, mean, sizeof(char)*SIZE_MEAN);
	if(status == 0) return 0;
	else
		return -1;
}

void search_mean(BTA *rootBT, char *word)
{
	int status, rsize; 
	status = bfndky(rootBT, word, &rsize);
	
	if (status != 0) printf("Can't Found!\n");
	else print_mean(rootBT, word);
}

void del_word(BTA *rootBT, char *word)
{
	int status, rsize;
	status = btdel(rootBT, word);
	if (status != 0) printf("Can't Found!\n");
	else print_mean(rootBT, word);

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
