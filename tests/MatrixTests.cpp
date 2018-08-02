#include "matrix/Matrix.h"
#include "matrix/Matrix2DAdapter.h"

#include "CatchInclude.h"

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

TEST_CASE("Matrix has a basic interface", "[Matrix2D]") 
{
	mtx::Matrix2D<int> mat(5, 5, 5);
	const mtx::Matrix2D<int> matConst(5, 5, 5);
	mtx::Matrix2D<int> matZeros(5, 5);
	mtx::Matrix2D<int> mat2(5, 4);
	mtx::Matrix2D<int> mat3(4, 5);

	//Test constructor.
	REQUIRE(mat.GetRows() == 5);
	REQUIRE(mat.GetColumns() == 5);
	REQUIRE(mat(0, 0) == 5);
	// Test Slice function.
	REQUIRE(mat.Slice(2, 2, 3)[0] == 5);
	REQUIRE(mat.Slice(2, 2, 3).size() == 1);
	REQUIRE(mat.Slice(2).size() == mat.GetColumns());
	REQUIRE(mat.Slice(2)[0] == 5);
	// Test Begin and End functions.
	REQUIRE(*mat.Begin() == 5);
	REQUIRE(*matConst.Begin() == 5);
	REQUIRE(*(mat.End() - 1) == 5);
	REQUIRE(*(matConst.End() - 1) == 5);
	// Test operators.
	REQUIRE(mat == matConst);
	REQUIRE(!(mat == matZeros));
	REQUIRE(!(mat == mat2));
	REQUIRE(!(mat == mat3));

	mat3 = mat + matConst;
	REQUIRE(mat3(0, 0) == 10);

	mat3 = mat - matConst;
	REQUIRE(mat3(0, 0) == 0);

	mat = matZeros;
	REQUIRE(mat(0, 0) == 0);
}

TEST_CASE("MatrixAdapter has specialized interface", "[Matrix2DAdapter]")
{
	auto mat = std::make_shared<mtx::Matrix2D<int>>(3, 3, 3);
	auto mat2 = std::make_shared<mtx::Matrix2D<int>>(2, 3, 3);
	auto mat3 = std::make_shared<mtx::Matrix2D<int>>(3, 2, 3);
	auto emptyMat = std::make_shared<mtx::Matrix2D<int>>();
	mtx::Matrix2DAdapter<int> matAdapter(mat);
	
	// Test CountLocalMinimums function.
	SECTION("Matrix2DAdapter::CountLocalMinimums")
	{
		REQUIRE(matAdapter.CountLocalMinimums() == 0);

		(*mat)(0, 0) = 1;
		(*mat)(2, 2) = 2;
		REQUIRE(matAdapter.CountLocalMinimums() == 2);

		(*mat)(1, 1) = 0;
		REQUIRE(matAdapter.CountLocalMinimums() == 1);

		(*mat)(2, 0) = -1;
		(*mat)(0, 2) = -1;
		REQUIRE(matAdapter.CountLocalMinimums() == 2);

		matAdapter = mtx::Matrix2DAdapter<int>(emptyMat);
		REQUIRE(matAdapter.CountLocalMinimums() == 0);

		// Matrix with size 2x3.
		matAdapter = mtx::Matrix2DAdapter<int>(mat2);
		REQUIRE(matAdapter.CountLocalMinimums() == 0);
	}
	// Test CyclicShift function.
	SECTION("Matrix2DAdapter::CyclicShift")
	{
		(*mat)(0, 0) = 1;
		matAdapter.CyclicShift();

		REQUIRE((*mat)(0, 1) == 1);

		matAdapter.CyclicShift(2);
		REQUIRE((*mat)(1, 2) == 1);

		mat = std::make_shared<mtx::Matrix2D<int>>(5, 4, 5);
		matAdapter = mtx::Matrix2DAdapter<int>(mat);
		(*mat)(1, 1) = 0;
		(*mat)(0, 0) = 0;
		matAdapter.CyclicShift();

		REQUIRE((*mat)(1, 2) == 0);
		REQUIRE((*mat)(0, 1) == 0);

		matAdapter.CyclicShift(2);
		REQUIRE((*mat)(3, 2) == 0);
		REQUIRE((*mat)(0, 3) == 0);
	}
	// Test LongestIdenticalSet function.
	SECTION("Matrix2DAdapter::LongestIdenticalSet")
	{
		REQUIRE(matAdapter.LongestIdenticalSet() == 0);

		for (size_t i = 0; i < mat->GetRows(); ++i) {
			(*mat)(i, 1) = 1;
		}

		REQUIRE(matAdapter.LongestIdenticalSet() == 0);

		(*mat)(1, 0) = 1;
		REQUIRE(matAdapter.LongestIdenticalSet() == 1);

		matAdapter = mtx::Matrix2DAdapter<int>(emptyMat);
		REQUIRE(matAdapter.LongestIdenticalSet() == -1);
	}
	// Test NonNegativeRowsMultiplication function.
	SECTION("Matrix2DAdapter::NonNegativeRowsMultiplication")
	{
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == std::pow(3, 9));

		(*mat)(2, 0) = -1;
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == std::pow(3, 6));

		(*mat)(1, 0) = -1;
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == std::pow(3, 3));

		(*mat)(0, 0) = -1;
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == 1);

		(*mat)(0, 0) = 0;
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == 0);

		matAdapter = mtx::Matrix2DAdapter<int>(emptyMat);
		REQUIRE(matAdapter.NonNegativeRowsMultiplication() == 1);
	}
	// Test SumOverMainDiagonal function.
	SECTION("Matrix2DAdapter::SumOverMainDiagonal")
	{
		REQUIRE(matAdapter.SumOverMainDiagonal() == 9);
		// If Matrix has only negative elements.
		mat = std::make_shared<mtx::Matrix2D<int>>(3, 3, -1);
		matAdapter = mtx::Matrix2DAdapter<int>(mat);
		REQUIRE(matAdapter.SumOverMainDiagonal() == -3);
		// Matrix with size 2x3.
		matAdapter = mtx::Matrix2DAdapter<int>(mat2);
		REQUIRE(matAdapter.SumOverMainDiagonal() == 9);
		// Matrix with size 3x2.
		matAdapter = mtx::Matrix2DAdapter<int>(mat3);
		REQUIRE(matAdapter.SumOverMainDiagonal() == 3);

		matAdapter = mtx::Matrix2DAdapter<int>(emptyMat);
		REQUIRE(matAdapter.SumOverMainDiagonal() == 0);
	}
}