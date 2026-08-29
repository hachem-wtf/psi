#include "maths/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static enum PsiVectorKind flip_kind(enum PsiVectorKind kind)
{
	if (kind == PSI_ROW_VECTOR)
		return PSI_COLUMN_VECTOR;

	return PSI_ROW_VECTOR;
}

struct PsiVector psi_new_vector(size_t size, enum PsiVectorKind kind)
{
	// all-zero bits == 0.0 for both components
	struct PsiComplex *data = calloc(size, sizeof(struct PsiComplex));
	assert(data != NULL || size == 0);

	return (struct PsiVector)
	{
		data,
		size,
		kind,
	};
}

struct PsiVector psi_new_vector_from(const struct PsiComplex *data, size_t size, enum PsiVectorKind kind)
{
	struct PsiVector v = psi_new_vector(size, kind);
	memcpy(v.data, data, size * sizeof(struct PsiComplex));

	return v;
}

struct PsiVector psi_clone_vector(struct PsiVector v)
{
	return psi_new_vector_from(v.data, v.size, v.kind);
}

void psi_free_vector(struct PsiVector *v)
{
	free(v->data);
	v->data = NULL;
	v->size = 0;
}

struct PsiComplex psi_get_vector(struct PsiVector v, size_t index)
{
	assert(index < v.size);
	return v.data[index];
}

void psi_set_vector(struct PsiVector *v, size_t index, struct PsiComplex value)
{
	assert(index < v->size);
	v->data[index] = value;
}

struct PsiComplex psi_dot_vector(struct PsiVector a, struct PsiVector b)
{
	assert(a.size == b.size);

	struct PsiComplex sum = psi_new_complex(0.0, 0.0);
	for (size_t i = 0; i < a.size; i++)
		sum = psi_add_complex(sum, psi_mul_complex(a.data[i], b.data[i]));

	return sum;
}

struct PsiComplex psi_norm_vector(struct PsiVector v)
{
	struct PsiComplex sum = psi_new_complex(0.0, 0.0);
	for (size_t i = 0; i < v.size; i++)
		sum = psi_add_complex(sum, psi_mul_complex(v.data[i], v.data[i]));

	return psi_sqrt_complex(sum);
}

struct PsiComplex psi_sum_vector(struct PsiVector v)
{
	struct PsiComplex sum = psi_new_complex(0.0, 0.0);
	for (size_t i = 0; i < v.size; i++)
		sum = psi_add_complex(sum, v.data[i]);

	return sum;
}

// ordering mirrors rust's derived PartialOrd: real first, then imaginary
static int less_than(struct PsiComplex a, struct PsiComplex b)
{
	if (a.real != b.real)
		return a.real < b.real;

	return a.imaginary < b.imaginary;
}

struct PsiComplex psi_max_vector(struct PsiVector v)
{
	if (v.size == 0)
		return psi_new_complex(0.0, 0.0);

	struct PsiComplex best = v.data[0];
	for (size_t i = 1; i < v.size; i++)
		if (less_than(best, v.data[i]))
			best = v.data[i];

	return best;
}

struct PsiComplex psi_min_vector(struct PsiVector v)
{
	if (v.size == 0)
		return psi_new_complex(0.0, 0.0);

	struct PsiComplex best = v.data[0];
	for (size_t i = 1; i < v.size; i++)
		if (less_than(v.data[i], best))
			best = v.data[i];

	return best;
}

struct PsiVector psi_add_vector(struct PsiVector a, struct PsiVector b)
{
	assert(a.size == b.size);

	struct PsiVector result = psi_new_vector(a.size, a.kind);
	for (size_t i = 0; i < a.size; i++)
		result.data[i] = psi_add_complex(a.data[i], b.data[i]);

	return result;
}

struct PsiVector psi_sub_vector(struct PsiVector a, struct PsiVector b)
{
	assert(a.size == b.size);

	struct PsiVector result = psi_new_vector(a.size, a.kind);
	for (size_t i = 0; i < a.size; i++)
		result.data[i] = psi_sub_complex(a.data[i], b.data[i]);

	return result;
}

struct PsiVector psi_scale_vector(struct PsiVector v, struct PsiComplex scalar)
{
	struct PsiVector result = psi_new_vector(v.size, v.kind);
	for (size_t i = 0; i < v.size; i++)
		result.data[i] = psi_mul_complex(v.data[i], scalar);

	return result;
}

struct PsiVector psi_transpose_vector(struct PsiVector v)
{
	struct PsiVector result = psi_clone_vector(v);
	result.kind = flip_kind(v.kind);

	return result;
}
