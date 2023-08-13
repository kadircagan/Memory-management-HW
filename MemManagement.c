#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define sayfa_Boyutu 4096
#define sayfa_Sayisi 256
#define assumed_ProcessSize 100 


int fake_pageTable[assumed_ProcessSize][sayfa_Sayisi];
char page_table[sayfa_Sayisi][sayfa_Boyutu];
char process_Table[assumed_ProcessSize][sayfa_Boyutu][sayfa_Boyutu];
int currentPage = 500;
int hitCount =0;
int hitMissCount=0;
int doeshit[256]={0};


int firstLineCounter();
int countPageSize(char *fileName);
void  readPageFile(char * filename, int whichFile);
void readfunc();

int main(int argc, char **argv)
{
    srand(time(NULL));
    FILE * fp;
	fp = fopen("task_list.txt", "r");
    int lineCount = firstLineCounter("tasks.txt");
    for(int i =0;i<lineCount;i++){
        int j=0;
        while(fscanf(fp, " %s", process_Table[i][j]) != EOF )
		{
			char next = fgetc(fp);
			fseek(fp,-1L,SEEK_CUR);
			if(next == '\n')
			{
				break;
			}
			j++;
        }
    }

	for(int i=0;i<lineCount;i++)
	{
		int pageSize=countPageSize(process_Table[i][0]);
		if(pageSize<currentPage){
			readPageFile(process_Table[i][0],i);
		}else{
			strcpy(process_Table[i][0],"pass");
			printf("Bu dosya çok buyuk %s\n", process_Table[i][0]);
		}

	}

	int processControl[lineCount];
	for (int i = 0; i < lineCount; i++)
	{
		processControl[i]=1;
	}

	
	int count=0;
	int i=0;
	while(1){
	//	printf("i geliyor= %d    %d\n",i,processControl[i]);
		int j=processControl[i];
		int forExit=5+processControl[i];
		//printf(" %d  %d \n",forExit,j);
		for(;j<forExit;j++){
			if(process_Table[i][j][0]== '\0'){
				break;
			}
			else{
				readfunc(process_Table[i][j], i);
				//printf("%s ",process_Table[i][j]);
				count=0;
			}
			
		}
		processControl[i]=j;

		if(i<lineCount){
			i++;
		}else{
			i=0;
		}
		if(count==lineCount+1){
		break;
		}
		else{
		count++;
		}

	}
    printf("Total hit Sayisi: %d \n",hitCount);
	printf("Total miss Sayisi: %d \n",hitMissCount);


}

void readfunc(char* okunacak,int whichFile){
	if(strcmp(process_Table[whichFile][0],"pass")){
		
	}else{
		return;
	}
	//printf("%s \n\n\n\n\n\n",page_table[fake_pageTable[0][i]]);
	int okunacakInt;
	sscanf(okunacak, "%d", &okunacakInt);
	//printf("%d ",okunacakInt);
	int pageNumber;
	pageNumber=okunacakInt/4096;
	int asilAdres=fake_pageTable[whichFile][pageNumber];
	char* sonuc=page_table[asilAdres];
	int okunacakSonuc=okunacakInt%4096;
	if(sonuc[okunacakSonuc]=='\0'){
	printf("Okunacak Sonuc Bulunamadi\n");
	return;
	}
	printf("aradıgımız byte = '%c' , okunacak process =%d , processinPagesi = %d , asilAdres = %d\n", sonuc[okunacakSonuc],okunacakSonuc,pageNumber,asilAdres);
	if(doeshit[asilAdres]==0){
		//printf("miss");
		doeshit[asilAdres]=1;
		hitMissCount++;
	}else{
		//printf("hit");
		hitCount++;
	}


	
}
int countPageSize(char *fileName){
	FILE *fp;
		fp = fopen(fileName, "r");
		fseek(fp,0L,SEEK_END);
		int size = ftell(fp);
		fclose(fp);
		int space=0;
		if(size%4096==0){
		space = size/4096;
		}
		else{
		space = (size/4096)+1;
		}
		//printf("length of space and sz %d   %d \n",space,size);
		return space;
}

int firstLineCounter()
{
	FILE *fp;
	int count = 0; // Line counter (result)
	char *filename;
	char c; // To store a character read from file
	filename="tasks.txt";

	// Open the file
	fp = fopen(filename, "r");

	// Check if file exists
	if (fp == NULL)
	{
		printf("Could not open file %s", filename);
		return 0;
	}

	// Extract characters from file and store in character c
	for (c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n') // Increment count if this character is newline
			count = count + 1;

	// Close the file
	fclose(fp);

	return count;
}
void  readPageFile(char * filename, int whichFile){
	printf("reading %s\n", filename);

	FILE *fp;
	fp = fopen(filename, "r");
	int printCount = 0;

    int c;
	int size = 10;
	int count = 0;
	int countpage = -1;

	while((c = getc(fp)) != EOF)//hocam dosya okumalarda internetten yardım aldım 
	{
		int rand_page;
		if(count == 0) 
		{
			while((rand_page = rand() % sayfa_Sayisi))
			{
				if (page_table[rand_page][0] == '\0')
				{
					countpage++;
					currentPage--;
					page_table[rand_page][count] = c;
					fake_pageTable[whichFile][countpage] = rand_page;
					printCount++;
					break;				
				}
			}
		}
		else
		{
			page_table[rand_page][count] = c;
		}
		count++;
		if(count == 4096)
		{
		 count = 0;
		}
		
	}


	for(int i = 0; i < printCount; i ++)
	{
		printf("%d ",fake_pageTable[whichFile][i]);
	}
	printf("\n");


}



