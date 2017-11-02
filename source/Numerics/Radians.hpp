//
//  Radians.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <cmath>

#include "Float.hpp"

namespace Numerics
{
	/// Radians to degrees multiplier
	constexpr double R2D = (180.0 / M_PI);
	constexpr double D2R = (M_PI / 180.0);
	
	template <typename NumericT>
	struct Number;
	
	template <typename FloatT = RealT>
	struct Radians
	{
		FloatT value;

		constexpr Radians() = default;

		explicit constexpr Radians(const FloatT & _value) : value(_value) {}

		// Implicit convertion between Radians<float> and Radians<double>.
		template <typename OtherNumericT>
		constexpr Radians(const Radians<OtherNumericT> angle) : value(angle.value) {}

		constexpr operator FloatT () const {
			return value;
		}

		constexpr Radians operator+ (const Radians & other) const {
			return Radians{value + other.value};
		}

		constexpr Radians operator- (const Radians & other) const {
			return Radians{value - other.value};
		}

		template <typename OtherT>
		constexpr Radians operator* (const OtherT & other) const {
			return Radians{value * other};
		}

		constexpr Radians operator/ (const Radians & other) const {
			return Radians{value / other.value};
		}

		template <typename OtherT>
		constexpr Radians operator/ (const OtherT & other) const {
			return Radians{value / other};
		}

		constexpr Radians operator- () const {
			return Radians{-value};
		}

		Radians & operator+= (const Radians & other) {
			value += other.value;
			return *this;
		}

		Radians & operator-= (const Radians & other) {
			value -= other.value;
			return *this;
		}

		template <typename OtherT>
		Radians & operator*= (const OtherT & other) {
			value *= other;
			return *this;
		}

		Radians & operator/= (const Radians & other) {
			value /= other.value;
			return *this;
		}

		template <typename OtherT>
		Radians & operator/= (const OtherT & other) {
			value /= other;
			return *this;
		}

		Number<FloatT> sin() const {
			return std::sin(value);
		}

		Number<FloatT> cos() const {
			return std::cos(value);
		}

		Number<FloatT> tan() const {
			return std::tan(value);
		}

		Radians offset_to (const Radians & other) const {
			auto x = this->value, y = other.value;
			return Radians{std::atan2(std::sin(x-y), std::cos(x-y))};
		}

		bool equivalent(const Radians & other) const {
			return Numerics::equivalent(this->offset_to(other).value, 0);
		}
	};

	template <typename FloatT>
	constexpr Radians<FloatT> radians(const FloatT & value) {
		return Radians<FloatT>{value};
	}

	template <typename FloatT>
	constexpr Radians<FloatT> degrees(const FloatT & value) {
		return Radians<FloatT>{value * D2R};
	}

	template <typename FloatT>
	constexpr Radians<FloatT> asin(const FloatT & value)
	{
		return Radians<FloatT>{std::asin(value)};
	}

	template <typename FloatT>
	constexpr Radians<FloatT> acos(const FloatT & value)
	{
		return Radians<FloatT>{std::asin(value)};
	}

	template <typename FloatT>
	constexpr Radians<FloatT> atan(const FloatT & value)
	{
		return Radians<FloatT>{std::asin(value)};
	}

	constexpr Radians<double> operator"" _rad(long double r) { return Radians<double>(r); }
	constexpr Radians<double> operator"" _deg(long double d) { return Radians<double>(d * D2R); }
	
	constexpr Radians<double> operator"" _rad(unsigned long long r) { return Radians<double>(r); }
	constexpr Radians<double> operator"" _deg(unsigned long long d) { return Radians<double>(static_cast<double>(d) * D2R); }
	
	extern template struct Radians<float>;
	extern template struct Radians<double>;
}
