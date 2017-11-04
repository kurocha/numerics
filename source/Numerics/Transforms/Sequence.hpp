//
//  Sequence.hpp
//  This file is part of the "Numerics" project and released under the MIT License.
//
//  Created by Samuel Williams on 24/11/2012.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

namespace Numerics
{
	namespace Transforms
	{
		template <typename LeftT, typename RightT>
		struct Sequence
		{
			LeftT left;
			RightT right;

		protected:
			template <typename ApplyT, typename TransformT>
			void apply(ApplyT & to, const TransformT & transform) const
			{
				//if (!transform.identity())
				to *= ApplyT(transform);
			}

			template <typename ApplyT, typename A, typename B>
			void apply(ApplyT & to, const Sequence<A, B> & sequence) const
			{
				apply(to, sequence.left);
				apply(to, sequence.right);
			}

		public:
			template <typename ApplyT>
			void apply(ApplyT & to) const
			{
				apply(to, *this);
			}
		};

		template <typename A, typename B, typename RightT>
		Sequence<Sequence<A, B>, RightT> operator<<(Sequence<A, B> && sequence, RightT && right)
		{
			return {sequence, right};
		}
	}
}
