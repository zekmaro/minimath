#include <sstream>
#include <pybind11/pybind11.h>

#include "Vector.hpp"
#include "Matrix.hpp"

using namespace minimath;
namespace py = pybind11;




PYBIND11_MODULE(bla, m) {
    m.doc() = "Basic linear algebra module"; // optional module docstring
    
    py::class_<Vector<double>> (m, "Vector")
      .def(py::init<size_t>(),
           py::arg("size"), "create vector of given size")
      .def("__len__", &Vector<double>::size,
           "return size of vector")
      
      .def("__setitem__", [](Vector<double> & self, int i, double v) {
        if (i < 0) i += self.size();
        if (i < 0 || i >= self.size()) throw py::index_error("vector index out of range");
        self(i) = v;
      })
      .def("__getitem__", [](Vector<double> & self, int i) { return self(i); })
      
      .def("__setitem__", [](Vector<double> & self, py::slice inds, double val)
      {
        size_t start, stop, step, n;
        if (!inds.compute(self.size(), &start, &stop, &step, &n))
          throw py::error_already_set();
        self.range(start, stop).slice(0,step) = val;
      })
      
      .def("__add__", [](Vector<double> & self, Vector<double> & other)
      { return Vector<double> (self+other); })

      .def("__rmul__", [](Vector<double> & self, double scal)
      { return Vector<double> (scal*self); })
      
      .def("__str__", [](const Vector<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Vector<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.size(),
                                py::bytes((char*)(void*)&self(0), self.size()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 2)
            throw std::runtime_error("should be a 2-tuple!");

          Vector<double> v(t[0].cast<size_t>());
          py::bytes mem = t[1].cast<py::bytes>();
          std::memcpy(&v(0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.size()*sizeof(double));
          return v;
        }))
    ;

    py::class_<Matrix<double, ROW_MAJOR>> (m, "Matrix")
        .def(py::init<size_t, size_t>(),
          py::arg("rows"), py::arg("cols"),
          "create matrix with given number of rows and columns")

        .def("__mul__", [](const Matrix<double, ROW_MAJOR>& A, const Matrix<double, ROW_MAJOR>& B) {
          return Matrix<double, ROW_MAJOR>(A * B);
        },
        "Matrices multiplication")

        .def("__rmul__", [](double scalar, const Matrix<double, ROW_MAJOR> &A) {
          return Matrix<double, ROW_MAJOR>(scalar * A);
        },
        "Scalar times Matrix")

        .def("__str__", [](const Matrix<double, ROW_MAJOR> &A) {
          std::stringstream s;
          s << A;
          return s.str();
        })

        .def("__getitem__",
          [](Matrix<double, ROW_MAJOR> self, std::tuple<int, int> ind) {
              return self(std::get<0>(ind), std::get<1>(ind));
          })

        .def_property_readonly("shape",
          [](const Matrix<double, ROW_MAJOR>& self) {
              return std::tuple(self.rows(), self.cols());
          })
    
    ;
}
