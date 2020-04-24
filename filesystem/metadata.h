
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
#define USED 1

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

typedef struct SuperBlock{
  unsigned short total_blocks;
  unsigned short inode_blocks;//blocks dedicated for inodes
  unsigned short inodes;//total inodes in FS
} SuperBlock;//6 Bytes

typedef struct BitMap{
  unsigned short size;
  char * map;
} BitMap;//40 Bytes (including char* content)

typedef struct INode{
  char file_name[33];
  unsigned short status;
  unsigned short size;//size of file
  unsigned short data_blocks[5];
} INode;


typedef struct FileTableEntry{
  int fd;
  int offset;
  int refCount;
  int inodeIdx;
} FileTableEntry;

typedef struct FileTable{
  FileTableEntry entries[MAX_FILES];//TODO: Preguntar a Jose el maximo en esta tabla
} FileTable; 

