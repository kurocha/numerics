//
//  Transforms.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 20/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Matrix.hpp>

namespace Numerics
{
	UnitTest::Suite TransformsTestSuite {
		"Numerics::Transforms",

		{"it can project into clip space using an orthographic projection",
			[](UnitTest::Examiner & examiner) {
				// This is the natural clip space box:
				auto min = vector(-1.0, -1.0, 0.0), max = vector(1.0, 1.0, 1.0);
				
				auto projection = Transforms::orthographic_projection(min, max);
				
				examiner.expect(Matrix<4, 4, double>(projection)) == Matrix<4, 4, double>(IDENTITY);
			}
		},
		
		// {"Unit Space Orthographic Projection",
		// 	[](UnitTest::Examiner & examiner) {
		// 		// This is the natural clip space box:
		// 		auto unit_box = Geometry::AlignedBox3::from_center_and_size(ZERO, Vec3{2, 2, 2});
		// 		
		// 		auto projection = orthographic_projection_matrix(unit_box);
		// 		
		// 		examiner.expect(projection * unit_box.corner({false, false, false})) == Vec3{-1, -1, 0};
		// 		examiner.expect(projection * unit_box.corner({true, true, true})) == Vec3{1, 1, 1};
		// 	}
		// },
	};
}
