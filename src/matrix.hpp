#ifndef FILE_MATRIX
#define FILE_MATRIX

#include <iostream>
#include <cstddef>

#include "vector.hpp"

enum ORDERING { ROW_MAJOR, COL_MAJOR };

namespace minimath {

	template <typename T, template Order>
	class MatrixView {
		protected:
			size_t	m_rows_;
			size_t	m_cols_;
			size_t	m_dist_;
			T*		m_data_;

		public:

			// m_dist should be checked by construction
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


	template <typename T, template Order>
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
					std::copy(other.m_data_, other.m_data_ + other.m_rows_ * other.m_cols_, m_data_);
				}
	
			Matrix(Matrix&& other) noexcept
				: MatrixView<T, Order>(
					other.m_rows,
					other.m_cols,
					other.m_dist,
					other.m_data
				) {
					other.m_data = nullptr;
				}
	
			~Matrix( void ) { delete [] m_data_; }
	
			class MatrixException : public std::runtime_error {
				public:
					MatrixException(const std::string& msg)
						: std::runtime_error(msg) {}
			};
	
			Matrix& operator=( const Matrix& other ) {
				if (this != &other) {
					if (rows_ != other.rows_ || cols_ != other.cols_) {
						delete [] m_data_;
						m_rows_ = other.rows_;
						m_cols_ = other.cols_;
						m_dist_ = other.m_dist_;
						m_data_ = new T[rows_ * cols_];
					}
					std::copy(other.data_, other.data_ + m_rows_ * m_cols_, m_data_);
				}
				return *this;
			}
	
			Matrix& operator=( Matrix&& other ) noexcept {
				std::swap(m_rows_, other.m_rows_);
				std::swap(m_cols_, other.m_cols_);
				std::swap(m_dist_, other.m_dist_)
				std::swap(m_data_, other.m_data_);
				return *this;
			}
	
			size_t getRows( void ) {
				return rows_;
			}
	
			size_t getCols( void ) {
				return cols_;
			}
	
			T& operator()(size_t i, size_t j) {
				if (i >= m_rows || j >= m_cols) {
					throw MatrixException("Matrix index out of range");
				}
				if constexpr (Order == ROW_MAJOR) {
					return m_data[i * m_dist + j];
				}
				else {
					return m_data[j * m_dist + i];
				}
			}
	
			const T& operator()(size_t i, size_t j) {
				if (i >= m_rows || j >= m_cols) {
					throw MatrixException("Matrix index out of range");
				}
				if constexpr (Order == ROW_MAJOR) {
					return m_data[i * m_dist + j];
				}
				else {
					return m_data[j * m_dist + i];
				}
			}

			Vector<T>& operator*(const Vector<T>& vec) {
				if (cols_ != vec.size) {
					throw MatrixException("Incorrect dimentions!")
				}
				
			}
	};

}

#endif
