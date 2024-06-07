    ///////////////////////////////////////////////////////////
//
// Υλοποίηση του set_utils με γενικό τρόπο, χρησιμοποιώντας
// μια οποιαδήποτε υλοποίηση του ADT Set.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include "../../include/ADTSet.h"
#include "../../include/ADTVector.h"
#include "../../include/set_utils.h"


Set set_from_vector(Vector vec, CompareFunc compare) {
    Set set = set_create(compare, NULL);
    for (int i = 0; i < vector_size(vec); i++) {
            set_insert(set, vector_get_at(vec, i));
    }
    return set;
}

Vector set_to_vector(Set set) {
    Vector vec = vector_create(set_size(set), NULL);
    int i = 0;
    for (SetNode node = set_first(set); node != SET_EOF; node = set_next(set, node)) {
        vector_set_at(vec, i++, set_node_value(set, node));
    }
    return vec;
}

void set_traverse(Set set, TraverseFunc f) {
    for (SetNode node = set_first(set); node != SET_EOF; node = set_next(set, node)) {
        f(set, set_node_value(set, node));
    }
}

Set set_merge(Set set1, Set set2, CompareFunc compare) {
    Set merged_set = set_create(compare, NULL);

    for (SetNode node = set_first(set1); node != SET_EOF; node = set_next(set1, node)) {
        set_insert(merged_set, set_node_value(set1, node));
    }

    for (SetNode node = set_first(set2); node != SET_EOF; node = set_next(set2, node)) {
        set_insert(merged_set, set_node_value(set2, node));
    }

    return merged_set;
}

Pointer set_find_k_smallest(Set set, int k) {
    SetNode node = set_first(set);
    for (int i = 0; i < k; i++) {
        node = set_next(set, node);
    }
    return set_node_value(set, node);
}
