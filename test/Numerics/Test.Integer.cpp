//
//  Test.Integer.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Integer.hpp>

namespace Numerics
{
	UnitTest::Suite IntegerTestSuite {
		"Numerics::Integer",
		
		{"Powers of 2",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(power_of_2(0U)) == 2;
				examiner.expect(power_of_2(15U)) == 16;
				examiner.expect(power_of_2(16U)) == 16;
				examiner.expect(power_of_2(17U)) == 32;
			}
		},
	};
}
