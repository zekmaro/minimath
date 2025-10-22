#ifndef MATRIX_EXPRESSION
#define MATRIX_EXPRESSION

namespace minimath {
	template <typename T>
	class MatExpr {
		public:
			auto derived() const { return static_cast<const T&>(*this); }
			size_t rows() const { return derived().rows(); }
			size_t cols() const { return derived().cols(); }
			auto operator()(size_t i, size_t j) const { return derived()(i, j); }
	};

	template <typename A, typename B>
	class SumMatExpr: public MatExpr<SumMatExpr<A, B>> {
		A a_;
		B b_;
	public:
		SumMatExpr(A a, B b): a_(a), b_(b) {}
		size_t rows() const { return a_.rows(); }
    	size_t cols() const { return a_.cols(); }
    	auto operator()(size_t i, size_t j) const { return a_(i,j) + b_(i,j); }
	};

	template <typename A, typename B>
	auto operator+(const MatExpr<A>& a, const MatExpr<B>& b) {
		assert(a.rows() == b.rows() && a.cols() == b.cols());
		return SumMatExpr(a.derived(), b.derived());
	}

	template <typename A, typename B>
	class SubMatExpr: public MatExpr<SumMatExpr<A, B>> {
		A a_;
		B b_;
	public:
		SubMatExpr(A a, B b): a_(a), b_(b) {}
		size_t rows() const { return a_.rows(); }
    	size_t cols() const { return a_.cols(); }
    	auto operator()(size_t i, size_t j) const { return a_(i,j) - b_(i,j); }
	};

	template <typename A, typename B>
	auto operator-(const MatExpr<A>& a, const MatExpr<B>& b) {
		assert(a.rows() == b.rows() && a.cols() == b.cols());
		return SubMatExpr(a.derived(), b.derived());
	}

	template <typename Scaler, typename A>
	class ScaleMatExpr: public MatExpr<ScaleMatExpr<Scaler, A>> {
		Scaler s_;
		A a_;
	public:
		ScaleMatExpr(Scaler s, A a): s_(s), a_(a) {}
		size_t rows() const { return a_.rows(); }
		size_t cols() const { return a_.cols(); }
    	auto operator()(size_t i, size_t j) const { return s_ * a_(i,j); }
	};

	template <typename Scaler, typename A>
	auto operator*(const Scaler& s, const MatExpr<A>& a) {
		return ScaleMatExpr<Scaler, A>(s, a.derived());
	}

	template <typename A>
	class TransposeMatExpr : public MatExpr<TransposeMatExpr<A>> {
		A a_;
	public:
		TransposeMatExpr(A a) : a_(a) {}
		size_t rows() const { return a_.cols(); }
		size_t cols() const { return a_.rows(); }
		auto operator()(size_t i, size_t j) const { return a_(j, i); }
	};

	template <typename A>
	auto transpose(const MatExpr<A>& a) {
		return TransposeMatExpr(a.derived());
	}

	template <typename A, typename B>
	class MatMulExpr : public MatExpr<MatMulExpr<A,B>> {
		A a_;
		B b_;
	public:
		auto operator()(size_t i, size_t j) const {
			auto sum = decltype(a_(i,0) * b_(0,j))();
			for (size_t k = 0; k < a_.cols(); k++)
				sum += a_(i,k) * b_(k,j);
			return sum;
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const MatExpr<T>& A) {
		size_t rows = A.rows();
    	size_t cols = A.cols();

		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < cols; ++j) {
				os << A(i, j);
			}
			os << "\n";
		}
		return os;
	}
}

#endif