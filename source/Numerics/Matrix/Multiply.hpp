//
//  Multiply.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 27/11/2012.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

// Platform specific optimizations:
#include "NEON.hpp"
#include "SSE.hpp"

namespace Numerics
{
	/*
	 *              top
	 *                  | |1|
	 *                  | |2|
	 *              C = 4 |3|
	 *  left            | |4|
	 *        - C = 4 -
	 *      | |1 2 3 4|   | |
	 *  R = 4 |5 6 7 8|   |R|
	 *      | |9 8 7 6|   | |
	 *      | |5 4 3 2|   | |
	 *
	 */

	template <std::size_t R, std::size_t C, typename NumericT>
	void multiply(Vector<R, NumericT> & result, const Matrix<R, C, NumericT> & left, const Vector<C, NumericT> & right)
	{
		for (std::size_t r = 0; r < R; ++r)
			for (std::size_t c = 0; c < C; ++c)
				result[r] += right[c] * left.at(r, c);
	}

	/*
	 *                top
	 *                    -  C = 5  -
	 *                  | |1 2 3 4 5|
	 *                  | |6 7 8 9 8|
	 *              T = 4 |7 6 5 4 3|
	 *   left           | |2 1 0 1 2|
	 *        - T = 4 -
	 *      | |1 2 3 4|   |         |
	 *  R = 3 |5 6 7 8|   |  R x C  |
	 *      | |9 8 7 6|   |         |
	 *                        result
	 *
	 */
	
	template <std::size_t R, std::size_t C, std::size_t T, typename NumericT>
	void multiply(Matrix<R, C, NumericT> & result, const Matrix<R, T, NumericT> & left, const Matrix<T, C, NumericT> & top)
	{
		for (std::size_t r = 0; r < R; ++r) {
			for (std::size_t c = 0; c < C; ++c) {
				NumericT & value = result.at(r, c);
				
				for (std::size_t t = 0; t < T; t++)
					value += left.at(r, t) * top.at(t, c);
			}
		}
	}

	/// Short-hand notation
	template <std::size_t R, std::size_t C, typename NumericT>
	Vector<C, NumericT> operator*(const Matrix<R, C, NumericT> & left, const Vector<R, NumericT> & right)
	{
		Vector<R, NumericT> result(ZERO);

		multiply(result, left, right);

		return result;
	}

	/// Short-hand notation for non-homogeneous vectors
	template <std::size_t R, std::size_t C, typename NumericT>
	Vector<C-1, NumericT> operator*(const Matrix<R, C, NumericT> & left, const Vector<R-1, NumericT> & right)
	{
		Vector<C, NumericT> result(ZERO);

		multiply(result, left, right.append(1));

		result /= result[C-1];

		return result.reduce();
	}

	/// Short hand for matrix multiplication
	template <std::size_t R, std::size_t C, std::size_t T, typename NumericT>
	Matrix<R, C, NumericT> operator*(const Matrix<R, T, NumericT> & left, const Matrix<T, C, NumericT> & right)
	{
		Matrix<R, C, NumericT> result(ZERO);

		// For column-major matricies, the right hand transform is applied first when result * vector
		multiply(result, left, right);

		return result;
	}

	template <std::size_t R, std::size_t C, typename NumericT>
	Matrix<R, C, NumericT> & operator*=(Matrix<R, C, NumericT> & transform, const Matrix<R, C, NumericT> & step)
	{
		return (transform = transform * step);
	}
}
