//
//  Test.Float.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Float.hpp>
#include <Numerics/Number.hpp>
#include <Numerics/Radians.hpp>

namespace Numerics
{
	using namespace UnitTest::Expectations;
	
	template <typename FloatT>
	void calculate_accuracy(UnitTest::Examiner & examiner, FloatT at) {
		using F = FloatEquivalenceTraits<FloatT>;

		auto epsilon = EpsilonTraits<FloatT, 0>::EPSILON;
		
		// We compare the integral value of the float at 0, and 0+epsilon. As we move further from zero, this number would get smaller and smaller.
		auto zero = F::convert_to_integer(at);
		auto zero_plus_epsilon = F::convert_to_integer(at + EpsilonTraits<FloatT, 0>::EPSILON);

		examiner << "Accuracy of float(" << sizeof(FloatT) << ") at " << at << " = " << (zero_plus_epsilon - zero) << "; epsion = " << epsilon << std::endl;
	}
	
	UnitTest::Suite FloatTestSuite {
		"Numerics::Float",
		
		{"they are accurate",
			[](UnitTest::Examiner & examiner) {
				calculate_accuracy<float>(examiner, 0.0);
				calculate_accuracy<float>(examiner, 1.0);
				calculate_accuracy<double>(examiner, 0.0);
				calculate_accuracy<double>(examiner, 1.0);
			}
		},
		
		{"float is accurate to about 7 places",
			[](auto examiner) {
				examiner.expect(equivalent<float>(0.1, 0.1000000001)).to(be_true);
			}
		},
		
		{"double is accurate to more than 7 places",
			[](auto examiner) {
				examiner.expect(equivalent<double>(0.1, 0.1000000001)).to(be_false);
			}
		},
		
		{"similar numbers are equivalent",
			[](auto examiner) {
				auto a = number(0.000001f), b = number(0.0000011f); //, c = number(0.000002f);
				examiner.expect(a).to(be_equivalent(b));
			}
		},
		
		{"different numbers are not equivalent",
			[](auto examiner) {
				auto a = number(0.000001f), b = number(0.000002f);
				examiner.expect(a).to_not(be_equivalent(b));
			}
		},
		
		{"they can be equivalent",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(equivalent(1.0, 1.0)) == true;
				examiner.expect(equivalent(1.0, 1.1)) == false;
				examiner.expect(equivalent(0.0, -0.0)) == true;
			}
		},
		
		{"it can be converted to integral",
			[](UnitTest::Examiner & examiner) {
				examiner.check_equal(FloatEquivalenceTraits<float>::convert_to_integer(0.0f), 0);
				examiner.check_equal(FloatEquivalenceTraits<float>::convert_to_integer(-0.0f), 0);
				examiner.check_equal(FloatEquivalenceTraits<double>::convert_to_integer(0.0), 0);
				examiner.check_equal(FloatEquivalenceTraits<double>::convert_to_integer(-0.0), 0);

				examiner.check(FloatEquivalenceTraits<float>::equivalent(0.0f, -4.37114e-08f));

				typedef FloatEquivalenceTraits<float> F;

				// Convert an integer to a float and then back again - this process should be non-lossy.
				auto integer_value = F::convert_to_integer(1.5f);
				auto float_value = F::convert_to_float(integer_value);

				examiner.check_equal(1.5f, float_value);

				// cos(R90) should normally be considered zero:
				examiner << "cos(R90) is zero = " << R90.cos() << std::endl;
				examiner.expect(R90.cos()).to(be_equivalent(0.0));
			}
		},
	};
}
