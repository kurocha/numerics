//
//  Matrix.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Float.hpp"
#include "Transforms.hpp"

#include <array>
#include <cstddef>
#include <cassert>

namespace Numerics
{
	template <typename NumericT>
	class Quaternion;
	
	std::size_t row_major_offset(std::size_t row, std::size_t col, std::size_t sz);
	std::size_t column_major_offset(std::size_t row, std::size_t col, std::size_t sz);

	/// A 2-dimentional set of numbers that can represent useful transformations in n-space.
	/// Standard mathematical notation is column order, therefore regardless of row-major or column-major memory layout, the interface will assume access is done via rows and columns according to this standard notation.
	template <std::size_t R = 4, std::size_t C = 4, typename NumericT = RealT>
	class alignas(16) Matrix : public std::array<NumericT, R*C> {
	public:
		/// The type of the vector elements.
		typedef typename RealTypeTraits<NumericT>::RealT RealT;
		
		// Uninitialized constructor
		Matrix() = default;

		Matrix(const NumericT & identity)
		{
			this->fill(0);

			if (identity == 0) return;

			for (std::size_t i = 0; i < std::min(R, C); i += 1)
				at(i, i) = identity;
		}

		Matrix(const Matrix & other) : std::array<NumericT, R*C>(other) {}

		template <typename QuaternionNumericT>
		Matrix(const Quaternion<QuaternionNumericT> & rotation) : Matrix(IDENTITY)
		{
			static_assert(R >= 3 && C >= 3, "Matrix must be at least 3x3 to contain rotation!");
			assert(rotation.length().equivalent(1) && "Quaternion magnitude must be 1");

			auto x = rotation[X], y = rotation[Y], z = rotation[Z], w = rotation[W];

			at(0, 0) = 1.0 - 2.0 * (y*y + z*z);
			at(0, 1) =       2.0 * (x*y - w*z);
			at(0, 2) =       2.0 * (x*z + w*y);

			at(1, 0) =       2.0 * (x*y + w*z);
			at(1, 1) = 1.0 - 2.0 * (x*x + z*z);
			at(1, 2) =       2.0 * (y*z - w*x);

			at(2, 0) =       2.0 * (x*z - w*y);
			at(2, 1) =       2.0 * (y*z + w*x);
			at(2, 2) = 1.0 - 2.0 * (x*x + y*y);
		}

		template <typename... TailT>
		Matrix(const NumericT & head, const TailT&&... tail) : std::array<NumericT, R*C>{{head, (NumericT)tail...}} {}

		template <std::size_t S, std::size_t T, typename OtherNumericT>
		Matrix(const Matrix<S, T, OtherNumericT> & other)
		{
			for (std::size_t t = 0; t < std::min(C, T); t += 1) {
				auto column = &other.at(0, t);
				std::copy(column, column + std::min(R, S), &at(0, t));
			}
		}

		template <typename OtherNumericT>
		Matrix(const OtherNumericT (&data)[R*C])
		{
			std::copy(data, data + R*C, this->begin());
		}

		// Transform Constructors:
		template <std::size_t N, typename AxisNumericT>
		Matrix(const Transforms::Translation<N, AxisNumericT> & translation) : Matrix(IDENTITY)
		{
			for (std::size_t i = 0; i < std::min(R, N); i += 1) {
				at(i, C-1) = translation.offset[i];
			}
		}

		template <std::size_t N>
		Matrix(const Transforms::Scale<N, NumericT> & scale) : Matrix(IDENTITY)
		{
			for (std::size_t i = 0; i < N; i += 1) {
				at(i, i) = scale.factor[i];
			}
		}

		template <std::size_t K = 1, typename ScaleFactorT>
		Matrix(const Transforms::UniformScale<ScaleFactorT> & scale) : Matrix(IDENTITY)
		{
			for (std::size_t i = 0; i < std::min(R, C) - K; i += 1) {
				at(i, i) = scale.factor;
			}
		}
		
		template <std::size_t N, typename AngleNumericT, typename AxisNumericT>
		Matrix(const Transforms::AngleAxisRotation<N, AngleNumericT, AxisNumericT> & rotation) : Matrix(IDENTITY) {
			static_assert(R == C && (R == 3 || R == 4), "Angle axis rotation is only valid for 3x3 or 4x4 matrices!");

			if (!rotation.identity()) {
				NumericT s = rotation.angle.sin();
				NumericT c = rotation.angle.cos();
				NumericT a = 1.0 - c;

				auto & p = rotation.axis;
				auto sp = p * s;

				at(0, 0) = p[X] * p[X] * a + c;
				at(0, 1) = p[X] * p[Y] * a - sp[Z];
				at(0, 2) = p[X] * p[Z] * a + sp[Y];

				at(1, 0) = p[Y] * p[X] * a + sp[Z];
				at(1, 1) = p[Y] * p[Y] * a + c;
				at(1, 2) = p[Y] * p[Z] * a - sp[X];

				at(2, 0) = p[Z] * p[X] * a - sp[Y];
				at(2, 1) = p[Z] * p[Y] * a + sp[X];
				at(2, 2) = p[Z] * p[Z] * a + c;
			}
		}

		template <std::size_t N, typename AngleNumericT, typename AxisNumericT>
		Matrix(const Transforms::OffsetAngleAxisRotation<N, AngleNumericT, AxisNumericT> & offset_rotation) : Matrix(IDENTITY) {
			if (offset_rotation.origin.equivalent(0)) {
				(*this) = offset_rotation.rotation;
			} else {
				(*this) = translate(-offset_rotation.origin) << offset_rotation.rotation << translate(offset_rotation.origin);
			}
		}

		template <typename AxisNumericT>
		Matrix(const Transforms::FixedAxisRotation<X, AxisNumericT> & fixed_rotation) : Matrix(IDENTITY) {
			static_assert(R >= 3 && C >= 3, "Matrix must be size 3 or bigger!");

			if (!fixed_rotation.identity()) {
				NumericT ca = fixed_rotation.angle.cos();
				NumericT sa = fixed_rotation.angle.sin();

				at(1, 1) = ca;
				at(1, 2) = -sa;
				at(2, 1) = sa;
				at(2, 2) = ca;
			}
		}

		template <typename AxisNumericT>
		Matrix(const Transforms::FixedAxisRotation<Y, AxisNumericT> & fixed_rotation) : Matrix(IDENTITY) {
			static_assert(R >= 3 && C >= 3, "Matrix must be size 3 or bigger!");

			if (!fixed_rotation.identity()) {
				NumericT ca = fixed_rotation.angle.cos();
				NumericT sa = fixed_rotation.angle.sin();

				at(0, 0) = ca;
				at(2, 2) = ca;
				at(2, 0) = -sa;
				at(0, 2) = sa;
			}
		}

		template <typename AxisNumericT>
		Matrix(const Transforms::FixedAxisRotation<Z, AxisNumericT> & fixed_rotation) : Matrix(IDENTITY) {
			static_assert(R >= 2 && C >= 2, "Matrix must be size 2 or bigger!");

			if (!fixed_rotation.identity()) {
				NumericT ca = fixed_rotation.angle.cos();
				NumericT sa = fixed_rotation.angle.sin();

				at(0, 0) = ca;
				at(1, 1) = ca;
				at(0, 1) = -sa;
				at(1, 0) = sa;
			}
		}

		template <typename A, typename B>
		Matrix(const Transforms::Sequence<A, B> & sequence) : Matrix(IDENTITY)
		{
			sequence.apply(*this);
		}

		// Getters and setters
		template <typename OtherNumericT>
		void set (const OtherNumericT * data) {
			std::copy(data, data + (R*C), this->begin());
		}

		std::size_t offset(std::size_t row, std::size_t column) const {
			assert(row < R && column < C);
			
			return column_major_offset(row, column, R);
		}

		// Accessors
		const NumericT & at (std::size_t r, std::size_t c) const
		{
			return (*this)[offset(r, c)];
		}

		NumericT & at (std::size_t r, std::size_t c)
		{
			return (*this)[offset(r, c)];
		}

		/// Copy a vector into the matix at position r, c
		/// This copies the vector in the direction of the major format,
		/// i.e. in column major format it will appear as a column
		template <std::size_t D>
		void set (const std::size_t & r, const std::size_t & c, const Vector<D, NumericT> & values) {
			std::copy(values.begin(), values.end(), &at(r, c));
		}

		/// Copy a vector into the matrix at position r, c, with element_offset distance between each element.
		/// The purpose of this function is primarily to facilitate copying a vector into a matrix in an order
		/// other than the major.
		/// i.e. set(0, 0, Vec4(...), 4) will set a row in a column major matrix.
		template <std::size_t D>
		void set (const std::size_t & r, const std::size_t & c, const Vector<D, NumericT> & values, std::size_t element_offset)
		{
			std::size_t start_offset = offset(r, c);

			for (std::size_t i = 0; i < D; i += 1) {
				(*this)[start_offset + element_offset * i] = values[i];
			}
		}

		/// Return a copy of this matrix, transposed.
		Matrix<C, R, NumericT> transpose () const
		{
			Matrix<C, R, NumericT> result;

			for (std::size_t c = 0; c < C; ++c)
				for (std::size_t r = 0; r < R; ++r)
					result.at(c, r) = at(r, c);

			return result;
		}

		bool equivalent(const Matrix & other) const
		{
			for (std::size_t i = 0; i < R*C; i += 1) {
				if (!Numerics::equivalent((*this)[i], other[i])) {
					return false;
				}
			}

			return true;
		}

		template <typename RightT>
		Transforms::Sequence<Matrix, RightT> operator<<(const RightT & right) const
		{
			return {*this, right};
		}
	};
	
	extern template class Matrix<4, 4, float>;
	extern template class Matrix<4, 4, double>;
}

#include "Matrix/Multiply.hpp"
