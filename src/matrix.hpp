#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cstddef>

#include "vector.hpp"

enum ORDERING { ROW_MAJOR, COL_MAJOR };

namespace minimath {

	template <typename T, template Order>
	class MatrixView {
		size_t	m_rows_;
		size_t	m_cols_;
		size_t	m_dist_;
		T*		m_data_;
		public:
			MatrixView(size_t m_rows, size_t m_cols, size_t m_dist, T* m_data)
				: m_rows_(m_rows), m_cols_(m_cols), m_dist_(m_dist), m_data_(m_data) {}
			
			size_t getRows() const { return m_rows; }
			size_t getCols() const { return m_cols; }
			size_t getDist() const { return m_dist; }

			T& operator()(size_t i, size_t j) {
				size_t index;

				if constexpr (Order == ROW_MAJOR) {
					index = i * m_rows_ + j;
				} else {
					index = j * m_rows_ + i;
				}
				if (index >= m_dist * (m_rows - 1) + m_cols)
    				throw std::out_of_range("MatrixView: access out of bounds");

				return m_data_[index];
			}

			const T& operator()(size_t i, size_t j) const {
				size_t index;

				if constexpr (Order == ROW_MAJOR) {
					index = i * m_rows_ + j;
				} else {
					index = j * m_rows_ + i;
				}
				if (index >= m_dist * (m_rows - 1) + m_cols)
    				throw std::out_of_range("MatrixView: access out of bounds");

				return m_data_[index];
			}

			void print() const {
				for (size_t i = 0; i < m_rows; ++i) {
					for (size_t j = 0; j < m_cols; ++j)
						std::cout << (*this)(i, j) << " ";
					std::cout << "\n";
				}
			}
	}


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
	
			class MatrixException : public std::runtime_error {
				public:
					MatrixException(const std::string& msg)
						: std::runtime_error(msg) {}
			};
	
			Matrix& operator=( const Matrix& other ) {
				if (this != &other) {
					if (rows_ != other.rows_ || cols_ != other.cols_) {
						delete [] data_;
						rows_ = other.rows_;
						cols_ = other.cols_;
						data_ = new T[rows_ * cols_];
					}
					std::copy(other.data_, other.data_ + rows_ * cols_, data_);
				}
				return *this;
			}
	
			Matrix& operator=( Matrix&& other ) {
				std::swap(rows_, other.rows_);
				std::swap(cols_, other.cols_);
				std::swap(data_, other.data_);
				return *this;
			}
	
	
			size_t getRows( void ) {
				return rows_;
			}
	
			size_t getCols( void ) {
				return cols_;
			}
	
			T& operator()(size_t i, size_t j) {
				if (i >= rows_ || j >= cols_)
					throw MatrixException("Matrix index out of range");
				return data_[cols_ * i + j];
			}
	
			const T& operator()(size_t i, size_t j) const {
				if (i >= rows_ || j >= cols_)
					throw MatrixException("Matrix index out of range");
				return data_[cols_ * i + j];
			}

			Vector<T>& operator*(const Vector<T>& vec) {
				if (cols_ != vec.size) {
					throw MatrixException("Incorrect dimentions!")
				}
				
			}
	};

}

#endif
