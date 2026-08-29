#include <stdio.h>

#include "maths/complex.h"
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

	return 0;
}
