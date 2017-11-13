
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME "dic.txt" //단어장의 이름을 정의
#define LINE 300 //단어의 갯수를 정의

int randnum();
void input_word(char word[]);
char dicload();
void wordquiz();
void flashcard();
void hangword(char word[]);
void hangman();
int dicadd();
int dicsave();
void dic();

static int linecount=0; //단어의 갯수를 정적변수로 선언하여 변동시 그대로 유지되도록 하였다
static char diceng[LINE][16], dickor[LINE][61]; //영어 단어와 한글 단어를 저장하는 배열로서, 배열이 프로그램 실행내내 유지되도록 정적변수로 선언하였다.

int main()
{
        system("clear");
        dicload();
        int sel;        //항목 지정하는 숫자
        printf(">> 영어 단어 암기 프로그램 <<\n");
        printf("1. 영어 단어 맞추기                2. 플래쉬카드\n");
        printf("3. 행맨 (hangman)                4. 단어장 관리\n");
        printf("5. 프로그램 종료\n\n");
        printf("번호를 선택하세요 : ");
        scanf("%d", &sel);
        system("clear");
        if (sel==1)
                wordquiz();
        if (sel==2)
                flashcard();
        if (sel==3)
                hangman();
        if (sel==4)
                dic();
        if (sel==5)
                system("exit");
}


int randnum() //랜덤변수를 취한 단어가 없는 줄에 있어서 표시가 아무것도 안될 경우를 대비하여 그 경우에 계속 랜덤변수를 취하게 해줌
{
        srand(time(NULL));
        int i;
        while (1)
        {
                i=rand()%(LINE+1);
                if (('a' <= diceng[i][0]) && (diceng[i][0] <= 'z'))
                        break;
        }
        return i;
}

void input_word(char word[]) //정상적 단어 입력을 위한 단어입력함수
{
        int i=0, c;
        while (1)
        {
                c=getchar();
                if ((c==' ') || (i==16))
                {
                        while ((c=getchar()) != '\n')
                                ;
                        printf("공란 없이 단어를 다시 입력해주세요 : ");
                        i = 0;
                        continue;
                }
                if (c == '\n' || (c == EOF))
                {
                        word[i++] = '\0';
                        break;
                }
        word[i++] = c;
        }
}

char dicload() //사전을 로드하는 함수
{
        FILE *dict;
        int i=0;
        dict = fopen(FILENAME, "r");
        while (fscanf(dict, "%s %s", diceng[i], dickor[i]) !=EOF)
        {
                i++;
                linecount++;
        }
        fclose(dict);
}

void wordquiz() //영어 단어 맞추기 함수
{
        printf(">> 영어 단어 암기 프로그램 : 단어 퀴즈 <<\n");
        srand(time(NULL));
        int j=0, k=0; //맞는 것과 전체 문제 수를 카운트하는 변수
        char answer[16], quit[] = ".quit", ent; //ent는 엔터입력시 초기메뉴로
        getchar();
        while (1)
        {
                int i = randnum();
                printf("%s -> ", dickor[i]);
                input_word(answer);
                if (strcmp(answer, diceng[i])==0)
                {
                        printf("correct!\n");
                        j++;
                        k++;
                }
                if ((strcmp(answer, diceng[i])!=0) && (strcmp(answer, quit)!=0))
                {
                        printf("incorrect!\n");
                        k++;
                }
                if (strcmp(answer, quit)==0)
                        break;        
        }
        if (k!=0) //한 문제 이상 풀었을 경우 점수 출력
                {
                        printf("당신의 점수는 %.2f점입니다. (Enter입력시 초기메뉴로...) ", (j*100.0/k));
                }
        else //문제를 전혀 풀지 않았을경우 0으로 나눠지는 오류 대비
                system("main_structure");
        scanf("%c", &ent);
        if (ent == '\n')
        {
                system("clear");
                main(); //프로그램 메인메뉴로 돌아가기
        }
}
void flashcard() //플래쉬카드 함수
{
        printf(">> 영어 단어 암기 프로그램 : 플래쉬 카드 <<\n");
        int a, t, w, i;
        srand(time(NULL));
        printf("단어를 출력하는 시간 간격을 입력해주세요(초) : ");
        scanf("%d", &t);
        printf("출력하고 싶은 단어의 개수를 입력해주세요 : ");
        scanf("%d", &w);
        for(i=0;i<w;i++)
        {
                a = randnum();
                printf("-> %s\n", diceng[a]);
                sleep(t);
                printf("-> %s\n", dickor[a]);
                sleep(t);
        }
        main();
}

void hangword(char word[]) //행맨 단어를  ____이런 형식으로 바꿔주는 함수
{
        int i;
        for(i=0;i<=15;i++)
        {
                if (word[i]=='\0')
                        break;
                word[i]='_';
                
        }

}
void hangman()//행맨
{        
        srand(time(NULL));
        char hint[31], word[16], answer, ent;
        int ran=randnum(), anscount=1, tr=1, count, i, j=0, hang=1;//ran=랜덤단어, anscount=행맨이미지변화 카운트, count=전체 카운트, i,j=본문안에 여러방도로 사용됨
        strcpy(word, diceng[ran]);
        strcpy(hint, dickor[ran]);
        hangword(word);
        
        for(count=1;count<=16;count++)
        {
                printf(">> 영어 단어 암기 프로그램 : 헹맨 <<\n\n");
                printf("(힌트) %s\n\n", hint);

        switch(hang){
                case 1:{
                        printf("-------------------------+\n");
                        printf("                          \n");
                        printf("                          \n");
                        printf("                          \n\n");
                                for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;
                
                case 2:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                          \n");
                        printf("                          \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;

                case 3:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                         l\n");
                        printf("                          \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;
                case 4:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                        /l\n");
                        printf("                          \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;

                case 5:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                        /l\\ \n");
                        printf("                          \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;

                case 6:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                        /l\\ \n");
                        printf("                        / \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                                
                           }break;

                case 7:{
                        printf("-------------------------+\n");
                        printf("                         O\n");
                        printf("                        /l\\ \n");
                        printf("                        / \\ \n\n");
                        for(i=0;i<=15;i++)
                                {
                                        printf("%c ", word[i]);
                                                if (word[i]=='\0')
                                                        break;
                                }
                        printf("메인으로 돌아가시려면 *을 입력해주세요^^\n");
                        printf("%d 번째 시도 : ", tr);
                            }break;
                        }
                getchar();
                tr++;
                scanf("%c", &answer);
                
                if (answer=='*')
                        break;
                else
                {
                        j=0;
                        for(i=0;i<=14;i++) //입력한 문자와 정답의 문자열을 비교함
                        {
                                if (answer == diceng[ran][i])
                                {
                                        word[i] = diceng[ran][i];
                                        j++;                                
                                }
                        }
                
                        if (j==0) //헹맨 이미지와 n번째 시도 횟수를 증가시켜줌
                        {
                                anscount++;
                                hang++;
                        }
                }

                if (hang>=7) //헹맨 마지막 그림까지 갔을 때 그 그림을 유지시켜줌
                                hang=7;
                if (strcmp(diceng[ran], word) == 0)
                {
                if (anscount<7) //축하메시지 출력
                        {
                                printf("##########################\n");
                                printf("### Congratulations!!! ###\n");
                                printf("##########################\n");
                                break;
                        }
                        else //횟수가 많을경우 맞췄다는 메시지 출력
                        {
                                printf("횟수가 경과하였지만 정답을 맞추셨습니다~\n");
                        }
                                break;
                }
                        system("clear");                
        }
        getchar();
        printf("Enter키를 누르면 메인메뉴로 돌아갑니다 : \n");
        scanf("%c", &ent);
        if (ent=='\n')
        {
                system("clear");
                main();
        }
}
        
void dic()
{
        int sel;
        char ent; //단어장 관리 메뉴로 다시 돌아오는 엔터키
        printf(">> 영어 단어 암기 프로그램 : 단어장 관리 <<\n");
        printf("1. 단어장 읽어오기                2. 단어 추가하기\n");
        printf("3. 단어장 저장하기                4. 단어장 보기\n");
        printf("5. 단어장 관리 종료\n\n번호를 입력하세요 : ");
        scanf("%d", &sel);
        switch (sel){
                case 1:
                        {
                        dicload();
                        printf("단어장을 읽어왔습니다.\n");
                                system("clear");
                                dic();
                        }
                        break;
                case 2:
                        dicadd();
                        break;
                case 3:
                        dicsave();
                        break;
                case 4:
                        {
                        printf("----------단어장--------------\n");
                        system("cat dic");
                        printf("\nEnter키를 누르면 단어장 메뉴로 돌아갑니다\n");
                        getchar();
                        scanf("%c", &ent);
                        if (ent == '\n')
                        {
                                system("clear");
                                dic();
                        }
                        }
                        break;
                case 5:
                        main();
                        break;
        }
}

int dicadd()
{
        printf(".add 입력시에 저장후 단어장 메뉴로 돌아갑니다.\n");
        while (1)
        {
                printf("영어 단어 : ");
                scanf("%s", diceng[linecount]);
                if (strcmp(diceng[linecount], ".add") == 0)
                        break;
                printf("한글 뜻 : ");
                scanf("%s", dickor[linecount]);
                linecount++;
        }
        system("clear");
         dic();
}
int dicsave()
{
        int i, j;
        char temp[41], ent;
        FILE *dict;
        dict = fopen(FILENAME, "w+");
        for(i=0; i<linecount; i++){
        for(j=0; j<linecount ; j++){
                if(strcmp(diceng[i],diceng[j]) <= 0){
                        strcpy(temp,diceng[i]);
                        strcpy(diceng[i],diceng[j]);
                        strcpy(diceng[j],temp);
                        strcpy(temp,dickor[i]);
                        strcpy(dickor[i],dickor[j]);
                        strcpy(dickor[j],temp);
                }
        }
        }
        for(i=0;i<linecount;i++)
                fprintf(dict, "%s %s\n", diceng[i], dickor[i]);
        fclose(dict);
        printf("단어장을 저장했습니다.\n");
        printf("Enter키를 누르면 단어장 관리메뉴로 돌아갑니다\n");
        scanf("%c", &ent);
        if (ent=='\n')
        dic();
}

