//
//  Float.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Integer.hpp"

#include <cstddef>
#include <limits>
#include <cmath>
#include <utility>

namespace Numerics
{
	// Default to floating point precision.
	using RealT = float;
	
	/// Helper to get floating point type from a fixed point type.
	/// By default we map all integer types to the deault floating point precision.
	template <typename TypeT>
	struct RealTypeTraits {
		using RealT = Numerics::RealT;
	};

	template <>
	struct RealTypeTraits<double> {
		typedef double RealT;
	};
	
	/// These traits give specific accuracy information around +/- zero. The calculated precision is proportional to the number of integral positions between 0.0 and 0.0+EPSILON (where epsilon is the smallest increment from 10.0^exponent).
	template <typename FloatT, std::size_t Exponent>
	struct EpsilonTraits {
	};

	template <>
	struct EpsilonTraits<float, 0> {
		typedef typename IntegerSizeTraits<sizeof(float)>::UnsignedT UnitT;
		
		// The point on the number line at which the floating point epsilon = 1 ulps:
		constexpr static float SCALE = 1.0;
		
		// Floating point calculations are allowed to be at most UNITS deviations:
		constexpr static UnitT UNITS = 8;
		
		// The actual epsilon scaled by the deviation we allow:
		constexpr static float EPSILON = std::numeric_limits<float>::epsilon() * UNITS;
	};

	template <>
	struct EpsilonTraits<double, 0> {
		typedef typename IntegerSizeTraits<sizeof(double)>::UnsignedT UnitT;

		constexpr static double SCALE = 1.0;
		constexpr static UnitT UNITS = 8;
		constexpr static double EPSILON = std::numeric_limits<double>::epsilon() * UNITS;
	};

	template <typename _FloatT>
	struct FloatEquivalenceTraits {
		typedef _FloatT FloatT;

		using IntegralT = typename IntegerSizeTraits<sizeof(FloatT)>::SignedT;
		using UnsignedT = typename IntegerSizeTraits<sizeof(FloatT)>::UnsignedT;

		union Conversion {
			FloatT float_value;
			IntegralT integer_value;
		};

		static constexpr IntegralT NEGATIVE_OFFSET = IntegralT(0x80ULL << ((sizeof(FloatT) - 1) * 8));

		static IntegralT convert_to_integer(const FloatT & value)
		{
			Conversion conversion;
			conversion.float_value = value;

			if (conversion.integer_value < 0)
				return NEGATIVE_OFFSET - conversion.integer_value;
			else
				return conversion.integer_value;
		}

		static FloatT convert_to_float(const IntegralT & value)
		{
			Conversion conversion;
			conversion.integer_value = value;

			if (conversion.integer_value < 0)
				conversion.integer_value = NEGATIVE_OFFSET - conversion.integer_value;

			return conversion.float_value;
		}

		static UnsignedT integral_difference(const FloatT & a, const FloatT & b)
		{
			// Make lexicographically ordered as a twos-complement int
			IntegralT i = convert_to_integer(a), j = convert_to_integer(b);

			if (i < j)
				std::swap(i, j);

			return i - j;
		}

		// Large MAX_DEVIATIONS may allow NAN to compare equivalent to large floating point numbers and other strange edge cases.
		static bool equivalent(const FloatT & a, const FloatT & b)
		{
			if (std::isnan(a) || std::isnan(b))
				return false;

			using E = EpsilonTraits<FloatT, 0>;

			if (std::abs(a) < E::SCALE || std::abs(b) < E::SCALE) {
				auto epsilon = E::EPSILON;
				
				// We compare the difference between the two numbers:
				auto difference = std::abs(a - b);
				
				// It needs to be less than the epsilon value:
				return difference <= epsilon;
			} else {
				return integral_difference(a, b) <= E::UNITS;
			}
		}
	};
	
	// Proportional equivalence of floating point numbers.
	inline bool equivalent(const float & a, const float & b)
	{
		return FloatEquivalenceTraits<float>::equivalent(a, b);
	}

	inline bool equivalent(const double & a, const double & b)
	{
		return FloatEquivalenceTraits<double>::equivalent(a, b);
	}
	
	// inline bool equivalent(const long double & a, const long double & b)
	// {
	// 	return FloatEquivalenceTraits<long double>::equivalent(a, b);
	// }
}
