#include "Camera.h"

using namespace Cefied::Game;

TypeSetup(Camera)

Camera::Camera(const IViewport & viewport) : 
	m_viewport(viewport),
    m_up     (XMVectorSet(0, 1,  0, 0)),
	m_forward(XMVectorSet(0, 0,  1, 0))
{ }

bool Camera::initialize()
{
	m_view = m_projection = XMMatrixIdentity();

	return true;
}

void Camera::project(XMVECTOR& v, int& xi, int& yi, float& depth) const
{
	int width, height;
	m_viewport.getSize(width, height);

	XMVECTOR p = XMVector3Project(v,
		0, 0,
		width, height,
		0, 1, // TODO: These should not be constants
		m_projection, m_view, XMMatrixIdentity());

	xi = (int)XMVectorGetX(p);
	yi = (int)XMVectorGetY(p);
	depth = XMVectorGetZ(p);
}

void Camera::unProject(const int xi, const int yi, const float depth, XMVECTOR& v) const
{
	int width, height;
	m_viewport.getSize(width, height);

	v = XMVector3Unproject(XMVectorSet((float)xi, (float)yi, depth, 1),
		0, 0,
		width, height,
		0, 1, // TODO: These should not be constants
		m_projection, m_view, XMMatrixIdentity());
}

void Camera::update(const GameTime& gameTime)
{
	{
		const auto focus = XMVector3Rotate(m_forward, m_transformation.getRotation());
		const auto up = XMVector3Rotate(m_up, m_transformation.getRotation());

		m_view = XMMatrixLookAtRH(m_transformation.getTranslation(), m_transformation.getTranslation() - focus, up);

		if (m_transformation.getParent() != nullptr)
		{
			m_view = m_transformation.getParent()->getMatrix() * m_view;
		}
	}

	// TODO: if viewport changed
	{
		int width, height;
		m_viewport.getSize(width, height);
		// TODO: The far and near planes should not be constants
		m_projection = XMMatrixPerspectiveFovRH(XM_PIDIV4, width / (float)height, .1f, 1000.0f);
	}
}