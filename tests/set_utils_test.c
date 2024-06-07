//////////////////////////////////////////////////////////////////
//
// Unit tests για το set_utils.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "set_utils.h"


#include "ADTSet.h"
#include "ADTVector.h"
#include "acutest.h"

void test_set_from_vector(void) {
    Vector vec = vector_create(0, NULL);

    vector_insert_last(vec, (Pointer)1);
    vector_insert_last(vec, (Pointer)2);
    vector_insert_last(vec, (Pointer)3);
    vector_insert_last(vec, (Pointer)2); // duplicate

    Set set = set_from_vector(vec, compare_ints);

    TEST_CHECK(set_size(set) == 3);
    TEST_CHECK(set_find(set, (Pointer)1) == (Pointer)1);
    TEST_CHECK(set_find(set, (Pointer)2) == (Pointer)2);
    TEST_CHECK(set_find(set, (Pointer)3) == (Pointer)3);

    set_destroy(set);
    vector_destroy(vec);
}

void test_set_to_vector(void) {
    Set set = set_create(compare_ints, NULL);

    set_insert(set, (Pointer)3);
    set_insert(set, (Pointer)1);
    set_insert(set, (Pointer)2);

    Vector vec = set_to_vector(set);

    TEST_CHECK(vector_size(vec) == 3);
    TEST_CHECK(vector_get_at(vec, 0) == (Pointer)1);
    TEST_CHECK(vector_get_at(vec, 1) == (Pointer)2);
    TEST_CHECK(vector_get_at(vec, 2) == (Pointer)3);

    set_destroy(set);
    vector_destroy(vec);
}

void traverse_increment(Set set, Pointer value) {
    (*(int*)value)++;
}

void test_set_traverse(void) {
    Set set = set_create(compare_ints, free);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    int* val3 = malloc(sizeof(int)); *val3 = 3;

    set_insert(set, val1);
    set_insert(set, val2);
    set_insert(set, val3);

    set_traverse(set, traverse_increment);

    TEST_CHECK(*(int*)set_find(set, val1) == 2);
    TEST_CHECK(*(int*)set_find(set, val2) == 3);
    TEST_CHECK(*(int*)set_find(set, val3) == 4);

    set_destroy(set);
}

void test_set_merge(void) {
    Set set1 = set_create(compare_ints, NULL);
    Set set2 = set_create(compare_ints, NULL);

    set_insert(set1, (Pointer)1);
    set_insert(set1, (Pointer)2);
    set_insert(set2, (Pointer)3);
    set_insert(set2, (Pointer)4);

    Set merged_set = set_merge(set1, set2, compare_ints);

    TEST_CHECK(set_size(merged_set) == 4);
    TEST_CHECK(set_find(merged_set, (Pointer)1) == (Pointer)1);
    TEST_CHECK(set_find(merged_set, (Pointer)2) == (Pointer)2);
    TEST_CHECK(set_find(merged_set, (Pointer)3) == (Pointer)3);
    TEST_CHECK(set_find(merged_set, (Pointer)4) == (Pointer)4);

    set_destroy(set1);
    set_destroy(set2);
    set_destroy(merged_set);
}

void test_set_find_k_smallest(void) {
    Set set = set_create(compare_ints, NULL);

    set_insert(set, (Pointer)3);
    set_insert(set, (Pointer)1);
    set_insert(set, (Pointer)2);

    TEST_CHECK(set_find_k_smallest(set, 0) == (Pointer)1);
    TEST_CHECK(set_find_k_smallest(set, 1) == (Pointer)2);
    TEST_CHECK(set_find_k_smallest(set, 2) == (Pointer)3);

    set_destroy(set);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_set_import_from_vector",	test_set_from_vector },
	{ "test_set_export_to_vector",		test_set_to_vector },
	{ "test_set_traverse",				test_set_traverse },
	{ "test_set_merge",					test_set_merge },
	{ "test_set_find_k_smallest",		test_set_find_k_smallest },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
