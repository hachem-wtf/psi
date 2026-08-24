use crate::Complex;
use core::ops;

macro_rules! impl_numeric {
    ($($t:ty),*) => {
        $(
            impl Numeric for $t {
                fn zero() -> Self {
                    0 as $t
                }

                fn one() -> Self {
                    1 as $t
                }
            }
        )*
    };
}

macro_rules! impl_cnumeric {
    ($($t:ty),*) => {
        $(impl Numeric for Complex<$t> {
            fn zero() -> Self { Complex::new(0.0, 0.0) }
            fn one() -> Self { Complex::new(1.0, 0.0) }
        })*
    };
}

macro_rules! impl_float {
    ($($t:ty, $sqrt_fn:path, $atan2_fn:path),*) => {
        $(
            impl Float for $t {
                fn sqrt(self) -> Self {
                    $sqrt_fn(self)
                }

                fn atan2(y: Self, x: Self) -> Self {
                    $atan2_fn(y, x)
                }
            }
        )*
    };
}

macro_rules! impl_cfloat {
    ($($t:ty, $sqrt_fn:path, $atan2_fn:path, $cos_fn:path, $sin_fn:path),*) => {
        $(
            impl Float for Complex<$t> {
                fn sqrt(self) -> Self {
                    let r = self.abs();
                    let theta = self.phase();

                    let sqrt_r = $sqrt_fn(r);
                    let sqrt_theta = theta / 2.0;

                    Complex::new(
                        sqrt_r * $cos_fn(sqrt_theta),
                        sqrt_r * $sin_fn(sqrt_theta),
                    )
                }

                fn atan2(y: Self, x: Self) -> Self {
                    Complex::new(
                        $atan2_fn(y.real, x.real),
                        $atan2_fn(y.imaginary, x.imaginary),
                    )
                }
            }
        )*
    };
}

pub trait Numeric:
    Copy
    + PartialOrd
    + ops::Add<Output = Self>
    + ops::Mul<Output = Self>
    + ops::Sub<Output = Self>
    + ops::Div<Output = Self>
    + ops::Neg<Output = Self>
    + ops::AddAssign
    + ops::SubAssign
    + ops::MulAssign
    + ops::DivAssign
{
    fn zero() -> Self;
    fn one() -> Self;
}

impl_numeric!(i32, i64, f32, f64);
impl_cnumeric!(f32, f64);
impl_float!(f32, libm::sqrtf, libm::atan2f);
impl_float!(f64, libm::sqrt, libm::atan2);
impl_cfloat!(f32, libm::sqrtf, libm::atan2f, libm::cosf, libm::sinf);
impl_cfloat!(f64, libm::sqrt, libm::atan2, libm::cos, libm::sin);

pub trait Integer: Numeric {}
pub trait Float: Numeric {
    fn sqrt(self) -> Self;
    fn atan2(y: Self, x: Self) -> Self;
}
