/* A simple doubly linked list implementation in C.
 *	Copyright (C) 2015	Yang Zhang <yzfedora@gmail.com>
 *
 * Internals:
 *	for eaxmple, we want to declare a doubly linked list called
 *	"struct job" which used to store the basic informations of the job.
 *	like: name, place, salary..., etc.
 *
 *	struct job {
 *		char *name;
 *		char *place;
 *		unsigned salary;
 *		....			// other members you wants
 *
 *		struct list list;
 *	};
 *	-------------------------------------------------
 *	... list <----> list <----> list <----> ...
 *	-----|-----------|-----------|-------------------
 *           |           |           |
 *           ^           ^           ^
 *      ... struct job; struct job; struct job; ...
 *
 *      we use a list structure to administrate and links every node in the
 *      doubly linked list. you just need embeded a "struct list" and use it
 *      to manipulate your real doubly linked list("struct job"), it somewhat
 *      likes the "basic class" in Object Oriented Programming.
 *
 *      when you are defined a structure and embeded a "struct list", you can
 *      using the following API to manipulate you doubly linked list. without
 *      need to process the basic node operation.
 */
#ifndef _LIST_H
#define _LIST_H

#ifndef container_of
#define container_of(ptr, type, member) \
	({ (type *)((char *)ptr - (char *)&((type *)0)->member)})
#endif
#ifndef __container_of
#define __container_of(ptr, type, member) \
	({ (typeof(type))((char *)ptr - (char *)&((typeof(type))0)->member); })
#endif

struct list {
	struct list *prev;
	struct list *next;
};

/*
 * Used to define a list by use given name, and initialize it. or you can
 * use list_init to initialize a list also.
 */
#define LIST_HEAD(name) struct list name = { &(name), &(name) }


/*
 * Initialize the list head, and the head node will not including any real
 * data. it's a "sentry".
 */
static inline void list_init(struct list *head)
{
	head->prev = head->next = head;
}

static inline void _list_add(struct list *new,
			     struct list *prev,
			     struct list *next)
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}

/*
 * Insert the new node into list, it will be placed after head.
 */
static inline void list_add(struct list *new, struct list *head)
{
	_list_add(new, head, head->next);
}

/*
 * Insert new node to the tail of list(head->prev will be the last node).
 */
static inline void list_add_tail(struct list *new, struct list *head)
{
	_list_add(new, head->prev, head);
}

/*
 * Remove the current node from the list.
 */
static inline void list_del(struct list *node)
{
	struct list *prev = node->prev, *next = node->next;

	prev->next = next;
	next->prev = prev;
}

/*
 * If list is empty, true will be returned, else false will be returned.
 */
static inline int list_empty(struct list *head)
{
	return (head->next == head);
}


/*
 * Move the list node to the head of list.
 */
static inline void list_move(struct list *list, struct list *head)
{
	list_del(list);
	list_add(list, head);
}

/*
 * Move the list node to the tail of the list.
 */
static inline void list_move_tail(struct list *list, struct list *head)
{
	list_del(list);
	list_add_tail(list, head);
}

/*
 * Get the container structure.
 * ptr    pointer to the member in the type.
 * type   the container structure.
 * member the name of ptr in the container structure.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/*
 * traval all entrys in the list.
 * pos    a pointer to entry(pointer to the container structure).
 * head   head pointer of list.
 * member the list name in the container structure("struct list" will be
 *        embeded into container structure).
 *
 * Note: head just is a sentry, actually pos is not a valid pointer when
 * &pos->member is equal to head. but there is safe since we are not
 * reference or access it.
 */
#define list_for_each(pos, head, member) \
	for (pos = __container_of((head)->next, pos, member); \
	     &pos->member != head; \
	     pos = __container_of(pos->member.next, pos, member))

#define list_for_each_reverse(pos, head, member) \
	for (pos = __container_of((head)->prev, pos, member); \
	     &pos->member != head; \
	     pos = __container_of(pos->member.prev, pos, member))


#endif
