//
//  Rotation.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 4/11/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Sequence.hpp"

#include "../Radians.hpp"
#include "../Vector.hpp"

namespace Numerics
{
	namespace Transforms
	{
		/// A fixed rotation around a specific unit axis, e.g. X, Y, Z.
		template <std::size_t AXIS, typename NumericT>
		struct FixedAxisRotation {
			Radians<NumericT> angle;

			bool identity() const { return angle == 0; }

			template <std::size_t E>
			Vector<E, NumericT> axis () const
			{
				Vector<E, NumericT> result = 0;

				result[AXIS] = 1;

				return result;
			}

			template <typename RightT>
			Sequence<FixedAxisRotation, RightT> operator<<(const RightT & right) const
			{
				return {*this, right};
			}
		};

		/// Rotation around a fixed axis: rotation<X>(R90)
		template <std::size_t AXIS, typename NumericT>
		FixedAxisRotation<AXIS, NumericT> rotate(const Radians<NumericT> & angle) {
			return {angle};
		}
		
		template <std::size_t E, typename AngleNumericT, typename AxisNumericT>
		struct OffsetAngleAxisRotation;

		/// Fairly standard angle-axis transform:
		template <std::size_t E, typename AngleNumericT, typename AxisNumericT>
		struct AngleAxisRotation {
			/// The angle to rotate by:
			Radians<AngleNumericT> angle;

			/// The axis to rotate around:
			Vector<E, AxisNumericT> axis;

			OffsetAngleAxisRotation<E, AngleNumericT, AxisNumericT> around_origin(const Vector<E, AxisNumericT> & origin) {
				return {*this, origin};
			}

			bool identity () const { return angle == 0; }

			template <typename RightT>
			Sequence<AngleAxisRotation, RightT> operator<< (const RightT & right) const
			{
				return {*this, right};
			}
		};

		template <std::size_t E, typename AngleNumericT, typename AxisNumericT>
		AngleAxisRotation<E, AngleNumericT, AxisNumericT> rotate(const Radians<AngleNumericT> & angle, const Vector<E, AxisNumericT> & axis) {
			return {angle, axis};
		}

		template <std::size_t E, typename NumericT>
		AngleAxisRotation<E, NumericT, NumericT> rotate(const Vector<E, NumericT> & from, const Vector<E, NumericT> & to, const Vector<E, NumericT> & normal) {
			auto angle = to.angle_between(from);

			if (angle.equivalent(0_deg)) {
				return {angle, normal};
			} else if (angle.equivalent(180_deg)) {
				return {angle, cross_product(from, normal).normalize()};
			} else {
				return {angle, cross_product(from, to).normalize()};
			}
		}

		template <std::size_t E, typename AngleNumericT, typename AxisNumericT>
		struct OffsetAngleAxisRotation {
			AngleAxisRotation<E, AngleNumericT, AxisNumericT> rotation;
			Vector<E, AxisNumericT> origin;

			bool identity () const { return rotation.identity(); }

			template <typename RightT>
			Sequence<OffsetAngleAxisRotation, RightT> operator<< (const RightT & right) const
			{
				return {*this, right};
			}
		};
	}
}
