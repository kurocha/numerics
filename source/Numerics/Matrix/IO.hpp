//
//  Matrix/IO.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#pragma once

#include "../Matrix.hpp"

#include <iostream>
#include <iomanip>

namespace Numerics {
	template <std::size_t R, std::size_t C, typename NumericT>
	inline std::ostream & operator<< (std::ostream & out, const Matrix<R, C, NumericT> & m)
	{
		using namespace std;
		
		std::size_t k = 0;
		for (std::size_t c = 0; c < C; ++c)
			for (std::size_t r = 0; r < R; ++r) {
				out << setw(10) << setprecision(4) << m.at(r, c);
				
				if (k % R == (R-1))
					out << endl;
				else
					out << " ";
					
				k += 1;
			}
			
		return out;
	}
}
