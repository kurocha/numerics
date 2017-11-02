//
//  Test.Interpolate.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Interpolate.hpp>

namespace Numerics
{
	namespace Interpolate
	{
		UnitTest::Suite InterpolateTestSuite {
			"Numerics::Interpolate",
			
			{"Linear Interpolation",
				[](UnitTest::Examiner & examiner) {
					examiner.expect(linear(0.75, -10.0, 10.0)) == 5.0;
					examiner.expect(linear(-1.0, -10.0, 10.0)) == -30.0;
					examiner.expect(linear(0.0, -10.0, 10.0)) == -10.0;
					examiner.expect(linear(1.0, -10.0, 10.0)) == 10.0;
					examiner.expect(linear(2.0, -10.0, 10.0)) == 30.0;
				}
			},
			
			{"Cosine Interpolation",
				[](UnitTest::Examiner & examiner) {
					examiner.expect(cosine(0.0, -10.0, 10.0)) == -10.0;
					examiner.expect(cosine(1.0, -10.0, 10.0)) == 10.0;
				}
			},
			
			// Would be nice to have additional tests for other interpolation methods.
		};
	}
}
