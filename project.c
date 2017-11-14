#include <stdio.h>
#include <stdlib.h>

void wordQuiz();
void flashCard();
void hangman();
void wordList();
void addFile();

int fileNum = 0 ;

typedef struct {
	char* word;
	word_list* next;
} word_list;

int main(void)
{
		int menu;
		system("clear");
		printf(">> 영어 단어 암기 프로그램 <<\n");
		printf("1. 영어 단어 맞추기		2. 플래쉬카드\n");
		printf("3. 행맨(hangman)		4. 단어장 관리\n");
		printf("5. 프로그램 종료\n\n");

		printf("번호를 선택하세요 : ");
		scanf("%d",&menu);
		
		switch(menu)
		{
				case 1 : wordQuiz(); break;
				case 2 : flashCard(); break;
				case 3 : hangman() ; break;
				case 4 : wordList(); break;
				case 5 : system("exit"); 
		}
		return 0;
}

void wordQuiz()
{
		printf("파일명(일차)\n");
		printf("출력방식(알파벳 순서대로 : 1, 무작위 :2) : ");
}

void flashCard()
{
		printf("속도(초) : ");
}

void hangman()
{
		printf("hangman::");
}

void wordList()
{
		int wordList_menu;
		system("clear");
		printf(">> 영어 단어 암기 프로그램 : 단어장 관리 <<\n");
		printf("1. 새 파일 추가하기			2. 새 단어 추가하기\n");
		printf("3. 단어장 보기				4. 단어 파일 목록보기\n");
		printf("5. 단어장 관리 종료\n\n");

		printf("번호를 입력하세요 : ");
		scanf("%d",&wordList_menu);
		while(1)
		{ 
			switch(wordList_menu)
				{
					case 1 : addFile(); break;
					case 2 : addWord() ; break;
				}
		}
}

void addFile(void)
{
	FILE * ifp;
	ifp = fopen("1.dic","w");

}

void addWord(void)
{
	FILE * ifp;
	char word[
}
