//
//  Test.Radians.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Radians.hpp>
#include <Numerics/Number.hpp>

namespace Numerics
{
	UnitTest::Suite RadiansTestSuite {
		"Numerics::Radians",
		
		{"Multiplication",
			[](UnitTest::Examiner & examiner) {
				examiner << "45_deg is correct" << std::endl;
				examiner.check(equivalent(45_deg * 2.0, 90_deg));
				examiner << "90_deg is correct" << std::endl;
				examiner.check(equivalent(90_deg * 2.0, 180_deg));
				examiner << "360_deg is correct" << std::endl;
				examiner.check(equivalent(180_deg * 2.0, 360_deg));
			}
		},
		
		{"Constants",
			[](UnitTest::Examiner & examiner) {
				examiner.check_equal(M_PI_2, (90_deg).value);
				examiner.check_equal((90_deg).sin(), 1.0);
			}
		},
		
		{"Division",
			[](UnitTest::Examiner & examiner) {
				examiner << "Angle was divided." << std::endl;
				examiner.check((90_deg / 2).equivalent(45_deg));
			}
		},
	};
}
