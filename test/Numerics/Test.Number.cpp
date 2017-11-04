
#include <UnitTest/UnitTest.hpp>

#include <Numerics/Number.hpp>
#include <Numerics/Radians.hpp>

namespace Numerics
{
	UnitTest::Suite NumberTestSuite {
		"Numerics::Numbers",

		{"it can do basic arithmetic correctly",
			[](UnitTest::Examiner & examiner) {
				auto n = number(10.0);

				// Basic operators:
				examiner.check_equal(n + 1, 11.0);
				examiner.check_equal(n - 1, 9.0);
				examiner.check_equal(n * 2, 20.0);
				examiner.check_equal(n / 2, 5.0);

				// Modulo operator:
				examiner.check_equal(n % 3, 1.0);
			}
		},
		
		{"it can construct from IDENTITY",
			[](UnitTest::Examiner & examiner) {
				float a = IDENTITY;
				examiner.check_equal(a, 1.0f);
			}
		},
		
		{"it can construct from ZERO",
			[](UnitTest::Examiner & examiner) {
				float a = ZERO;
				examiner.check_equal(a, 0.0f);
			}
		},
		
		{"it can clamp numbers",
			[](UnitTest::Examiner & examiner) {
				// Using default 0...1 range:
				examiner.check_equal(number(-10).clamp(), 0);
				examiner.check_equal(number(10).clamp(), 1);
				examiner.check_equal(number(0.5).clamp(), 0.5);

				// Custom ranges:
				examiner.check_equal(number(1).clamp(5, 20), 5);
				examiner.check_equal(number(25).clamp(5, 20), 20);
				examiner.check_equal(number(15).clamp(5, 20), 15);
			}
		},

		{"it can compute absolute values",
			[](UnitTest::Examiner & examiner) {
				// Using default 0...1 range:
				examiner.check_equal(number(-10).absolute(), 10);
				examiner.check_equal(number(10).absolute(), 10);
			}
		},

		{"it can truncate to integers",
			[](UnitTest::Examiner & examiner) {
				auto n = number(10.1);

				examiner.check_equal(n.truncate(), 10);
				examiner.check_equal(n.truncate(true), 11);

				auto m = number(12);

				examiner.check_equal(m.truncate(), 12);
				examiner.check_equal(m.truncate(true), 12);
			}
		},

		{"it can compute modulos",
			[](UnitTest::Examiner & examiner) {
				// Using default 0...1 range:
				examiner.check_equal(number(10).modulo(4), 2);
				
				examiner.check(number(10.2).modulo(2.4).equivalent(0.6));
				examiner.check(number(10).as<double>().modulo(2.4).equivalent(0.4));

				examiner.check_equal(number(-55).wrap(10), 5);
				examiner.check_equal(number(55).wrap(10), 5);

				examiner.check_equal(number(1.5).fraction(), 0.5);
			}
		},

		{"it can compute exponents",
			[](UnitTest::Examiner & examiner) {
				examiner.check_equal(number(5).raise(3U), 125);
				examiner.check_equal(number(-5).raise(3U), -125);

				examiner.check_equal(number(5).raise(9U), 1953125);
				examiner.check_equal(number(5.0).raise(9U), 1953125);

				examiner.check_equal(number(36).raise(2U), 1296);
				examiner.check_equal(number(1296).square_root(), 36);
			}
		},

		{"it can generate angles",
			[](UnitTest::Examiner & examiner) {
				examiner.check_equal(90_deg, (90_deg).sin().asin());
			}
		},
	};
}
