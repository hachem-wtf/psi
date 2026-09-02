#include "maths/matrix.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct PsiMatrix psi_new_matrix(size_t rows, size_t cols)
{
	struct PsiComplex *data = calloc(rows * cols, sizeof(struct PsiComplex));
	assert(data != NULL || rows * cols == 0);

	return (struct PsiMatrix)
	{
		data,
		rows,
		cols,
	};
}

struct PsiMatrix psi_new_matrix_from(const struct PsiComplex *data, size_t rows, size_t cols)
{
	struct PsiMatrix m = psi_new_matrix(rows, cols);
	memcpy(m.data, data, rows * cols * sizeof(struct PsiComplex));

	return m;
}

struct PsiMatrix psi_clone_matrix(struct PsiMatrix m)
{
	return psi_new_matrix_from(m.data, m.rows, m.cols);
}

void psi_free_matrix(struct PsiMatrix *m)
{
	free(m->data);
	m->data = NULL;
	m->rows = 0;
	m->cols = 0;
}

struct PsiComplex psi_get_matrix(struct PsiMatrix m, size_t row, size_t col)
{
	assert(row < m.rows && col < m.cols);
	return m.data[row * m.cols + col];
}

void psi_set_matrix(struct PsiMatrix *m, size_t row, size_t col, struct PsiComplex value)
{
	assert(row < m->rows && col < m->cols);
	m->data[row * m->cols + col] = value;
}

struct PsiMatrix psi_dot_matrix(struct PsiMatrix a, struct PsiMatrix b)
{
	assert(a.cols == b.rows);

	struct PsiMatrix result = psi_new_matrix(a.rows, b.cols);
	for (size_t i = 0; i < a.rows; i++)
		for (size_t j = 0; j < b.cols; j++)
		{
			struct PsiComplex sum = psi_new_complex(0.0, 0.0);
			for (size_t k = 0; k < a.cols; k++)
				sum = psi_add_complex(sum, psi_mul_complex(a.data[i * a.cols + k], b.data[k * b.cols + j]));

			result.data[i * result.cols + j] = sum;
		}

	return result;
}

struct PsiMatrix psi_kronecker_matrix(struct PsiMatrix a, struct PsiMatrix b)
{
	struct PsiMatrix result = psi_new_matrix(a.rows * b.rows, a.cols * b.cols);
	for (size_t i = 0; i < a.rows; i++)
		for (size_t j = 0; j < a.cols; j++)
		{
			struct PsiComplex a_val = a.data[i * a.cols + j];
			for (size_t k = 0; k < b.rows; k++)
				for (size_t l = 0; l < b.cols; l++)
				{
					size_t row = i * b.rows + k;
					size_t col = j * b.cols + l;
					result.data[row * result.cols + col] = psi_mul_complex(a_val, b.data[k * b.cols + l]);
				}
		}

	return result;
}

struct PsiMatrix psi_transpose_matrix(struct PsiMatrix m)
{
	struct PsiMatrix result = psi_new_matrix(m.cols, m.rows);
	for (size_t i = 0; i < m.rows; i++)
		for (size_t j = 0; j < m.cols; j++)
			result.data[j * result.cols + i] = m.data[i * m.cols + j];

	return result;
}

struct PsiMatrix psi_add_matrix(struct PsiMatrix a, struct PsiMatrix b)
{
	assert(a.rows == b.rows && a.cols == b.cols);

	struct PsiMatrix result = psi_new_matrix(a.rows, a.cols);
	for (size_t i = 0; i < a.rows * a.cols; i++)
		result.data[i] = psi_add_complex(a.data[i], b.data[i]);

	return result;
}

struct PsiMatrix psi_sub_matrix(struct PsiMatrix a, struct PsiMatrix b)
{
	assert(a.rows == b.rows && a.cols == b.cols);

	struct PsiMatrix result = psi_new_matrix(a.rows, a.cols);
	for (size_t i = 0; i < a.rows * a.cols; i++)
		result.data[i] = psi_sub_complex(a.data[i], b.data[i]);

	return result;
}

struct PsiMatrix psi_scale_matrix(struct PsiMatrix m, struct PsiComplex scalar)
{
	struct PsiMatrix result = psi_new_matrix(m.rows, m.cols);
	for (size_t i = 0; i < m.rows * m.cols; i++)
		result.data[i] = psi_mul_complex(m.data[i], scalar);

	return result;
}

struct PsiMatrix psi_matrix_from_vector(struct PsiVector v)
{
	if (v.kind == PSI_ROW_VECTOR)
		return psi_new_matrix_from(v.data, 1, v.size);

	return psi_new_matrix_from(v.data, v.size, 1);
}

struct PsiVector psi_vector_from_matrix(struct PsiMatrix m, enum PsiVectorKind kind)
{
	return psi_new_vector_from(m.data, m.rows * m.cols, kind);
}

struct PsiVector psi_mul_vector_matrix(struct PsiVector v, struct PsiMatrix m)
{
	if (v.kind == PSI_COLUMN_VECTOR)
	{
		assert(m.cols == v.size);

		struct PsiVector result = psi_new_vector(m.rows, PSI_COLUMN_VECTOR);
		for (size_t i = 0; i < m.rows; i++)
		{
			struct PsiComplex sum = psi_new_complex(0.0, 0.0);
			for (size_t j = 0; j < m.cols; j++)
				sum = psi_add_complex(sum, psi_mul_complex(m.data[i * m.cols + j], v.data[j]));

			result.data[i] = sum;
		}

		return result;
	}

	assert(v.size == m.rows);

	struct PsiVector result = psi_new_vector(m.cols, PSI_ROW_VECTOR);
	for (size_t j = 0; j < m.cols; j++)
	{
		struct PsiComplex sum = psi_new_complex(0.0, 0.0);
		for (size_t i = 0; i < m.rows; i++)
			sum = psi_add_complex(sum, psi_mul_complex(v.data[i], m.data[i * m.cols + j]));

		result.data[j] = sum;
	}

	return result;
}
