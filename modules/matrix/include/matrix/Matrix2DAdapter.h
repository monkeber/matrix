#pragma once

#include "matrix/Matrix.h"

#include <cmath>
#include <functional>
#include <memory>
#include <numeric>
#include <vector>

namespace mtx
{

template<typename T>
class Matrix2DAdapter
	: private Matrix2D<T>
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	Matrix2DAdapter() = default;
	//! Constructor.
	Matrix2DAdapter(std::shared_ptr<Matrix2D<T>> dataMatrix);
	//! Destructor.
	~Matrix2DAdapter() noexcept = default;
	//! Move constructor.
	Matrix2DAdapter(Matrix2DAdapter&& other) noexcept;
	//! Move assignment operator.
	Matrix2DAdapter& operator=(Matrix2DAdapter&& other) noexcept;
	//! Copy constructor.
	Matrix2DAdapter(const Matrix2DAdapter& other);
	//! Copy assignment operator.
	Matrix2DAdapter& operator=(const Matrix2DAdapter& other);

	//
	// Public interface.
	//
public:
	//! Returns number of local minimums in the matrix.
	int CountLocalMinimums() const;
	//! Applies cyclic shift to the matrix.
	void CyclicShift(const size_t step = 1);
	//! Returns number of row with longest set of identical elements.
	//! Returns -1 if matrix is empty.
	int LongestIdenticalSet() const;
	//! Calculates multiplication of elements in rows with all non negative elements.
	T NonNegativeRowsMultiplication() const;
	//! Calculates sum of elements situated over the main diagonal.
	T SumOverMainDiagonal() const;

	//
	// Private methods.
	//
private:
	//! Checks if the specified with row and column element is local minimum of the matrix.
	bool CheckNeighbors(const size_t row, const size_t column) const;
	//! Returns vector containing all elements situated n indexes from the edge.
	//! Function doesn't check whether n is allowed number, you should
	//! check it yourself (floor(smallerDimension/2)).
	std::vector<T> GetCycle(const size_t n) const;
	//! Replaces cycle n with given vector elements.
	//! Doesn't check any of parameters.
	void SetCycle(const size_t n, std::vector<T> elements);

	//
	// Private data members.
	//
private:
	//! Pointer to Matrix object for processing.
	std::shared_ptr<Matrix2D<T>> matPtr_;
};

template<typename T>
Matrix2DAdapter<T>::Matrix2DAdapter(std::shared_ptr<Matrix2D<T>> dataMatrix)
	: matPtr_{ std::move(dataMatrix) }
{
}

template<typename T>
Matrix2DAdapter<T>::Matrix2DAdapter(Matrix2DAdapter<T>&& other) noexcept
{
	std::swap(matPtr_, other.matPtr_);
}

template<typename T>
Matrix2DAdapter<T>& Matrix2DAdapter<T>::operator=(Matrix2DAdapter<T>&& other) noexcept
{
	if (this == &other) {
		return *this;
	}

	std::swap(matPtr_, other.matPtr_);

	return *this;
}

template<typename T>
Matrix2DAdapter<T>::Matrix2DAdapter(const Matrix2DAdapter<T>& other)
	: matPtr_{ other.matPtr_ }
{
}

template<typename T>
Matrix2DAdapter<T>& Matrix2DAdapter<T>::operator=(const Matrix2DAdapter<T>& other)
{
	if (this == &other) {
		return *this;
	}

	matPtr_ = other.matPtr_;

	return *this;
}

template<typename T>
int Matrix2DAdapter<T>::CountLocalMinimums() const
{
	int counter = 0;
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();

	for (size_t r = 0; r < rows; ++r)
	{
		for (size_t c = 0; c < columns; ++c)
		{
			if (CheckNeighbors(r, c)) {
				++counter;
			}
		}
	}

	return counter;
}

template<typename T>
void Matrix2DAdapter<T>::CyclicShift(const size_t step)
{
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	const size_t smallerDimension = (rows < columns) ? rows : columns;

	const auto depth = static_cast<size_t>(smallerDimension/2);
	for (size_t i = 0; i < depth; ++i) 
	{
		std::vector<T> cycleVec = GetCycle(i);
		std::rotate(cycleVec.begin(), cycleVec.end() - step, cycleVec.end());
		SetCycle(i, cycleVec);
	}
}

template<typename T>
int Matrix2DAdapter<T>::LongestIdenticalSet() const
{
	// If matrix is empty.
	if (matPtr_->GetRows() == 0) {
		return -1;
	}

	int number = 0;
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	size_t longestSet = 1;

	// Check every row and look for identical elements.
	for (size_t r = 0; r < rows; ++r)
	{
		size_t currentLength = 1;
		for (size_t c = 1; c < columns; ++c)
		{
			if ((*matPtr_)(r, c) == (*matPtr_)(r, c - 1)) {
				++currentLength;
			}

			if (currentLength > longestSet)
			{
				longestSet = currentLength;
				number = static_cast<int>(r);
			}
		}
	}

	return number;
}

template<typename T>
T Matrix2DAdapter<T>::NonNegativeRowsMultiplication() const
{
	T multiplication = 1;
	const size_t columns = matPtr_->GetColumns();
	// Pointers to first and past-the-end elements in the row.
	auto startOfRow = matPtr_->Begin();
	const auto endOfVector = matPtr_->End();
	while (startOfRow != endOfVector)
	{
		// Check if there are negative elements in the row.
		bool hasNegativeElems = std::any_of(
			startOfRow,
			startOfRow + columns,
			[] (const T& val)
			{
				return val < 0;
			});
		
		if (!hasNegativeElems)
		{
			multiplication = std::accumulate(
				startOfRow,
				startOfRow + columns,
				multiplication,
				std::multiplies<T>());
		}

		startOfRow += columns;
	}

	return multiplication;
}

template<typename T>
T Matrix2DAdapter<T>::SumOverMainDiagonal() const
{
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	if (rows == 0 || columns == 0) {
		return 0;
	}
	if (rows == 1 || columns == 1) {
		return (*matPtr_)(0, 0);
	}
	// Choose size of smaller dimension.
	const size_t smallerDimension = (rows < columns) ? rows : columns;
	T sumVal = 0;
	// Sum elements from main diagonal (not including elements on main diagonal itself)
	// to the end of i'th row until end of the vector is reached.
	auto endOfRow = matPtr_->Begin() + columns;
	const auto endOfVector = matPtr_->End();
	for (size_t i = 0; i < smallerDimension; ++i)
	{
		sumVal += std::accumulate(endOfRow - columns + i + 1, endOfRow, 0);
		if (endOfRow == endOfVector) {
			break;
		}
		endOfRow += columns;
	}

	return sumVal;
}

template<typename T>
bool Matrix2DAdapter<T>::CheckNeighbors(const size_t r, const size_t c) const
{
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	const T& currentElement = (*matPtr_)(r, c);
	// Check neighbors to the left, to the right, above and under
	// the given element respectively.
	if (c > 0 && (*matPtr_)(r, c - 1) <= currentElement) {
		return false;
	}

	if (c < columns - 1 && (*matPtr_)(r, c + 1) <= currentElement) {
		return false;
	}

	if (r > 0 && (*matPtr_)(r - 1, c) <= currentElement) {
		return false;
	}

	if (r < rows - 1 && (*matPtr_)(r + 1, c) <= currentElement) {
		return false;
	}
	// Check neighbors on upper left, upper right, bottom left
	// and bottom right corners of the given element.
	if (r > 0 && c > 0 && (*matPtr_)(r - 1, c - 1) <= currentElement) {
		return false;
	} 

	if (r > 0 && c < columns - 1 && (*matPtr_)(r - 1, c + 1) <= currentElement) {
		return false;
	}

	if (r < rows - 1 && c > 0 && (*matPtr_)(r + 1, c - 1) <= currentElement) {
		return false;
	}

	if (r < rows - 1 && c < columns - 1 && (*matPtr_)(r + 1, c + 1) <= currentElement) {
		return false;
	}

	return true;
}

template<typename T>
std::vector<T> Matrix2DAdapter<T>::GetCycle(const size_t n) const
{
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	// Find out size of the slice, for example for n == 1
	// and Matrix 3*3 it will be 8 elements.
	std::vector<T> cycleVec;
	cycleVec.reserve(2 * columns + 2 * (rows - 2));
	// Copy upper edge.
	for (size_t i = n; i < columns - n; ++i) {
		cycleVec.push_back((*matPtr_)(n, i));
	}
	// Copy right edge.
	for (size_t i = n + 1; i < rows - n - 1; ++i) {
		cycleVec.push_back((*matPtr_)(i, columns - n - 1));
	}
	// Copy bottom edge.
	for (size_t i = columns - n - 1; i >= n; --i)
	{
		cycleVec.push_back((*matPtr_)(rows - n - 1, i));
		if (i == 0) {
			break;
		}
	}
	// Copy left edge.
	for (size_t i = rows - n - 2; i >= n + 1; --i)
	{
		cycleVec.push_back((*matPtr_)(i, n));
	}

	return cycleVec;
}

template<typename T>
void Matrix2DAdapter<T>::SetCycle(const size_t n, std::vector<T> elements)
{
	const size_t rows = matPtr_->GetRows();
	const size_t columns = matPtr_->GetColumns();
	size_t vectorElem = 0;

	// Copy upper edge.
	for (size_t i = n; i < columns - n; ++i) {
		(*matPtr_)(n, i) = elements[vectorElem++];
	}
	// Copy right edge.
	for (size_t i = n + 1; i < rows - n - 1; ++i) {
		(*matPtr_)(i, columns - n - 1) = elements[vectorElem++];
	}
	// Copy bottom edge.
	for (size_t i = columns - n - 1; i >= n; --i)
	{
		(*matPtr_)(rows - n - 1, i) = elements[vectorElem++];
		if (i == 0) {
			break;
		}
	}
	// Copy left edge.
	for (size_t i = rows - n - 2; i >= n + 1; --i) 
	{
		(*matPtr_)(i, n) = elements[vectorElem++];
	}
}

}	// namespace mtx