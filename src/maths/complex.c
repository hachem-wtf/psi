#include "maths/complex.h"

#include <math.h>

struct PsiComplex psi_new_complex(double real, double imaginary)
{
	return (struct PsiComplex)
	{
		real,
		imaginary,
	};
}

struct PsiComplex psi_new_complex_from_real(double real)
{
	return (struct PsiComplex)
	{
		real,
		0.0,
	};
}

struct PsiComplex psi_conjugate_complex(struct PsiComplex z)
{
	return (struct PsiComplex)
	{
		z.real,
		-z.imaginary,
	};
}

struct PsiComplex psi_neg_complex(struct PsiComplex z)
{
	return (struct PsiComplex)
	{
		-z.real,
		-z.imaginary,
	};
}

double psi_phase_complex(struct PsiComplex z)
{
	return atan2(z.imaginary, z.real);
}

double psi_norm2_complex(struct PsiComplex z)
{
	return z.real * z.real + z.imaginary * z.imaginary;
}

double psi_abs_complex(struct PsiComplex z)
{
	return sqrt(psi_norm2_complex(z));
}

struct PsiComplex psi_sqrt_complex(struct PsiComplex z)
{
	double r = psi_abs_complex(z);
	double half_theta = psi_phase_complex(z) / 2.0;
	double sqrt_r = sqrt(r);

	return (struct PsiComplex)
	{
		sqrt_r * cos(half_theta),
		sqrt_r * sin(half_theta),
	};
}

struct PsiComplex psi_add_complex(struct PsiComplex a, struct PsiComplex b)
{
	return (struct PsiComplex)
	{
		a.real + b.real,
		a.imaginary + b.imaginary,
	};
}

struct PsiComplex psi_sub_complex(struct PsiComplex a, struct PsiComplex b)
{
	return (struct PsiComplex)
	{
		a.real - b.real,
		a.imaginary - b.imaginary,
	};
}

struct PsiComplex psi_mul_complex(struct PsiComplex a, struct PsiComplex b)
{
	// (a + bi)(c + di) = (ac - bd) + (ad + bc)i
	return (struct PsiComplex)
	{
		a.real * b.real - a.imaginary * b.imaginary,
		a.real * b.imaginary + a.imaginary * b.real,
	};
}

struct PsiComplex psi_div_complex(struct PsiComplex a, struct PsiComplex b)
{
	// (a + bi) / (c + di) = ((ac + bd) + (bc - ad)i) / (c² + d²)
	double denom = b.real * b.real + b.imaginary * b.imaginary;
	return (struct PsiComplex)
	{
		(a.real * b.real + a.imaginary * b.imaginary) / denom,
		(a.imaginary * b.real - a.real * b.imaginary) / denom,
	};
}

struct PsiComplex psi_add_complex_real(struct PsiComplex a, double b)
{
	return (struct PsiComplex)
	{
		a.real + b,
		a.imaginary,
	};
}

struct PsiComplex psi_sub_complex_real(struct PsiComplex a, double b)
{
	return (struct PsiComplex)
	{
		a.real - b,
		a.imaginary,
	};
}

struct PsiComplex psi_mul_complex_real(struct PsiComplex a, double b)
{
	return (struct PsiComplex)
	{
		a.real * b,
		a.imaginary * b,
	};
}

struct PsiComplex psi_div_complex_real(struct PsiComplex a, double b)
{
	return (struct PsiComplex)
	{
		a.real / b,
		a.imaginary / b,
	};
}
