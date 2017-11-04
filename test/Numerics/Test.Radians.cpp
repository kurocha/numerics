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
	using namespace UnitTest::Expectations;
	
	UnitTest::Suite RadiansTestSuite {
		"Numerics::Radians",
		
		{"it can by multiplied by a constant",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(45_deg * 2.0).to(be_equivalent(90_deg));
				examiner.expect(90_deg * 2.0).to(be_equivalent(180_deg));
				examiner.expect(180_deg * 2.0).to(be_equivalent(360_deg));
			}
		},
		
		{"it matches mathematical constants",
			[](UnitTest::Examiner & examiner) {
				examiner.check_equal(M_PI_2, (90_deg).value);
				examiner.check_equal((90_deg).sin(), 1.0);
			}
		},
		
		{"it can be divided by a constant",
			[](UnitTest::Examiner & examiner) {
				examiner.check((90_deg / 2).equivalent(45_deg));
			}
		},
	};
}
