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

int compare_ints(Pointer a, Pointer b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;

    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}



void test_set_from_vector(void) {
    Vector vec = vector_create(0, NULL);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    int* val3 = malloc(sizeof(int)); *val3 = 3;
    int* val4 = malloc(sizeof(int)); *val4 = 2; // duplicate

    vector_insert_last(vec, val1);
    vector_insert_last(vec, val2);
    vector_insert_last(vec, val3);
    vector_insert_last(vec, val4);

    Set set = set_from_vector(vec, compare_ints);

    printf("Expected size: 3, Actual size: %d\n", set_size(set));
    TEST_CHECK(set_size(set) == 3);
    TEST_CHECK(*(int*)set_find(set, val1) == 1);
    TEST_CHECK(*(int*)set_find(set, val2) == 2);
    TEST_CHECK(*(int*)set_find(set, val3) == 3);

    set_destroy(set);
    vector_destroy(vec);
    free(val1);
    free(val2);
    free(val3);
    free(val4);
}

void test_set_to_vector(void) {
    Set set = set_create(compare_ints, free);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    int* val3 = malloc(sizeof(int)); *val3 = 3;

    set_insert(set, val1);
    set_insert(set, val2);
    set_insert(set, val3);

    Vector vec = set_to_vector(set);

    printf("Expected size: 3, Actual size: %d\n", vector_size(vec));
    for (int i = 0; i < vector_size(vec); i++) {
        int* val = (int*)vector_get_at(vec, i);
        if (val == NULL) {
            printf("vec[%d]: NULL\n", i);
        } else {
            printf("vec[%d]: %d\n", i, *val);
        }
    }
    TEST_CHECK(vector_size(vec) == 3);
    if (vector_size(vec) >= 3) { // Additional safety check
        TEST_CHECK(*(int*)vector_get_at(vec, 0) == 1);
        TEST_CHECK(*(int*)vector_get_at(vec, 1) == 2);
        TEST_CHECK(*(int*)vector_get_at(vec, 2) == 3);
    }

    vector_destroy(vec); // Ensure vector is destroyed
    set_destroy(set); // Ensure set is destroyed
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
    Set set1 = set_create(compare_ints, free);
    Set set2 = set_create(compare_ints, free);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    int* val3 = malloc(sizeof(int)); *val3 = 3;
    int* val4 = malloc(sizeof(int)); *val4 = 4;

    set_insert(set1, val1);
    set_insert(set1, val2);
    set_insert(set2, val3);
    set_insert(set2, val4);

    Set merged_set = set_merge(set1, set2, compare_ints);
    Vector vec = set_to_vector(merged_set);

    printf("Expected size: 4, Actual size: %d\n", vector_size(vec));
    for (int i = 0; i < vector_size(vec); i++) {
        int* val = (int*)vector_get_at(vec, i);
        if (val == NULL) {
            printf("vec[%d]: NULL\n", i);
        } else {
            printf("vec[%d]: %d\n", i, *val);
        }
    }
    TEST_CHECK(vector_size(vec) == 4);
    if (vector_size(vec) >= 4) { // Additional safety check
        TEST_CHECK(*(int*)vector_get_at(vec, 0) == 1);
        TEST_CHECK(*(int*)vector_get_at(vec, 1) == 2);
        TEST_CHECK(*(int*)vector_get_at(vec, 2) == 3);
        TEST_CHECK(*(int*)vector_get_at(vec, 3) == 4);
    }

    vector_destroy(vec);
    set_destroy(set1);
    set_destroy(set2);
    set_destroy(merged_set);
}


void test_set_find_k_smallest(void) {
    Set set = set_create(compare_ints, free);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    int* val3 = malloc(sizeof(int)); *val3 = 3;

    set_insert(set, val1);
    set_insert(set, val2);
    set_insert(set, val3);

    TEST_CHECK(*(int*)set_find_k_smallest(set, 0) == 1);
    TEST_CHECK(*(int*)set_find_k_smallest(set, 1) == 2);
    TEST_CHECK(*(int*)set_find_k_smallest(set, 2) == 3);

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
