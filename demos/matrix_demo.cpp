#include <iostream>

#include <Vector.hpp>
#include <Matrix.hpp>

using namespace minimath;

int main() {
	Matrix<double, ROW_MAJOR> A(3, 3);

	A(0, 0) = 1.0;
	std::cout << A(0, 0);
	
	
}