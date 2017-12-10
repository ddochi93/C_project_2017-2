#include <stdio.h>
#include <unistd.h>            //sleep()�� �������
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#pragma warning(disable: 4996)   //visual���� scanf��� ����

int fileNum = 0;
typedef struct word_list {
	char * eng_word;
	char * kor_word[3];
	struct word_list* next;
} word_list;
typedef word_list Node;
Node * head = NULL;
Node * cursor = NULL;

void wordQuiz();    // ����ܾ� ���߱� �Լ�
void flashCard();    // �÷��� ī�� �Լ�
void hangman();
void manageWord();   // �ܾ������ ����
void addFile();     // �ܾ������ - �� ���� �߰��ϱ� ����
void newWords();    // �ܾ������ - �� �ܾ� �߰��ϱ� ����
void addWords(int num, bool is_new_file);  // num�� �ܾ������Ͽ� �ܾ �߰��ϴµ�, is_new_file�� ���ο� ���Ͽ� �߰��ϴ��� ���� ���Ͽ� �߰��ϴ��� Ȯ���Ͽ� ����
void showDic();      // �ܾ������ - �ܾ��� ���� ����
void ShowDicFiles(); // �ܾ������ - �ܾ����� ��Ϻ��� ����
void myflush();
void gotoxy(int x, int y);
void ascendingOrderWords(char * buffer, int buffersize, FILE * fp);
void randomWords(char *buffer, int buffersize, FILE *fp);
FILE * DayDicLoad();             //�Է��� ������ �ܾ����� �ε��ϴ� �Լ�.
int getSplit(char* msg, char* split, char*** result); //msg���ڿ��� split���ڿ� �������� �и��Ͽ� result�� ����, int�� ���ϰ��� split�� ����
void freeSplit(char** result, int count);  //getSpit()�Լ����� ��ȯ�� ���� ���ڿ� �迭 result�� ���� free�� ����
int compare(const void *a, const void *b); //qsort()�Լ��� ����� ���Լ�

void myFree(Node *);            //�� ���ӿ���  �Ҵ�� newNode���� free��Ű�� �Լ�. (1204 �ǽ� ����)


void myFree(Node * head)
{
	if (head->next != NULL)
		myFree(head->next);
	free(head);
}

int main(void)
{
	int menu;
	while (1)
	{
		system("clear");
		printf(">> ���� �ܾ� �ϱ� ���α׷� <<\n");
		printf("1. ���� �ܾ� ���߱�     2. �÷���ī��\n");
		printf("3. ���(hangman)        4. �ܾ��� ����\n");
		printf("5. ���α׷� ����\n\n");

		printf("��ȣ�� �����ϼ��� : ");
		scanf("%d", &menu);
		switch (menu)
		{

		case 1: wordQuiz(); break;
		case 2: flashCard(); break;
		case 3: hangman(); break;
		case 4: manageWord(); break;
		case 5:printf("�ܾ��� �����մϴ�!!\n"); sleep(5);  return 0;
		}
	}
	printf("��!!!\n");
	return 0;
}

void showDic()
{
	FILE* fp = DayDicLoad();
	char str[150];
	printf("\n----- �ܾ��� ----------\n");

	fgets(str, 150, fp);
	while (!feof(fp)) {
		printf("%s", str);
		fgets(str, 150, fp);
	}
	printf("\n�ܾ��� ���� ����(press enter)");
	gets(str);
}

void showDicFiles()
{
	char* temp;
	DIR* dir;
	struct dirent *ent;
	int num = 0;
	int count;
	int total_count = 0;
	char** split;
	int* sorted_idx;
	char** prefix = (char**)malloc(sizeof(char*));
	int* sorted_prefix = (int*)malloc(sizeof(int));
	char** dic_files = (char**)malloc(sizeof(char*));
	char temp_str[100];
	printf("\n----- �ܾ� ���� ��� ----------\n");

	dir = opendir("./");
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			strncpy(temp_str, ent->d_name, strlen(ent->d_name));
			count = getSplit(temp_str, ".", &split);
			if (count == 2) {
				if (strcmp(split[1], "dic") == 0) {
					num++;
					prefix = (char**)realloc(prefix, sizeof(char*)*num);
					prefix[num - 1] = (char*)malloc(sizeof(char)*(strlen(split[0]) + 1));
					strncpy(prefix[num - 1], split[0], strlen(split[0]));

					sorted_prefix = (int*)realloc(sorted_prefix, sizeof(int)*num);
					sorted_prefix[num - 1] = atoi(split[0]);

					dic_files = (char**)realloc(dic_files, sizeof(char*)*num);
					dic_files[num - 1] = (char*)malloc(sizeof(char)*(strlen(ent->d_name) + 1));
					strncpy(dic_files[num - 1], ent->d_name, strlen(ent->d_name));
				}
			}
			for (int a = 0; a<100; a++)
				temp_str[a] = '\0';
		}
		closedir(dir);
		freeSplit(split, count);

		qsort(sorted_prefix, num, sizeof(int), compare);
		sorted_idx = (int*)malloc(sizeof(int)*num);
		for (int i = 0; i<num; i++) {
			for (int j = 0; j<num; j++) {
				if (sorted_prefix[i] == atoi(prefix[j]))
					sorted_idx[i] = j;
			}
		}

		for (int i = 0; i<num; i++) {
			if (i % 5 == 0) {
				if (i != 0)
					printf("\n");
				printf(" ");
			}
			printf("%8s", dic_files[sorted_idx[i]]);
		}
		myflush();
		fprintf(stdout, "\n\n�ܾ����� ��� ���� ����(press enter)");
		gets(temp);
		for (int a = 0; a<num; a++) {
			free(prefix[a]);
			free(dic_files[a]);
		}
		free(prefix);
		free(sorted_prefix);
		free(sorted_idx);
		free(dic_files);
	}
	else {
		printf("���͸��� �� �� �����ϴ�.\n");
	}
}

void newWords()
{
	int day;
	printf("���ϸ�(����) : ");
	scanf("%d", &day);
	addWords(day, false);
}

void addWords(int num, bool is_new_file)
{
	FILE* fp;
	int max = 20;
	char eng_word[max][16];    //�ִ� 15����
	char kor_word[max][3][61]; //�ִ� 30����
	for (int a = 0; a<max; a++)
		for (int b = 0; b<16; b++)
			eng_word[a][b] = '\0';
	for (int a = 0; a<max; a++)
		for (int b = 0; b<3; b++)
			for (int c = 0; c<61; c++)
				kor_word[a][b][c] = '\0';
	char num_str[50];
	int i;
	char temp_str[150];
	int count;
	char** result;

	sprintf(num_str, "%d", num);
	char* file_name = strcat(num_str, ".dic");
	system("clear");
	if (is_new_file) {
		printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� : �� ���� �߰� <<\n");
		fp = fopen(file_name, "w");
	}
	else {
		printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� : �� �ܾ� �߰� <<\n");
		fp = fopen(file_name, "r");
		if (fp == NULL) {
		}
		else {
			fclose(fp);
			fp = fopen(file_name, "a");
		}
	}
	if (fp == NULL) {
		printf("\n�ش� ������ �������� �ʰų� ������ �� �� �����ϴ�\n");
		sleep(2);
	}
	else if (fp != NULL) {
		myflush();
		for (i = 0; i<max; i++) {
			gets(temp_str);
			if (strcmp(temp_str, ".add") == 0)
				break;
			count = getSplit(temp_str, " ", &result);
			if (count < 5) {
				switch (count) {
				case 4:
					strncpy(kor_word[i][2], result[3], 60);
					kor_word[i][2][60] = '\0';
				case 3:
					strncpy(kor_word[i][1], result[2], 60);
					kor_word[i][1][60] = '\0';
				case 2:
					strncpy(kor_word[i][0], result[1], 60);
					kor_word[i][0][60] = '\0';
				case 1:
					strncpy(eng_word[i], result[0], 15);
					eng_word[i][15] = '\0';
				}
			}
			if (count >= 5 || eng_word[i][16] != '\0' || kor_word[i][0][60] != '\0' || kor_word[i][1][60] != '\0' || kor_word[i][2][60] != '\0') {
				printf("�Է°��� ���� ������ �ʰ��Ͽ����ϴ�. �ٽ� �Է����ּ���.\n");
				system("clear");
				if (is_new_file)
					printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� : �� ���� �߰� <<\n");
				else
					printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� : �� �ܾ� �߰� <<\n");
				for (int j = 0; j<i; j++) {
					printf("%s %s %s %s\n", eng_word[j], kor_word[j][0], kor_word[j][1], kor_word[j][2]);
				}
				i--;
				continue;
			}
			freeSplit(result, count);
		}
		for (int k = 0; k<i; k++)
			fprintf(fp, "%s %s %s %s\n", eng_word[k], kor_word[k][0], kor_word[k][1], kor_word[k][2]);
		fclose(fp);
	}
}

void addFile()
{
	DIR *dir;
	struct dirent *ent;
	char* temp_prefix = "0";
	int num_prefix;
	int count;
	int total_num = 0;
	char** split;
	char** temp_split;
	char* new_filename;

	split = (char**)malloc(sizeof(char*));


	dir = opendir("./");
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			count = getSplit(ent->d_name, ".", &temp_split);
			total_num += count;
			split = (char**)realloc(split, sizeof(char*)*total_num);
			for (int i = total_num - count, j = 0; i<total_num; i++, j++) {
				split[i] = temp_split[j];
			}


		}
		closedir(dir);
	}
	else {
		printf("���͸��� �� �� �����ϴ�.\n");
	}

	for (int i = 0; i<total_num; i++) {
		if (strcmp(split[i], "dic") == 0) {
			if (atoi(temp_prefix) < atoi(split[i - 1])) {
				temp_prefix = (char*)calloc(strlen(split[i - 1]) + 1, sizeof(char));
				strcpy(temp_prefix, split[i - 1]);
			}
		}
	}
	freeSplit(split, total_num);
	num_prefix = atoi(temp_prefix);
	num_prefix += 1;
	addWords(num_prefix, true);
}

void manageWord()
{
	int sel;
	bool want_out = false;
	while (1) {
		system("clear");
		printf(">> ���� �ܾ� �ϱ� ���α׷� : �ܾ��� ���� <<\n");
		printf("1. �� ���� �߰��ϱ�             2. �� �ܾ� �߰��ϱ�\n");
		printf("3. �ܾ��� ����                  4. �ܾ� ���� ��Ϻ���\n");
		printf("5. �ܾ��� ���� ����\n");
		printf("\n��ȣ�� �Է��ϼ��� : ");
		scanf("%d", &sel);
		switch (sel) {
		case 1:
			addFile();
			break;
		case 2:
			newWords();
			break;
		case 3:
			showDic();
			break;
		case 4:
			showDicFiles();
			break;
		case 5:
			want_out = true;
			break;
		}
		if (want_out)
			break;
	}
}

void flashCard()
{
	int t;     //������ �ð�
	int i = 0;   // �ѱ��� �ܾ� ��½� �ݺ���
	printf("�ӵ�(��) : ");
	scanf("%d", &t);

	FILE * ffp = DayDicLoad();
	if (ffp == NULL)
		return;
	//	char word[20];         
	int output_select;
	char buffer[50];

	printf("��¹��(���ĺ� ������� : 1, ������ :2) : ");
	while (scanf("%d", &output_select) != 1)
	{
		printf("���ڸ� �Է��Ͻÿ�!\n");
		myflush();
		continue;
	}
	system("clear");
	printf(">> ���� �ܾ� �ϱ� ���α׷� : �÷���ī�� <<\n");

	if (output_select == 1)
	{
		ascendingOrderWords(buffer, 100, ffp);
	}
	else if (output_select == 2)          //��� ������ ���
		randomWords(buffer, 100, ffp);
	else
	{
		printf("1�� 2�� �Է��Ͻÿ�!!\n");
		sleep(1);
		return;
	}

	cursor = head; //������ ������ Node�� ����Ű�� cursor�� �ٽ� ù ��° Node�� ����Ű���� ����.

	while (1)
	{
		gotoxy(50, 25);
		printf("%s : ", cursor->eng_word);
		fflush(stdout);             //������� sleep�� �Ǽ� ȭ�鿡 �ȳ���. ///////////////////////////////////////////
		sleep(t);
		system("clear");
		gotoxy(60, 25);
		//for(i=0 ; i<3 ; i++);
		printf("%s\n", cursor->kor_word[0]);
		fflush(stdout);
		sleep(t);
		system("clear");

		cursor = cursor->next;
		if (cursor == NULL)
			break;
	}

	myFree(head);
	fclose(ffp); //�����߰�

}

void wordQuiz(void)                //�ܾ� ���߱� ����(�赵��)
{
	FILE * fp = DayDicLoad();

	if (fp == NULL)
		return;
	char word[20];   //�ܾ� ���ӽ� �Է¹޴� �迭
	int i = 0;
	int cnt = 0;        //���� �ܾ� ����
	int output_select; //���������� �������� �������� ���� �޴�.
	char buffer[50];  // .dic���� ������� �� ���� �Է¹ޱ� ���� �迭

	printf("��¹��(���ĺ� ������� : 1, ������ : 2) : ");

	while (scanf("%d", &output_select) != 1)   //����ó��
	{
		printf("���ڸ� �Է��Ͻÿ�!\n");
		myflush();
		continue;
	}

	system("clear");
	printf(">> ���� �ܾ� �ϱ� ���α׷� : ���� �ܾ� ���߱� <<\n");

	if (output_select == 1)
		ascendingOrderWords(buffer, 100, fp);  //�ܾ� �������� ���.
	else if (output_select == 2)          //��� ������ ���
		randomWords(buffer, 100, fp);     //������ ������ ���.
	else                                  //����ó��(1,2�� �ƴ� �����϶�)
	{
		printf("1�� 2�� �Է��Ͻÿ�!!\n");
		sleep(1);
		return;
	}
	cursor = head;  //�ʿ�? (Ŀ���� ������ ���� ��带 ����Ű�Ƿ� �� �ʿ�!! ���ϸ�����)
	i = 0;     //i�� 3�� �������Ƿ� �ٽ� 0���� ����.

	while (1)                         //(�������� ������ �����) ���� word�迭�� �Էµ� �ܾ �Է��Ͽ� �´��� Ʋ���� ���.
	{
		printf("%s -> ", cursor->kor_word[0]);
		scanf("%s", word);
		if (strcmp(cursor->eng_word, word) == 0)
		{
			printf("correct!\n");
			cnt++;            //���� ������ �¾ƾ߸� ����
		}
		else if (strcmp(".quit", word) == 0)
		{

			break;
		}
		else
			printf("incorrect!\n");
		cursor = cursor->next;
		i++;   //Ǭ ������ �������
		if (cursor == NULL)             //�̾����� �ܾ ���ٸ�...(������ �ܾ���)
			break;
	}

	printf("���� ����(cnt) : %d     , Ǭ ����(i) : %d\n", cnt, i);
	printf("����� ������ %.2f ���Դϴ�.(enter �Է��ϸ� �ʱ�޴��� �̵�)\n", (float)cnt / i * 100);

	myflush();
	myFree(head);
	fclose(fp);   // ������ ���� close... 
	if (getchar() == '\n')
		return;
}

FILE * DayDicLoad()              //�Է��� ������ �ܾ����� �ε��ϴ� �Լ�.
{
	myflush();        //�Է� ���� �����.
	FILE * fp = NULL;
	char fileHead[100];
	char fileTail[100] = ".dic";
	printf("���ϸ�(����) : ");
	gets(fileHead);
	strcat(fileHead, fileTail);    //fileHead�� "filehead.dic"���� �������� ����� ��.

	fp = fopen(fileHead, "r");
	if (fp == NULL)
	{
		printf("%s������ �����ϴ�!!!\n", fileHead);
		return NULL;
	}
	return fp;
}

void randomWords(char *buffer, int buffersize, FILE *fp)
{
	srand(time(NULL));

	head = NULL;
	cursor = NULL;                   //480 481 �����߰�



	int flag = 0; //����������϶� switch case ��������
	char * point;
	int i = 0;
	while (fgets(buffer, buffersize, fp) != NULL) {
		//fgets�� '\n'�� ���ڿ��� �Ϻη� �����Ƿ� �����ϰ� NULL���� �����ϴ� �°���
		buffer[strlen(buffer) - 1] = '\0';

		Node * newNode;
		newNode = (Node *)malloc(sizeof(Node));
		//�ؿ��� �� : newNode�ʱ�ȭ
		newNode->eng_word = NULL;
		for (i = 0; i<3; i++)
			newNode->kor_word[i] = NULL;
		newNode->next = NULL;

		point = strtok(buffer, " ");

		newNode->eng_word = (char*)malloc(sizeof(char) * strlen(point) + 1);
		strcpy(newNode->eng_word, point);

		point = strtok(NULL, " ");
		newNode->kor_word[0] = (char *)malloc(sizeof(char) * strlen(point) + 1);
		strcpy(newNode->kor_word[0], point);
		//------------------------------------------------------------------------
		//
		if (head == NULL)              //��带 �������� ����..(�̰� ����)
		{
			head = newNode;
		}
		else {
			switch (flag = rand() % 2) {
			case 0:     // ���λ��� ��带 �밡���� ����
			{
				cursor = head; //�ʿ�?
				newNode->next = head;
				head = newNode;
				break;
			}
			case 1:     // ���λ��� ��带 ������ ����
			{
				for (cursor = head; cursor->next != NULL; cursor = cursor->next)
					;
				cursor->next = newNode;
			}
			}
		}
	}
}

void ascendingOrderWords(char * buffer, int buffersize, FILE * fp)
{
	int i = 0;
	char * point;     //�� �ٿ� �߶� �ѱ�,�� �ܾ ����Ʈ�ϴ� ������
	head = NULL;             //532 533 �߰��ϴ� �����۵�..........(1204) ??
	cursor = NULL;
	while (fgets(buffer, buffersize, fp) != NULL)  //fp���� �� �� �� buffer��  �о����...
	{
		//fgets�� '\n'�� ���ڿ��� �Ϻη� �����Ƿ� �����ϰ� NULL���� �����ϴ� �°���
		buffer[strlen(buffer) - 1] = '\0';

		Node * newNode;
		newNode = (Node *)malloc(sizeof(Node));
		//�ؿ��� �� : newNode�ʱ�ȭ
		newNode->eng_word = NULL;
		for (i = 0; i<3; i++)
			newNode->kor_word[i] = NULL;
		newNode->next = NULL;

		point = strtok(buffer, " ");                 //������ �������� ���ڿ��� �߶�.

		newNode->eng_word = (char*)malloc(sizeof(char) * strlen(point) + 1);
		strcpy(newNode->eng_word, point);
		//	newNode ->eng_word = point;

		point = strtok(NULL, " ");
		newNode->kor_word[0] = (char *)malloc(sizeof(char) * strlen(point) + 1);
		strcpy(newNode->kor_word[0], point);
		//newNode -> kor_word[0] = point;
		//-----------------------------------------------
		cursor = head;
		if (head == NULL)
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
			while (cursor->next != NULL)
			{
				if (strcmp(cursor->next->eng_word, newNode->eng_word) >0)
				{
					newNode->next = cursor->next;
					cursor->next = newNode;
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
	while (getchar() != '\n')
		;
}

void gotoxy(int x, int y)
{
	printf("\033[%d;%df", y, x);
}
int hangman_word(char eng_word_line[]) {
	//�� ���ڿ��� "_"���ڿ��� ��ġ
	int length;
	length = strnlen(eng_word_line, 20);
	for (int i = 0; i < length; i++) {
		if (eng_word_line[i] == '\0')
			break;
		eng_word_line[i] = '_';
	}
	return length;
}

void hangman() { //��� �Լ�

	//��� �Լ� ��������
	int hangman_count = 1; //��� Ƚ�� 
	int try_num = 1;	   //�õ� Ƚ��
	int correct_count = 0; //�� ���ھ� ���ϸ� ���� Ƚ��
	int loop_count;		   //�ݺ� Ƚ��
	_Bool answer_state = 0;//���� �������� �ƴ��� �˻縦 ���� ����
	char input_word;       //�Է� ���� �� ����
	char eng_word_line[20];//����� ���� �� ���ڿ�
	char kor_word[20];     //���Ͽ��� ���� �� ���ڿ�
	char dic_eng[20];      //���Ͽ��� ���� �� ���ڿ�
	char buffer[50];       //���ڿ� ������ ������ �ޱ����� ���ڿ� 

	//���Ͽ��� �ش� ������ �ҷ��ͼ�, �ش� ����ü������ ������ �迭�� ����
	FILE * ffp = DayDicLoad();
	if (ffp == NULL)
		return;
	randomWords(buffer, 100, ffp);
	cursor = head;

	strcpy(eng_word_line, cursor->eng_word);
	strcpy(dic_eng, cursor->eng_word);
	strcpy(kor_word, cursor->kor_word[0]);
	int word_length = hangman_word(eng_word_line);//hangman_word() �Լ��� ȣ��
	char cmd;
	
	//��� Ƚ���� ���� �ٲ�� �ٵ� �κ�
	for (loop_count = 1; loop_count <= 20; loop_count++) {
		system("clear");
		printf(">>   ���� �ܾ� �ϱ� ���α׷� : ���   <<\n\n");
		printf(" (��Ʈ) %s\n\n", kor_word);
		switch (hangman_count) {
		case 1: {
			printf("-------------------+\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("%d ��° �õ� : ", try_num);
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
			printf("\n\n");
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
			printf("\n\n");
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
			//��� Ƚ���� �ʰ��Ǿ� ������ ����Ǵ� �κ�
		default: {
			printf("-------------------+   \n");
			printf("                   O   \n");
			printf("                  /l\\ \n");
			printf("                  / \\ \n");
			printf("\n\n");
			for (int j = 0; j <= word_length; j++) {
				printf("%c ", eng_word_line[j]);
			}
			printf("\n\n");
			printf("   ##########################\n");
			printf("   ###       Fail!!!      ###\n");
			printf("   ##########################\n");

		}break;
		}
		if (hangman_count>6)
			break;
		scanf("%c", &input_word); //�ܾ� �� ���� �Է� �κ�
		
		for (int k = 0; k < word_length; k++) { //�ݺ������� ���ڿ������� �Է��� ���ڰ� �ִ��� Ȯ��
			if (input_word == dic_eng[k]) {
				eng_word_line[k] = input_word;//���� �����̶��,'_' ���ڸ� �ش� ���ڷ� ��ġ
				answer_state = 1;             //�κ� ����Ƚ��(��)

			}
			if (eng_word_line[k] != '_') {
				correct_count++;             //���� ����Ƚ�� ���� �κ�
			}
		}
		getchar();
		//���� �¸� �κ�
		if (correct_count == word_length) {//���� ����Ƚ���� ���ڿ� ���̰� ����->����
			printf("   ##########################\n");
			printf("   ### Congratulations!!! ###\n");
			printf("   ##########################\n");
			break;
		}
		else {
			correct_count = 0;
		}

		if (answer_state == 1) {//�κ� ������ ��
			try_num++;			//�õ� Ƚ�� ����
			answer_state = 0;	//�κ����� �ʱ�ȭ
		}
		else {					//�κ� ������ �ƴ� ��
			hangman_count++;    //��� Ƚ�� ����
			try_num++;          //�õ� Ƚ�� ����
			answer_state = 0;   //�κ� ���� �ʱ�ȭ
		}
	}
	//���� ���� �κ�
	printf("�Ѱ����� ���� �Ǿ����ϴ�. �޴��� ���Ʊ����� (enter)Ű�� ��������.");
	scanf("%c", &input_word);
	if (input_word == '\n')
		system("clear");
}

int getSplit(char* msg, char* split, char*** result) {
	int i = 0;
	int charCount = 0;
	int totalCount = 0;
	char *prevPoint = msg;
	char *currPoint = NULL;
	char **array2d = NULL;
	do {
		currPoint = strstr(prevPoint, split);
		if (currPoint != NULL) {
			totalCount = currPoint - msg;
			if (prevPoint == msg) charCount = totalCount;
			else charCount = currPoint - prevPoint;

			array2d = (char**)realloc(array2d, sizeof(char*)*(i + 1));
			array2d[i] = (char*)malloc(charCount);
			strncpy(array2d[i], prevPoint, charCount);
			array2d[i][charCount] = '\0';
			prevPoint = currPoint + strlen(split);
		}
	} while (currPoint != NULL && ++i);
	if (i>0) {
		array2d = (char**)realloc(array2d, sizeof(char*)*(i + 1));
		charCount = strlen(msg) - totalCount;
		array2d[i] = (char*)malloc(charCount);
		strncpy(array2d[i], prevPoint, charCount);
		array2d[i][charCount] = '\0';
		++i;
		*result = array2d;
	}
	return i;
}

void freeSplit(char** result, int count) {
	while (--count>-1) {
		free(result[count]);
	}
	free(result);
}

int compare(const void *a, const void *b)
{
	int num1 = *(int *)a;
	int num2 = *(int *)b;

	if (num1 < num2)
		return -1;

	if (num1 > num2)
		return 1;

	return 0;
}