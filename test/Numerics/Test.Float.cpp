//
//  Test.Float.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Float.hpp>
#include <Numerics/Radians.hpp>

namespace Numerics
{
	UnitTest::Suite FloatTestSuite {
		"Numerics::Float",
		
		{"Equivalence",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(equivalent(1.0, 1.0)) == true;
				examiner.expect(equivalent(1.0, 1.1)) == false;
				examiner.expect(equivalent(0.0, -0.0)) == true;
			}
		},
	};
}
