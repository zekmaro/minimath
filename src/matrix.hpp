#ifndef FILE_VECTOR
#define FILE_VECTOR

#include <cstddef>
#include <iostream>

template <typename T>
class Matrix {
	size_t	rows_;
	size_t	cols_;
	T*		data_;

	public:
		Matrix( void );

		Matrix( size_t rows, size_t cols )
			: rows_(rows), cols_(cols), data_(new T[rows * cols]) { }

		Matrix( const Matrix& other )
			: Matrix( other.rows_, other.cols_ ) {
				std::copy(other.data_, other.data_ + rows_ * cols_, data_);
			}

		Matrix( Matrix&& other )
			: rows_(0), cols_(0), data_(nullptr) {
				std::swap(rows_, other.rows_);
				std::swap(cols_, other.cols_);
				std::swap(data_, other.data_);
			}

		~Matrix( void ) { delete [] data_; }

		Matrix& operator=( const Matrix& other ) {

		}

		size_t getRows( void ) {
			return rows_;
		}

		size_t getCols( void ) {
			return cols_;
		}

		T& operator()(size_t i, size_t j) {
			if (i >= rows_ || j >= cols_)
    			throw std::out_of_range("Matrix index out of range");
			return data_[cols_ * i + j];
		}

		const T& operator()(size_t i, size_t j) const {
			if (i >= rows_ || j >= cols_)
    			throw std::out_of_range("Matrix index out of range");
			return data_[cols_ * i + j];
		}

};

#endif
