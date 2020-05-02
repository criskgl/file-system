
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
  unsigned short total_blocks;//total blocks allocated in our FS
  unsigned short inode_blocks;//blocks dedicated for inodes
  short block_size;//will be 2048 Bytes
  unsigned short inodes;//total inodes in FS
} SuperBlock;//6 Bytes

typedef struct BitMap{
  unsigned short size;//size in bytes of our bitmap
  char * map;//pointer to first 8 bits of our bitmap
} BitMap;//40 Bytes (including char* content)

typedef struct INode{
  char file_name[33];//name of  file
  unsigned short status;//either FREE or USED
  short size;//size of file
  short blocks_assigned;//number of blocks currently assigned to hold data
  short data_blocks[5];//block pointers
  short soft_link;//field that holds inode number in case of being a softlink
} INode;


typedef struct FileTableEntry{
  int fd;//file descriptor
  int offset;//pointer offset inside an open file
  int used;
  int inodeIdx;//inode associated with the openfile
} FileTableEntry;

typedef struct FileTable{
  FileTableEntry entries[MAX_FILES];//has a limitation of 48 open files
} FileTable; 

