#include <criterion/criterion.h>
#include "../src/inc/list.h"
#include "../src/inc/dbg.h"

typedef struct {
    const char *s;
    int len;
} foo_t;

REG_LIST(int, int);
REG_LIST(const_char, const char *);
REG_LIST(foo, foo_t *);
REG_LIST(int_list, int_node_t *);

Test(list, new__works_with_int) {
    int_node_t *node = int_node__new(123);
    cr_assert_eq(node->val, 123);
    free(node);
}

Test(list, new__works_with_const_char) {
    const_char_node_t *node = const_char_node__new("foobar");
    cr_assert_str_eq(node->val, "foobar");
    free(node);
}

Test(list, append_after__works_with_int) {
    int_node_t *head = int_node__new(1);
    int_node__append_after(head, int_node__new(2));
    int_node__append_after(head->next, int_node__new(3));
    int_node__append_after(head->next->next, int_node__new(4));

    cr_assert_eq(head->val, 1);
    cr_assert_eq(head->next->val, 2);
    cr_assert_eq(head->next->next->val, 3);
    cr_assert_eq(head->next->next->next->val, 4);

    free(head->next->next->next);
    free(head->next->next);
    free(head->next);
    free(head);
}

Test(list, append_after__works_with_const_char) {
    const_char_node_t *head = const_char_node__new("foo");
    const_char_node__append_after(head, const_char_node__new("bar"));
    const_char_node__append_after(head->next, const_char_node__new("baz"));
    const_char_node__append_after(head->next->next, const_char_node__new("quzz"));

    cr_assert_str_eq(head->val, "foo");
    cr_assert_str_eq(head->next->val, "bar");
    cr_assert_str_eq(head->next->next->val, "baz");
    cr_assert_str_eq(head->next->next->next->val, "quzz");

    free(head->next->next->next);
    free(head->next->next);
    free(head->next);
    free(head);
}

Test(list, append_after__works_with_structures) {
    foo_t foo = {"foo", 3};
    foo_t foo1 = {"bar", 3};
    foo_t foo2 = {"baz", 3};
    foo_t foo3 = {"quzz", 4};

    foo_node_t *head = foo_node__new(&foo);
    foo_node__append_after(head, foo_node__new(&foo1));
    foo_node__append_after(head->next, foo_node__new(&foo2));
    foo_node__append_after(head->next->next, foo_node__new(&foo3));

    cr_assert_str_eq(head->val->s, "foo");
    cr_assert_str_eq(head->next->val->s, "bar");
    cr_assert_eq(head->next->val->len, 3);
    cr_assert_str_eq(head->next->next->val->s, "baz");
    cr_assert_eq(head->next->next->val->len, 3);
    cr_assert_str_eq(head->next->next->next->val->s, "quzz");
    cr_assert_eq(head->next->next->next->val->len, 4);

    free(head->next->next->next);
    free(head->next->next);
    free(head->next);
    free(head);
}

Test(list, append_after__works_as_list_of_lists) {
    int_node_t *int_list_1 = int_node__new(1);
    int_node_t *int_list_2 = int_node__new(3);

    int_node__append_after(int_list_1, int_node__new(2));
    int_node__append_after(int_list_2, int_node__new(4));

    int_list_node_t *head = int_list_node__new(int_list_1);
    int_list_node__append_after(head, int_list_node__new(int_list_2));

    cr_assert_eq(head->val->val, 1);
    cr_assert_eq(head->val->next->val, 2);

    cr_assert_eq(head->next->val->val, 3);
    cr_assert_eq(head->next->val->next->val, 4);

    free(int_list_1->next);
    free(int_list_1);

    free(int_list_2->next);
    free(int_list_2);

    free(head);
}

Test(list, remove__works_with_integers) {
    int_node_t *head = int_node__new(1);

    int_node__append_after(head, int_node__new(2));
    int_node__append_after(head->next, int_node__new(3));
    int_node__append_after(head->next->next, int_node__new(4));
    int_node__append_after(head->next->next->next, int_node__new(5));

    free(int_node__remove(head->next));
    free(int_node__remove(head->next->next));

    cr_assert_eq(head->val, 1);
    cr_assert_eq(head->next->val, 3);
    cr_assert_eq(head->next->next->val, 5);

    free(int_node__remove(head->next));
    free(int_node__remove(head->next));
    free(head);
}

Test(list, remove__can_delete_head_and_tail) {
    int_node_t *head = int_node__new(1);
    int_node_t *head_;

    int_node__append_after(head, int_node__new(2));
    int_node__append_after(head->next, int_node__new(3));
    int_node__append_after(head->next->next, int_node__new(4));

    head_ = head->next;

    free(int_node__remove(head));
    head = head_;
    free(int_node__remove(head->next->next));

    cr_assert_eq(head->val, 2);
    cr_assert_eq(head->next->val, 3);

    free(int_node__remove(head->next));
    free(head);
}
