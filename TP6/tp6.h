#ifndef __MMU__H__
# define __MMU__H__

# define SIZE 65536

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
	byte_t mem[SIZE];
	hole_t *root; // holes list

	hole_t *last_hole;
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

void throw_error(const char *msg);

#endif // __MMU__H__
