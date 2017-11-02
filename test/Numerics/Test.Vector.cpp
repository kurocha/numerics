//
//  Test.Vector.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 20/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Vector.hpp>
#include <Numerics/Radians.hpp>

namespace Numerics
{
	UnitTest::Suite VectorTestSuite {
		"Numerics::Vector",
		
		{"it can do basic vector arithmetic",
			[](UnitTest::Examiner & examiner) {
				Vector<4> a = {1, 2, 3, 4}, b = {-1, 2, -3, 4};
				
				examiner.expect(a+b) == Vector<4>{0, 4, 0, 8};
				examiner.expect(a-b) == Vector<4>{2, 0, 6, 0};
				examiner.expect(a*b) == Vector<4>{-1, 4, -9, 16};
				examiner.expect(a/b) == Vector<4>{-1, 1, -1, 1};
			}
		},
		
		{"can compute length",
			[](UnitTest::Examiner & examiner) {
				Vector<4> a = {4, 4, 4, 4};
				
				examiner.expect(a.length()) == 8;
			}
		},
		
		{"it can normalize length",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(vector(0.0, 0.0, 2.5).normalize()) == vector(0.0, 0.0, 1.0);
				examiner.expect(vector(0.0, 0.0, 2.5).normalize(2)) == vector(0.0, 0.0, 2.0);
			}
		},
	};
}
