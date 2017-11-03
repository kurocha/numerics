//
//  Quaternion.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Vector.hpp"
#include "Transforms.hpp"

namespace Numerics
{
	template <std::size_t R, std::size_t C, typename NumericT>
	class Matrix;
	
	// An efficient representation of an angle/axis rotation in 3D.
	template <typename NumericT = RealT>
	class Quaternion : public Vector<4, NumericT> {
	public:
		/// Undefined constructor.
		Quaternion() = default;

		/// Identity constructor.
		Quaternion(const Identity &) : Vector<4, NumericT>{0, 0, 0, 1} {}

		Quaternion(const Vector<4, NumericT> & value) : Vector<4, NumericT>(value) {}
		
		// Quaternion(const Matrix<4, 4, NumericT> & m)
		// {
		// 	auto w = number(1.0 + m[0] + m[5] + m[10]).sqrt() / 2;
		// 
		// 	(*this)[X] = (m[6] - m[9]) / (4 * w);
		// 	(*this)[Y] = (m[8] - m[2]) / (4 * w);
		// 	(*this)[Z] = (m[1] - m[4]) / (4 * w);
		// 	(*this)[W] = w;
		// 	
		// 	this->normalize(this->length(), 1.0);
		// }

		/// Angle axis constructor.
		Quaternion(const Radians<NumericT> & angle, const Vector<3, NumericT> & axis)
		{
			auto half_angle = angle / 2.0;
			
			(*this) = (axis * half_angle.sin()).append(half_angle.cos());
		}
		
		template <std::size_t N, typename AngleNumericT, typename AxisNumericT>
		Quaternion(const AngleAxisRotation<N, AngleNumericT, AxisNumericT> & rotation) : Quaternion(rotation.angle, rotation.axis)
		{
		}

		template <std::size_t AXIS, typename AngleNumericT>
		Quaternion(const FixedAxisRotation<AXIS, AngleNumericT> rotation) : Quaternion(rotation.angle, rotation.template axis<3>())
		{
		}

		template <typename A, typename B>
		Quaternion(const Transforms<A, B> & transforms) : Quaternion(IDENTITY)
		{
			transforms.apply(*this);
		}

		/// Calculate the angle of rotation.
		Radians<NumericT> angle() const {
			return number((*this)[W]).acos() * 2.0;
		}

		/// Calculate the axis of rotation.
		Vector<3, NumericT> axis() const {
			return this->reduce().normalize();
		}

		/// Return a quaternion that rotates from this rotation to another.
		Quaternion rotation_to(const Quaternion & other) const {
			return this->conjugate() * other;
		}

		/// Conjugate the current quaternion in place.
		Quaternion conjugate() const {
			Quaternion copy(*this);

			copy[X] *= -1;
			copy[Y] *= -1;
			copy[Z] *= -1;

			return copy;
		}

		/// Rotate a point:
		Vector<3, NumericT> operator*(const Vector<3, NumericT> & v) const {
			auto qvec = this->reduce();

			auto uv = cross_product(qvec, v);
			auto uuv = cross_product(qvec, uv);

			uv *= (*this)[W] * 2;
			uuv *= 2;

			return v + uv + uuv;
		}

		template <typename RightT>
		Transforms<Quaternion, RightT> operator<<(const RightT & right) const
		{
			return {*this, right};
		}
		
		Quaternion operator*(const Quaternion & other) const noexcept
		{
			auto & q1 = *this;
			auto & q2 = other;
			
			Quaternion result;

			result[X] = (q1[W]*q2[X] + q1[X]*q2[W] + q1[Y]*q2[Z]) - q1[Z]*q2[Y];
			result[Y] = (q1[W]*q2[Y] + q1[Y]*q2[W] + q1[Z]*q2[X]) - q1[X]*q2[Z];
			result[Z] = (q1[W]*q2[Z] + q1[X]*q2[Y] + q1[Z]*q2[W]) - q1[Y]*q2[X];
			result[W] =  q1[W]*q2[W] - q1[X]*q2[X] - q1[Y]*q2[Y]  - q1[Z]*q2[Z];

			return result;
		}
		
		Quaternion & operator*=(const Quaternion & other)
		{
			return *this = (*this * other);
		}
	};
	
	namespace Interpolate
	{
		/// Spherical linear interpolation is typically used to interpolate between rotations represented in quaternion space.
		template <typename NumericT>
		Quaternion<NumericT> spherical_linear(NumericT t, const Quaternion<NumericT> & q0, const Quaternion<NumericT> & q1)
		{
			// Compute the cosine of the angle between the two vectors.
			auto dot = q0.dot(q1);

			const NumericT DOT_THRESHOLD = 0.9995;
			if (dot > DOT_THRESHOLD) {
				// If the inputs are too close for comfort, linearly interpolate
				// and normalize the result.
				return (q0 + (q1 - q0) * t).normalize();
			}

			// theta_0 = angle between input vectors:
			auto theta_0 = dot.acos();

			// theta = angle between v0 and result:
			auto theta = theta_0 * t;

			auto q2 = (q1 - q0 * dot).normalize();

			// { q0, q2 } is now an orthonormal basis.
			return q0*cos(theta) + q2*sin(theta);
		}
	}
	
	extern template class Quaternion<float>;
	extern template class Quaternion<double>;
}
