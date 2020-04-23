
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


void fileNameAlreadyExists(){ //Id: 1
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

void fileWasDeleted(){ //Id: 2
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




