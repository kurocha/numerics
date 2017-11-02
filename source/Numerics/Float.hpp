//
//  Float.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

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
	
	// Proportional equivalence of floating point numbers.
	bool equivalent(const float & a, const float & b);
	bool equivalent(const double & a, const double & b);
}
