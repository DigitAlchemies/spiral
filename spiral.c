/*
 *   Copyright (C) 2015  Raghavan Santhanam, raghavanil4m@gmail.com.
 *   I wrote this sprial code just for fun! Send any
 *   opinions/suggestions/bug reports to raghavanil4m@gmail.com.
 *
 *   spiral.c is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   spiral.c is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *   spiral.c - A very simple and small piece of code that transforms
 *   a specialized singly linked list into a spiral-list in O(n).
 *   The singly linked list is also a doubly linked list - the speciality.
 *   Each node points to (prev ^ next) which provides the speciality.
 *
 *   One day coding. One day debugging. 22-23, July 2015
 *
 *	  _______________________________
 * 	 | 				 |
 *	 | 	________________	 |
 *	 |	|		|	 |
 *	 |	|		V	 V
 *	 1 	2 	3 	4	 5
 *		^	^_______|	 |
 *		|			 |
 *		|________________________|			
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define __VERBOSE__
#ifdef __VERBOSE__
	#define printv(...) printf(__VA_ARGS__)
#else
	#define printv(...)
#endif

struct list {
	struct list *next;
	int i;
};

#define NUM_NODES 3
static struct list *head;
static int numnodes;

static struct list *lastnode(void)
{
	struct list *c = head;
	struct list *p = NULL;
	while (c) {
		struct list *n = (struct list *)((uintptr_t)c->next ^ (uintptr_t)p);
		p = c;
		c = n;
	}
	return p;
}

static void spiralify(void)
{
	struct list *f = head;
	struct list *fp = NULL;
	struct list *l = lastnode();
	struct list *lp = NULL;
	if (numnodes <= 1) {
		return;
	}	
	int i = 1;
	int numloops = numnodes / 2;
	while (i <= numloops) {
		// Retain the spiral shape.
		struct list *fn = (struct list *)((uintptr_t)f->next ^ (uintptr_t)fp);
		struct list *ln = (struct list *)((uintptr_t)l->next ^ (uintptr_t)lp);
		// Form the (prev ^ next) relation.
		f->next = (struct list *)((uintptr_t)l ^ (uintptr_t)lp);
		l->next = (struct list *)((uintptr_t)f ^ (uintptr_t)fn);
		// Move front and back pointers towards each other.
		fp = f;
		f = fn;
		lp = l;
		l = ln;
		i++;
	}
	bool even = !(numnodes & 0x1); 	// Even number of nodes need to be
					// differentiated w.r.t last node.
	if (even) {
		lp->next = l;
	} else {
		l->next = lp;
	}
}

static void addnode(int i)
{
	static struct list *prev;
	if (!head) { 	// List might have been deleted and head
			// might have been reset.
		prev = NULL;
	}
	struct list *n = malloc(sizeof *n);
	if (n) {
		n->i = i;
		n->next = prev;
		if (prev) {
			// Keep the (prev ^ next) relation intact.
			prev->next = ((struct list *)((uintptr_t)prev->next ^ (uintptr_t)n));
		} else {
			head = n;
		}
		prev = n;
	} else {
		fprintf(stderr, "mem alloc failed : %s\n", strerror(errno));
	}
}

static void createlist(void)
{
	int i = 1;
	while (i <= numnodes) {
		addnode(i++);
	}
}

static void destroylist(void)
{
	struct list *c = head;
	struct list *p = NULL;
	while (c) {
		struct list *n = (struct list *)((uintptr_t)c->next ^ (uintptr_t)p);
		p = c;
		free(p); // We just use the value(address) stored in p
			 // in the next iteration and it's not dereferenced.
			 // So, this is absolutely safe.
		c = n;
	}
	head = NULL;
}

static void printlist(void)
{
	struct list *c = head;
	struct list *p = NULL;
	while (c) {
		struct list *n = (struct list *)((uintptr_t)c->next ^ (uintptr_t)p);
		printf("%d%s", c->i, n ? "~>" : "");
		p = c;
		c = n;
	}
	printf("\n");
}

static void asknumnodes(void)
{
	printf("num nodes? : ");
	fflush(stdout);
	scanf("%d", &numnodes);
	if (numnodes < 1) {
		numnodes = NUM_NODES;
	}
}

int main(void)
{
	while (8) {
		asknumnodes();
		createlist();
		printlist();
		spiralify();
		printlist();
		destroylist();
	}

	return 0;
}

