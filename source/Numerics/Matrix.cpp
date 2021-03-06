//
//  Matrix.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Matrix.hpp"

#include "Quaternion.hpp"

namespace Numerics
{
	template class Matrix<4, 4, float>;
	template class Matrix<4, 4, double>;
}
