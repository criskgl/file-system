
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

void testWriteLessThanBlock(){ 
	char* filename1 = "fileToBeWrittenLessThanBlock.txt";
    createFile(filename1);
	int fd = openFile(filename1);
	if(fd < 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteLessThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	
	char buffer[2047] = "HOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLA";
	int ret = 0;
	for(int i = 0; i < 5; i++){
		ret = writeFile(fd, buffer, 2047);
		if(ret != 2047){
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteLessThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			return;
		}
	}
	char buffer2[6] = "ABCDEF";
	ret = writeFile(fd, buffer2, 6);
	if(ret != 5){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteLessThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteLessThanBlock ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
}

void testWriteMoreThanBlock(){ 
	char* filename1 = "fileToBeWrittenMoreThanBlock.txt";
    createFile(filename1);
	int fd = openFile(filename1);
	if(fd < 0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteMoreThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	
	char buffer[2049] = "HOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHOLAHO";
	int ret = 0;
	for(int i = 0; i < 4; i++){
		ret = writeFile(fd, buffer, 2049);
		if(ret != 2049){
			fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteMoreThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			return;
		}
	}

	ret = writeFile(fd, buffer, 2049);
	if(ret != 2044){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteMoreThanBlock ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testWriteMoreThanBlock ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
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
	closeFile(fd);
	fd = openFile(filename1);
	char read_buf[2048];
	ret = readFile(fd, read_buf, 20000);
	if(ret == -1) fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testReadFileNoOffset ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
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

void testCreateLn(){ 
	char* filename = "originalFile.txt";
    createFile(filename);
	int fd = openFile(filename);

	writeFile(fd, "0123456789", 10);
	
	closeFile(fd);

	char* linkname = "linkToOriginalFile.txt";

	createLn(filename, linkname);

	fd = openFile(linkname);

	char read_buf[10];
	readFile(fd, read_buf, 10);

	if (strcmp(read_buf,"0123456789") == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testCreateLn ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST testCreateLn ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
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
	//testWriteLessThanBlock();
	/////
	//testWriteMoreThanBlock();
	/////
	testReadFileNoOffset();
	/////
	//testLseekEnd();
	/////
	//testLseekBegin();
	/////
	//testLseekAfterSize();
	/////
	//testLseekBelow0();
	/////
	testCreateLn();
	////
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	return 0;
}




