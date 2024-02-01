#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int rows;
    unsigned int columns;
    double *data;
} matrix;

matrix make_matrix(unsigned int rows, unsigned int columns) {
    matrix m;
    m.rows = rows;
    m.columns = columns;
    // to access the element (i, j) get *(m.data + i*columns + j)
    m.data = (double *)calloc(rows * columns, sizeof(double));
    return m;
}

void destroy_matrix(matrix *m) { free(m->data); }

int idx(int i, int j, matrix *m) {
    return i * (int)m->columns + j;
} // goes from (i, j) to the index of the array in matrix.data

void show(matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            printf("%f | ", m->data[idx(i, j, m)]);
        }
        printf("\n");
    }
}

void add(matrix *a, matrix *b) {
    // a will get modified
    if (a->rows != b->rows && a->columns != b->columns) {
        printf("MATRIX addition not possible, size incompatible A: (%d, %d) B: "
               "(%d, %d)",
               a->rows, a->columns, b->rows, b->columns);
        return;
    }

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->columns; j++) {
            a->data[idx(i, j, a)] += b->data[idx(i, j, b)];
        }
    }
}

void sub(matrix *a, matrix *b) {
    // a will get modified
    if (a->rows != b->rows && a->columns != b->columns) {
        printf("MATRIX subtraction not possible, size incompatible A: (%d, %d) B: "
               "(%d, %d)",
               a->rows, a->columns, b->rows, b->columns);
        return;
    }

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->columns; j++) {
            a->data[idx(i, j, a)] -= b->data[idx(i, j, b)];
        }
    }
}

void scale(matrix *a, double x) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->columns; j++) {
            a->data[idx(i, j, a)] *= x;
        }
    }
}

void swap(double *x, double *y) {
    double temp = *x;
    *x = *y;
    *y = temp;
}

int trans_permutate(int index, matrix *m) {
    // https://en.wikipedia.org/wiki/In-place_matrix_transposition
    if (index == m->rows * m->columns - 1)
        return index;
    return ((int) m->rows * index) % ((int)m->rows * (int)m->columns - 1);
}

struct node {
    struct node *next;
    struct node *previous;
    int index;
};

struct node *search_cycles(int start, matrix *m) {
    // returns a pointer to the first node of a transpose permutation cycle
    struct node *current = (struct node *)malloc(sizeof(struct node));
    current->index = start;
    current->next = NULL;
    current->previous = NULL;

    // keeping this handy
    struct node *first = current;

    while (1) {
        int next_index = trans_permutate(current->index, m);

        if (next_index == start) {
            current->next = first;
            first->previous = current;
            return first;
        }

        struct node *next = (struct node *)malloc(sizeof(struct node));
        next->index = next_index;
        next->previous = current;

        current->next = next;
        current = next;
    }
}

void destroy_cycle(struct node *head) {
    struct node *current = head;
    struct node *next;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);
}

void transpose(matrix *m) {
    if (m->rows == m->columns) {
        // square matrices are obvious
        for (int i = 0; i < m->rows; i++) {
            for (int j = 0; j < i; j++) {
                swap(&(m->data[idx(i, j, m)]), &(m->data[idx(j, i, m)]));
            }
        }
        return;
    }

    // This is where the retarded part is, an implementation of the pseudocode on
    // wikipedia
    int *visited = (int *)calloc(m->rows * m->columns, sizeof(int));
    for (int i = 0; i < m->rows * m->columns; i++) {
        if (visited[i] == 1)
            continue;

        struct node *head = search_cycles(i, m);

        visited[head->index] = 1;
        double d = m->data[head->index];
        struct node *x = head->previous;
        while (x != head) {
            m->data[x->next->index] = m->data[x->index];
            visited[x->index] = 1;
            x = x->previous;
        }
        m->data[head->next->index] = d;

        destroy_cycle(head);
    }
    free(visited);
    unsigned int temp = m->columns;
    m->columns = m->rows;
    m->rows = temp;
}

int main() {
    matrix a = make_matrix(3, 4);

    // printf("Address: %p  Data: %f\n", m.data + 3, m.data[3]);
    // struct node *head = search_cycles(2, &a);
    // struct node *current = head;
    // do {
    //   printf("Index: %d,  Current: %p, Next: %p, Previous: %p\n",
    //   current->index,
    //          current, current->next, current->previous);
    //   current = current->next;
    // } while (current != head);

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

