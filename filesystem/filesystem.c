
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
FileTable filetable;

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

	//Create buffer to write 2 full blocks of disk with previously defined structures
	char block_buffer[BLOCK_SIZE*2];

	//Fill up superblock parameters
	superblock_tmp.total_blocks = deviceSize/BLOCK_SIZE;
	superblock_tmp.inode_blocks = BLOCK_SIZE/sizeof(INode);
	superblock_tmp.inodes = MAX_FILES;

	//Create array of Inodes
	for(int i=0; i<MAX_FILES; i++){
		inodes_tmp[i] = (INode){ "",FREE,0,{}};
	}

	//Declare bitmap size
	bitmap_tmp.size = superblock_tmp.total_blocks/(8*sizeof(char)) + 1;
	bitmap_tmp.map = malloc(bitmap_tmp.size);

	//Set all blocks to FREE in bitmap
	for(int i = 0; i < bitmap_tmp.size*8; i++){
		bitmap_setbit(bitmap_tmp.map, i, FREE);
	}
	//Set first two blocks as used
	bitmap_setbit(bitmap_tmp.map, 0, USED);
	bitmap_setbit(bitmap_tmp.map, 1, USED);

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
	memcpy(&block_buffer[sizeof(superblock_tmp)],bitmap_tmp.map,bitmap_tmp.size);
	memcpy(&block_buffer[sizeof(superblock_tmp)+bitmap_tmp.size],&inodes_tmp,sizeof(INode)*MAX_FILES);
	
	//Write buffer to disk
	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;
	if (bwrite(DEVICE_IMAGE, 1, ((char *)&(block_buffer[2048]))) == -1) return -1;
	return 0; 
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 * leo info del disco y cargo las estructuras en memoria
 */
int mountFS(void)
{
	//TODO::Check for incoming errors in disk.

	//Save 2 first blocks of disk in memory (structures)
	char block_buffer[BLOCK_SIZE*2];
	if (bread(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;
	if (bread(DEVICE_IMAGE, 1, ((char *)&(block_buffer[2048]))) == -1) return -1;

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
	char block_buffer[BLOCK_SIZE*2];

	//Pad the rest of the block
	for(int i=0; i<sizeof(block_buffer); i++){
		block_buffer[i] = 'X';
	}

	//Fill up buffer with structures in memory
	memcpy(&block_buffer,&superblock,sizeof(SuperBlock));
	memcpy(&block_buffer[sizeof(SuperBlock)],bitmap.map,bitmap.size);
	memcpy(&block_buffer[sizeof(SuperBlock)+bitmap.size],&inodes,sizeof(INode)*MAX_FILES);

	//Write data structures back to first 2 blocks of disk
	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;
	if (bwrite(DEVICE_IMAGE, 1, ((char *)&(block_buffer[BLOCK_SIZE]))) == -1) return -1;

	//Free up memory used
	if(mounted == 1){
		free(bitmap.map);
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
	//TODO:: Integrity

	//Check if filename does not already exist
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0) return -1;
	}

	//Check for free inode scanning through inodes array
	int freeInodeIndex = -1;
	for(int i = 0; i < superblock.inodes; i++){
		if(inodes[i].status == FREE) {
			//if free inode found set up info
			freeInodeIndex = i;
			inodes[i].status = USED;//mark inode as used
			inodes[freeInodeIndex].size = 0;
			//--inodes datablocks are not assigned yet
			break;
		}
	}
	if(freeInodeIndex == -1) return -2;//no free inodes left

	//At this point we have a free inode
	strcpy(inodes[freeInodeIndex].file_name, fileName);

	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{
	//TODO: Check before reomving file completelly from disk (existance of links)

	//Check if file name exists within the filesystem
	int iNodeIndex = -1;
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}
	if(iNodeIndex < 0) return -1;//file does not exist: error code -1 

	//reset the block in disk
	char block_buffer[BLOCK_SIZE]; 
	for(int i=0; i<sizeof(block_buffer); i++){
		block_buffer[i] = 'D';
	}

	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(block_buffer))) == -1) return -1;

	//clean bitmap only when needed
	if(inodes[iNodeIndex].size > 0){
		int blocksOccupiedInInode;
		if(inodes[iNodeIndex].size%BLOCK_SIZE == 0){
			blocksOccupiedInInode = inodes[iNodeIndex].size/BLOCK_SIZE;
		}else{
			blocksOccupiedInInode = inodes[iNodeIndex].size/BLOCK_SIZE + 1;
		}
		for(int i = 0; i < blocksOccupiedInInode; i++){	
			bitmap_setbit(bitmap.map, inodes[iNodeIndex].data_blocks[i], FREE);
		}
	}


	//clean inode
	inodes[iNodeIndex] = (INode){"",FREE,0,{}};

	return 0;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *fileName)
{
	//Check if filename exists
	int iNodeIndex = -1; 
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}
	if(iNodeIndex == -1) return -1;//file does not exist

	//look for free entry in SFT
	int fileEntryIndex = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].refCount == 0){
			fileEntryIndex = i;
			break;
		}
	}
	if(fileEntryIndex == -1) return -2;//not anough entries in filetable

	//set values in filetable entry
	filetable.entries[fileEntryIndex].fd = fileEntryIndex;
	filetable.entries[fileEntryIndex].offset = 0;
	filetable.entries[fileEntryIndex].refCount = 1;
	filetable.entries[fileEntryIndex].inodeIdx = iNodeIndex;
	
	return fileEntryIndex;//file descriptor
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	//Check if filedescriptor exists
	int fd = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].fd == fileDescriptor){
			fd = i;
			break;
		}
	}
	if(fd < 0) return -1;//fileDescriptor was not found in fileTable

	//free filetable entry
	filetable.entries[fileDescriptor].fd = MAX_FILES+1;
	filetable.entries[fileDescriptor].inodeIdx = MAX_FILES+1;
	filetable.entries[fileDescriptor].offset = 0;
	filetable.entries[fileDescriptor].refCount = 0;

	return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer(from a possible offset).
 * 			Particularly if the offset is at the very end of file. the bytes returned must be 0.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{	
	//TODO: can be optimized! (do not use master_buf)

	//check if fileDescriptor is valid
	int fd = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].fd == fileDescriptor){
			fd = i;
			break;
		}
	}
	if(fd < 0) return -1;//fileDescriptor was not found in fileTable

	//retrieve necessary data structures
	FileTableEntry fileEntry= filetable.entries[fileDescriptor];
	int InodeIndex = fileEntry.inodeIdx;
	INode inode = inodes[InodeIndex];

	//check if file is not empty or if offset is at end of written bytes
	if(inode.size == 0 || inode.size == fileEntry.offset) return 0;

	//read all file to buffer
	char master_buf[5*BLOCK_SIZE];
	int bytesRead = 0;
	//int currentBlock = 0;
	while(bytesRead < inode.size){
		if (bread(DEVICE_IMAGE, 0, ((char *)&(master_buf[bytesRead]))) == -1) return -1;
		bytesRead += BLOCK_SIZE;
	}
	memcpy(buffer, master_buf, inode.size - fileEntry.offset);
	return bytesRead-fileEntry.offset;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	//check if fileDescriptor is valid
	int fd = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].fd == fileDescriptor){
			fd = i;
			break;
		}
	}

	//fileDescriptor was not found in fileTable
	if(fd < 0) return -1;

	FileTableEntry fileEntry = filetable.entries[fileDescriptor];
	int inode_index = fileEntry.inodeIdx;

	//Check if inode can address enough blocks starting from offset to store numBytes
	if(numBytes+fileEntry.offset > BLOCK_SIZE*superblock.inode_blocks) return -1;

	//Allocate enough data blocks in iNode to write all bytes
	int blockIndex = inodes[inode_index].size/BLOCK_SIZE; 
	for(int i=0; i < bitmap.size*8; i++){
		if(blockIndex > (numBytes+fileEntry.offset)/BLOCK_SIZE) break;
		if(bitmap_getbit(bitmap.map,i) == 0){
			inodes[inode_index].data_blocks[blockIndex] = i;
			blockIndex++;
			bitmap_setbit(bitmap.map,i,1);
		}
		//No free blocks available
		if(i == bitmap.size*8) return -1;
	}
	
	char block_buffer[BLOCK_SIZE];
	int bytesWritten = 0;
	int currentBlock = fileEntry.offset/BLOCK_SIZE;

	//Write the rest of first offset(if any) block to disk
	if(inodes[inode_index].size > 0){
		if (bread(DEVICE_IMAGE, inodes[inode_index].data_blocks[currentBlock], ((char *)&(block_buffer))) == -1) return -1;
		memcpy(block_buffer,buffer,BLOCK_SIZE-fileEntry.offset);
		if (bwrite(DEVICE_IMAGE, currentBlock, ((char *)&(block_buffer))) == -1) return -1;
		currentBlock++;
		bytesWritten += BLOCK_SIZE-fileEntry.offset;
	}
		
	//Write the rest of blocks until all bytes have been written
	int bytesToWrite = BLOCK_SIZE;
	while(bytesWritten < numBytes){

		//Check if remaining bytes are less than a full block
		if(numBytes-bytesWritten < BLOCK_SIZE){
			bytesToWrite = numBytes-bytesWritten;
		}

		//Flush buffer
		for(int i=0; i<sizeof(block_buffer); i++){
			block_buffer[i] = '0';
		}

		//Fill up buffer with bytes to write
		memcpy(block_buffer,(char *)buffer + bytesWritten,bytesToWrite);

		//Write buffer to disk on current block
		if (bwrite(DEVICE_IMAGE, inodes[inode_index].data_blocks[currentBlock], ((char *)&(buffer))) == -1) return -1;
		
		currentBlock++;
		bytesWritten += bytesToWrite;
	}

	inodes[fileEntry.inodeIdx].size += numBytes;
	fileEntry.offset += numBytes; //TODO: Ask Jose

	return bytesWritten;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	//TODO:
	return -1;
}

/*
 * @brief	Checks the integrity of the file.
 * @return	0 if success, -1 if the file is corrupted, -2 in case of error.
 */

int checkFile (char * fileName)
{
	//TODO:
    return -2;
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
	//TODO:
    return -2;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{
	//TODO:	
    return -2;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{
	//TODO:
    return -1;
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{
	//TODO:
    return -1;
}

/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{
	//TODO:
    return -2;
}
