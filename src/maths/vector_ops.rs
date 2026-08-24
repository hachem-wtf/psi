use super::{Float, Matrix};
use crate::{ColumnVector, RowVector, VectorImpl};
use core::ops;

impl<T: Float, const ROWS: usize, const COLS: usize> ops::Add<&VectorImpl<T, ROWS, COLS>>
    for VectorImpl<T, ROWS, COLS>
{
    type Output = Option<VectorImpl<T, ROWS, COLS>>;

    fn add(self, other: &VectorImpl<T, ROWS, COLS>) -> Self::Output {
        self.add_to(other)
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::Sub<&VectorImpl<T, ROWS, COLS>>
    for VectorImpl<T, ROWS, COLS>
{
    type Output = Option<VectorImpl<T, ROWS, COLS>>;

    fn sub(self, other: &VectorImpl<T, ROWS, COLS>) -> Self::Output {
        self.subtract(other)
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::Mul<T> for VectorImpl<T, ROWS, COLS> {
    type Output = VectorImpl<T, ROWS, COLS>;

    fn mul(self, scalar: T) -> Self::Output {
        self.scale(scalar)
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::Div<T> for VectorImpl<T, ROWS, COLS> {
    type Output = VectorImpl<T, ROWS, COLS>;

    fn div(self, scalar: T) -> Self::Output {
        self.scale(T::one() / scalar)
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::AddAssign<VectorImpl<T, ROWS, COLS>>
    for VectorImpl<T, ROWS, COLS>
{
    fn add_assign(&mut self, other: VectorImpl<T, ROWS, COLS>) {
        if let Some(result) = self.add_to(&other) {
            *self = result;
        }
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::SubAssign<VectorImpl<T, ROWS, COLS>>
    for VectorImpl<T, ROWS, COLS>
{
    fn sub_assign(&mut self, other: VectorImpl<T, ROWS, COLS>) {
        if let Some(result) = self.subtract(&other) {
            *self = result;
        }
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::MulAssign<T>
    for VectorImpl<T, ROWS, COLS>
{
    fn mul_assign(&mut self, scalar: T) {
        *self = self.scale(scalar);
    }
}

impl<T: Float, const ROWS: usize, const COLS: usize> ops::DivAssign<T>
    for VectorImpl<T, ROWS, COLS>
{
    fn div_assign(&mut self, scalar: T) {
        *self = self.scale(T::one() / scalar);
    }
}

impl<T: Float> ops::Mul<&Matrix<T>> for RowVector<T> {
    type Output = Option<RowVector<T>>;

    fn mul(self, matrix: &Matrix<T>) -> Self::Output {
        self.mul_matrix(matrix)
    }
}

impl<T: Float> ops::Mul<&Matrix<T>> for ColumnVector<T> {
    type Output = Option<ColumnVector<T>>;

    fn mul(self, matrix: &Matrix<T>) -> Self::Output {
        self.mul_matrix(matrix)
    }
}

impl<T: Float> ops::MulAssign<&Matrix<T>> for RowVector<T> {
    fn mul_assign(&mut self, matrix: &Matrix<T>) {
        if let Some(result) = self.mul_matrix(matrix) {
            *self = result;
        }
    }
}

impl<T: Float> ops::MulAssign<&Matrix<T>> for ColumnVector<T> {
    fn mul_assign(&mut self, matrix: &Matrix<T>) {
        if let Some(result) = self.mul_matrix(matrix) {
            *self = result;
        }
    }
}
