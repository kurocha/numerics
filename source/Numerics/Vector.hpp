//
//  Vector.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 19/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Number.hpp"
#include "Float.hpp"
#include "Interpolate.hpp"

#include <type_traits>
#include <array>

namespace Numerics
{
	// Convenient/readable indexes for vectors.
	enum {
		X = 0, Y = 1, Z = 2, W = 3
	};
	
	template <std::size_t D, typename NumericT = RealT>
	class Vector : public std::array<NumericT, D>
	{
	public:
		/// Empty constructor. Value of vector is undefined.
		Vector() {}

		Vector(const NumericT & value)
		{
			this->fill(value);
		}
		
		template <typename... TailT>
		Vector(const NumericT & head, const TailT... tail) : std::array<NumericT, D>{{head, (NumericT)tail...}} {}

		template <std::size_t E, typename OtherNumericT>
		Vector(const Vector<E, OtherNumericT> & other)
		{
			auto next = std::copy(other.begin(), other.begin() + std::min(D, E), this->begin());
			std::fill(next, this->end(), 0);
		}
		
		bool operator==(const Vector & other) const noexcept
		{
			return reduce(true, other, [](bool r, NumericT a, NumericT b){return r && (a == b);});
		}
		
		bool operator!=(const Vector & other) const noexcept
		{
			return !((*this) == other);
		}
		
		bool equivalent(const Vector & other) const
		{
			return reduce(true, other, [](bool r, NumericT a, NumericT b){return r && number(a).equivalent(b);});
		}
		
		template <typename OtherT>
		Vector & operator+=(const OtherT & other)
		{
			return fold(other, [](NumericT a, NumericT b){return a + b;});
		}
		
		template <typename OtherT>
		Vector operator+(const OtherT & other) const
		{
			return Vector(*this) += other;
		}
		
		template <typename OtherT>
		Vector & operator-=(const OtherT & other)
		{
			return fold(other, [](NumericT a, NumericT b){return a - b;});
		}
		
		template <typename OtherT>
		Vector operator-(const OtherT & other) const
		{
			return Vector(*this) -= other;
		}
		
		template <typename OtherT>
		Vector & operator*=(const OtherT & other)
		{
			return fold(other, [](auto a, auto b){return a * b;});
		}
		
		template <typename OtherT>
		Vector operator*(const OtherT & other) const
		{
			return Vector(*this) *= other;
		}
		
		template <typename OtherT>
		Vector & operator/=(const OtherT & other)
		{
			return fold(other, [](auto a, auto b){return a / b;});
		}
		
		template <typename OtherT>
		Vector operator/(const OtherT & other) const
		{
			return Vector(*this) /= other;
		}
		
		template <typename OtherT>
		Vector & operator%=(const OtherT & other)
		{
			return fold(other, [](auto a, auto b){return a % b;});
		}
		
		template <typename OtherT>
		Vector operator%(const OtherT & other) const
		{
			return Vector(*this) %= other;
		}
		
		template <typename OtherT>
		Vector & operator&=(const OtherT & other)
		{
			return fold(other, [](auto a, auto b){return a & b;});
		}
		
		template <typename OtherT>
		Vector operator&(const OtherT & other) const
		{
			return Vector(*this) &= other;
		}
		
		template <typename OtherT>
		Vector & operator|=(const OtherT & other)
		{
			return fold(other, [](auto a, auto b){return a | b;});
		}
		
		template <typename OtherT>
		Vector operator|(const OtherT & other) const
		{
			return Vector(*this) |= other;
		}
		
		Vector operator!() const noexcept
		{
			return Vector(*this).fold([](NumericT a){return !a;});
		}
		
		/// Returns a vector with F components, by default one less than the current size.
		template <std::size_t E = D - 1>
		Vector<E, NumericT> reduce() const
		{
			static_assert(E <= D, "Cannot reduce size of vector to larger size");

			Vector<E, NumericT> result;

			std::copy(this->begin(), this->begin() + E, result.begin());

			return result;
		}

		template <typename... ArgumentsT>
		Vector<D+sizeof...(ArgumentsT), NumericT> append(ArgumentsT... arguments) const
		{
			Vector<D+sizeof...(ArgumentsT), NumericT> result;

			auto next = std::copy(this->begin(), this->end(), result.begin());

			NumericT tail[] = {(NumericT)arguments...};
			std::copy(std::begin(tail), std::end(tail), next);

			return result;
		}
		
		Number<NumericT> sum() const
		{
			return reduce(static_cast<NumericT>(0), [](NumericT a, NumericT b){return a+b;});
		}
		
		Number<NumericT> product() const
		{
			return reduce(static_cast<NumericT>(1), [](NumericT a, NumericT b){return a*b;});
		}
		
		template <typename OtherT>
		Number<NumericT> dot(const OtherT & other) const
		{
			return ((*this) * other).sum();
		}
		
		/// Return the length of the vector squared.
		/// This method avoids calculating the square root, therefore is faster when you only need to compare the relative lengths of vectors.
		Number<NumericT> length_squared() const
		{
			return dot(*this);
		}

		/// Return the length of the vector.
		auto length() const
		{
			return length_squared().square_root();
		}
		
		/// Normalize the vector to the given length. Defaults to 1.
		Vector & normalize(const NumericT & length, const NumericT & desired_length)
		{
			if (!number(length).equivalent(desired_length)) {
				auto factor = desired_length / length;
				
				(*this) *= factor;
			}
			
			return *this;
		}

		/// Normalize the vector to the given length. Defaults to 1.
		Vector normalize(const NumericT & desired_length = 1) const
		{
			auto current_length = length();
			
			// Can't normalize zero length vector.
			if (current_length.equivalent(0)) return *this;
			
			return Vector(*this).normalize(current_length, desired_length);
		}
		
		/// Calculates the angle between this vector and another.
		Radians<NumericT> angle_between(const Vector & other) const
		{
			auto r = dot(other) / (length() * other.length());

			return number(r).clamp(-1, 1).acos();
		}
		
		/// Reflect a vector around a given normal.
		/// Incoming directions are reflected about the normal.
		Vector reflect(const Vector & normal) const
		{
			return (*this) - (normal * (2.0 * this->dot(normal)));
		}
		
		/// Distribute an index into a given space.
		/// For example, when considering a size vector <tt><10, 15></tt>, we have defined a space which is 10 units wide and 15 units high. Therefore, an index in the range 0 to 9 will be in the first row, and 10 to 19 will be in the second row.
		/// @sa index
		Vector distribute(NumericT k) const
		{
			Vector r(ZERO);
			
			NumericT m = this->product();

			if (m == 0) return ZERO;

			for (std::size_t i = D; i > 0; i -= 1) {
				m /= (*this)[i-1];

				r[i-1] = number(k / m).truncate();

				k = number(k).modulo(m);
			}

			//assert(m == 1);

			return r;
		}

		/// Given a size vector (this) and a coordinate, return an index.
		/// @sa distribute
		Number<NumericT> index(const Vector & coord) const
		{
			NumericT idx = 0;
			NumericT m = 1;

			//		x(1) + y(sx) + z(sx*sy)
			for (std::size_t i = 0; i < D; i += 1) {
				idx += coord[i] * m;
				m *= (*this)[i];
			}
			
			return idx;
		}

		/// Clamp all components of the vector between given values.
		Vector clamp(const NumericT & min = 0, const NumericT & max = 1) const
		{
			Vector result;

			for (std::size_t i = 0; i < D; ++i)
				result[i] = number((*this)[i]).clamp(min, max);

			return result;
		}
		
		// Raise components of the vector to the given power.
		Vector raise(const NumericT & power) const
		{
			Vector result;
			
			for (std::size_t i = 0; i < D; ++i)
				result[i] = number((*this)[i]).raise(power);

			return result;
		}

		/// Linearly interpolate from (-1...1) between the smallest and largest components of the individual axes.
		Vector constrain(const Vector & b, const Vector<D, RealT> & constraints) const
		{
			const Vector & a = *this;

			Vector result;

			for (std::size_t i = 0; i < D; i += 1) {
				NumericT minimum = a[i], maximum = b[i];

				if (minimum > maximum) std::swap(minimum, maximum);

				result[i] = Interpolate::linear(constraints[i], minimum, maximum);
			}

			return result;
		}

		/// Linearly interpolate from (-1...1) between the smallest and largest components of the individual axes.
		Vector constrain(const Vector & b, bool maximum = true) const
		{
			const Vector & a = *this;

			Vector result;

			for (std::size_t i = 0; i < D; i += 1) {
				if (!maximum)
					result[i] = std::min(a[i], b[i]);
				else
					result[i] = std::max(a[i], b[i]);
			}

			return result;
		}

		/// Calculate the absolute value of the vector:
		Vector absolute() const
		{
			Vector result;

			for (std::size_t i = 0; i < D; ++i)
				result[i] = number((*this)[i]).absolute();

			return result;
		}

		/// Truncate the values of the vector:
		Vector truncate(bool up = false) const
		{
			Vector result;

			for (std::size_t i = 0; i < D; ++i)
				result[i] = number((*this)[i]).truncate(up);

			return result;
		}

		Vector fraction() const
		{
			return *this - truncate();
		}
		
	private:
		template <typename FunctionT>
		Vector & fold(FunctionT function)
		{
			for (auto i = 0; i < D; i += 1)
				(*this)[i] = function((*this)[i]);
			
			return *this;
		}
		
		template <typename FunctionT>
		Vector & fold(const Number<NumericT> & other, FunctionT function)
		{
			return fold(other.value, function);
		}
		
		template <typename FunctionT>
		Vector & fold(const NumericT & other, FunctionT function)
		{
			for (auto i = 0; i < D; i += 1)
				(*this)[i] = function((*this)[i], other);
			
			return *this;
		}
		
		template <typename FunctionT>
		Vector & fold(const Vector & other, FunctionT function)
		{
			for (auto i = 0; i < D; i += 1)
				(*this)[i] = function((*this)[i], other[i]);
			
			return *this;
		}
		
		template <typename ValueT, typename FunctionT>
		ValueT reduce(ValueT value, FunctionT function) const
		{
			for (auto i = 0; i < D; i += 1)
				value = function(value, (*this)[i]);
			
			return value;
		}
		
		template <typename ValueT, typename FunctionT>
		ValueT reduce(ValueT value, const Vector & other, FunctionT function) const
		{
			for (auto i = 0; i < D; i += 1)
				value = function(value, (*this)[i], other[i]);
			
			return value;
		}
	};
	
	// Construct a vector.
	template <typename HeadT, typename... TailT>
	constexpr inline Vector<1+sizeof...(TailT), HeadT> vector(const HeadT & head, TailT... tail)
	{
		return {head, (HeadT)tail...};
	}
	
	/// The 3-dimentional cross product:
	template <typename NumericT>
	Vector<3, NumericT> cross_product(const Vector<3, NumericT> & u, const Vector<3, NumericT> & v) 
	{
		Vector<3, NumericT> result;

		result[0] = u[1] * v[2] - u[2] * v[1];
		result[1] = u[2] * v[0] - u[0] * v[2];
		result[2] = u[0] * v[1] - u[1] * v[0];

		return result;
	}

	/// The 4-dimentional cross product:
	template <typename NumericT>
	Vector<4, NumericT> cross_product(const Vector<4, NumericT> & u, const Vector<4, NumericT> & v, const Vector<4, NumericT> & w)
	{
		Vector<4, NumericT> result;

		// intermediate values
		NumericT a, b, c, d, e, f;

		// calculate intermediate values.
		a = (v[0] * w[1]) - (v[1] * w[0]);
		b = (v[0] * w[2]) - (v[2] * w[0]);
		c = (v[0] * w[3]) - (v[3] * w[0]);
		d = (v[1] * w[2]) - (v[2] * w[1]);
		e = (v[1] * w[3]) - (v[3] * w[1]);
		f = (v[2] * w[3]) - (v[3] * w[2]);

		// calculate the result-vector components.
		result[0] =  (u[1] * f) - (u[2] * e) + (u[3] * d);
		result[1] = -(u[0] * f) + (u[2] * c) - (u[3] * b);
		result[2] =  (u[0] * e) - (u[1] * c) + (u[3] * a);
		result[3] = -(u[0] * d) + (u[1] * b) - (u[2] * a);

		return result;
	}

	/// Calculates the surface normal of a triangle given by three points.
	template <typename NumericT>
	Vector<3, NumericT> surface_normal(const Vector<3, NumericT> & a, const Vector<3, NumericT> & b, const Vector<3, NumericT> & c)
	{
		Vector<3, NumericT> a1 = b - a;
		Vector<3, NumericT> b1 = c - b;

		return cross_product(a1, b1).normalize();
	}

	// Calculate a clockwise normal to the 2d vector.
	template <typename NumericT>
	Vector<2, NumericT> normal(const Vector<2, NumericT> & direction)
	{
		return Vector<2, NumericT>(direction[1], -direction[0]);
	}
	
	using Vec2 = Vector<2>;
	using Vec3 = Vector<3>;
	using Vec4 = Vector<4>;
	
	extern template class Vector<3, float>;
	extern template class Vector<4, float>;
}
