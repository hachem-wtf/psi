#pragma once

#include <stddef.h>

#include "maths/complex.h"

enum PsiVectorKind
{
	PSI_ROW_VECTOR,
	PSI_COLUMN_VECTOR,
};

struct PsiVector
{
	struct PsiComplex *data;
	size_t size;
	enum PsiVectorKind kind;
};

struct PsiVector psi_new_vector(size_t size, enum PsiVectorKind kind);
struct PsiVector psi_new_vector_from(const struct PsiComplex *data, size_t size, enum PsiVectorKind kind);
struct PsiVector psi_clone_vector(struct PsiVector v);
void psi_free_vector(struct PsiVector *v);

#define psi_row_vector(...) \
	psi_new_vector_from((struct PsiComplex[]){ __VA_ARGS__ }, \
		sizeof((struct PsiComplex[]){ __VA_ARGS__ }) / sizeof(struct PsiComplex), \
		PSI_ROW_VECTOR)
#define psi_column_vector(...) \
	psi_new_vector_from((struct PsiComplex[]){ __VA_ARGS__ }, \
		sizeof((struct PsiComplex[]){ __VA_ARGS__ }) / sizeof(struct PsiComplex), \
		PSI_COLUMN_VECTOR)

struct PsiComplex psi_get_vector(struct PsiVector v, size_t index);
void psi_set_vector(struct PsiVector *v, size_t index, struct PsiComplex value);

struct PsiComplex psi_dot_vector(struct PsiVector a, struct PsiVector b);
struct PsiComplex psi_norm_vector(struct PsiVector v);
struct PsiComplex psi_sum_vector(struct PsiVector v);
struct PsiComplex psi_max_vector(struct PsiVector v);
struct PsiComplex psi_min_vector(struct PsiVector v);

struct PsiVector psi_add_vector(struct PsiVector a, struct PsiVector b);
struct PsiVector psi_sub_vector(struct PsiVector a, struct PsiVector b);
struct PsiVector psi_scale_vector(struct PsiVector v, struct PsiComplex scalar);
struct PsiVector psi_transpose_vector(struct PsiVector v);
