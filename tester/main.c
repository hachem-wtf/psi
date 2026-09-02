#include <stdio.h>

#include "maths/complex.h"
#include "maths/matrix.h"
#include "maths/vector.h"
#include "psi.h"

int main(void)
{
	printf("psi %s\n", psi_version());

	struct PsiComplex a = psi_new_complex(1.0, 2.0);
	struct PsiComplex b = psi_new_complex(3.0, -1.0);

	struct PsiComplex sum = psi_add_complex(a, b);
	struct PsiComplex product = psi_mul_complex(a, b);

	printf("a       = %g + %gi\n", a.real, a.imaginary);
	printf("b       = %g + %gi\n", b.real, b.imaginary);
	printf("a + b   = %g + %gi\n", sum.real, sum.imaginary);
	printf("a * b   = %g + %gi\n", product.real, product.imaginary);
	printf("|a|     = %g\n", psi_abs_complex(a));

	struct PsiVector u = psi_column_vector(
		psi_new_complex(1.0, 0.0),
		psi_new_complex(2.0, 0.0),
		psi_new_complex(3.0, 0.0));
	struct PsiVector v = psi_column_vector(
		psi_new_complex(4.0, 0.0),
		psi_new_complex(5.0, 0.0),
		psi_new_complex(6.0, 0.0));

	struct PsiComplex dot = psi_dot_vector(u, v);
	struct PsiComplex norm = psi_norm_vector(u);

	printf("u . v   = %g + %gi\n", dot.real, dot.imaginary);
	printf("|u|     = %g + %gi\n", norm.real, norm.imaginary);

	psi_free_vector(&u);
	psi_free_vector(&v);

	struct PsiMatrix m = psi_matrix(2, 2,
		psi_new_complex(1.0, 0.0), psi_new_complex(2.0, 0.0),
		psi_new_complex(3.0, 0.0), psi_new_complex(4.0, 0.0));
	struct PsiMatrix mm = psi_dot_matrix(m, m);

	printf("m*m     = [%g %g; %g %g]\n",
		psi_get_matrix(mm, 0, 0).real, psi_get_matrix(mm, 0, 1).real,
		psi_get_matrix(mm, 1, 0).real, psi_get_matrix(mm, 1, 1).real);

	struct PsiVector x = psi_column_vector(
		psi_new_complex(1.0, 0.0),
		psi_new_complex(1.0, 0.0));
	struct PsiVector mx = psi_mul_vector_matrix(x, m);

	printf("m*x     = [%g, %g]\n", mx.data[0].real, mx.data[1].real);

	psi_free_vector(&x);
	psi_free_vector(&mx);
	psi_free_matrix(&m);
	psi_free_matrix(&mm);

	return 0;
}
