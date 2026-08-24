use super::Float;
use core::{fmt, ops};

#[macro_export]
macro_rules! matrix {
    ( $( $( $x:expr ),* );* ) => {{
        let mut data = Vec::new();
        let mut rows = 0;
        let mut cols = 0;

        $(
            let row_data = $( $x )*;
            if cols == 0 {
                cols = row_data.len();
            }
            assert_eq!(cols, row_data.len(), "All rows must have the same number of columns.");
            data.extend(row_data);
            rows += 1;
        )*

        $crate::Matrix::new(rows, cols, data)
    }};
}

macro_rules! impl_matrix_ops {
    ($($trait:ident, $method:ident, $other:ty, $output:ty, $scale_fn:ident),* $(,)?) => {
        $(
            impl<T: Float> core::ops::$trait<$other> for Matrix<T> {
                type Output = $output;

                fn $method(self, other: $other) -> Self::Output {
                    self.$scale_fn(other)
                }
            }
        )*
    };
    ($($trait:ident, $method:ident, $other:ty, $scale_fn:ident),* $(,)?) => {
        $(
            impl<T: Float> core::ops::$trait<$other> for Matrix<T> {
                fn $method(&mut self, other: $other) {
                    *self = self.$scale_fn(other);
                }
            }
        )*
    };
}

#[derive(Clone)]
pub struct Matrix<T: Float> {
    pub data: Vec<T>,
    pub rows: usize,
    pub cols: usize,
}

impl<T: Float> Matrix<T> {
    pub fn new(rows: usize, cols: usize, data: Vec<T>) -> Self {
        Matrix { data, rows, cols }
    }

    pub fn get(&self, row: usize, col: usize) -> T {
        self.data[row * self.cols + col]
    }

    pub fn set(&mut self, row: usize, col: usize, value: T) {
        self.data[row * self.cols + col] = value;
    }

    pub fn dot(&self, other: &Self) -> Option<Matrix<T>> {
        if self.cols != other.rows {
            return None;
        }

        let mut result = Matrix::new(
            self.rows,
            other.cols,
            vec![T::zero(); self.rows * other.cols],
        );
        for i in 0..self.rows {
            for j in 0..other.cols {
                let mut sum = T::zero();
                for k in 0..self.cols {
                    sum += self.get(i, k) * other.get(k, j) ;
                }
                result.set(i, j, sum);
            }
        }
        Some(result)
    }

    pub fn kronecker(&self, other: &Self) -> Matrix<T> {
        let new_rows = self.rows * other.rows;
        let new_cols = self.cols * other.cols;

        let mut result = Matrix::new(new_rows, new_cols, vec![T::zero(); new_rows * new_cols]);

        for i in 0..self.rows {
            for j in 0..self.cols {
                let self_val = self.get(i, j);
                for k in 0..other.rows {
                    for l in 0..other.cols {
                        let result_row = i * other.rows + k;
                        let result_col = j * other.cols + l;
                        result.set(result_row, result_col, self_val * other.get(k, l));
                    }
                }
            }
        }

        result
    }

    pub fn transpose(&self) -> Matrix<T> {
        let mut result = Matrix::new(self.cols, self.rows, vec![T::zero(); self.cols * self.rows]);

        for i in 0..self.rows {
            for j in 0..self.cols {
                let value = self.get(i, j);
                result.set(j, i, value);
            }
        }

        result
    }

    pub fn add_to(&self, other: &Self) -> Option<Matrix<T>> {
        if self.rows != other.rows || self.cols != other.cols {
            return None;
        }

        let mut result = Matrix::new(self.rows, self.cols, vec![T::zero(); self.rows * self.cols]);

        for i in 0..self.rows {
            for j in 0..self.cols {
                let sum = self.get(i, j) + other.get(i, j);
                result.set(i, j, sum);
            }
        }
        Some(result)
    }

    pub fn subtract(&self, other: &Self) -> Option<Matrix<T>> {
        if self.rows != other.rows || self.cols != other.cols {
            return None;
        }

        let mut result = Matrix::new(self.rows, self.cols, vec![T::zero(); self.rows * self.cols]);

        for i in 0..self.rows {
            for j in 0..self.cols {
                let diff = self.get(i, j) - other.get(i, j);
                result.set(i, j, diff);
            }
        }
        Some(result)
    }

    pub fn scale(&self, scalar: T) -> Matrix<T> {
        let mut result = Matrix::new(self.rows, self.cols, vec![T::zero(); self.rows * self.cols]);

        for i in 0..self.rows {
            for j in 0..self.cols {
                let scaled_value = self.get(i, j) * scalar;
                result.set(i, j, scaled_value);
            }
        }
        result
    }
}

impl<T: Float> ops::Index<(usize, usize)> for Matrix<T> {
    type Output = T;

    fn index(&self, index: (usize, usize)) -> &Self::Output {
        &self.data[index.0 * self.cols + index.1]
    }
}

impl<T: Float> ops::IndexMut<(usize, usize)> for Matrix<T> {
    fn index_mut(&mut self, index: (usize, usize)) -> &mut Self::Output {
        &mut self.data[index.0 * self.cols + index.1]
    }
}

impl<T: Float> ops::AddAssign<&Matrix<T>> for Matrix<T> {
    fn add_assign(&mut self, other: &Matrix<T>) {
        if let Some(result) = self.add_to(other) {
            *self = result;
        }
    }
}

impl<T: Float> ops::SubAssign<&Matrix<T>> for Matrix<T> {
    fn sub_assign(&mut self, other: &Matrix<T>) {
        if let Some(result) = self.subtract(other) {
            *self = result;
        }
    }
}

impl_matrix_ops! {
    Add, add, &Matrix<T>, Option<Matrix<T>>, add_to,
    Sub, sub, &Matrix<T>, Option<Matrix<T>>, subtract,
    Mul, mul, T, Matrix<T>, scale,
    Div, div, T, Matrix<T>, scale,
}

impl_matrix_ops! {
    MulAssign, mul_assign, T, scale,
    DivAssign, div_assign, T, scale,
}

impl<T: Float + fmt::Debug> fmt::Debug for Matrix<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for i in 0..self.rows {
            for j in 0..self.cols {
                write!(f, "{:?} ", self.get(i, j))?;
            }
            writeln!(f)?;
        }
        Ok(())
    }
}

impl<T: Float + fmt::Display> fmt::Display for Matrix<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let elements: Vec<String> = self.data.iter().map(ToString::to_string).collect();
        let is_complex = elements.iter().any(|element| element.contains("i"));

        let normalized: Vec<(f64, f64)> = self
            .data
            .iter()
            .map(|element| {
                let element_string = element.to_string();

                if is_complex {
                    let element_string = element_string.trim_end_matches('i').trim();
                    let element_split: Vec<&str> = element_string.split_whitespace().collect();
                    let real = element_split[0].parse::<f64>().unwrap();
                    let imaginary = element_split
                        .get(2)
                        .map_or(0.0, |&s| s.parse::<f64>().unwrap());
                    (real, imaginary)
                } else {
                    (element_string.parse::<f64>().unwrap(), 0.0)
                }
            })
            .collect();

        let max_widths = normalized
            .iter()
            .fold((0, 0), |(max_0, max_1), &(real, imag)| {
                let new_max_0 = max_0.max(format!("{:.2}", real).len());
                let new_max_1 = if is_complex {
                    max_1.max(format!("{:.2}", imag.abs()).len())
                } else {
                    max_1
                };
                (new_max_0, new_max_1)
            });

        let aligned: Vec<String> = normalized
            .iter()
            .map(|&(real, imag)| {
                if is_complex {
                    format!(
                        "{:>rewidth$.2} {} {:>imwidth$.2}i",
                        real,
                        if imag > 0.0 { "+" } else { "-" },
                        imag.abs(),
                        rewidth = max_widths.0,
                        imwidth = max_widths.1,
                    )
                } else {
                    format!("{:>width$.2}", real, width = max_widths.0)
                }
            })
            .collect();

        for i in 0..self.rows {
            if i == 0 {
                write!(f, "┌")?;
            } else if i == self.rows - 1 {
                write!(f, "└")?;
            } else {
                write!(f, "│")?;
            }

            for j in 0..self.cols {
                write!(f, "{}", aligned[i + j * self.rows])?;
                if j != self.cols - 1 {
                    write!(f, ", ")?;
                }
            }

            if i == 0 {
                write!(f, "┐")?;
            } else if i == self.rows - 1 {
                write!(f, "┘")?;
            } else {
                write!(f, "│")?;
            }

            if i != self.rows - 1 {
                writeln!(f)?;
            }
        }

        Ok(())
    }
}
