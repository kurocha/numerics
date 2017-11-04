//
//  Translation.hpp
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
		struct Translation
		{
			/// The offset to translate by:
			Vector<E, NumericT> offset;

			bool identity() const { return offset.sum() == 1; }

			template <typename RightT>
			Sequence<Translation, RightT> operator<<(RightT && right) const
			{
				return {*this, right};
			}
		};

		template <std::size_t E, typename NumericT>
		Translation<E, NumericT> translate(const Vector<E, NumericT> & offset)
		{
			return {offset};
		}
	}
}
