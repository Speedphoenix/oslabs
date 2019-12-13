#include "tp7.h"
#include <stdlib.h>

// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem()
{
	mem_t *rep;
	hole_t *root_hole;

	rep = (mem_t *)malloc(sizeof(mem_t));
	root_hole = (hole_t *)malloc(sizeof(hole_t));

	rep->curr_addr_limit = 0;
	rep->root = root_hole;
	rep->last_hole = root_hole;
	root_hole->adr = 0;
	root_hole->sz = MAXSIZE;
	root_hole->next = NULL;
	root_hole->prev = NULL;

	return rep;
}

// frees everything
void destroyMem(mem_t *mp)
{
	address_t to_free;
	hole_t *prev;
	hole_t *curr;

	curr = mp->root;
	while (curr != NULL)
	{
		prev = curr;
		curr = prev->next;
		free(prev);
	}
	to_free = 0;
	while (to_free < mp->curr_addr_limit)
	{
		lowmem_free(mp->mem, to_free);
		to_free += PAGESIZE;
	}
	free(mp->mem);
	free(mp);
}

void destroy_hole(mem_t *mp, hole_t *to_destroy)
{
	if (to_destroy->prev != NULL)
		to_destroy->prev->next = to_destroy->next;
	else
		mp->root = to_destroy->next;
	if (to_destroy->next != NULL)
		to_destroy->next->prev = to_destroy->prev;
	else
		mp->last_hole = to_destroy->prev;
	free(to_destroy);
}

void insert_hole(mem_t *mp, hole_t *prev, hole_t *next, hole_t *what)
{
	what->prev = prev;
	what->next = next;
	if (prev != NULL)
		prev->next = what;
	else
		mp->root = what;
	if (next != NULL)
		next->prev = what;
	else
		mp->last_hole = what;
}

hole_t *get_first_fit(hole_t *root, int sz)
{
	hole_t *curr;

	curr = root;
	while (curr != NULL)
	{
		if (curr->sz >= sz)
			return curr;
		curr = curr->next;
	}
	return (NULL);
}

hole_t *get_best_fit(hole_t *root, int sz)
{
	hole_t *curr;
	hole_t *best;

	best = NULL;
	curr = root;
	while (curr != NULL)
	{
		if (curr->sz >= sz && (best == NULL || best->sz > curr->sz))
			best = curr;
		curr = curr->next;
	}
	return best;
}

hole_t *get_worst_fit(hole_t *root, int sz)
{
	hole_t *curr;
	hole_t *worst;

	worst = NULL;
	curr = root;
	while (curr != NULL)
	{
		if (curr->sz >= sz && (worst == NULL || worst->sz < curr->sz))
			worst = curr;
		curr = curr->next;
	}
	return worst;
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int sz)
{
	hole_t *to_use;
	address_t rep;

	to_use = FIT_FN(mp->root, sz);
	if (to_use == NULL)
		throw_error("There is no sequence of this size available");

	// remove the hole
	if (to_use->sz == sz)
	{
		rep = to_use->adr;
		destroy_hole(mp, to_use);
	}
	else // reduce the size of the hole
	{
		rep = to_use->adr;
		to_use->adr += sz;
		to_use->sz -= sz;
	}
	return rep;
}

// TODO: support for when there are no holes to start with (the memory is fully used)
// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz)
{
	hole_t *prev_one;
	hole_t *next_one;
	hole_t *new_one;
	address_t start;
	address_t end;

	if (sz <= 0)
		return;

	prev_one = NULL;
	next_one = NULL;
	if (mp->root->adr == p)
		return;
	else if (mp->root->adr > p)
		next_one = mp->root;
	else
	{
		prev_one = mp->root;
		while (prev_one->next != NULL && prev_one->next->adr > p)
			prev_one = prev_one->next;
		if (prev_one->adr == p)
			return;
		next_one = prev_one->next;
	}

	start = p;
	end = p + sz;
	new_one = NULL;
	// if we need to add it to the previous hole
	if (prev_one != NULL && (prev_one->adr + prev_one->sz) >= start)
	{
		new_one = prev_one;
		prev_one->sz = end - prev_one->adr;
	}
	// if we need to add it to the next hole
	if (next_one != NULL && next_one->adr <= end)
	{
		next_one->sz = next_one->adr + next_one->sz - start;
		next_one->adr = start;
		if (new_one != NULL)
			destroy_hole(mp, new_one);
		new_one = next_one;
	}

	if (new_one == NULL)
	{
		new_one = (hole_t *)malloc(sizeof(hole_t));
		new_one->adr = p;
		new_one->sz = sz;
		insert_hole(mp, prev_one, next_one, new_one);
	}
}

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, byte_t val)
{
	if (p > SIZE || p < 0)
		throw_error("Trying to write outside the memory space");
	// should we check if this is allocated in the first place?
	mp->mem[p] = val;
}

// read memory from a byte
byte_t myRead(mem_t *mp, address_t p)
{
	if (p > SIZE || p < 0)
		throw_error("Trying to read outside the memory space");
	// should we check if this is allocated in the first place?
	return (mp->mem[p]);
}

// returns the new address limit
void lowmem_alloc(lowmem_t *where, address_t containing_page)
{
	address_t pagenum;

	pagenum = p >> PAGEOFFSET;
	where->page_table[pagenum] = malloc(PAGESIZE * sizeof(byte_t));
}

void lowmem_free(lowmem_t *where, address_t containing_page)
{
	address_t pagenum;

	pagenum = p >> PAGEOFFSET;
	free(where->page_table[pagenum]);
}

void lowmem_write(lowmem_t *where, address_t p, byte_t val)
{
	address_t pagenum, offset;

	pagenum = p >> PAGEOFFSET;
	offset = p & PAGEMASK;
	where->page_table[pagenum][offset] = val;
}

byte_t lowmem_read(lowmem_t *where, address_t p)
{
	address_t pagenum, offset;

	pagenum = p >> PAGEOFFSET;
	offset = p & PAGEMASK;
	return where->page_table[pagenum][offset];
}
