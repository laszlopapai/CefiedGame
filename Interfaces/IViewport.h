#pragma once

#include "IAny.h"

namespace Cefied
{
	namespace Game
	{
		class IResizeable : public IAny
		{
		public:
			virtual bool resize(int width, int height) = 0;

			TypeInherit1(IAny)
		};

		// TODO: Is size changed
		class IViewport : public IAny {
		public:
			virtual void getPosition(int & left, int & top) const = 0;
			virtual void getSize(int & width, int & height) const = 0;
			virtual void setViewportResizedCallback(IResizeable * resizeable) = 0;
			virtual ~IViewport() = default;

			TypeInherit1(IAny)
		};
	}
}