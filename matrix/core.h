//
// Created by clodman84 on 1/2/24.
//

#ifndef UNTITLED_CORE_H
#define UNTITLED_CORE_H

typedef struct {
    unsigned int rows;
    unsigned int columns;
    double *data;
} matrix;

matrix make_matrix(unsigned int rows, unsigned int columns);
void destroy_matrix(matrix *m);
int idx(int i, int j, matrix *m);
void show(matrix *m);

void add(matrix *a, matrix *b) ;
void sub(matrix *a, matrix *b);
void scale(matrix *a, double x);

void transpose(matrix *m);
#endif //UNTITLED_CORE_H
