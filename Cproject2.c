#include <stdio.h>
#include <unistd.h>            //sleep()의 헤더파일
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#pragma warning(disable: 4996)

int fileNum = 0;
typedef struct word_list {
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
void manageWord();
void addFile();
void newWords();
void addWords(int num, bool is_new_file);
void showDic();
void ShowDicFiles();
void myflush();
void gotoxy(int x, int y);
void ascendingOrderWords(char * buffer, int buffersize, FILE * fp);
void randomWords(char *buffer, int buffersize, FILE *fp);
FILE * DayDicLoad();             //입력한 일차의 단어장을 로드하는 함수.
int getSplit(char* msg, char* split, char*** result); //msg문자열을 split문자열 기준으로 분리하여 result로 리턴, int형 리턴값은 split된 개수
void freeSplit(char** result, int count);
int compare(const void *a, const void *b); //qsort()함수에 사용할 비교함수

int main(void)
{
	int menu;
	while (1)
	{
		system("clear");
		printf(">> 영어 단어 암기 프로그램 <<\n");
		printf("1. 영어 단어 맞추기     2. 플래쉬카드\n");
		printf("3. 행맨(hangman)        4. 단어장 관리\n");
		printf("5. 프로그램 종료\n\n");

		printf("번호를 선택하세요 : ");
		scanf("%d", &menu);
		switch (menu)
		{

		case 1: wordQuiz(); break;
		case 2: flashCard(); break;
		case 3: hangman(); break;
		case 4: manageWord(); break;
		case 5:printf("단어장 종료합니다!!\n"); sleep(5);  return 0;
		}
	}
	printf("끝!!!\n");
	return 0;
}

void showDic()
{
	FILE* fp = DayDicLoad();
	char str[150];
	printf("\n----- 단어장 ----------\n");

	fgets(str, 150, fp);
	while (!feof(fp)) {
		printf("%s", str);
		fgets(str, 150, fp);
	}
	printf("\n단어장 보기 종료(press enter)");
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
	printf("\n----- 단어 파일 목록 ----------\n");

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
		fprintf(stdout, "\n\n단어파일 목록 보기 종료(press enter)");
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
		printf("디렉터리를 열 수 없습니다.\n");
	}
}

void newWords()
{
	int day;
	printf("파일명(일차) : ");
	scanf("%d", &day);
	addWords(day, false);
}

void addWords(int num, bool is_new_file)
{
	FILE* fp;
	int max = 20;
	char eng_word[max][16];    //최대 15문자
	char kor_word[max][3][61]; //최대 30문자
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
		printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 파일 추가 <<\n");
		fp = fopen(file_name, "w");
	}
	else {
		printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 단어 추가 <<\n");
		fp = fopen(file_name, "r");
		if (fp == NULL) {
		}
		else {
			fclose(fp);
			fp = fopen(file_name, "a");
		}
	}
	if (fp == NULL) {
		printf("\n해당 파일이 존재하지 않거나 파일을 열 수 없습니다\n");
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
				printf("입력가능 문자 개수를 초과하였습니다. 다시 입력해주세요.\n");
				system("clear");
				if (is_new_file)
					printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 파일 추가 <<\n");
				else
					printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 단어 추가 <<\n");
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
		printf("디렉터리를 열 수 없습니다.\n");
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
		printf(">> 영어 단어 암기 프로그램 : 단어장 관리 <<\n");
		printf("1. 새 파일 추가하기             2. 새 단어 추가하기\n");
		printf("3. 단어장 보기                  4. 단어 파일 목록보기\n");
		printf("5. 단어장 관리 종료\n");
		printf("\n번호를 입력하세요 : ");
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
	//Node * head = NULL;
	//Node * cursor = NULL;
	int t;     //지연될 시간
	printf("속도(초) : ");
	scanf("%d", &t);

	FILE * ffp = DayDicLoad();
	if (ffp == NULL)
		return;
	char word[20];             //변수명 같아도 되나?
	int output_select;
	char buffer[50];

	printf("출력방식(알파벳 순서대로 : 1, 무작위 :2) : ");
	while (scanf("%d", &output_select) != 1)
	{
		printf("숫자를 입력하시오!\n");
		myflush();
		continue;
	}
	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 플래쉬카드 <<\n");

	if (output_select == 1)
	{
		ascendingOrderWords(buffer, 100, ffp);
	}
	else if (output_select == 2)          //노드 무작위 출력
		randomWords(buffer, 100, ffp);
	else
	{
		printf("1과 2만 입력하시오!!\n");
		sleep(1);
		return;
	}

	cursor = head;

	while (1)
	{
		gotoxy(50, 25);
		printf("%s : ", cursor->eng_word);
		fflush(stdout);             //출력전에 sleep이 되서 화면에 안나옴. ///////////////////////////////////////////
		sleep(t);
		printf("%s\n", cursor->kor_word[0]);
		fflush(stdout);
		sleep(t);
		system("clear");

		cursor = cursor->next;
		if (cursor == NULL)
			break;
	}

}

void wordQuiz(void)
{
	FILE * fp = DayDicLoad();

	if (fp == NULL)
		return;
	char word[20];   //단어 게임시 입력받는 배열
	int i = 0;
	int cnt = 0;        //맞힌 단어 갯수
	int output_select; //무작위인지 오름차순 정렬인지 고르는 메뉴.
	char buffer[50];  // .dic에서 개행기준 한 줄을 입력받기 위한 배열

	printf("출력방식(알파벳 순서대로 : 1, 무작위 : 2) : ");

	while (scanf("%d", &output_select) != 1)
	{
		printf("숫자를 입력하시오!\n");
		myflush();
		continue;
	}

	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 영어 단어 맞추기 <<\n");

	if (output_select == 1)
		ascendingOrderWords(buffer, 100, fp);

	else if (output_select == 2)          //노드 무작위 출력
		randomWords(buffer, 100, fp);
	else
	{
		printf("1과 2만 입력하시오!!\n");
		sleep(1);
		return;
	}

	cursor = head;  //필요? (커서가 마지막 전의 노드를 가리키므로 꼭 필요!! 안하면좆됨)
	i = 0;  //위에서 i 가3 로 되어있으므로 다시 0으로 초기화시킨다.
	printf("cursor 주송:%d\n\n\n", cursor->eng_word);
	while (1)                         //(무작위로 연결된) 노드를 출력...
	{
		printf("%s -> ", cursor->kor_word[0]);
		scanf("%s", word);
		if (strcmp(cursor->eng_word, word) == 0)
		{
			printf("correct!\n");
			cnt++;            //맞은 문제는 맞아야만 증가
		}
		else if (strcmp(".quit", word) == 0)
		{

			break;
		}
		else
			printf("incorrect!\n");
		cursor = cursor->next;
		i++;   //푼 문제는 계속증가
		if (cursor == NULL)
			break;
	}
	//}

	printf("맞은 문제(cnt) : %d     , 푼 문제(i) : %d\n", cnt, i);
	printf("당신의 점수는 %.2f 점입니다.(enter 입력하면 초기메뉴로 이동)\n", (float)cnt / i * 100);

	myflush();
	if (getchar() == '\n')
		return;
}

FILE * DayDicLoad()              //입력한 일차의 단어장을 로드하는 함수.
{
	myflush();
	FILE * fp = NULL;
	char fileHead[100];
	char fileTail[100] = ".dic";
	printf("파일명(일차) : ");
	gets(fileHead);
	strcat(fileHead, fileTail);

	fp = fopen(fileHead, "r");
	if (fp == NULL)
	{
		printf("%s파일이 없습니다!!!\n", fileHead);
		return NULL;
	}
	return fp;
}

void randomWords(char *buffer, int buffersize, FILE *fp)
{
	srand(time(NULL));
	int flag = 0; //무작위출력하때 switch case 쓰기위함
	char * point;
	int i = 0;
	while (fgets(buffer, buffersize, fp) != NULL) {
		//fgets는 '\n'도 문자열의 일부로 받으므로 제거하고 NULL문자 삽입하는 는과정
		buffer[strlen(buffer) - 1] = '\0';

		Node * newNode;
		newNode = (Node *)malloc(sizeof(Node));
		//밑에세 줄 : newNode초기화
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
		// head = NULL 이거 해야되나??(계속 호출할시 head가 멀 가리키고 있으면 안될듯-나중에 확인)
		if (head == NULL)              //노드를 무작위로 연결..(이게 시작)
		{
			head = newNode;
			//  continue;
		}
		else {
			switch (flag = rand() % 2) {
			case 0:     // 새로생긴 노드를 대가리에 이음
			{
				cursor = head; //필요?
				newNode->next = head;
				head = newNode;
				break;
			}
			case 1:     // 새로생긴 노드를 꼬리에 이음
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
	char * point;     //각 줄에 잘라낸 한글,영 단어를 포인트하는 포인터
	while (fgets(buffer, buffersize, fp) != NULL)
	{
		//fgets는 '\n'도 문자열의 일부로 받으므로 제거하고 NULL문자 삽입하는 는과정
		buffer[strlen(buffer) - 1] = '\0';

		Node * newNode;
		newNode = (Node *)malloc(sizeof(Node));
		//밑에세 줄 : newNode초기화
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

	int length;
	length = strnlen(eng_word_line, 20);
	for (int i = 0; i < length; i++) {
		if (eng_word_line[i] == '\0')
			break;
		eng_word_line[i] = '_';
	}
	return length;
}

void hangman() {
	int hangman_count = 1;
	int try_num = 1;
	int correct_count = 0;
	int loop_count;
	_Bool answer_state = 0;
	char input_word;
	char eng_word_line[20];
	char kor_word[20];
	char dic_eng[20];
	char buffer[50];

	FILE * ffp = DayDicLoad();
	if (ffp == NULL)
		return;
	randomWords(buffer, 100, ffp);
	cursor = head;

	strcpy(eng_word_line, cursor->eng_word);
	strcpy(dic_eng, cursor->eng_word);
	strcpy(kor_word, cursor->kor_word[0]);
	int word_length = hangman_word(eng_word_line);
	char cmd;

	for (loop_count = 1; loop_count <= 20; loop_count++) {
		system("clear");
		printf(">>   영어 단어 암기 프로그램 : 행맨   <<\n\n");
		printf(" (힌트) %s\n\n", kor_word);
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
			printf("%d 번째 시도 : ", try_num);
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
			printf("\n\n");
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
		if(hangman_count>6)
			break;
		scanf("%c", &input_word);

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
	printf("한게임이 종료 되었습니다. 메뉴로 돌아기위해 (enter)키를 누르세요.");
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
