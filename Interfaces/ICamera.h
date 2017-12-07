#pragma once

#include "IObject.h"
#include "Scene/Transformation.h"

namespace Cefied
{
	namespace Game
	{
		class ICamera : public IObject {
		public:
			virtual Transformation & getTransformation() = 0;
			virtual const XMMATRIX & getView() const = 0;
			virtual const XMMATRIX & getProjection() const = 0;
			virtual const XMVECTOR & getPosition() const = 0;

			virtual void project(XMVECTOR &v, int &xi, int &yi, float &depth) const = 0;
			virtual void unProject(int xi, int yi, float depth, XMVECTOR &v) const = 0;

			virtual ~ICamera() = default;

			TypeInherit1(IObject)
		};
	}
}
