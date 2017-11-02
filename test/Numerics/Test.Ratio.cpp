//
//  Test.Ratio.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Ratio.hpp>

namespace Numerics
{
	UnitTest::Suite RatioTestSuite {
		"Numerics::Ratio",

		{"Ratios",
			[](UnitTest::Examiner & examiner) {
				auto size = Ratio<float>{10.0, 20.0};
				examiner.expect((float)size) == 0.5;

				auto resized_smaller = Ratio<float>{20.0, 10.0}.shrink_to_fit_aspect_ratio(0.5);
				examiner.expect(resized_smaller) == vector(5.0, 10.0);

				auto resized_larger = Ratio<float>{20.0, 10.0}.expand_to_fit_aspect_ratio(0.5);
				examiner.expect(resized_larger) == vector(20.0, 40.0);
			}
		},
	};
}
