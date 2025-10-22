#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cstddef>

#include "MatrixExpr.hpp"


namespace minimath {

	enum ORDERING { ROW_MAJOR, COL_MAJOR };

	template <typename T, ORDERING Order>
	class MatrixView : public MatExpr<MatrixView<T, Order>> {
		protected:
			size_t	m_rows_;
			size_t	m_cols_;
			size_t	m_dist_;
			T*		m_data_;

		public:

			// m_dist should be checked by construction
			MatrixView(size_t m_rows, size_t m_cols, size_t m_dist, T* m_data)
				: m_rows_(m_rows), m_cols_(m_cols), m_dist_(m_dist), m_data_(m_data) {}
			
			size_t getRows() const { return m_rows_; }
			size_t getCols() const { return m_cols_; }
			size_t getDist() const { return m_dist_; }

			T& operator()(size_t i, size_t j) {
				size_t index;

				if constexpr (Order == ROW_MAJOR) {
					index = i * m_rows_ + j;
				} else {
					index = j * m_rows_ + i;
				}
				if (index >= m_dist_ * (m_rows_ - 1) + m_cols_)
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
				if (index >= m_dist_ * (m_rows_ - 1) + m_cols_)
    				throw std::out_of_range("MatrixView: access out of bounds");

				return m_data_[index];
			}

			void print() const {
				for (size_t i = 0; i < m_rows_; ++i) {
					for (size_t j = 0; j < m_cols_; ++j)
						std::cout << (*this)(i, j) << " ";
					std::cout << "\n";
				}
			}
	};


	template <typename T, ORDERING Order>
	class Matrix: public MatrixView<T, Order> {
		public:
			Matrix( void );
	
			Matrix( size_t rows, size_t cols )
				: MatrixView<T, Order>(
					rows,
					cols,
					(Order == ROW_MAJOR ? cols : rows),
					new T[rows * cols]
				) {}
	
			Matrix(const Matrix& other)
				: MatrixView<T, Order>(
					other.m_rows_,
					other.m_cols_,
					other.m_dist_,
					new T[other.m_rows_ * other.m_cols_]
				) {
					std::copy(other.m_data_, other.m_data_ + other.m_rows_ * other.m_cols_, this->m_data_);
				}
	
			Matrix(Matrix&& other) noexcept
				: MatrixView<T, Order>(
					other.m_rows_,
					other.m_cols_,
					other.m_dist_,
					other.m_data_
				) {
					other.m_data_ = nullptr;
				}
	
			~Matrix( void ) { delete [] this->m_data_; }
	
			class MatrixException : public std::runtime_error {
				public:
					MatrixException(const std::string& msg)
						: std::runtime_error(msg) {}
			};
	
			Matrix& operator=( const Matrix& other ) {
				if (this != &other) {
					if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
						delete [] this->m_data_;
						this->m_rows_ = other.rows_;
						this->m_cols_ = other.cols_;
						this->m_dist_ = other.m_dist_;
						this->m_data_ = new T[this->rows_ * this->cols_];
					}
					std::copy(other.data_, other.data_ + this->m_rows_ * this->m_cols_, this->m_data_);
				}
				return *this;
			}
	
			Matrix& operator=( Matrix&& other ) noexcept {
				std::swap(this->m_rows_, other.m_rows_);
				std::swap(this->m_cols_, other.m_cols_);
				std::swap(this->m_dist_, other.m_dist_);
				std::swap(this->m_data_, other.m_data_);
				return *this;
			}
	
			size_t getRows( void ) {
				return this->m_rows_;
			}
	
			size_t getCols( void ) {
				return this->m_cols_;
			}
	
			T& operator()(size_t i, size_t j) {
				if (i >= this->m_rows_ || j >= this->m_cols_) {
					throw MatrixException("Matrix index out of range");
				}
				if constexpr (Order == ROW_MAJOR) {
					return this->m_data_[i * this->m_dist_ + j];
				}
				else {
					return this->m_data_[j * this->m_dist_ + i];
				}
			}
	
			const T& operator()(size_t i, size_t j) const {
				if (i >= this->m_rows_ || j >= this->m_cols_) {
					throw MatrixException("Matrix index out of range");
				}
				if constexpr (Order == ROW_MAJOR) {
					return this->m_data_[i * this->m_dist_ + j];
				}
				else {
					return this->m_data_[j * this->m_dist_ + i];
				}
			}

			Vector<T>& operator*(const Vector<T>& vec) {
				if (this->m_cols_ != vec.size) {
					throw MatrixException("Incorrect dimentions!");
				}
				
			}

			// Sum with Matrix
			// Multplication with Matrix
			// Inverse
			// Transpose
	};

}

#endif
