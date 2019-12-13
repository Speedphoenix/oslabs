#ifndef __MMU__H__
# define __MMU__H__

/*
** In this lab, you are going to implement virtual memory with a one-level paging scheme,
** and run the same code in the main as in the subject given for contiguous allocation.
** The previous implementation of myAlloc should be renamed as myAllocCont
** and the logical memory (the mem[2^16]) should be removed.
**
** In this implementation, the page size is fixed to 128 words, the physical
** memory size to 2^15 words (call it physmem[2^15])
** and the logical memory remains at 2^16 words.
**
** 1- identify the size of the page table
**
** 2- identifiy the number of pages in such a system
**
** 3- identify the number of frames
**
** 4- implement myAlloc (based on the algorithm given in class), myWrite, myRead and myFree
**
** Big Bonus : If you're comfortable with the concept,
** implement a two level paging scheme and explain it.
*/

// 128 - 1000 0000 (7 bits are used for addressing inside a page)
# define PAGESIZE 0x80
# define PAGEOFFSET 7
# define PAGEMASK 0x7F

// addresses are 32 bits, so 25 bits are used to find the page,
// and 7 as offset inside the page

// 33 554 432
// 2 ^ 25
# define PAGETABLESIZE (1 << 24)

// 2 ^ 31 - 1
# define MAXSIZE 2147483647

# define FIT_FN get_first_fit

typedef short byte_t;
typedef int address_t;

typedef struct hole {
	address_t adr;
	int sz;
	struct hole *next;
	struct hole *prev;
} hole_t;

typedef struct {
	byte_t *page_table[PAGETABLESIZE];
} lowmem_t;

typedef struct {
	lowmem_t *mem;
	hole_t *root; // holes list

	hole_t *last_hole;

	address_t curr_addr_limit;
} mem_t;

// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem();

// frees everything
void destroyMem(mem_t *mp);

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int sz);

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz);

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, byte_t val);

// read memory from a byte
byte_t myRead(mem_t *mp, address_t p);

void lowmem_write(lowmem_t *where, address_t p, byte_t val);

byte_t lowmem_read(lowmem_t *where, address_t p);

void lowmem_alloc(lowmem_t *where, address_t up_to);

void lowmem_free(lowmem_t *where, address_t p);

void throw_error(const char *msg);

#endif // __MMU__H__
