#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));
    if (!new)
        return NULL;
    INIT_LIST_HEAD(new);
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *node, *safe = NULL;
    list_for_each_entry_safe (node, safe, head, list) {
        free(node->value);
        free(node);
    }
    free(head);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    int len = strlen(s);
    char *str = malloc(len + 1);
    if (!str) {
        free(new);
        return false;
    }
    new->value = strncpy(str, s, len + 1);
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    int len = strlen(s);
    char *str = malloc(len + 1);
    if (!str) {
        free(new);
        return false;
    }
    new->value = strncpy(str, s, len + 1);
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node;
    node = list_first_entry(head, element_t, list);
    list_del(&node->list);
    if (sp) {
        strncpy(sp, node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node;
    node = list_last_entry(head, element_t, list);
    list_del(&node->list);
    if (sp) {
        strncpy(sp, node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int len = 0;
    struct list_head *node;
    list_for_each (node, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *slow = head->next, *fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    element_t *node = list_entry(slow, element_t, list);
    list_del(slow);
    free(node->value);
    free(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    element_t *node, *safe = NULL;
    bool del = false;
    list_for_each_entry_safe (node, safe, head, list) {
        if (&safe->list != head && strcmp(node->value, safe->value) == 0) {
            list_del(&node->list);
            free(node->value);
            free(node);
            del = true;
        } else if (del) {
            list_del(&node->list);
            free(node->value);
            free(node);
            del = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head))
        return;
    struct list_head *node, *safe;
    bool s = false;
    list_for_each_safe (node, safe, head) {
        if (s)
            list_move(node, node->prev->prev);
        s = !s;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}


struct list_head *merge(struct list_head *l1,
                        struct list_head *l2,
                        bool descend)
{
    if (!l1)
        return l2;
    if (!l2)
        return l1;

    if (strcmp(list_entry(l1, element_t, list)->value,
               list_entry(l2, element_t, list)->value) <= 0) {
        if (descend) {
            l2->next = merge(l1, l2->next, descend);
            return l2;
        } else {
            l1->next = merge(l1->next, l2, descend);
            return l1;
        }
    } else {
        if (descend) {
            l1->next = merge(l1->next, l2, descend);
            return l1;
        } else {
            l2->next = merge(l1, l2->next, descend);
            return l2;
        }
    }
}
struct list_head *merge_sort(struct list_head *head, bool descend)
{
    if (!head || !head->next)
        return head;
    struct list_head *fast = head->next;
    struct list_head *slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;
    struct list_head *l1 = merge_sort(head, descend);
    struct list_head *l2 = merge_sort(fast, descend);
    return merge(l1, l2, descend);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    head->prev->next = NULL;
    struct list_head *sorted = merge_sort(head->next, descend);
    struct list_head *temp = head;
    while (sorted) {
        temp->next = sorted;
        sorted->prev = temp;
        temp = temp->next;
        sorted = sorted->next;
    }
    temp->next = head;
    head->prev = temp;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
