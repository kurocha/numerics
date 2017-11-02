//
//  SSE.hpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/08/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#pragma once

#ifdef __SSE2__

#define NUMERICS_MATRIX_SSE

#include "../Matrix.hpp"
#include "../Vector.hpp"

namespace Numerics
{
	// This is an optimised specialization for SSE2:
	void multiply(Matrix<4, 4, float> & result, const Matrix<4, 4, float> & left, const Matrix<4, 4, float> & right);
	//void multiply(Vector<4, float> & result, const Matrix<4, 4, float> & left, const Vector<4, float> & right);
}

#endif
