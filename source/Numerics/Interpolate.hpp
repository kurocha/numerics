//
//  Interpolate.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 29/09/2006.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <cmath>

namespace Numerics
{
	namespace Interpolate
	{
		/// Linear interpolate between two values
		template <typename InterpolateT, typename AnyT>
		inline AnyT linear(const InterpolateT & t, const AnyT & a, const AnyT & b)
		{
			return (a * (1.0 - t)) + (b * t);
		}

		/// Cosine interpolate between two values
		template <typename InterpolateT, typename AnyT>
		inline AnyT cosine(const InterpolateT & t, const AnyT & a, const AnyT & b)
		{
			auto f = (1.0 - cos(t * M_PI)) * 0.5;

			return a*(1-f) + b*f;
		}

		/// Cubic interpolate between four values
		template <typename InterpolateT, typename AnyT>
		inline AnyT cubic(const InterpolateT & t, const AnyT & a, const AnyT & b, const AnyT & c, const AnyT & d)
		{
			auto p = (d - c) - (a - b);
			auto q = (a - b) - p;
			auto r = c - a;
			auto s = b;

			return p*(t*t*t) + q*(t*t) + r * t + s;
		}

		/// Hermite polynomial interpolation function.
		template <typename InterpolateT, typename AnyT>
		inline AnyT hermite(const InterpolateT & t, const AnyT & p0, const AnyT & m0, const AnyT & p1, const AnyT & m1)
		{
			auto t3 = t*t*t, t2 = t*t;

			auto h00 = 2*t3 - 3*t2 + 1;
			auto h10 = t3 - 2*t2 + t;
			auto h01 = -2*t3 + 3*t2;
			auto h11 = t3 - t2;

			return p0 * h00 + m0 * h10 + p1 * h01 + m1 * h11;
		}
	}
}
