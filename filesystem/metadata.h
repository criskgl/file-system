
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	Last revision 01/04/2020
 *
 */

#define MAX_FILES 48        //NF1
#define MAX_NAME_LEN 32     //NF2
#define MAX_FILE_SIZE 10240 //NF3
#define BLOCK_SIZE 2048     //NF4
#define MIN_DEV_SIZE 471040 //NF6
#define MAX_DEV_SIZE 614400 //NF6
#define FREE 0
#define USED 2

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

typedef struct SuperBlock{
  unsigned short int magic_number;
  unsigned int total_blocks;
  unsigned int inode_blocks;
  unsigned int inodes;
  char padding[];
} SuperBlock;

typedef struct INode{
  char * file_name;
  unsigned short int status;
  unsigned int size;
  unsigned short int data_blocks[5];
} INode;