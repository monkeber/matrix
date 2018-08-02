#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace mtx
{

template<typename T>
class Matrix2D
{
	//
	// Nested types.
	//
public:
	//! Represents sizes of different dimensions of matrix.
	struct Dimension
	{
		//! Number of rows in the Matrix.
		size_t rowsNumber_ = { 0 };
		//! Number of columns in the Matrix.
		size_t colsNumber_ = { 0 };
	};

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	Matrix2D() = default;
	//! Constructor.
	Matrix2D(const size_t rows, const size_t columns);
	//! Constructor.
	Matrix2D(const size_t rows, const size_t columns, const T& defVal);
	//! Constructor.
	Matrix2D(const Dimension& matSize);
	//! Constructor.
	Matrix2D(const Dimension& matSize, const T& defVal);
	//! Destructor.
	~Matrix2D() noexcept = default;
	//! Move constructor.
	Matrix2D(Matrix2D&& other) noexcept;
	//! Move assignment operator.
	Matrix2D& operator=(Matrix2D&& other) noexcept;
	//! Copy constructor.
	Matrix2D(const Matrix2D& other);
	//! Copy assignment operator.
	Matrix2D& operator=(const Matrix2D& other);

	//
	// Alias declaration.
	//
public:
	//! Use vector as a result of Slice function.
	using Row = std::vector<T>;

	//
	// Public interface.
	//
public:
	//! Returns an iterator pointing to the first element in the matrix.
	typename Row::iterator Begin() noexcept;
	typename Row::const_iterator Begin() const noexcept;
	//! Returns an iterator pointing to the past-the-end element in the matrix.
	typename Row::iterator End() noexcept;
	typename Row::const_iterator End() const noexcept;
	//! Returns number of columns.
	size_t GetColumns() const;
	//! Returns number of rows.
	size_t GetRows() const;
	//! Returns size structure of the matrix.
	Dimension GetSize() const;
	//! Provides access to the elements.
	T& operator()(const size_t row, const size_t column);
	const T& operator()(const size_t row, const size_t column) const;
	//! Returns the row n.
	Row Slice(const size_t n) const;
	//! Returns slice of the row specified by arguments:
	//! n - a number of row which need to be sliced
	//! and first and last elements in this row.
	Row Slice(const size_t n, const size_t first, const size_t last) const;

	//
	// Private data members.
	//
private:
	//! Size of the matrix.
	Dimension sz_;
	//! Storage for elements of the matrix.
	//! All elements will be stored in a single vector.
	std::vector<T> elems_;
};

template<typename T>
Matrix2D<T>::Matrix2D(const size_t rows, const size_t columns)
	: sz_{ rows, columns }
{
	elems_.resize(sz_.rowsNumber_ * sz_.colsNumber_);
}

template<typename T>
Matrix2D<T>::Matrix2D(const size_t rows, const size_t columns, const T& defVal)
	: sz_{ rows, columns }
{
	elems_.resize(sz_.rowsNumber_ * sz_.colsNumber_, defVal);
}

template<typename T>
Matrix2D<T>::Matrix2D(const Dimension& matSize)
	: sz_{ matSize }
{
	elems_.resize(sz_.rowsNumber_ * sz_.colsNumber_);
}

template<typename T>
Matrix2D<T>::Matrix2D(const Dimension& matSize, const T& defVal)
	: sz_{ matSize }
{
	elems_.resize(sz_.rowsNumber_ * sz_.colsNumber_, defVal);
}

template<typename T>
Matrix2D<T>::Matrix2D(const Matrix2D<T>& other)
	: sz_{ other.sz_.rowsNumber_, other.sz_.colsNumber_ }
{
	elems_ = other.elems_;
}

template<typename T>
Matrix2D<T>::Matrix2D(Matrix2D&& other) noexcept
	: sz_{ other.sz_.rowsNumber_, other.sz_.colsNumber_ }
{
	std::swap(elems_, other.elems_);
}

template<typename T>
Matrix2D<T>& Matrix2D<T>::operator=(const Matrix2D& other)
{
	if (this == &other) {
		return *this;
	}

	sz_ = other.sz_;
	elems_ = other.elems_;

	return *this;
}

template<typename T>
Matrix2D<T>& Matrix2D<T>::operator=(Matrix2D&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	std::swap(sz_, other.sz_);
	std::swap(elems_, other.elems_);

	return *this;
}

template<typename T>
typename Matrix2D<T>::Row::iterator Matrix2D<T>::Begin() noexcept
{
	return elems_.begin();
}

template<typename T>
typename Matrix2D<T>::Row::const_iterator Matrix2D<T>::Begin() const noexcept
{
	return elems_.begin();
}

template<typename T>
typename Matrix2D<T>::Row::iterator Matrix2D<T>::End() noexcept
{
	return elems_.end();
}

template<typename T>
typename Matrix2D<T>::Row::const_iterator Matrix2D<T>::End() const noexcept
{
	return elems_.end();
}

template<typename T>
size_t Matrix2D<T>::GetColumns() const
{
	return sz_.colsNumber_;
}

template<typename T>
size_t Matrix2D<T>::GetRows() const
{
	return sz_.rowsNumber_;
}

template<typename T>
typename Matrix2D<T>::Dimension Matrix2D<T>::GetSize() const
{
	return sz_;
}

template<typename T>
T& Matrix2D<T>::operator()(const size_t row, const size_t column)
{
	return elems_[row * sz_.colsNumber_ + column];
}

template<typename T>
const T& Matrix2D<T>::operator()(const size_t row, const size_t column) const
{
	return elems_[row * sz_.colsNumber_ + column];
}

template<typename T>
typename Matrix2D<T>::Row Matrix2D<T>::Slice(const size_t n) const
{
	return Slice(n, 0, sz_.colsNumber_);
}

template<typename T>
typename Matrix2D<T>::Row Matrix2D<T>::Slice(
	const size_t n,
	const size_t first,
	const size_t last) const
{
	bool outOfRange =
		n < 0
		|| n >= sz_.rowsNumber_
		|| first < 0
		|| first > sz_.colsNumber_
		|| last < 0
		|| last > sz_.colsNumber_;

	if (outOfRange) {
		throw std::out_of_range("Matrix2D::Slice: arguments are out of range.");
	}

	Row tempRow = Row(last - first);
	auto startPtr = elems_.begin() + n * sz_.colsNumber_ + first;
	auto endPtr = elems_.begin() + n * sz_.colsNumber_ + last;
	std::copy(startPtr, endPtr, tempRow.begin());

	return tempRow;
}

//
// Utility functions.
//

//! Takes a matrix and fills("in place") it with elements generated from specified generator
//! and distribution.
//! Function does not modify explicitly generator and distribution, but requires these objects
//! to be non-const.
template<typename T, typename G, typename D>
void RandomizeElements(Matrix2D<T>& mat, G& generator, D& distribution)
{
	for (size_t r = 0; r < mat.GetRows(); ++r)
	{
		for (size_t c = 0; c < mat.GetColumns(); ++c) {
			mat(r, c) = distribution(generator);
		}
	}
}

//! Takes two arguments as sizes: number of rows and columns respectively.
//! Returns Matrix2D object with specified size, matrix has randomized elements.
//! Function works only with integral and floating point types, for allowed template
//! parameters see std::is_floating_point and std::is_integral EXCEPT bool type,
//! there is no support for bool.
//! Checks type whether or not it is allowed at compile time.
template<typename T>
Matrix2D<typename std::enable_if_t<std::is_floating_point<T>::value, T>>
CreateRandomMatrix2D(const size_t rows, const size_t columns)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<T> distribution(static_cast<T>(0), static_cast<T>(100));

	mtx::Matrix2D<T> tempMatrix{ rows, columns };
	RandomizeElements(tempMatrix, generator, distribution);

	return tempMatrix;
}

template<typename T>
Matrix2D<typename std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>>
CreateRandomMatrix2D(const size_t rows, const size_t columns)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<T> distribution(static_cast<T>(0), static_cast<T>(100));

	mtx::Matrix2D<T> tempMatrix{ rows, columns };
	RandomizeElements(tempMatrix, generator, distribution);

	return tempMatrix;
}

//! Prints elements of the specified matrix.
template<typename T>
void PrintMatrix2D(const Matrix2D<T>& mat)
{
	const size_t rows = mat.GetRows();
	const size_t columns = mat.GetColumns();
	std::cout << "{\n";
	for (size_t r = 0; r < rows; ++r)
	{
		for (size_t c = 0; c < columns; ++c) {
			std::cout << mat(r, c) << ", ";
		}
		std::cout << '\n';
	}
	std::cout << "}\n";
}

//! Provides "equal to" operator for matrix.
template<typename T>
bool operator==(const Matrix2D<T>& left, const Matrix2D<T>& right)
{
	if (&left == &right)
	{
		return true;
	}
	else if (left.GetRows() != right.GetRows() || left.GetColumns() != right.GetColumns())
	{
		return false;
	}
	
	return std::equal(left.Begin(), left.End(), right.Begin());
}

//! Returns matrix where elements are results of
//! resultMat(r, c) = func(left(r, c), right(r, c)).
template<typename T, typename BinaryOp>
Matrix2D<T> TransformMatrices(const Matrix2D<T>& left, const Matrix2D<T>& right, BinaryOp func)
{
	if (left.GetRows() != right.GetRows() || left.GetColumns() != right.GetColumns()) {
		throw std::length_error("TransformMatrices: sizes of matrices don't match.");
	}

	Matrix2D<T> resultMat(left.GetSize());
	std::transform(left.Begin(), left.End(), right.Begin(), resultMat.Begin(), func);

	return resultMat;
}

//! Returns the sum of two matrices.
template<typename T>
Matrix2D<T> operator+(const Matrix2D<T>& left, const Matrix2D<T>& right)
{
	return TransformMatrices(left, right, std::plus<T>());
}

//! Returns the difference of two matrices.
template<typename T>
Matrix2D<T> operator-(const Matrix2D<T>& left, const Matrix2D<T>& right)
{
	return TransformMatrices(left, right, std::minus<T>());
}

}	// namespace mtx