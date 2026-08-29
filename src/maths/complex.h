#pragma once

struct PsiComplex
{
	double real;
	double imaginary;
};

struct PsiComplex psi_new_complex(double real, double imaginary);
struct PsiComplex psi_new_complex_from_real(double real);

struct PsiComplex psi_conjugate_complex(struct PsiComplex z);
struct PsiComplex psi_neg_complex(struct PsiComplex z);

double psi_phase_complex(struct PsiComplex z);
double psi_norm2_complex(struct PsiComplex z);
double psi_abs_complex(struct PsiComplex z);
struct PsiComplex psi_sqrt_complex(struct PsiComplex z);

struct PsiComplex psi_add_complex(struct PsiComplex a, struct PsiComplex b);
struct PsiComplex psi_sub_complex(struct PsiComplex a, struct PsiComplex b);
struct PsiComplex psi_mul_complex(struct PsiComplex a, struct PsiComplex b);
struct PsiComplex psi_div_complex(struct PsiComplex a, struct PsiComplex b);

struct PsiComplex psi_add_complex_real(struct PsiComplex a, double b);
struct PsiComplex psi_sub_complex_real(struct PsiComplex a, double b);
struct PsiComplex psi_mul_complex_real(struct PsiComplex a, double b);
struct PsiComplex psi_div_complex_real(struct PsiComplex a, double b);
