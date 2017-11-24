#include <stdio.h>
#include <unistd.h>            //sleep()의 헤더파일
#include <stdlib.h>
#include <time.h>
#include <string.h>
#pragma warning(disable: 4996)
void wordQuiz();
void flashCard();
void hangman();

void wordList();
void addFile();
void myflush();
void gotoxy(int x, int y);


int fileNum = 0 ;


typedef struct word_list{
    char * eng_word;
    char * kor_word[3];
    struct word_list* next;
} word_list;

typedef word_list Node;



Node * head = NULL;
Node * cursor = NULL;

void ascendingOrderWords(char * buffer, int buffersize, FILE * fp)
{
    int i=0;
    char * point;     //각 줄에 잘라낸 한글,영 단어를 포인트하는 포인터
    while( fgets(buffer, buffersize, fp) != NULL)
    {
        //fgets는 '\n'도 문자열의 일부로 받으므로 제거하고 NULL문자 삽입하는 는과정
        buffer[ strlen(buffer) -1] = '\0';

        Node * newNode;
        newNode = (Node *)malloc( sizeof(Node));
        //밑에세 줄 : newNode초기화
        newNode->eng_word = NULL;
        for(i=0 ; i<3; i++)
            newNode->kor_word[i] = NULL;
        newNode ->next = NULL;

        point = strtok(buffer," ");

        newNode ->eng_word = (char*)malloc( sizeof(char) * strlen(point) + 1);
        strcpy(newNode ->eng_word,point);

        point = strtok(NULL," ");
        newNode->kor_word[0] = (char *)malloc( sizeof(char) * strlen(point) +1);
        strcpy(newNode ->kor_word[0],point);
        //-----------------------------------------------
        cursor = head;
        if(head == NULL)
        {
            head = newNode;
            continue;
        }
        else if (strcmp(head->eng_word, newNode->eng_word) > 0)
        {
            newNode->next = head;
            head = newNode;
            continue;
        }
        else
        {
            while(cursor->next != NULL)
            {
                if( strcmp(cursor->next->eng_word, newNode->eng_word) >0)
                {
                    newNode ->next = cursor -> next;
                    cursor ->next = newNode;
                    break;
                }
                cursor = cursor->next;
            }
        }
        cursor->next = newNode;

    }
}

void randomWords(char *buffer, int buffersize, FILE *fp)
{
    srand(time(NULL));
    int flag = 0; //무작위출력하때 switch case 쓰기위함
    char * point;
    int i =0;
    while( fgets(buffer,buffersize , fp) != NULL) {
        //fgets는 '\n'도 문자열의 일부로 받으므로 제거하고 NULL문자 삽입하는 는과정
        buffer[ strlen(buffer) -1] = '\0';

        Node * newNode;
        newNode = (Node *)malloc( sizeof(Node));
        //밑에세 줄 : newNode초기화
        newNode->eng_word = NULL;
        for(i=0 ; i<3; i++)
            newNode->kor_word[i] = NULL;
        newNode ->next = NULL;

        point = strtok(buffer," ");

        newNode ->eng_word = (char*)malloc( sizeof(char) * strlen(point) + 1);
        strcpy(newNode ->eng_word,point);

        point = strtok(NULL," ");
        newNode->kor_word[0] = (char *)malloc( sizeof(char) * strlen(point) +1);
        strcpy(newNode ->kor_word[0],point);
        //------------------------------------------------------------------------
        // head = NULL 이거 해야되나??(계속 호출할시 head가 멀 가리키고 있으면 안될듯-나중에 확인)
        if(head == NULL)              //노드를 무작위로 연결..(이게 시작)
        {
            head = newNode;
            //  continue;
        }
        else {
            switch(flag = rand() % 2) {
            case 0:     // 새로생긴 노드를 대가리에 이음
                {
                    cursor = head; //필요?
                    newNode ->next = head;
                    head = newNode;
                    break;
                }
            case 1:     // 새로생긴 노드를 꼬리에 이음
                {
                    for( cursor = head  ; cursor ->next != NULL ; cursor = cursor->next)
                        ;
                    cursor->next  = newNode;
                }
            }
        }
    }
}

FILE * DayDicLoad()              //입력한 일차의 단어장을 로드하는 함수.
{
    myflush();
    FILE * fp = NULL;
    char fileHead[100];
    char fileTail[100] = ".dic";
    printf("파일명(일차) : ");
    gets(fileHead);
    strcat(fileHead,fileTail);

    fp = fopen(fileHead,"r");
    if( fp == NULL)
    {
        printf("%s파일이 없습니다!!!\n",fileHead);
        return NULL;
    }
    return fp;
}

void wordQuiz(void)
{
    FILE * fp = DayDicLoad();

    if(fp == NULL)
        return;
    char word[20];   //단어 게임시 입력받는 배열
    int i = 0 ;
    int cnt = 0;        //맞힌 단어 갯수
    int output_select; //무작위인지 오름차순 정렬인지 고르는 메뉴.
    char buffer[50];  // .dic에서 개행기준 한 줄을 입력받기 위한 배열

    printf("출력방식(알파벳 순서대로 : 1, 무작위 : 2) : ");

    while( scanf("%d",&output_select) != 1)
    {
        printf("숫자를 입력하시오!\n");
        myflush();
        continue;
    }

    system("clear");
    printf(">> 영어 단어 암기 프로그램 : 영어 단어 맞추기 <<\n");

    if(output_select == 1)
        ascendingOrderWords(buffer,100,fp);

    else if(output_select == 2)          //노드 무작위 출력
        randomWords(buffer,100,fp);
    else
    {
        printf("1과 2만 입력하시오!!\n");
        sleep(1);
        return;
    }

    cursor = head;  //필요? (커서가 마지막 전의 노드를 가리키므로 꼭 필요!! 안하면좆됨)
    i = 0 ;  //위에서 i 가3 로 되어있으므로 다시 0으로 초기화시킨다.
    printf("cursor 주송:%d\n\n\n",cursor->eng_word);
    while(1)                         //(무작위로 연결된) 노드를 출력...
    {
        printf("%s -> ", cursor->kor_word[0]);
        scanf("%s",word);
        if( strcmp(cursor->eng_word,word) == 0 )
        {
            printf("correct!\n");
            cnt++;            //맞은 문제는 맞아야만 증가
        }
        else if( strcmp(".quit",word) == 0)
        {

            break;
        }
        else
            printf("incorrect!\n");
        cursor = cursor ->next;
        i++;   //푼 문제는 계속증가
        if(cursor == NULL)
            break;
    }
    //}

    printf("맞은 문제(cnt) : %d     , 푼 문제(i) : %d\n",cnt,i);
    printf("당신의 점수는 %.2f 점입니다.(enter 입력하면 초기메뉴로 이동)\n", (float)cnt/i * 100);

    myflush();
    if( getchar() == '\n')
        return;
}

void flashCard()
{
    //Node * head = NULL;
    //Node * cursor = NULL;
    int t;     //지연될 시간
    printf("속도(초) : ");
    scanf("%d",&t);

    FILE * ffp = DayDicLoad();
    if( ffp == NULL)
        return;
    char word[20] ;             //변수명 같아도 되나?
    int output_select;
    char buffer[50];

    printf("출력방식(알파벳 순서대로 : 1, 무작위 :2) : ");
    while( scanf("%d",&output_select) != 1)
    {
        printf("숫자를 입력하시오!\n");
        myflush();
        continue;
    }
     system("clear");
    printf(">> 영어 단어 암기 프로그램 : 플래쉬카드 <<\n");

    if(output_select == 1)
    {
        ascendingOrderWords(buffer,100,ffp);
    }
    else if(output_select == 2)          //노드 무작위 출력
        randomWords(buffer,100,ffp);
    else
    {
        printf("1과 2만 입력하시오!!\n");
        sleep(1);
        return;
    }

    cursor = head;

    while(1)
    {
        gotoxy(50,25);
        printf("%s : ", cursor->eng_word);
        fflush(stdout);             //출력전에 sleep이 되서 화면에 안나옴. ///////////////////////////////////////////
        sleep(t);
        printf("%s\n",cursor->kor_word[0]);
        fflush(stdout);
        sleep(t);
        system("clear");

        cursor = cursor -> next;
        if(cursor ==NULL)
            break;
    }

}

int main(void)
{
    int menu;
    while(1)
    {
        system("clear");
        printf(">> 영어 단어 암기 프로그램 <<\n");
        printf("1. 영어 단어 맞추기     2. 플래쉬카드\n");
        printf("3. 행맨(hangman)        4. 단어장 관리\n");
        printf("5. 프로그램 종료\n\n");

        printf("번호를 선택하세요 : ");
        scanf("%d",&menu);
        switch(menu)
        {

        case 1 : wordQuiz(); break;
        case 2 : flashCard(); break;
        case 3 : hangman() ; break;
       // case 4 : wordList(); break;
        case 5 :printf("단어장 종료합니다!!\n"); sleep(5);  return 0;
        }
    }
    printf("끝!!!\n");
    return 0;
}
void myflush()
{
    while( getchar() != '\n')
        ;
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%df",y,x);
}
int hangman_word(char eng_word_line[]) {
	
	int length;
	length=strnlen(eng_word_line, 20);
	for (int i = 0; i < length; i++) {
		if (eng_word_line[i] == '\0')
			break;
		eng_word_line[i] = '_';
	}
	return length;
}
void hangman() {
	int hangman_count  = 1;
	int try_num = 1;
	int correct_count  = 0;
	int loop_count;
	_Bool answer_state = 0;
	char input_word;
	char eng_word_line[20];
	char kor_word[20];
	char dic_eng[20];
    char buffer[50];


	FILE * ffp = DayDicLoad();
      if( ffp == NULL)
          return;
    randomWords(buffer,100,ffp);
    cursor=head;

	strcpy(eng_word_line,cursor->eng_word);
	strcpy(dic_eng, cursor->eng_word);
	strcpy(kor_word, cursor->kor_word[0]);
	int word_length = hangman_word(eng_word_line);
	char cmd;
  
	for ( loop_count= 1; loop_count <= 20; loop_count++) {
		system("clear");	
		printf(">>   영어 단어 암기 프로그램 : 행맨   <<\n\n");
		printf(" (힌트) %s\n\n", kor_word);
		switch (hangman_count) {
		case 1: {
			printf("-------------------+\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			for (int j = 0; j <=word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("%d 번째 시도 : ",try_num);
		}break;
		case 2: {
			printf("-------------------+\n");
			printf("                   O\n");
			printf("\n");
			printf("\n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
		case 3: {
			printf("-------------------+\n");
			printf("                   O\n");
			printf("                   l\n");
			printf("\n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
		case 4: {
			printf("-------------------+\n");
			printf("                   O\n");
			printf("                  /l\n");
			printf("\n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
		case 5: {
			printf("-------------------+\n");
			printf("                   O\n");
			printf("                  /l\n");
			printf("                  / \n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
		case 6: {
			printf("-------------------+   \n");
			printf("                   O   \n");
			printf("                  /l\\ \n");
			printf("                  /    \n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
		default: {
			printf("-------------------+   \n");
			printf("                   O   \n");
			printf("                  /l\\ \n");
			printf("                  /    \n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n");
			printf("%d 번째 시도 : ", try_num);
		}break;
	}   
		   
		   scanf("%c",&input_word);
		
		   for (int k = 0; k < word_length; k++) {
			   if (input_word == dic_eng[k]) {
				   eng_word_line[k] = input_word;
					   answer_state = 1;
				   	   
			   }
			   if (eng_word_line[k] != '_') {
				   correct_count++;
			   }
		   }
		   getchar();
		   if (correct_count == word_length) {
			   printf("   ##########################\n");
			   printf("   ### Congratulations!!! ###\n");
			   printf("   ##########################\n");
			   break;
		   }
		   else {
			   correct_count = 0;
		   }
		   if (answer_state == 1) {
			   try_num++;
			   answer_state = 0;
		   }
		   else {
			   hangman_count++;
			   try_num++;
			   answer_state = 0;
 		   }
   } 
  printf("한게임이 종료 됬습니다. (enter)키를 누르세요.");
  
  scanf("%c",&input_word);
  if(input_word=='\n')
    system("clear");
}
