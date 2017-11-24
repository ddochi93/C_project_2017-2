#include <stdio.h>
#include <unistd.h>            //sleep()�� �������
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#pragma warning(disable: 4996)

int fileNum = 0 ;
typedef struct word_list{
    char * eng_word;
    char * kor_word[3];
    struct word_list* next;
} word_list;
typedef word_list Node;
Node * head = NULL;
Node * cursor = NULL;

void wordQuiz();
void flashCard();
void hangman();
void wordManage();
void addFile();
void myflush();
void gotoxy(int x, int y);
void ascendingOrderWords(char * buffer, int buffersize, FILE * fp);
void randomWords(char *buffer, int buffersize, FILE *fp);
FILE * DayDicLoad();             //�Է��� ������ �ܾ����� �ε��ϴ� �Լ�.

int main(void)
{
    int menu;
    while(1)
    {
        system("clear");
        printf(">> ���� �ܾ� �ϱ� ���α׷� <<\n");
        printf("1. ���� �ܾ� ���߱�     2. �÷���ī��\n");
        printf("3. ���(hangman)        4. �ܾ��� ����\n");
        printf("5. ���α׷� ����\n\n");

        printf("��ȣ�� �����ϼ��� : ");
        scanf("%d",&menu);
        switch(menu)
        {

        case 1 : wordQuiz(); break;
        case 2 : flashCard(); break;
        case 3 : hangman() ; break;
        case 4 : wordManage(); break;
        case 5 :printf("�ܾ��� �����մϴ�!!\n"); sleep(5);  return 0;
        }
    }
    printf("��!!!\n");
    return 0;
}

void addFile(){
        DIR *dir;
        struct dirent *ent;
        char* token;
        char* temp_token;
        char* filename;
        char* temp_filename = "0";

        dir = opendir ("./");
        if (dir != NULL) {
                while ((ent = readdir(dir)) != NULL) {
                        token = (char*)calloc(strlen(ent->d_name),sizeof(char));
                        token = strtok(ent->d_name,".");
                        printf("%s\n",token);
                        //temp_token = (char*)calloc(strlen(token),sizeof(char));
                        //*temp_token = *token;
                        //while(token!=NULL){
                        //      token = strtok(NULL,".");
                        //      if(strcmp(token,"dic")==0){
                                        //if(temp_filename < temp_token)
                                                //strcpy(temp_filename, temp_token);
                        //      }
                //      }
                }
                //printf("%s\n",temp_filename);
                closedir (dir);
        } else {
                printf("���͸��� �� �� �����ϴ�.\n");
    }
}

void wordManage()
{
        int sel;
        system("clear");
        printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� <<\n");
        printf("1. �� ���� �߰��ϱ�             2. �� �ܾ� �߰��ϱ�\n");
        printf("3. �ܾ��� ����                  4. �ܾ� ���� ��Ϻ���\n");
        printf("5. �ܾ��� ���� ����\n");
        printf("\n��ȣ�� �Է��ϼ��� : ");
        scanf("%d",&sel);
        switch(sel){
                case 1:
                        addFile();
                        break;
                case 2:
                        break;
                case 3:
                        break;
                case 4:
                        break;
                case 5:
                        break;
        }
}

void flashCard()
{
    //Node * head = NULL;
    //Node * cursor = NULL;
    int t;     //������ �ð�
    printf("�ӵ�(��) : ");
    scanf("%d",&t);

    FILE * ffp = DayDicLoad();
    if( ffp == NULL)
        return;
    char word[20] ;             //������ ���Ƶ� �ǳ�?
    int output_select;
    char buffer[50];

    printf("��¹��(���ĺ� ������� : 1, ������ :2) : ");
    while( scanf("%d",&output_select) != 1)
    {
        printf("���ڸ� �Է��Ͻÿ�!\n");
        myflush();
        continue;
    }
     system("clear");
    printf(">> ���� �ܾ� �ϱ� ���α׷� : �÷���ī�� <<\n");

    if(output_select == 1)
    {
        ascendingOrderWords(buffer,100,ffp);
    }
    else if(output_select == 2)          //��� ������ ���
        randomWords(buffer,100,ffp);
    else
    {
        printf("1�� 2�� �Է��Ͻÿ�!!\n");
        sleep(1);
        return;
    }

    cursor = head;

    while(1)
    {
        gotoxy(50,25);
        printf("%s : ", cursor->eng_word);
        fflush(stdout);             //������� sleep�� �Ǽ� ȭ�鿡 �ȳ���. ///////////////////////////////////////////
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

void wordQuiz(void)
{
    FILE * fp = DayDicLoad();

    if(fp == NULL)
        return;
    char word[20];   //�ܾ� ���ӽ� �Է¹޴� �迭
    int i = 0 ;
    int cnt = 0;        //���� �ܾ� ����
    int output_select; //���������� �������� �������� ���� �޴�.
    char buffer[50];  // .dic���� ������� �� ���� �Է¹ޱ� ���� �迭

    printf("��¹��(���ĺ� ������� : 1, ������ : 2) : ");

    while( scanf("%d",&output_select) != 1)
    {
        printf("���ڸ� �Է��Ͻÿ�!\n");
        myflush();
        continue;
    }

    system("clear");
    printf(">> ���� �ܾ� �ϱ� ���α׷� : ���� �ܾ� ���߱� <<\n");

    if(output_select == 1)
        ascendingOrderWords(buffer,100,fp);

    else if(output_select == 2)          //��� ������ ���
        randomWords(buffer,100,fp);
    else
    {
        printf("1�� 2�� �Է��Ͻÿ�!!\n");
        sleep(1);
        return;
    }

    cursor = head;  //�ʿ�? (Ŀ���� ������ ���� ��带 ����Ű�Ƿ� �� �ʿ�!! ���ϸ�����)
    i = 0 ;  //������ i ��3 �� �Ǿ������Ƿ� �ٽ� 0���� �ʱ�ȭ��Ų��.
    printf("cursor �ּ�:%d\n\n\n",cursor->eng_word);
    while(1)                         //(�������� �����) ��带 ���...
    {
        printf("%s -> ", cursor->kor_word[0]);
        scanf("%s",word);
        if( strcmp(cursor->eng_word,word) == 0 )
        {
            printf("correct!\n");
            cnt++;            //���� ������ �¾ƾ߸� ����
        }
        else if( strcmp(".quit",word) == 0)
        {

            break;
        }
        else
            printf("incorrect!\n");
        cursor = cursor ->next;
        i++;   //Ǭ ������ �������
        if(cursor == NULL)
            break;
    }
    //}

    printf("���� ����(cnt) : %d     , Ǭ ����(i) : %d\n",cnt,i);
    printf("����� ������ %.2f ���Դϴ�.(enter �Է��ϸ� �ʱ�޴��� �̵�)\n", (float)cnt/i * 100);

    myflush();
    if( getchar() == '\n')
        return;
}


FILE * DayDicLoad()              //�Է��� ������ �ܾ����� �ε��ϴ� �Լ�.
{
    myflush();
    FILE * fp = NULL;
    char fileHead[100];
    char fileTail[100] = ".dic";
    printf("���ϸ�(����) : ");
    gets(fileHead);
    strcat(fileHead,fileTail);

    fp = fopen(fileHead,"r");
    if( fp == NULL)
    {
        printf("%s������ �����ϴ�!!!\n",fileHead);
        return NULL;
    }
    return fp;
}

void randomWords(char *buffer, int buffersize, FILE *fp)
{
    srand(time(NULL));
    int flag = 0; //����������϶� switch case ��������
    char * point;
    int i =0;
    while( fgets(buffer,buffersize , fp) != NULL) {
        //fgets�� '\n'�� ���ڿ��� �Ϻη� �����Ƿ� �����ϰ� NULL���� �����ϴ� �°���
        buffer[ strlen(buffer) -1] = '\0';

        Node * newNode;
        newNode = (Node *)malloc( sizeof(Node));
        //�ؿ��� �� : newNode�ʱ�ȭ
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
        // head = NULL �̰� �ؾߵǳ�??(��� ȣ���ҽ� head�� �� ����Ű�� ������ �ȵɵ�-���߿� Ȯ��)
        if(head == NULL)              //��带 �������� ����..(�̰� ����)
        {
            head = newNode;
            //  continue;
        }
        else {
            switch(flag = rand() % 2) {
            case 0:     // ���λ��� ��带 �밡���� ����
                {
                    cursor = head; //�ʿ�?
                    newNode ->next = head;
                    head = newNode;
                    break;
                }
            case 1:     // ���λ��� ��带 ������ ����
                {
                    for( cursor = head  ; cursor ->next != NULL ; cursor = cursor->next)
                        ;
                    cursor->next  = newNode;
                }
            }
        }
    }
}

void ascendingOrderWords(char * buffer, int buffersize, FILE * fp)
{
    int i=0;
    char * point;     //�� �ٿ� �߶� �ѱ�,�� �ܾ ����Ʈ�ϴ� ������
    while( fgets(buffer, buffersize, fp) != NULL)
    {
        //fgets�� '\n'�� ���ڿ��� �Ϻη� �����Ƿ� �����ϰ� NULL���� �����ϴ� �°���
        buffer[ strlen(buffer) -1] = '\0';

        Node * newNode;
        newNode = (Node *)malloc( sizeof(Node));
        //�ؿ��� �� : newNode�ʱ�ȭ
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
                printf(">>   ���� �ܾ� �ϱ� ���α׷� : ���   <<\n\n");
                printf(" (��Ʈ) %s\n\n", kor_word);
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
                        printf("%d ��° �õ� : ",try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
                        printf("%d ��° �õ� : ", try_num);
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
  printf("�Ѱ����� ���� ����ϴ�. (enter)Ű�� ��������.");

  scanf("%c",&input_word);
  if(input_word=='\n')
    system("clear");
}