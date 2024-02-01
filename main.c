#include <stdio.h>
#include "matrix/core.h"


int main() {
    matrix a = make_matrix(3, 4);

    a.data[idx(0, 1, &a)] = 5;
    a.data[idx(0, 2, &a)] = 5;
    a.data[idx(0, 3, &a)] = 5;
    a.data[idx(1, 1, &a)] = 6;

    show(&a);
    printf("\n");
    transpose(&a);
    scale(&a, 5);
    show(&a);

    destroy_matrix(&a);
    // destroy_cycle(head);
}

