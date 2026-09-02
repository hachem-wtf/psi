#pragma once

#include <stddef.h>

#include "maths/complex.h"
#include "maths/vector.h"

struct PsiMatrix
{
	struct PsiComplex *data;
	size_t rows;
	size_t cols;
};

struct PsiMatrix psi_new_matrix(size_t rows, size_t cols);
struct PsiMatrix psi_new_matrix_from(const struct PsiComplex *data, size_t rows, size_t cols);
struct PsiMatrix psi_clone_matrix(struct PsiMatrix m);
void psi_free_matrix(struct PsiMatrix *m);

#define psi_matrix(rows, cols, ...) \
	psi_new_matrix_from((struct PsiComplex[]){ __VA_ARGS__ }, rows, cols)

struct PsiComplex psi_get_matrix(struct PsiMatrix m, size_t row, size_t col);
void psi_set_matrix(struct PsiMatrix *m, size_t row, size_t col, struct PsiComplex value);

struct PsiMatrix psi_dot_matrix(struct PsiMatrix a, struct PsiMatrix b);
struct PsiMatrix psi_kronecker_matrix(struct PsiMatrix a, struct PsiMatrix b);
struct PsiMatrix psi_transpose_matrix(struct PsiMatrix m);
struct PsiMatrix psi_add_matrix(struct PsiMatrix a, struct PsiMatrix b);
struct PsiMatrix psi_sub_matrix(struct PsiMatrix a, struct PsiMatrix b);
struct PsiMatrix psi_scale_matrix(struct PsiMatrix m, struct PsiComplex scalar);

struct PsiMatrix psi_matrix_from_vector(struct PsiVector v);
struct PsiVector psi_vector_from_matrix(struct PsiMatrix m, enum PsiVectorKind kind);
struct PsiVector psi_mul_vector_matrix(struct PsiVector v, struct PsiMatrix m);
