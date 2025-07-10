/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfx_list.h -- sfx for list api (base kernel list)
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#ifndef __SFX_LIST_H
#define __SFX_LIST_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stddef.h>

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 */
#define sfx_container_of(ptr, type, member) ({              \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


struct sfx_list_head {
    struct sfx_list_head *next, *prev;
};

#define SFX_LIST_HEAD_INIT(name) { &(name), &(name) }

#define SFX_LIST_HEAD(name) \
    static struct sfx_list_head name = SFX_LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct sfx_list_head *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * Insert a _new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __sfx_list_add(struct sfx_list_head *_new,
                                  struct sfx_list_head *prev,
                                  struct sfx_list_head *next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

/**
 * sfx_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void sfx_list_add(struct sfx_list_head *_new, struct sfx_list_head *head)
{
    __sfx_list_add(_new, head, head->next);
}

/**
 * sfx_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void sfx_list_add_tail(struct sfx_list_head *_new, struct sfx_list_head *head)
{
    __sfx_list_add(_new, head->prev, head);
}

/**
 * sfx_list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int sfx_list_empty(const struct sfx_list_head *head)
{
    return head->next == head;
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __sfx_list_del(struct sfx_list_head *prev, struct sfx_list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * sfx_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: sfx_list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void sfx_list_del(struct sfx_list_head *entry)
{
    __sfx_list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

/**
 * sfx_list_entry - get the struct for this entry
 * @ptr:    the &struct sfx_list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define sfx_list_entry(ptr, type, member) \
    sfx_container_of(ptr, type, member)

/**
 * sfx_list_for_each    -    iterate over a list
 * @pos:    the &struct sfx_list_head to use as a loop cursor.
 * @head:    the head for your list.
 */
#define sfx_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * sfx_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the &struct sfx_list_head to use as a loop cursor.
 * @n:        another &struct sfx_list_head to use as temporary storage
 * @head:    the head for your list.
 */
#define sfx_list_for_each_safe(pos, n, head)                \
    for (pos = (head)->next, n = pos->next; pos != (head);  \
        pos = n, n = pos->next)

/**
 * sfx_list_for_each_entry    -    iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:    the head for your list.
 * @member:    the name of the list_struct within the struct.
 */
#define sfx_list_for_each_entry(pos, head, member)                  \
    for (pos = sfx_list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = sfx_list_entry(pos->member.next, typeof(*pos), member))

/**
 * sfx_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:        another type * to use as temporary storage
 * @head:    the head for your list.
 * @member:    the name of the list_struct within the struct.
 */
#define sfx_list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = sfx_list_entry((head)->next, typeof(*pos), member),  \
        n = sfx_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                                    \
         pos = n, n = sfx_list_entry(n->member.next, typeof(*n), member))

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __SFX_LIST_H */
