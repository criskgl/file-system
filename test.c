
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 *
 */


#include <stdio.h>
#include <string.h>
#include "filesystem/filesystem.h"


// Color definitions for asserts
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 300				  // Number of blocks in the device
#define DEV_SIZE N_BLOCKS *BLOCK_SIZE // Device size, in bytes


void fileNameAlreadyExists(){ 
    char* filename1 = "file.txt";
    createFile(filename1);
    char * filename2 = "file.txt";
    int ret = createFile(filename2);

    if (ret == -1)
	{
        fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST fileNameAlreadyExists ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
    	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST fileNameAlreadyExists ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void fileWasDeleted(){ 
	char* filename1 = "deleteme.txt";
    createFile(filename1);
	int ret = removeFile(filename1);
	if (ret == 0)
	{
        fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST fileWasDeleted ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else if (ret == -1){
    	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST fileWasDeleted ", ANSI_COLOR_RED, "FAILED: filename not found\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST fileWasDeleted ", ANSI_COLOR_RED, "FAILED: no suggestion\n", ANSI_COLOR_RESET);
	}
}

void testOpenFile(){ 
	char* filename1 = "fileToBeOpened.txt";
    createFile(filename1);
	int ret = openFile(filename1);
	if (ret == 0)
	{
        fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testOpenFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testOpenFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testCloseFile(){ 
	char* filename1 = "fileToBeClosed.txt";
    createFile(filename1);
	int fd = openFile(filename1);
	if(fd < 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testCloseFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	int ret = closeFile(fd);
	if (ret == 0)
	{
        fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testCloseFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testCloseFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testWriteFile(){ 
	char* filename1 = "fileToBeWritten.txt";
    createFile(filename1);
	int fd = openFile(filename1);
	if(fd < 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	
	char buffer[1023] = "HOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOL";
	int ret = 0;
	for(int i = 0; i < 4; i++){
		if(i == 3){
			printf("hello");
		}
		ret = writeFile(fd, buffer, 1023);
		printf("%d\n", ret);
	}
	if (ret == -1)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}
}

void testReadFileNoOffset(){ 
	char* filename1 = "fileToBeReadNoOffset.txt";
    createFile(filename1);
	int fd = openFile(filename1);
	if(fd < 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	
	char buffer[2048] = "Hello this is the content of my file. It is quite quite quite quite quite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quite large";
	int ret = writeFile(fd, buffer, 212);
	if (ret == -1)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}

	char read_buf[2048];
	ret = readFile(fd, read_buf, 20000);
	if(ret == -1) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	printf("%s", read_buf);
	if(strcmp(read_buf, buffer) == 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testLseekEnd(){ 
	char* filename1 = "fileToSeekEnd.txt";
    createFile(filename1);
	int fd = openFile(filename1);

	writeFile(fd, "0123456789", 10);
	
	int ret = lseekFile(fd, 0, FS_SEEK_END);

	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekEnd ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekEnd ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testLseekBegin(){ 
	char* filename1 = "fileToSeekBegin.txt";
    createFile(filename1);
	int fd = openFile(filename1);

	writeFile(fd, "0123456789", 10);
	
	int ret = lseekFile(fd, 10, FS_SEEK_BEGIN);

	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekBegin ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekBegin ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testLseekAfterSize(){ 
	char* filename1 = "fileToSeekAfterSize.txt";
    createFile(filename1);
	int fd = openFile(filename1);

	writeFile(fd, "0123456789", 10);
	
	int ret = lseekFile(fd, 1, FS_SEEK_CUR);

	if (ret == -1)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekAfterSize ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekAfterSize ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

void testLseekBelow0(){ 
	char* filename1 = "fileToSeekBelow0.txt";
    createFile(filename1);
	int fd = openFile(filename1);

	writeFile(fd, "0123456789", 10);
	
	int ret = lseekFile(fd, -11, FS_SEEK_CUR);

	if (ret == -1)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekBelow0 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testLseekBelow0 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
}

int main()
{
	int ret;
	///////
	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	//fileNameAlreadyExists();

	ret = createFile("test.txt");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////
	//fileWasDeleted();
	//////
	//testOpenFile();
	//////
	//testCloseFile();
	/////
	testWriteFile();
	/////
	//testReadFileNoOffset();
	/////
	//testLseekEnd();
	/////
	//testLseekBegin();
	/////
	//testLseekAfterSize();
	/////
	//testLseekBelow0();
	/////
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	return 0;
}




