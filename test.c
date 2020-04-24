
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
	
	char buffer[2048] = "Hello this is the content of my file. It is quite quite quite quite quite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quitequite quite quite quite large";
	int ret = writeFile(fd, buffer, 212);
	if (ret == -1)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}
}

void testReadFileNoOffset(){ 
	char* filename1 = "fileToBeWritten.txt";
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
	ret = readFile(fd, read_buf, 2048);
	if(ret == -1) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	printf("%s", read_buf);
	if(strcmp(read_buf, buffer) == 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
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

	fileNameAlreadyExists();

	ret = createFile("/test.txt");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////
	fileWasDeleted();
	//////
	testOpenFile();
	//////
	testCloseFile();
	/////
	testWriteFile();
	/////
	testReadFileNoOffset();
	/////
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	return 0;
}




