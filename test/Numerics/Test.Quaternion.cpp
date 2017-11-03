//
//  Test.Quaternion.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Quaternion.hpp>
#include <Numerics/Matrix.hpp>
#include <Numerics/Transforms.hpp>

#include <iomanip>

namespace Numerics
{
	using namespace UnitTest::Expectations;
	
	UnitTest::Suite QuaternionTestSuite {
		"Numerics::Quaternion",
		
		{"it has correct rotation axis",
			[](auto & examiner) {
				Quaternion<double> q(90_deg, {1.0, 0.0, 0.0});
				
				examiner.expect(q.axis()).to(be_equivalent<Vector<3, double>>({1.0, 0.0, 0.0}));
			}
		},
		
		{"it has correct rotation angle",
			[](auto & examiner) {
				Quaternion<double> q(90_deg, {1.0, 0.0, 0.0});
				
				examiner.expect(q.angle()).to(be_equivalent(90_deg));
			}
		},
		
		{"it can generate equivalent rotation matrix",
			[](auto & examiner) {
				Quaternion<double> q(90_deg, {1.0, 0.0, 0.0});
				
				Matrix<4, 4, double> a = rotate(90_deg, vector(1.0, 0.0, 0.0));
				Matrix<4, 4, double> b = q;
				
				examiner.expect(a).to(be_equivalent(b));
			}
		},

		{"Multiplication",
			[](UnitTest::Examiner & examiner) {
				Quaternion<>q = rotate(90_deg, vector(1.0, 0.0, 0.0));
				Matrix<4,4> m = rotate(90_deg, vector(1.0, 0.0, 0.0));

				Vec3 si(15.0, -12.5, 4.0);

				Vec3 r1, r2;

				r1 = q * si;
				r2 = m * si;

				examiner << "Represented rotation is same" << std::endl;
				examiner.expect(r1).to(be_equivalent(r2));

				Quaternion<>a(90_deg, Vec3(1, 0, 0).normalize());
				Quaternion<>b(90_deg, Vec3(0, 1, 0).normalize());
				Quaternion<>c = a.rotation_to(b);

				examiner << "Quaternion rotation is correct" << std::endl;
				examiner.expect(a).to(be_equivalent(Vec4(0.707107, 0, 0, 0.707107)));
				examiner << "Quaternion rotation is correct" << std::endl;
				examiner.expect(b).to(be_equivalent(Vec4(0, 0.707107, 0, 0.707107)));
				examiner << "Quaternion rotation is correct" << std::endl;
				examiner.expect(c).to(be_equivalent(Vec4(-0.5, 0.5, -0.5, 0.5)));

				examiner << "Quaternion conjugate is correct" << std::endl;
				examiner.expect(a.conjugate()).to(be_equivalent(Vec4(-0.707107, 0, 0, 0.707107)));

				examiner << "Rotations are equivalent" << std::endl;
				examiner.expect(a * c).to(be_equivalent(b));
			}
		},

		{"Rotation Matrix",
			[](UnitTest::Examiner & examiner) {
				Quaternion<>identity(IDENTITY);
				Matrix<4,4> m1 = identity;

				examiner << "X axis is correct" << std::endl;
				examiner.expect((m1 * Vec3(1, 0, 0))).to(be_equivalent(Vec3(1, 0, 0)));
				examiner << "Y axis is correct" << std::endl;
				examiner.expect((m1 * Vec3(0, 1, 0))).to(be_equivalent(Vec3(0, 1, 0)));
				examiner << "Z axis is correct" << std::endl;
				examiner.expect((m1 * Vec3(0, 0, 1))).to(be_equivalent(Vec3(0, 0, 1)));

				Quaternion<>a(90_deg, Vec3(1, 0, 0).normalize());
				Matrix<4,4> m2 = a;

				examiner << "X axis is correct" << std::endl;
				examiner.expect((m2 * Vec3(1, 0, 0))).to(be_equivalent(Vec3(1, 0, 0)));
				examiner << "Y axis is correct" << std::endl;
				examiner.expect((m2 * Vec3(0, 1, 0))).to(be_equivalent(Vec3(0, 0, 1)));
				examiner << "Z axis is correct" << std::endl;
				examiner.expect((m2 * Vec3(0, 0, 1))).to(be_equivalent(Vec3(0, -1, 0)));
			}
		},
		
		{"Transforms",
			[](UnitTest::Examiner & examiner) {
				Quaternion<> q1 = rotate(90_deg, Vec3(1, 0, 0));

				examiner.expect(q1.angle()).to(be_equivalent(90_deg));
				examiner.expect(q1.axis()).to(be_equivalent(Vec4{1, 0, 0}));

				Quaternion<> q2 = rotate<X>(90_deg);

				examiner.expect(q1).to(be_equivalent(q2));

				Quaternion<> q3 = rotate(90_deg, Vec3(1, 0, 0)) << rotate(-90_deg, Vec3(1, 0, 0));

				examiner.expect(q3).to(be_equivalent(Quaternion<>(IDENTITY)));
			}
		},
	};
}
