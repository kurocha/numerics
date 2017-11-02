//
//  NEON.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/08/12.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#ifdef __arm__

#include <arm/arch.h>
#ifdef _ARM_ARCH_7

#define NUMERICS_MATRIX_NEON

#include "../Matrix.hpp"
#include "../Vector.hpp"

namespace Numerics
{
	// This is an optimised specialization for ARMv7 NEON:
	void multiply(Matrix<4, 4, float> & result, const Matrix<4, 4, float> & left, const Matrix<4, 4, float> & right);
	void multiply(Vector<4, float> & result, const Matrix<4, 4, float> & left, const Vector<4, float> & right);
}

#endif
#endif
