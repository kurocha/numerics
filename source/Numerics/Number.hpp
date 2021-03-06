//
//  Number.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 31/10/2008.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <cmath>

#include "Integer.hpp"
#include "Float.hpp"

#include <algorithm>

namespace Numerics
{
	template <typename NumericT>
	struct Radians;
	
	/// Zero type. Used in some Numerics constructors.
	enum Zero
	{
		ZERO = 0
	};

	/// Identity type. Used in some Numerics constructors.
	enum Identity
	{
		IDENTITY = 1
	};
	
	// Private base implementation of a variety of common numerical operations:
	namespace {
		template <typename NumericT>
		typename std::enable_if<std::is_floating_point<NumericT>::value, NumericT>::type
		/* NumericT */ _truncate (const NumericT & value, bool up = false) {
			return up ? std::ceil(value) : std::floor(value);
		}

		template <typename NumericT>
		typename std::enable_if<std::is_integral<NumericT>::value, NumericT>::type
		/* NumericT */ _truncate (const NumericT & value, bool) {
			return value;
		}

		template <typename ValueT, typename ModulusT>
		typename std::enable_if<std::is_floating_point<ValueT>::value || std::is_floating_point<ModulusT>::value, ModulusT>::type
		/* NumericT */ _modulo (const ValueT & value, const ModulusT & modulus) {
			return std::fmod(value, modulus);
		}

		template <typename ValueT, typename ModulusT>
		typename std::enable_if<std::is_integral<ValueT>::value && std::is_integral<ModulusT>::value, ValueT>::type
		/* NumericT */ _modulo (const ValueT & value, const ModulusT & modulus) {
			return value % modulus;
		}

		template <typename BaseT, typename ExponentT>
		typename std::enable_if<std::is_floating_point<BaseT>::value || std::is_signed<ExponentT>::value, BaseT>::type
		/* BaseT */ _raise (const BaseT & base, const ExponentT & exponent) {
			return std::pow(base, exponent);
		}

		template <typename BaseT, typename ExponentT>
		typename std::enable_if<std::is_integral<BaseT>::value && std::is_unsigned<ExponentT>::value, BaseT>::type
		/* BaseT */ _raise (BaseT base, ExponentT exponent) {
			BaseT result = 1;

			while (true) {
				if ((exponent & 1) == 1)
					result *= base;
				
				exponent >>= 1;
				
				// We specifically check here whether we need to continue further to avoid overflow of base.
				if (exponent == 0) break;
				
				base *= base;
			}
			
			return result;
		}
	}

	/// Functionality common to all numeric types.
	template <typename NumericT>
	struct Number
	{
		static_assert(std::is_arithmetic<NumericT>::value, "Number can only work with arithmetic data-types!");
		using RealT = typename RealTypeTraits<NumericT>::RealT;

		NumericT value;

		template <typename ValueT>
		static ValueT value_of(const ValueT & value)
		{
			return value;
		}

		template <typename ValueT>
		static ValueT value_of(const Number<ValueT> & number)
		{
			return number.value;
		}

		constexpr Number (const NumericT & value_) : value(value_)
		{
		}

		template <typename OtherNumericT>
		constexpr Number (const OtherNumericT & value_) : value(value_)
		{
		}

		operator NumericT & ()
		{
			return value;
		}

		operator const NumericT & () const
		{
			return value;
		}

		constexpr Number clamp (const Number & lower = 0, const Number & upper = 1) const
		{
			return value < lower.value ? lower : (value > upper.value ? upper : *this);
		}

		constexpr Number absolute() const
		{
			return value * (value < 0 ? -1 : 1);
		}

		Number truncate(bool up = false) const
		{
			return _truncate(value, up);
		}

		Number modulo(const Number & modulus) const
		{
			return _modulo(value, (NumericT)modulus);
		}

		Number wrap(const Number & modulus) const
		{
			return modulo(modulus) + (value < 0 ? modulus : (Number)0);
		}

		template <typename ExponentT>
		Number raise(const ExponentT & exponent) const
		{
			return _raise(value, value_of(exponent));
		}

		bool equivalent(const Number & other) const
		{
			return Numerics::equivalent(value, other.value);
		}

		Number<RealT> square_root() const
		{
			return std::sqrt(value);
		}

		Number fraction() const
		{
			return value - _truncate(value);
		}

		template <typename OtherNumericT>
		Number operator+ (const OtherNumericT & other) const
		{
			return value + other;
		}

		template <typename OtherNumericT>
		Number operator- (const OtherNumericT & other) const
		{
			return value - other;
		}

		template <typename OtherNumericT>
		Number operator* (const OtherNumericT & other) const
		{
			return value * other;
		}

		template <typename OtherNumericT>
		Number operator/ (const OtherNumericT & other) const
		{
			return value / other;
		}

		template <typename OtherNumericT>
		Number operator%(const OtherNumericT & modulus) const
		{
			return modulo(modulus);
		}

		Number max (const Number & other) const
		{
			return std::max(value, other.value);
		}

		Number min (const Number & other) const
		{
			return std::min(value, other.value);
		}
		
		Radians<RealT> asin() const
		{
			return Radians<RealT>{std::asin(value)};
		}
		
		Radians<RealT> acos() const
		{
			return Radians<RealT>{std::acos(value)};
		}
		
		Radians<RealT> atan() const
		{
			return Radians<RealT>{std::atan(value)};
		}
		
		template <typename OtherNumericT>
		Number<OtherNumericT> as()
		{
			return value;
		}
	};

	template <typename NumericT>
	inline constexpr Number<NumericT> number(const Number<NumericT> & value)
	{
		return value;
	}

	template <typename NumericT>
	inline constexpr Number<NumericT> number(const NumericT & value)
	{
		return {value};
	}
}
