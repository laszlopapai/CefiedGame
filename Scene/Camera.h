#pragma once

#include "Interfaces/ICamera.h"
#include "Interfaces/IViewport.h"

using namespace DirectX;

namespace Cefied
{
	namespace Game
	{
		class Camera : public ICamera
		{
		public:
			explicit Camera(const IViewport & viewport);
			bool initialize() override;
			void project(XMVECTOR &v, int &xi, int &yi, float &depth) const override;
			void unProject(int xi, int yi, float depth, XMVECTOR &v) const override;
			void update(const GameTime& gameTime) override;


			Transformation& getTransformation() override
			{
				return m_transformation;
			}

			const XMMATRIX& getView() const override
			{
				return m_view;
			}

			const XMMATRIX& getProjection() const override
			{
				return m_projection;
			}

			const XMVECTOR & getPosition() const override
			{
				return m_transformation.getTranslation();
			}

		private:
			const IViewport  &m_viewport;
			Transformation    m_transformation;
			
			XMMATRIX          m_view;
			XMMATRIX          m_projection;

			const XMVECTOR    m_up;
			const XMVECTOR    m_forward;

			TypeInherit1(ICamera)
		};
	}
}
