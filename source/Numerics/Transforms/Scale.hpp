//
//  Scale.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 4/11/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Sequence.hpp"

#include "../Vector.hpp"

namespace Numerics
{
	namespace Transforms
	{
		template <std::size_t E, typename NumericT>
		struct Scale {
			Vector<E, NumericT> factor;

			bool identity () const { return factor == 1; }

			template <typename RightT>
			Sequence<Scale, RightT> operator<< (RightT && right) const
			{
				return {*this, right};
			}
		};

		template <std::size_t E, typename NumericT>
		Scale<E, NumericT> scale(const Vector<E, NumericT> & factor) {
			return {factor};
		}

		template <typename NumericT>
		struct UniformScale {
			NumericT factor;

			bool identity () const { return factor == 1; }

			template <typename RightT>
			Sequence<UniformScale, RightT> operator<< (const RightT & right) const
			{
				return {*this, right};
			}
		};

		template <typename NumericT>
		UniformScale<NumericT> scale(const NumericT & factor) {
			return {factor};
		}
	}
}
