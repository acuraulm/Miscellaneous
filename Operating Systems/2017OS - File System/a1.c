#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_PATH_LEN 256
#define __DEBUG
#ifdef __DEBUG
void debug_info (const char *file, const char *function, const int line){
        fprintf(stderr, "DEBUG. ERROR PLACE: File=\"%s\", Function=\"%s\", Line=\"%d\"\n", file, function, line);
}
#define ERR_MSG(DBG_MSG) { \
        perror(DBG_MSG); \
        debug_info(__FILE__, __FUNCTION__, __LINE__); \
}
#else
#define ERR_MSG(DBG_MSG) { \
        perror(DBG_MSG); \
}
#endif
char *path;
char *recursive;
char *nameEnds;
char *token;
char *sizeSmalls;
int sizeSmall=0;
char name[MAX_PATH_LEN];
void variant(){
    printf("24706\n");
}
void listDir(char *dirName){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0) {
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")== 0 || strcmp(dirEntry->d_name, "..")== 0 )
            continue;
		if (S_ISDIR(inode.st_mode))
            printf("%s/%s\n",dirName,dirEntry->d_name);
		else if (S_ISREG(inode.st_mode))
				printf("%s/%s\n",dirName,dirEntry->d_name);
        else if (S_ISLNK(inode.st_mode))
                printf("%s/%s\n",dirName,dirEntry->d_name);
        else;
	}
	closedir(dir);
}
void listDirRec(char *dirName){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	char name[MAX_PATH_LEN];
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0) {
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")== 0 || strcmp(dirEntry->d_name, "..")== 0 )
            continue;
		if (S_ISDIR(inode.st_mode)){
			listDirRec(name);
            printf("%s/%s\n",dirName,dirEntry->d_name);
		}
		else if (S_ISREG(inode.st_mode))
				printf("%s/%s\n",dirName,dirEntry->d_name);
        else if (S_ISLNK(inode.st_mode))
					printf("%s/%s\n",dirName,dirEntry->d_name);
        else;
	}
	closedir(dir);
}
int matches(char *name, char *nameEnds){
    char *string;
    string = name;
    char *string2;
    string2 = nameEnds;
    int i=strlen(string)-1;
    int j=strlen(string2)-1;
    int matched=0;
    while(i>=0){
        if(string[i]==string2[j]){
            matched++;
            j--;
        }
        else
            break;
        i--;
    }
    return matched;
}
void listNameEndsRec(char *dirName, char *nameEnds){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	char name[MAX_PATH_LEN];
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0) {
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")==0 || strcmp(dirEntry->d_name, "..")==0 )
            continue;
		if (S_ISDIR(inode.st_mode)){
            if(matches(name,nameEnds)==strlen(nameEnds))
                printf("%s/%s\n",dirName,dirEntry->d_name);
            listNameEndsRec(name, nameEnds);
		}
		else if (S_ISREG(inode.st_mode)){
                if(matches(name,nameEnds)==strlen(nameEnds))
                    printf("%s/%s\n",dirName,dirEntry->d_name);
                }
			else if (S_ISLNK(inode.st_mode)){
                    if(matches(name,nameEnds)==strlen(nameEnds))
                        printf("%s/%s\n",dirName,dirEntry->d_name);
                    }
				else;
	}
	closedir(dir);
}
void listNameEnds(char *dirName, char *nameEnds){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	char name[MAX_PATH_LEN];
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0) {
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")==0 || strcmp(dirEntry->d_name, "..")==0 )
            continue;
		if (S_ISDIR(inode.st_mode)){
            if(matches(name,nameEnds)==strlen(nameEnds))
                printf("%s/%s\n",dirName,dirEntry->d_name);
        }
		else if (S_ISREG(inode.st_mode)){
                if(matches(name,nameEnds)==strlen(nameEnds))
                    printf("%s/%s\n",dirName,dirEntry->d_name);
                }
			else if (S_ISLNK(inode.st_mode)){
                    if(matches(name,nameEnds)==strlen(nameEnds))
                        printf("%s/%s\n",dirName,dirEntry->d_name);
                    }
				else;
	}
	closedir(dir);
}
void listSizeSmallerRec(char *dirName, int sizeSmall){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	char name[MAX_PATH_LEN];
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0) {
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")==0 || strcmp(dirEntry->d_name, "..")==0 )
            continue;
		if (S_ISDIR(inode.st_mode))
            listSizeSmallerRec(name, sizeSmall);
		else if (S_ISREG(inode.st_mode)){
                if(inode.st_size < sizeSmall)
                printf("%s/%s\n",dirName,dirEntry->d_name);
                }
			else if (S_ISLNK(inode.st_mode));
            else;

	}
	closedir(dir);
}
void listSizeSmaller(char *dirName, int sizeSmall){
	DIR* dir;
	struct dirent *dirEntry;
	struct stat inode;
	char name[MAX_PATH_LEN];
	dir = opendir(dirName);
	if (dir == 0) {
		ERR_MSG ("Error opening directory");
		exit(4);
	}
	while ((dirEntry=readdir(dir)) != 0){
		snprintf(name, MAX_PATH_LEN, "%s/%s",dirName,dirEntry->d_name);
		lstat (name, &inode);
		if(strcmp(dirEntry->d_name, ".")== 0 || strcmp(dirEntry->d_name, "..")== 0 )
            continue;
		if (S_ISDIR(inode.st_mode));
        else if (S_ISREG(inode.st_mode)){
            if(inode.st_size < sizeSmall)
                    printf("%s/%s\n",dirName,dirEntry->d_name);
            }
        else if (S_ISLNK(inode.st_mode));
        else;
	}
	closedir(dir);
}
void parseFile(char* path) {
    char MAGIC[2];
    int VERSION = 0;
    int HEADER_SIZE = 0;
    int NO_OF_SECTIONS = 0;
    char SECT_NAME[10];
    int SECT_TYPE;
    int SECT_SIZE;
    int SECT_OFFSET;
    int err=0;
    DIR* dir;
	//struct dirent *dirEntry;
	//struct stat inode;
	dir = opendir(path);
	if (dir != 0) {
		printf("ERROR\nGiven path is not a file\n");
		exit(6);
	}
	int FILE;
    FILE = open(path, O_RDONLY);
    if(FILE == -1){
        perror("ERROR\nbad path\n");
        exit(0);
    }
    lseek(FILE,0,SEEK_SET);
    read(FILE,MAGIC,2);
    read(FILE, &HEADER_SIZE, 2);
    read(FILE, &VERSION, 1);
    read(FILE, &NO_OF_SECTIONS, 1);
    if(strcmp("FH",MAGIC))
        err=1;
    if ((VERSION < 47) || (VERSION > 156))
        err=2;
    if ((NO_OF_SECTIONS <6) || (NO_OF_SECTIONS > 16))
        err=3;
    for (int i = 1; i <= NO_OF_SECTIONS; i++) {
        read(FILE,SECT_NAME,10);
        read(FILE,&SECT_TYPE,2);
        read(FILE,&SECT_OFFSET,4);
        read(FILE,&SECT_SIZE,4);
        if (!((SECT_TYPE == 69) || (SECT_TYPE == 31) || (SECT_TYPE == 29) || (SECT_TYPE == 80) ))
            err=4;
    }
    switch(err){
    case 0: printf("SUCCESS\n");
            printf("version=%d\n",VERSION);
            printf("nr_sections=%d\n",NO_OF_SECTIONS);
            lseek(FILE,6,SEEK_SET);
            for(int i=1; i<= NO_OF_SECTIONS; i++){
                read(FILE,SECT_NAME,10);
                read(FILE,&SECT_TYPE,2);
                read(FILE,&SECT_OFFSET,4);
                read(FILE,&SECT_SIZE,4);
                printf("section%d: %s %d %d\n",i, SECT_NAME, SECT_TYPE, SECT_SIZE);
            }
    break;
    case 1: printf("ERROR\nwrong magic\n");
            exit(1);
    break;
    case 2: printf("ERROR\nwrong version\n");
            exit(2);
    break;
    case 3: printf("ERROR\nwrong sect_nr\n");
            exit(3);
    case 4: printf("ERROR\nwrong sect_types\n");
            exit(4);
    break;
    }
closedir(dir);
}
int main(int argc, char **argv){
    if(argc==1){
    printf("Usage:\n./a1 list path=<dir_path> <name_ends_with= > or <size_smaller= >\n./a1 list recursive path=<dir_path> <name_ends_with= > or <size_smaller= >\n./a1 parse path=<file_path>\n");
    exit(5);
    }
        int pathGiven=0,recursiveListing=0,filterName=0,filterSize=0,j=0,i=0;
            for(i=0;i<argc;i++){
                if(strstr(argv[i], "path="))
                    pathGiven=1;
                if(strstr(argv[i], "recursive"))
                    recursiveListing=1;
                else if(strstr(argv[i], "name_ends_with="))
                    filterName=1;
                else if (strstr(argv[i], "size_smaller="))
                    filterSize=1;
            }
        if(strstr(argv[1], "variant")){
            variant();
        }
        if(strstr(argv[1], "list")){
            if(pathGiven == 1){
                if(recursiveListing==0 && filterName==0 && filterSize==0){ // Simple listing
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            printf("SUCCESS\n");
                            listDir(path);
                        }
                    }
                }
                if(recursiveListing==1 && filterName==0 && filterSize==0){ // Recursive listing
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            printf("SUCCESS\n");
                            listDirRec(path);
                        }
                    }
                }
                if(recursiveListing==1 && filterName==1 && filterSize==0){ // Recursive name
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "name_ends_with=")){
                            token = strtok(argv[j],"=");
                            nameEnds = strtok(NULL,"=");
                            }
                    }
                            printf("SUCCESS\n");
                            listNameEndsRec(path,nameEnds);
                }
                if(recursiveListing==0 && filterName==1 && filterSize==0){ // Simple name
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "name_ends_with=")){
                            token = strtok(argv[j],"=");
                            nameEnds = strtok(NULL,"=");
                            }
                    }
                            printf("SUCCESS\n");
                            listNameEnds(path,nameEnds);
                }
                if(recursiveListing==1 && filterName==0 && filterSize==1){ // Recursive size
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "size_smaller=")){
                            token = strtok(argv[j],"=");
                            sizeSmalls = strtok(NULL,"=");
                            sizeSmall = atoi(sizeSmalls);
                            }
                    }
                            printf("SUCCESS\n");
                            listSizeSmallerRec(path,sizeSmall);
                }
                if(recursiveListing==0 && filterName==0 && filterSize==1){ // Simple size
                    for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            }
                        if(strstr(argv[j], "size_smaller=")){
                            token = strtok(argv[j],"=");
                            sizeSmalls = strtok(NULL,"=");
                            sizeSmall = atoi(sizeSmalls);
                            }
                    }
                            printf("SUCCESS\n");
                            listSizeSmaller(path,sizeSmall);
                }
            }
            else
                printf("Path required\n");
        }
        if(strstr(argv[1], "parse")){
            if(pathGiven == 1){
                for(j=0;j<argc;j++){
                        if(strstr(argv[j], "path=")){
                            token = strtok(argv[j],"=");
                            path = strtok(NULL,"=");
                            parseFile(path);
                            }
                }
            }
            else
                printf("Path required\n");

        }
return 0;
}
//Endingz

