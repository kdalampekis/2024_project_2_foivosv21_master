#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ADTVector.h"

// Το αρχικό μέγεθος που δεσμεύουμε
#define VECTOR_MIN_CAPACITY 10

struct vector_node {
    Pointer value; // Η τιμή του κόμβου.
};

struct vector {
    VectorNode array; // Τα δεδομένα, πίνακας από struct vector_node
    int size; // Πόσα στοιχεία έχουμε προσθέσει
    int capacity; // Πόσο χώρο έχουμε δεσμεύσει (το μέγεθος του array). Πάντα capacity >= size, αλλά μπορεί να έχουμε
    DestroyFunc destroy_value; // Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};

Vector vector_create(int size, DestroyFunc destroy_value) {
    // Δημιουργία του struct
    Vector vec = malloc(sizeof(*vec));
    if (!vec) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    vec->size = 0; // Initial size is 0, we'll grow as needed
    vec->destroy_value = destroy_value;

    // Δέσμευση μνήμης για τον πίνακα.
    vec->capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;
    vec->array = calloc(vec->capacity, sizeof(*vec->array)); // αρχικοποίηση σε 0 (NULL)
    if (!vec->array) {
        fprintf(stderr, "Memory allocation failed\n");
        free(vec);
        exit(EXIT_FAILURE);
    }

    return vec;
}

int vector_size(Vector vec) {
    return vec->size;
}

Pointer vector_get_at(Vector vec, int pos) {
    assert(pos >= 0 && pos < vec->size);
    return vec->array[pos].value;
}


void vector_set_at(Vector vec, int pos, Pointer value) {
    if (pos < 0 || pos >= vec->size) {
        fprintf(stderr, "Invalid position in vector_set_at: pos=%d, vec->size=%d\n", pos, vec->size);
        assert(pos >= 0 && pos < vec->size);
    }

    // Αν υπάρχει συνάρτηση destroy_value, την καλούμε για το στοιχείο που αντικαθίσταται
    if (value != vec->array[pos].value && vec->destroy_value != NULL)
        vec->destroy_value(vec->array[pos].value);

    vec->array[pos].value = value;
}


void vector_insert_last(Vector vec, Pointer value) {
    // Μεγαλώνουμε τον πίνακα (αν χρειαστεί)
    if (vec->capacity == vec->size) {
        vec->capacity *= 2;
        vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
        if (!vec->array) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Προσθέτουμε το στοιχείο
    vec->array[vec->size].value = value;
    vec->size++;
}


void vector_remove_last(Vector vec) {
    assert(vec->size != 0);

    // Αν υπάρχει συνάρτηση destroy_value, την καλούμε για το στοιχείο που αφαιρείται
    if (vec->destroy_value != NULL)
        vec->destroy_value(vec->array[vec->size - 1].value);

    // Αφαιρούμε στοιχείο οπότε ο πίνακας μικραίνει
    vec->size--;

    // Μικραίνουμε τον πίνακα αν χρειαστεί
    if (vec->capacity > vec->size * 4 && vec->capacity > 2 * VECTOR_MIN_CAPACITY) {
        vec->capacity /= 2;
        vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
        if (!vec->array) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare) {
    // Διάσχιση του vector
    for (int i = 0; i < vec->size; i++)
        if (compare(vec->array[i].value, value) == 0)
            return vec->array[i].value; // βρέθηκε

    return NULL; // δεν υπάρχει
}

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value) {
    DestroyFunc old = vec->destroy_value;
    vec->destroy_value = destroy_value;
    return old;
}

void vector_destroy(Vector vec) {
    // Αν υπάρχει συνάρτηση destroy_value, την καλούμε για όλα τα στοιχεία
    if (vec->destroy_value != NULL)
        for (int i = 0; i < vec->size; i++)
            vec->destroy_value(vec->array[i].value);

    // Πρέπει να κάνουμε free τόσο τον πίνακα όσο και το struct!
    free(vec->array);
    free(vec);
}

// Συναρτήσεις για διάσχιση μέσω node
VectorNode vector_first(Vector vec) {
    if (vec->size == 0)
        return VECTOR_BOF;
    else
        return &vec->array[0];
}

VectorNode vector_last(Vector vec) {
    if (vec->size == 0)
        return VECTOR_EOF;
    else
        return &vec->array[vec->size - 1];
}

VectorNode vector_next(Vector vec, VectorNode node) {
    if (node == &vec->array[vec->size - 1])
        return VECTOR_EOF;
    else
        return node + 1;
}

VectorNode vector_previous(Vector vec, VectorNode node) {
    if (node == &vec->array[0])
        return VECTOR_BOF;
    else
        return node - 1;
}

Pointer vector_node_value(Vector vec, VectorNode node) {
    return node->value;
}

VectorNode vector_find_node(Vector vec, Pointer value, CompareFunc compare) {
    // Διάσχιση του vector
    for (int i = 0; i < vec->size; i++)
        if (compare(vec->array[i].value, value) == 0)
            return &vec->array[i]; // βρέθηκε

    return VECTOR_EOF; // δεν υπάρχει
}
