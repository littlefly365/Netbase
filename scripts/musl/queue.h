#ifndef _SYS_QUEUE_H
#define _SYS_QUEUE_H

#define STAILQ_HEAD(name, type)                                         \
struct name {                                                           \
        struct type *stqh_first;        /* first element */             \
        struct type **stqh_last;        /* addr of last next element */ \
}

#define STAILQ_HEAD_INITIALIZER(head)                                   \
        { NULL, &(head).stqh_first }

#define STAILQ_ENTRY(type)                                              \
struct {                                                                \
        struct type *stqe_next; /* next element */                      \
}

#define STAILQ_FIRST(head)      ((head)->stqh_first)
#define STAILQ_END(head)        NULL
#define STAILQ_NEXT(elm, field) ((elm)->field.stqe_next)
#define STAILQ_EMPTY(head)      (STAILQ_FIRST(head) == STAILQ_END(head))

#define STAILQ_INSERT_HEAD(head, elm, field) do {                       \
        if (((elm)->field.stqe_next = (head)->stqh_first) == NULL)      \
                (head)->stqh_last = &(elm)->field.stqe_next;            \
        (head)->stqh_first = (elm);                                     \
} while (/*CONSTCOND*/0)

#define STAILQ_INSERT_TAIL(head, elm, field) do {                       \
        (elm)->field.stqe_next = NULL;                                  \
        *(head)->stqh_last = (elm);                                     \
        (head)->stqh_last = &(elm)->field.stqe_next;                    \
} while (/*CONSTCOND*/0)

#define STAILQ_REMOVE_HEAD(head, field) do {                            \
        if (((head)->stqh_first = (head)->stqh_first->field.stqe_next) == NULL) \
                (head)->stqh_last = &(head)->stqh_first;                        \
} while (/*CONSTCOND*/0)

#define STAILQ_REMOVE(head, elm, type, field) do {                      \
        if ((head)->stqh_first == (elm)) {                              \
                STAILQ_REMOVE_HEAD((head), field);                      \
        } else {                                                        \
                struct type *curelm = (head)->stqh_first;               \
                while (curelm->field.stqe_next != (elm))                        \
                        curelm = curelm->field.stqe_next;               \
                if ((curelm->field.stqe_next =                          \
                        curelm->field.stqe_next->field.stqe_next) == NULL) \
                            (head)->stqh_last = &(curelm)->field.stqe_next; \
        }                                                               \
} while (/*CONSTCOND*/0)

#endif
