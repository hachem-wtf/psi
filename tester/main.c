#include <stdio.h>

#include "maths/complex.h"
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

	return 0;
}
