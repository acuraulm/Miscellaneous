#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_LEN 9999

void executeCommand(char*);
void PING(char*); // 2.3
void CREATE_SHM(char*); // 2.4
void WRITE_TO_SHM(char*); // 2.5
void aMAP_FILE(char*); // 2.6
void READ_FROM_FILE_OFFSET(char*); // 2.7 not implemented
void READ_FROM_FILE_SECTION(char*); // 2.8 not implemented
void READ_FROM_LOGICAL_SPACE_OFFSET(char*); // 2.9 not implemented
void aEXIT(char*); // 2.10

void writeString(int, char*);
void readString(int, char*);
void writeNumber(int, unsigned int);
void readNumber(int, unsigned int);

int reqPipe; // global for Request pipe
int respPipe; // global for Response pipe
unsigned int sizeM; // size of the shared memory
int sharedM;        // shared memory identifier
void* sharedMat;    // shared memory address


int main(int argc, char **argv)
{
// 2.2 Pipe-Based Connection
    //2.2.1 creates a pipe named “RESP PIPE 24706”;
    if (mkfifo("RESP_PIPE_24706", 0640) < 0) {
		printf("ERROR\ncannot create the response pipe");
		exit(1);
	}
    //2.2.2 opens a pipe named “REQ_PIPE_24706”, supposed to be created automatically by our testing program;
    reqPipe = open("REQ_PIPE_24706", O_RDONLY);
	if (reqPipe < 0) {
		printf("ERROR\ncannot open the response pipe\n");
		exit(1);
	}
    //2.2.3 writes the following request message on the “RESP_PIPE_24706” pipe: "CONNECT"
    respPipe = open("RESP_PIPE_24706",O_RDWR);
	if (respPipe < 0) {
		printf("ERROR\ncannot open the response pipe\n");
		exit(1);
	}
    writeString(respPipe, "CONNECT");

    while(argc==1){
        char* request = (char*)malloc(sizeof(char)*MAX_LEN); // Allocates memory for the request string
        readString(reqPipe, request);                  // Stores in the request string the command recieved from the REQ_PIPE_24706 pipe
        executeCommand(request);                       // Checks and executes each command`s requirements
    }
    return 0;
}

void executeCommand(char* request){
        if(!strcmp(request,"PING")){
                printf("--> Executing *PING* request <--\n");
            PING(request);
        }
        else if(!strcmp(request,"CREATE_SHM")){
                printf("--> Executing *CREATE_SHM* request <--\n");
            CREATE_SHM(request);
        }
        else if(!strcmp(request,"WRITE_TO_SHM")){
                printf("--> Executing *WRITE_TO_SHM* request <--\n");
            WRITE_TO_SHM(request);
        }
        else if(!strcmp(request,"MAP_FILE")){
                printf("--> Executing *MAP_FILE* request <--\n");
            aMAP_FILE(request);
        }
        else if(!strcmp(request,"READ_FROM_FILE_OFFSET")){
                printf("--> Executing *READ_FROM_FILE_OFFSET* request <--\n");
            READ_FROM_FILE_OFFSET(request);
        }
        else if(!strcmp(request,"READ_FROM_FILE_SECTION")){
                printf("--> Executing *READ_FROM_FILE_SECTION* request <--\n");
            READ_FROM_FILE_SECTION(request);
        }
        else if(!strcmp(request,"READ_FROM_LOGICAL_SPACE_OFFSET")){
                printf("--> Executing *READ_FROM_LOGICAL_SPACE_OFFSET* request <--\n");
            READ_FROM_LOGICAL_SPACE_OFFSET(request);
        }
        else if(!strcmp(request,"EXIT")){
                printf("--> Executing *EXIT* request <--\n");
            aEXIT(request);
        }
        else{
            exit(1);
        }
}

void writeString(int pipeN, char* string){
        printf("--> Write string *[%s]* <---\n", string);
        int sizeString = strlen(string);
        if(write(pipeN, &sizeString, 1) < 0){
            printf("--> * Could not write * <--");
            exit(1);
        }
        if(write(pipeN, string, sizeString) < 0){
             printf("--> * Could not write * <--");
            exit(1);
        }
}
void writeNumber(int pipeN, unsigned int number){
        printf("--> Write unsigned int *[ %d ]* <---\n", number);
        if(write(pipeN, &number, sizeof(unsigned int)) < 0){
            printf("--> * Could not write * <--");
            exit(1);
        }
}
void readString(int pipeN, char* string){
        int sizeString = strlen(string);
        if(read(pipeN, &sizeString, 1) < 0){
            printf("--> * Could not read * <--");
            exit(1);
        }
        if(read(pipeN, string, sizeString) < 0){
            printf("--> * Could not read * <--");
            exit(1);
        }
        printf("--> Read string *[ %s ]* <---\n", string);
}

void PING(char* request){
/* The request message looks like
                "PING"
   In response to a ping request, your program must simply send back the following response
                "PING" "PONG" 24706 */

        writeString(respPipe, request);  // Sends "PING" to the response pipe
        writeString(respPipe, "PONG");   // Sends "PONG" to the response pipe
        writeNumber(respPipe, (unsigned int)24706);  // Sends the variant to the response pipe
}

void CREATE_SHM(char* request){
/* The request message looks like
                "CREATE_SHM" 1503146
   In response to getting such a request, your program must create a shared memory region of 1503146 bytes, using the key 14802.
   The permission rights for that shared memory region must be“664”.
   If successfully created, the shared memory region must be attached by your program in its own virtual address space.
   The response message indicate a success or a failure respectively in the following way
                "CREATE_SHM" "SUCCESS"
                "CREATE_SHM" "ERROR"    */

        read(reqPipe, &sizeM, sizeof(unsigned int));             // Stores in sizeM the number sent by the request
        printf("--> Read unsigned int *[ %d ]* <---\n", sizeM);

        sharedM = shmget((key_t)14802, (size_t)sizeM, IPC_CREAT | 0664);   // Creates a shared memory of a given key and size, with 664 as permition rights
            if (sharedM < 0) {                                           // In case of failure when creating memory, an error message is sent to the response pipe
                writeString(respPipe, request);
                writeString(respPipe, "ERROR");
                exit(1);
            }else{
               sharedMat = (int*)shmat(sharedM, NULL, 0);          // Attaches the shared memory region to it`s own virtual address space
                    if(sharedMat < 0){                                 // In case of failure, an error message is sent to the response pipe
                        writeString(respPipe, request);
                        writeString(respPipe, "ERROR");
                        exit(1);
                    }else{
                        writeString(respPipe, request);
                        writeString(respPipe, "SUCCESS");
                    }
            }
}

void WRITE_TO_SHM(char* request){
/* The request message looks like
                "WRITE_TO_SHM" <offset> <value>
   The fields “offset” and “value” are number-fields and indicate an offset and a value that
   must be written in the shared memory region at the specified offset (the type unsigned int).
   Your program must validate if the given offset is inside the shared memory region (i.e. between 0 and 1503146))
   and if all the bytes that should be written also correspond to offsets inside theshared memory region.
   After writing in the shared memory, your program must send a response message indicating
   the success or failure of the write operation, in a format like those below.
                "WRITE_TO_SHM" "SUCCESS"
                "WRITE_TO_SHM" "ERROR"   */

        unsigned int offset,value;
        read(reqPipe, &offset, sizeof(unsigned int));   // Stores the first number read into the offset variable
        read(reqPipe, &value, sizeof(unsigned int));    // Stores the second number read into the value variable
        printf("--> Read unsigned int *offset:[ %d ], value:[ %d ]* <--\n", offset, value);

        if(offset < 0 || offset + sizeof(offset) > sizeM){     // If the read offset is not inside the shared memory region, an error message is sent to the response pipe
            writeString(respPipe, request);
            writeString(respPipe, "ERROR");
            exit(1);
        }else{
            memcpy(sharedMat + offset, &value, sizeof(unsigned int));       // Writes in the given shared memory, at a given offset the desired value
            writeString(respPipe, request);
            writeString(respPipe, "SUCCESS");
        }
}
void aMAP_FILE(char* request){
/* The request message looks like
                "MAP_FILE" <file_name>
   Your program should map in memory the file whose name is given by the string-field “<file_name>”.
   After mapping in memory the requested file, your program must send a response message
   indicating the success or failure of that operation, in a format like those below respectively.
                "MAP_FILE" "SUCCESS"
                "MAP_FILE" "ERROR"   */

        char* file =  (char*)malloc(sizeof(char)*MAX_LEN);
        readString(reqPipe, file);

        int openFile = open(file, O_RDONLY);        // Opens the file
            if(openFile < 0){
                   writeString(respPipe, request);
                   writeString(respPipe, "ERROR");
                   printf("--> * Could not open file * <--\n");
                   exit(1);
            }else{
                void* mapMemory = mmap(sharedMat, lseek(openFile,0,SEEK_END), PROT_READ, MAP_SHARED, openFile, 0 ); // Maps the file into the memory
                    if(mapMemory < 0){
                            writeString(respPipe, request);
                            writeString(respPipe, "ERROR");
                            printf("--> * Could not map file * <--\n");
                            exit(1);
                    }else{
                            writeString(respPipe, request);
                            writeString(respPipe, "SUCCESS");
                    }
            }
}

void READ_FROM_FILE_OFFSET(char* request){
        writeString(respPipe, request);
        writeString(respPipe, "ERROR");
	exit(1);
}
void READ_FROM_FILE_SECTION(char* request){
        writeString(respPipe, request);
        writeString(respPipe, "ERROR");
	exit(1);
}
void READ_FROM_LOGICAL_SPACE_OFFSET(char* request){
        writeString(respPipe, request);
        writeString(respPipe, "ERROR");
	exit(1);
}
void aEXIT(char* request){

        close(reqPipe);
        close(respPipe);
        munmap(sharedMat, (size_t) sizeM);
        close(sharedM);
        exit(1);
}

