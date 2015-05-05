#ifndef BLOCH_H
#define BLOCK_H

#include "constants.h"
#include "directory_entry.h"

#ifdef DEBUG
#include <stdbool.h>
#include <stdint.h>
#else
#include <linux/types.h>
#endif

/*********************/
/** DATA STRUCTURES **/
/*********************/

/* RAMDISK Block */
typedef union _block_t{
  char data[BLOCKSIZE];
  directory_entry_t directories[BLOCKSIZE/(FILENAME_SIZE+2)];
  union _block_t* block_ptr[BLOCKSIZE/4];
}block_t;

typedef struct _super_block_t{
  uint16_t num_free_blocks;
  uint16_t num_inodes;
} super_block_t;

/* Block Bitmap Array 
 * A single bit indicates whether the block is occupied 
 * */
extern uint8_t* block_bitmap;
extern block_t* blocks;
extern super_block_t* super_block;

/*************************/
/** FUNCTION PROTOTYPES **/
/*************************/
/*
 * Initialize ramdisk block partition and the bitmap array
 * */
void block_initialize(char* blocks_ptr,
                      uint8_t* bitmap_ptr,
                      super_block_t* super_block);

/*
 * Allocates a new block
 * Returns:
 *  Pointer to a free ramdisk block
 *  NULL if no free blocks exist
 * */
block_t* block_get_free(void);

/*  
 * Set the block with a given index to be free
 * */
void block_remove(block_t*);

static inline void block_set_bitmap(uint16_t block_index){
  block_bitmap[block_index/8] |= 1 << (block_index%8);
}

static inline void block_clear_bitmap(uint16_t block_index){
  block_bitmap[block_index/8] &= ~(1 << (block_index%8));
}

static inline bool block_is_free(uint16_t block_index){
  return ((block_bitmap[block_index/8] & (1<<(block_index%8))) == 0);
}

static inline uint16_t block_addr_to_index(block_t* addr){
  return ((uint32_t) addr - (uint32_t) blocks) / BLOCKSIZE;
}
  
static inline block_t* block_index_to_addr(uint16_t block_index){
  return &blocks[block_index];
}

#endif