
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
	superblock_tmp.block_size = BLOCK_SIZE;
	superblock_tmp.inodes = MAX_FILES;

	//Create array of Inodes
	for(int i=0; i<MAX_FILES; i++){
		inodes_tmp[i] = (INode){ "",FREE,0,0,{-1,-1,-1,-1,-1},-1,FALSE,0};
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
	char block_buffer[superblock.block_size*2];

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
	if (bwrite(DEVICE_IMAGE, 1, ((char *)&(block_buffer[superblock.block_size]))) == -1) return -1;

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

	//Check if filename does not already exist
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0) return -1;
	}

	//check if length of name is less than 32 bytes
	int tmp = strlen(fileName);
	if(tmp > 32) return -2;

	//Check for free inode scanning through inodes array
	int freeInodeIndex = -1;
	for(int i = 0; i < superblock.inodes; i++){
		if(inodes[i].status == FREE) {
			//if free inode found set up info
			freeInodeIndex = i;
			inodes[i].status = USED;//mark inode as used
			inodes[freeInodeIndex].size = 0;
			inodes[freeInodeIndex].blocks_assigned = 0;
			strcpy(inodes[freeInodeIndex].file_name, fileName);
			break;
		}
	}
	//no free inodes left
	if(freeInodeIndex == -1) return -2;

	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{	
	//Check if file name exists within the filesystem
	int iNodeIndex = -1;
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}

	//file does not exist: error code -1 
	if(iNodeIndex < 0) return -1;

	//clean bitmap only when needed
	if(inodes[iNodeIndex].size > 0){
		int blocksOccupiedInInode;
		if(inodes[iNodeIndex].size%superblock.block_size == 0){
			blocksOccupiedInInode = inodes[iNodeIndex].size/superblock.block_size;
		}else{
			blocksOccupiedInInode = inodes[iNodeIndex].size/superblock.block_size + 1;
		}
		for(int i = 0; i < blocksOccupiedInInode; i++){	
			//free blocks
			bitmap_setbit(bitmap.map, inodes[iNodeIndex].data_blocks[i], FREE);
		}
	}

	//clean inode actually removes file from filesystem
	//(but data previously written in disk remains the same until overwritten)
	inodes[iNodeIndex] = (INode){ "",FREE,0,0,{-1,-1,-1,-1,-1},-1,FALSE,0};

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

	//file does not exist
	if(iNodeIndex == -1) return -1;

	//check if file has integrity
	if(inodes[iNodeIndex].has_integrity == TRUE){
		return -2;
	}

	//check if inode is soft link
	if(inodes[iNodeIndex].soft_link != -1){
		//look for original file
		iNodeIndex = inodes[iNodeIndex].soft_link;
	}

	//look for free entry in SFT
	int fileEntryIndex = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].used == 0){
			fileEntryIndex = i;
			break;
		}
	}

	//not anough entries in filetable
	if(fileEntryIndex == -1) return -2;

	//set values in filetable entry
	filetable.entries[fileEntryIndex].fd = fileEntryIndex;
	filetable.entries[fileEntryIndex].offset = 0;
	filetable.entries[fileEntryIndex].used = 1;
	filetable.entries[fileEntryIndex].inodeIdx = iNodeIndex;
	
	//file descriptor
	return fileEntryIndex;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{

	//check if file has integrity
	if(inodes[filetable.entries[fileDescriptor].inodeIdx].has_integrity == TRUE){
		return -1;
	}

	//Check if filedescriptor exists
	//--if it exists means that the file is open
	int fd = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].fd == fileDescriptor){
			fd = i;
			break;
		}
	}
	//fileDescriptor was not found in fileTable
	if(fd < 0) return -1;

	//free filetable entry
	filetable.entries[fileDescriptor].fd = -1;
	filetable.entries[fileDescriptor].inodeIdx = -1;
	filetable.entries[fileDescriptor].offset = -1;
	filetable.entries[fileDescriptor].used = -1;

	return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer(from a possible offset).
 * 			Particularly if the offset is at the very end of file. the bytes returned must be 0.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{	
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

	//Read all file to buffer
	int offset = filetable.entries[fileDescriptor].offset;
	char master_buf[inodes[InodeIndex].blocks_assigned*superblock.block_size];
	int bytesRead = 0;
	int currentBlock = 0;
	while(bytesRead < inode.size){
		if (bread(DEVICE_IMAGE, inode.data_blocks[currentBlock], ((char *)&(master_buf[bytesRead]))) == -1) return -1;
		bytesRead += superblock.block_size;
		currentBlock++;
	}
	//Masterbuf at this points looks something like:
	//[fffff][fffff][fff000]
	//			^
	//        offset
	//We would need to return "fffff"

	int bytesToPutInBuf = numBytes;
	if(offset + numBytes > inode.size){
		bytesToPutInBuf = inode.size - offset;
	}
	memcpy(buffer, &master_buf[offset], bytesToPutInBuf);
	filetable.entries[fileDescriptor].offset = bytesToPutInBuf;
	return bytesToPutInBuf;

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
	int iNodeIndex = fileEntry.inodeIdx;
	int currentBlock = fileEntry.offset/superblock.block_size;
	int bytesLeftToWrite = numBytes;


	while(bytesLeftToWrite > 0){
		
		//create a buffer for the new block and flush it
		char block_buffer[superblock.block_size];
		for(int i=0; i<sizeof(block_buffer); i++){
			block_buffer[i] = '_';
		}
		//SPECIAL CASE FOR LAST BLOCK
		if(inodes[iNodeIndex].blocks_assigned == MAX_FILE_SIZE/superblock.block_size){
			int bytesAlreadyInBlock = filetable.entries[fileDescriptor].offset - currentBlock*superblock.block_size;
			int lastAllowedBytes = superblock.block_size - bytesAlreadyInBlock;
			int bytesToWriteFromBuffer = 0;
			//take minimum between space left in last block and bytes left to write
			if(bytesLeftToWrite < lastAllowedBytes){
				bytesToWriteFromBuffer = bytesLeftToWrite;
			}else{
				bytesToWriteFromBuffer = lastAllowedBytes;
			}
			//Write to block
			//Save bytes already on disk block
			if (bread(DEVICE_IMAGE, inodes[iNodeIndex].data_blocks[currentBlock], ((char *)&(block_buffer))) == -1) return -1;
			memcpy(&block_buffer[bytesAlreadyInBlock], (char *)buffer + (numBytes-bytesLeftToWrite), bytesToWriteFromBuffer);
			filetable.entries[fd].offset += bytesToWriteFromBuffer;
			inodes[iNodeIndex].size += bytesToWriteFromBuffer;
			if (bwrite(DEVICE_IMAGE, inodes[iNodeIndex].data_blocks[currentBlock], ((char *)&(block_buffer))) == -1) return -1;
			return bytesToWriteFromBuffer;
		}
		//END SPECIAL CASE FOR LAST BLOCK

		//Block has not been assigned
		if(inodes[iNodeIndex].data_blocks[currentBlock] == -1){
			//Assign block
			for(int i=0; i<bitmap.size*8; i++){
				//search for next free block on bitmap and assign it to inode
				if(bitmap_getbit(bitmap.map, i) == FREE){
					inodes[iNodeIndex].data_blocks[currentBlock] = i;
					inodes[iNodeIndex].blocks_assigned ++;
					bitmap_setbit(bitmap.map, i, USED);
					break;
				}
				//return error if there are no free blocks
				if(i == (bitmap.size*8)-1) return -1;
			}

			//fill up buffer with new bytes
			if(bytesLeftToWrite<superblock.block_size){
				memcpy(block_buffer, (char *)buffer + (numBytes-bytesLeftToWrite), bytesLeftToWrite);
				filetable.entries[fileDescriptor].offset += bytesLeftToWrite;
				bytesLeftToWrite = 0;
			}else{
				memcpy(block_buffer, (char *)buffer + (numBytes-bytesLeftToWrite), superblock.block_size);
				bytesLeftToWrite -= superblock.block_size;
				filetable.entries[fileDescriptor].offset += superblock.block_size;
			}
			
		}else{ 
		//Block has been assigned before (and could contain data)

			//Calculate bytes already written in block and update offset
			int bytesAlreadyInBlock = filetable.entries[fileDescriptor].offset - currentBlock*superblock.block_size;

			//Save bytes already on disk block
			if (bread(DEVICE_IMAGE, inodes[iNodeIndex].data_blocks[currentBlock], ((char *)&(block_buffer))) == -1) return -1;

			if(bytesLeftToWrite<superblock.block_size-bytesAlreadyInBlock){
				memcpy(&block_buffer[bytesAlreadyInBlock], (char *)buffer + (numBytes-bytesLeftToWrite), bytesLeftToWrite);
				filetable.entries[fileDescriptor].offset += bytesLeftToWrite;
				bytesLeftToWrite = 0;
			}else{
				memcpy(&block_buffer[bytesAlreadyInBlock], (char *)buffer + (numBytes-bytesLeftToWrite), superblock.block_size-bytesAlreadyInBlock);
				filetable.entries[fileDescriptor].offset += superblock.block_size-bytesAlreadyInBlock;
				bytesLeftToWrite -= superblock.block_size-bytesAlreadyInBlock;
			}
		}

		//write buffer to disk
		if (bwrite(DEVICE_IMAGE, inodes[iNodeIndex].data_blocks[currentBlock], ((char *)&(block_buffer))) == -1) return -1;

		//update current block
		currentBlock++;
	}
	inodes[iNodeIndex].size += numBytes-bytesLeftToWrite; 
	return numBytes-bytesLeftToWrite;
}


/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
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

	switch (whence)
	{
	case  FS_SEEK_CUR:
		//Cannot move cursor outside file size
		if(filetable.entries[fileDescriptor].offset + offset > inodes[filetable.entries[fileDescriptor].inodeIdx].size){
			return -1;
		} 
		//Cannot move cursor below 0
		if(filetable.entries[fileDescriptor].offset + offset < 0){
			return -1;
		} 
		filetable.entries[fileDescriptor].offset += offset;
		break;
	case  FS_SEEK_END:
		filetable.entries[fileDescriptor].offset = inodes[filetable.entries[fileDescriptor].inodeIdx].size;
		break;
	case  FS_SEEK_BEGIN:
		filetable.entries[fileDescriptor].offset = 0;
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

/*
 * @brief	Checks the integrity of the file.
 * @return	0 if success, -1 if the file is corrupted, -2 in case of error.
 */

int checkFile (char * fileName)
{
	//TODO: Ask JOSE

	//check if file was already open
	for(int i = 0; i < MAX_FILES; i++){
		if(strcmp(inodes[filetable.entries[i].inodeIdx].file_name, fileName) == 0) return -2;
	}

	int fd = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(strcmp(inodes[filetable.entries[i].inodeIdx].file_name,fileName)== 0){
			fd = i;
			break;
		}
	}

	int iNodeIndex = filetable.entries[fd].inodeIdx; 

	//file not found
	if(fd < 0){
		return -2;
	}

	//Check if inode is soft link
	if(inodes[iNodeIndex].soft_link != -1){
		//look for original file
		iNodeIndex = inodes[iNodeIndex].soft_link;
	}

	//Check if integrity has not been established
	if(inodes[iNodeIndex].has_integrity == FALSE) return -2;

	//Read contesnts of file to a buffer
	unsigned int sizeOfFile = inodes[iNodeIndex].size;
	unsigned char buffer[sizeOfFile];
	int ret = readFile(fd, buffer, sizeOfFile);

	//read succesfully
	if(ret != sizeOfFile) return -2;

	//perform integrity check
	if(inodes[iNodeIndex].integrity != CRC32(buffer, sizeOfFile)){
		//file corrupted
		closeFile(fd);
		return -1;
	} 

	//file preserves integrity
	closeFile(fd);
    return 0;
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
	//Check if filename exists
	int iNodeIndex = -1; 
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}

	//File does not exist
	if(iNodeIndex == -1) return -1;

	//Check if inode is soft link
	if(inodes[iNodeIndex].soft_link != -1){
		//look for original file
		iNodeIndex = inodes[iNodeIndex].soft_link;
	}

	//Check if integrity has already been established
	if(inodes[iNodeIndex].has_integrity == TRUE) return -2;


	//Read contents of file to a buffer
	int fd = openFile(fileName);
	unsigned int sizeOfFile = inodes[iNodeIndex].size;
	unsigned char buffer[sizeOfFile];
	int ret = readFile(fd, buffer, sizeOfFile);
	//check if retrieved contents are same as size
	if(ret != sizeOfFile){
		closeFile(fd);
		return -2;
	}
	//Perform integrity (CRC32) calculation on the contents of the file(now in buffer)
	uint32_t integrityValue = CRC32(buffer, sizeOfFile);

	//Save the integrity value in the inode
	inodes[iNodeIndex].integrity = integrityValue;
	inodes[iNodeIndex].has_integrity = TRUE;

	//integrity has been set
	closeFileIntegrity(fd);
    return 0;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{

	//Check if filename exists
	int iNodeIndex = -1; 
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}

	//file does not exist
	if(iNodeIndex == -1) return -1;

	//check if file has integrity
	if(inodes[iNodeIndex].has_integrity == FALSE){
		return -3; //Cannot open a file without integrity
	}

	//check file integrity
	int ret = checkFile(fileName);
	if(ret == -1){	
		return -2; //File is corrupted
	}else if(ret == -2){
		return -3; //Other error
	}

	//check if inode is soft link
	if(inodes[iNodeIndex].soft_link != -1){
		//look for original file
		iNodeIndex = inodes[iNodeIndex].soft_link;
	}

	//look for free entry in SFT
	int fileEntryIndex = -1;
	for(int i = 0; i < MAX_FILES; i++){
		if(filetable.entries[i].used == 0){
			fileEntryIndex = i;
			break;
		}
	}

	//not anough entries in filetable
	if(fileEntryIndex == -1) return -3;

	//set values in filetable entry
	filetable.entries[fileEntryIndex].fd = fileEntryIndex;
	filetable.entries[fileEntryIndex].offset = 0;
	filetable.entries[fileEntryIndex].used = 1;
	filetable.entries[fileEntryIndex].inodeIdx = iNodeIndex;

    return fileEntryIndex;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{

	//check if file has integrity
	if(inodes[filetable.entries[fileDescriptor].inodeIdx].has_integrity == FALSE){
		return -1;
	}

	unsigned int sizeOfFile = inodes[filetable.entries[fileDescriptor].inodeIdx].size;
	unsigned char buffer[sizeOfFile];
	lseekFile(fileDescriptor,0,FS_SEEK_BEGIN);
	int ret = readFile(fileDescriptor, buffer, sizeOfFile);

	//check if retrieved contents are same as size
	if(ret != sizeOfFile){
		closeFile(fileDescriptor);
		return -1;
	}

	//Perform integrity (CRC32) calculation on the contents of the file(now in buffer)
	uint32_t integrityValue = CRC32(buffer, sizeOfFile);

	//Save the integrity value in the inode
	inodes[filetable.entries[fileDescriptor].inodeIdx].integrity = integrityValue;
	inodes[filetable.entries[fileDescriptor].inodeIdx].has_integrity = TRUE;

	//perform close operation
	filetable.entries[fileDescriptor].fd = -1;
	filetable.entries[fileDescriptor].inodeIdx = -1;
	filetable.entries[fileDescriptor].offset = -1;
	filetable.entries[fileDescriptor].used = -1;

    return 0;
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{
	//Check if filename exists
	int iNodeIndex = -1; 
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, fileName) == 0){
			iNodeIndex = i;
			break;
		}
	}

	//file does not exist
	if(iNodeIndex == -1) return -1;

	//check if link destination is not a soft link
	if(inodes[iNodeIndex].soft_link != -1) return -2;

	//check if link name is available
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, linkName) == 0){
			return -2;
		}
	}

	//Check for free inode scanning through inodes array
	int freeInodeIndex = -1;
	for(int i = 0; i < superblock.inodes; i++){
		if(inodes[i].status == FREE) {
			//if free inode found set up info
			freeInodeIndex = i;
			inodes[i].status = USED;//mark inode as used
			inodes[freeInodeIndex].size = -1;
			inodes[freeInodeIndex].blocks_assigned = -1;
			strcpy(inodes[freeInodeIndex].file_name, linkName);
			inodes[freeInodeIndex].soft_link = iNodeIndex;
			break;
		}
	}

	//no free inodes left
	if(freeInodeIndex == -1) return -2;

	return 0;
}

/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{
	//Check if linkName exists
	int iNodeIndex = -1; 
	for(int i = 0; i < superblock.inodes; i++){
		if(strcmp(inodes[i].file_name, linkName) == 0){
			iNodeIndex = i;
			break;
		}
	}

	//file does not exist
	if(iNodeIndex == -1) return -1;

	//check if target link is not a soft link, if so return back error
	if(inodes[iNodeIndex].soft_link == -1) return -2;

	//clean inode actually removes file from filesystem
	//(but data previously written in disk remains the same until overwritten)
	inodes[iNodeIndex] = (INode){ "",FREE,0,0,{-1,-1,-1,-1,-1},-1,FALSE,0};

	return 0;
}
