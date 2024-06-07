///////////////////////////////////////////////////////////
//
// Υλοποίηση του set_utils για Sets βασισμένα σε Binary Search Tree.
//
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "../../include/set_utils.h"
#include "../../include/ADTVector.h"


// Χρησιμοποιούμε τη συγκεκριμένη υλοποίηση του UsingBinarySearchTree/ADTSet.c,
// οπότε γνωρίζουμε την ακριβή δομή για την αναπαράσταση των δεδομένων.
// Αντιγράφουμε εδώ τον ορισμό των structs ώστε να μπορούμε να προσπελάσουμε
// τα περιεχόμενά τους.

struct set {
	SetNode root;				// η ρίζα, NULL αν είναι κενό δέντρο
	int size;					// μέγεθος, ώστε η set_size να είναι Ο(1)
	CompareFunc compare;		// η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του set
};

struct set_node {
	SetNode left, right;		// Παιδιά
	Pointer value;
    int size; // New field to track the size of the subtree

};

// Helper function to update the size of a node
void update_size(SetNode node) {
    if (node == NULL) return;
    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
}

int partition(Vector vec, int low, int high, CompareFunc compare) {
    Pointer pivot = vector_get_at(vec, high);
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (compare(vector_get_at(vec, j), pivot) <= 0) {
            i++;
            Pointer temp = vector_get_at(vec, i);
            vector_set_at(vec, i, vector_get_at(vec, j));
            vector_set_at(vec, j, temp);
        }
    }
    Pointer temp = vector_get_at(vec, i + 1);
    vector_set_at(vec, i + 1, vector_get_at(vec, high));
    vector_set_at(vec, high, temp);
    return (i + 1);
}

void quicksort(Vector vec, int low, int high, CompareFunc compare) {
    if (low < high) {
        int pi = partition(vec, low, high, compare);
        quicksort(vec, low, pi - 1, compare);
        quicksort(vec, pi + 1, high, compare);
    }
}


void vector_sort(Vector vec, CompareFunc compare) {
    if (vector_size(vec) > 0) {
        quicksort(vec, 0, vector_size(vec) - 1, compare);
    }
}


SetNode create_balanced_bst(Vector vec, int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    SetNode node = malloc(sizeof(*node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->value = vector_get_at(vec, mid);
    node->left = create_balanced_bst(vec, start, mid - 1);
    node->right = create_balanced_bst(vec, mid + 1, end);
    node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0); // Ensure size is updated correctly
    printf("Created node with value: %d, size: %d\n", *(int*)node->value, node->size);  // Debug print
    return node;
}

Set set_from_vector(Vector vec, CompareFunc compare) {
    int n = vector_size(vec);
    Vector sorted_vec = vector_create(0, NULL);

    // Copy unique elements to sorted_vec
    for (int i = 0; i < n; i++) {
        Pointer elem = vector_get_at(vec, i);
        if (vector_find(sorted_vec, elem, compare) == NULL) {
            vector_insert_last(sorted_vec, elem);
        }
    }

    // Sort the elements
    vector_sort(sorted_vec, compare);

    // Create a balanced BST from the sorted elements
    Set set = malloc(sizeof(*set));
    if (!set) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    set->root = create_balanced_bst(sorted_vec, 0, vector_size(sorted_vec) - 1);
    set->size = vector_size(sorted_vec);
    set->compare = compare;
    set->destroy_value = NULL;

    vector_destroy(sorted_vec);
    return set;
}


void inorder_traversal(SetNode node, Vector vec) {
    if (node == NULL)
        return;

    inorder_traversal(node->left, vec);
    printf("Inserting value to vector: %d\n", *(int*)node->value);  // Debug print
    vector_insert_last(vec, node->value);
    inorder_traversal(node->right, vec);
}

Vector set_to_vector(Set set) {
    Vector vec = vector_create(set->size, NULL);
    inorder_traversal(set->root, vec);
    return vec;
}

// Helper function for in-order traversal
void inorder_traverse(SetNode node, Set set, TraverseFunc f) {
    if (node == NULL)
        return;

    inorder_traverse(node->left, set, f);
    f(set, node->value);
    inorder_traverse(node->right, set, f);
}

void set_traverse(Set set, TraverseFunc f) {
    inorder_traverse(set->root, set, f);
}

// Merging two sorted arrays into one sorted array
Vector merge_sorted_vectors(Vector vec1, Vector vec2, CompareFunc compare) {
    int n1 = vector_size(vec1);
    int n2 = vector_size(vec2);
    Vector merged_vec = vector_create(0, NULL);

    int i = 0, j = 0;
    while (i < n1 && j < n2) {
        if (compare(vector_get_at(vec1, i), vector_get_at(vec2, j)) <= 0) {
            if (vector_find(merged_vec, vector_get_at(vec1, i), compare) == NULL) {
                vector_insert_last(merged_vec, vector_get_at(vec1, i));
            }
            i++;
        } else {
            if (vector_find(merged_vec, vector_get_at(vec2, j), compare) == NULL) {
                vector_insert_last(merged_vec, vector_get_at(vec2, j));
            }
            j++;
        }
    }
    while (i < n1) {
        if (vector_find(merged_vec, vector_get_at(vec1, i), compare) == NULL) {
            vector_insert_last(merged_vec, vector_get_at(vec1, i));
        }
        i++;
    }
    while (j < n2) {
        if (vector_find(merged_vec, vector_get_at(vec2, j), compare) == NULL) {
            vector_insert_last(merged_vec, vector_get_at(vec2, j));
        }
        j++;
    }

    return merged_vec;
}



Set set_merge(Set set1, Set set2, CompareFunc compare) {
    Vector vec1 = set_to_vector(set1);
    Vector vec2 = set_to_vector(set2);

    Vector merged_vec = merge_sorted_vectors(vec1, vec2, compare);

    Set merged_set = set_from_vector(merged_vec, compare);

    vector_destroy(vec1);
    vector_destroy(vec2);
    vector_destroy(merged_vec);

    return merged_set;
}


Pointer set_find_k_smallest(Set set, int k) {
    SetNode node = set->root;

    while (node != NULL) {
        int left_size = node->left ? node->left->size : 0;

        if (left_size > k) {
            node = node->left;
        } else if (left_size < k) {
            k -= left_size + 1;
            node = node->right;
        } else {
            return node->value;
        }
    }

    return NULL; // Should not reach here if k is valid
}
