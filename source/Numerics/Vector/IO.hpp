//
//  Vector/IO.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#pragma once

#include "../Vector.hpp"

#include <iostream>
#include <iomanip>

namespace Numerics {
	/// Write a vector to an std::ostream
	template <std::size_t D, typename NumericT>
	std::ostream & operator<<(std::ostream & output, const Vector<D, NumericT> & vec)
	{
		output << '{';
		for (std::size_t i = 0; i < D; ++i) {
			if (i != 0) output << ",";
			// We use this '+' operator to ensure that char and unsigned char are printed correctly:
			output << +vec[i];
		}
		output << '}';
		
		return output;
	}
	
	template <char C>
	std::istream & expect_character(std::istream & input)
	{
		char c;
		
		if (input >> c && c != C) {
				input.setstate(std::ios_base::failbit);
		}
		
		return input;
	}
	
	/// Read a vector from a std::istream
	template <std::size_t D, typename NumericT>
	std::istream & operator>>(std::istream & input, Vector<D, NumericT> & vec)
	{
		input >> expect_character<'{'>;
		
		for (std::size_t i = 0; i < D; ++i) {
			if (i != 0) input >> expect_character<','>;
			input >> vec[i];
		}
		
		input >> expect_character<'}'>;
		
		return input;
	}
}
