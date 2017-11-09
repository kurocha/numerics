//
//  Test.Matrix.cpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Numerics/Matrix.hpp>

#include <Numerics/Transforms.hpp>

namespace Numerics
{
	/// Load a test patern into the matrix. Used for testing.
	template <std::size_t R, std::size_t C, typename NumericT>
	void load_test_pattern (Matrix<R, C, NumericT> & matrix) {
		NumericT i = 0;

		for (std::size_t c = 0; c < C; c += 1)
			for (std::size_t r = 0; r < R; r += 1)
				matrix.at(r, c) = i++;
	}

	UnitTest::Suite MatrixTestSuite {
		"Numerics::Matrix",

		{"Construction",
			[](UnitTest::Examiner & examiner) {
				float data[] = {
					1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
				};

				Mat44 m = data;

				examiner << "Constructing matrix from float array." << std::endl;
				examiner.check(m.at(3, 3) == 16);
			}
		},

		{"Identities",
			[](UnitTest::Examiner & examiner) {
				Mat44 a, I(IDENTITY);
				load_test_pattern(a);

				examiner << "Matricies are equivalent after multiplication by identity" << std::endl;
				examiner.check(a == a * I);
				examiner << "Matricies are equivalent after multiplication by identity" << std::endl;
				examiner.check(I * a == a);
			}
		},

		{"Transpose",
			[](UnitTest::Examiner & examiner) {
				Matrix<2, 3, int> m;
				load_test_pattern(m);

				auto mt = m.transpose();

				examiner.check_equal(m.at(0, 0), mt.at(0, 0));
				examiner.check_equal(m.at(1, 1), mt.at(1, 1));
				examiner.check_equal(m.at(0, 1), mt.at(1, 0));
				examiner.check_equal(m.at(1, 0), mt.at(0, 1));
			}
		},

		{"Transforms",
			[](UnitTest::Examiner & examiner) {
				Mat44 transform = Transforms::translate(Vec4(5, 5, 5, 1));

				Vec3 point = transform * Vec3(0, 0, 0);

				examiner.check_equal(point, vector<RealT>(5, 5, 5));
			}
		},

		{"Rotations",
			[](UnitTest::Examiner & examiner) {
				// These tests assume column basis, column major order (traditional mathematical notation):
				Mat44 xa = Transforms::rotate(R90, Vec3(1.0, 0.0, 0.0));
				Mat44 xb = Transforms::rotate<X>(R90);

				examiner.check(xa.equivalent(xb));

				Mat44 xc(IDENTITY);

				xc.at(1, 1) = 0;
				xc.at(1, 2) = -1;
				xc.at(2, 1) = 1;
				xc.at(2, 2) = 0;

				examiner << "Matrix rotation is correct" << std::endl;
				examiner.check(xa.equivalent(xc));

				Mat44 ya = Transforms::rotate(R90, Vec3(0.0, 1.0, 0.0));
				Mat44 yb = Transforms::rotate<Y>(R90);

				examiner.check(ya.equivalent(yb));

				Mat44 yc(IDENTITY);

				yc.at(0, 0) = 0;
				yc.at(0, 2) = 1;
				yc.at(2, 0) = -1;
				yc.at(2, 2) = 0;

				examiner << "Matrix rotation is correct" << std::endl;
				examiner.check(ya.equivalent(yc));

				Mat44 za = Transforms::rotate(R90, Vec3(0.0, 0.0, 1.0));
				Mat44 zb = Transforms::rotate<Z>(R90);

				examiner.check(za.equivalent(zb));

				Mat44 zc(IDENTITY);

				zc.at(0, 0) = 0;
				zc.at(0, 1) = -1;
				zc.at(1, 0) = 1;
				zc.at(1, 1) = 0;

				examiner << "Matrix rotation is correct" << std::endl;
				examiner.check(za.equivalent(zc));

				auto matrix_product = xa * ya * za;

				RealT sample_data[16] = {0, 0, 1, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
				Mat44 sample; sample.set(sample_data);

				examiner.check(matrix_product.equivalent(sample));

				// Check that the rotation between two vectors is correct:
				Mat44 rk = Transforms::rotate(Vec3(1.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), Vec3(0.0, 0.0, 1.0));

				Vec3 pk = rk * Vec3(1.0, 0.0, 0.0);

				examiner << "Rotation between vectors is correct" << std::endl;
				examiner.check(pk.equivalent(Vec3(0.0, 1.0, 0.0)));
			}
		},

		{"Vectors",
			[](UnitTest::Examiner & examiner) {
				Mat44 a(ZERO);
				Mat44 mat_ident(IDENTITY);
				Vec4 vec_ident(IDENTITY);

				a.set(0, 0, vec_ident, 5);

				examiner << "Vector was copied correctly" << std::endl;
				examiner.check(a == mat_ident);

				Vec4 c1(0, 1, 2, 3), c2(4, 5, 6, 7), c3(8, 10, 12, 14), c4(9, 11, 13, 15);
				a = ZERO;

				a.set(0, 0, c1, 1);
				a.set(0, 1, c2, 1);
				a.set(0, 2, c3, 2);
				a.set(1, 2, c4, 2);

				Mat44 test_pattern;
				load_test_pattern(test_pattern);

				examiner << "Vector was copied correctly" << std::endl;
				examiner.check(a == test_pattern);

				Mat44 b;

				Vec4 r, pt(10.0, 0.0, 0.0, 1.0);
				b = Transforms::rotate(R90, vector(0.0, 1.0, 0.0));

				r = b * pt;

				examiner << "Rotation was successful" << std::endl;
				examiner.check(r.equivalent(vector(0.0, 0.0, -10.0, 1.0)));

				b = Transforms::rotate(R180, vector(0.0, 1.0, 0.0)).around_origin(vector(0.0, 10.0, 10.0));

				r = b * pt;

				examiner << "Rotation was successful" << std::endl;
				examiner.check(r.equivalent(vector(-10.0, 0.0, -20.0, 1.0)));
			}
		},

		{"Transforms",
			[](UnitTest::Examiner & examiner) {
				Mat44 a = Transforms::rotate(R90, Vec3(1.0, 0.0, 0.0)) << Transforms::translate(Vec3(1.0, 2.0, 3.0));

				Vec4 r, pt(0.0, 0.0, 0.0, 1.0);

				r = a * pt;

				examiner.check(r.equivalent(Vec4(1.0, -3.0, 2.0, 1.0)));

				Mat44 b = Transforms::scale(2);

				Vec4 q = b * Vec4(1, 1, 1);

				examiner << "Uniform scale doubled size of vector" << std::endl;
				examiner.check(q.equivalent({2, 2, 2}));
			}
		},

		{"Inverse",
			[](UnitTest::Examiner & examiner) {
				Mat44 m1 = Transforms::rotate<X>(R90);
				Mat44 m2 = inverse(m1);

				examiner << "Matrix * Inverse Matrix is identity." << std::endl;
				examiner.check((m1 * m2).equivalent(IDENTITY));
			}
		},

		{"Eigenvalues",
			[](UnitTest::Examiner & examiner) {
				std::vector<Vec2> points = {
					{4, 16}, {5, 11}, {6, 14}, {7, 25}, {8, 20}, {9, 27}, {10, 34}, {11, 29}, {12, 40},
					{13, 43}, {14, 42}, {15, 49}, {16, 44}, {17, 47}, {18, 58}, {19, 61}, {20, 56}
				};

				Matrix<2, 17> A;
				Vec2 total;
				for (auto point : points) {
					total += point;
				}
				
				auto average = total / points.size();

				examiner.check_equal(average, Vec2{12, 36.235294342});

				for (std::size_t i = 0; i < points.size(); i += 1) {
					auto point = points[i];
					A.set(0, i, point - average);
				}

				//std::cerr << "A = " << A << std::endl;

				auto result = A * A.transpose();

				examiner.check_equal(result[0], 408);
				examiner.check_equal(result[1], 1236);
				examiner.check_equal(result[2], 1236);
				examiner.check_equal(result[3], 3983.05908203125);

				// 408 - l, 1236
				// 1236, 3983 - l
			}
		},
	};
}
