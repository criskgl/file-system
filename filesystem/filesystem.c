
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	Last revision 01/04/2020
 *
 */

#include "filesystem/filesystem.h" // Headers for the core functionality
#include "filesystem/auxiliary.h"  // Headers for auxiliary functions
#include "filesystem/metadata.h"   // Type and structure declaration of the file system
#include <stdlib.h>
#include <string.h>

// Data structures stored in memory when fs is mounted
int mounted = 0;
SuperBlock superblock;
INode inodes[MAX_FILES];
BitMap bitmap;

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize) { 

	if (deviceSize < MIN_DEV_SIZE || deviceSize > MAX_DEV_SIZE) return -1;

	//Define basic structures for file system
	SuperBlock superblock_tmp;
 	INode inodes_tmp[MAX_FILES]; 
	BitMap bitmap_tmp;

	//Create buffer to write a full block of disk with previously defined structures
	char block_buffer[BLOCK_SIZE];

	//Fill up superblock parameters
	superblock_tmp.magic_number = 1234;
	superblock_tmp.total_blocks = deviceSize/BLOCK_SIZE;
	superblock_tmp.inode_blocks = BLOCK_SIZE/sizeof(INode);
	superblock_tmp.inodes = MAX_FILES;

	//Create array of Inodes
	for(int i=0; i<MAX_FILES; i++){
		inodes_tmp[i] = (INode){NULL,FREE,0,{}};
	}

	//Declare bitmap size
	bitmap_tmp.size = superblock_tmp.total_blocks/(8*sizeof(char)) + 1;
	bitmap_tmp.map = malloc(bitmap_tmp.size);

	//Set all blocks to FREE in bitmap
	for(int i = 0; i < bitmap_tmp.size*8; i++){
		bitmap_setbit(bitmap_tmp.map, i, FREE);
	}
	
	//Declare extra bit in imap as used to always ignore them
	for(int i = superblock_tmp.total_blocks; i < bitmap_tmp.size*8; i++){ 
		bitmap_setbit(bitmap_tmp.map, i, USED);
	}

	//Pad the rest of the block
	for(int i=0; i<sizeof(block_buffer); i++){
		block_buffer[i] = 'X';
	}

	//Write structures to disk
	memcpy(&block_buffer,&superblock_tmp,sizeof(SuperBlock));
	memcpy(&block_buffer[sizeof(SuperBlock)],bitmap_tmp.map,bitmap_tmp.size);
	memcpy(&block_buffer[sizeof(SuperBlock)+bitmap_tmp.size],&inodes_tmp,sizeof(INode)*MAX_FILES);

	//Write buffer to disk
	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;

	return 0; 
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 * leo info del disco y cargo las estructuras en memoria
 */
int mountFS(void)
{
	//Save first block of disk in memory
	char block_buffer[BLOCK_SIZE];
	if (bread(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;

	//Fill up memory structures from block buffer
	memcpy(&superblock, &block_buffer,sizeof(superblock));
	bitmap.size = superblock.total_blocks/(8*sizeof(char)) + 1;
	bitmap.map = malloc(bitmap.size);
	memcpy(bitmap.map, &block_buffer[sizeof(superblock)], bitmap.size);
	memcpy(&inodes, &block_buffer[sizeof(superblock)+bitmap.size], sizeof(INode)*superblock.inodes);

	//Mark device as mounted
	mounted = 1;

	return 0;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 * whatever we have in memory we need to put it in disk
 */
int unmountFS(void)
{
	char block_buffer[BLOCK_SIZE];
	superblock.magic_number = 4321;
	//Write data back to disk
	memcpy(&block_buffer,&superblock,sizeof(SuperBlock));
	memcpy(&block_buffer[sizeof(SuperBlock)],bitmap.map,bitmap.size);
	memcpy(&block_buffer[sizeof(SuperBlock)+bitmap.size],&inodes,sizeof(INode)*MAX_FILES);

	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;

	//Free up memory used
	if(mounted == 1){

		free(bitmap.map);

		for(int i=0; i<superblock.inodes; i++){
			free(inodes[i].file_name);
		}

		mounted = 0;
		return 0;
	}
	
	return -1;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *fileName)
{
	//TODO
	return -2;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{
	return -2;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *fileName)
{
	return -2;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	return -1;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	return -1;
}

/*
 * @brief	Checks the integrity of the file.
 * @return	0 if success, -1 if the file is corrupted, -2 in case of error.
 */

int checkFile (char * fileName)
{
    return -2;
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
    return -2;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{

    return -2;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{
    return -1;
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{
    return -1;
}

/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{
    return -2;
}
