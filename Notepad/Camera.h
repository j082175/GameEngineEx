#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using namespace DirectX;

class Camera
{
public:
	Camera()
	{
		updateViewDir();
	}

	Vector3 GetEyePos()
	{
		return m_position;
	}

	void GetViewProj(Matrix& viewProj)
	{
		Matrix viewRow = XMMatrixLookAtLH(m_position, m_viewDir, m_upDir);
		Matrix projRow = Matrix();

		if (m_usePerspectiveProjection)
		{
			projRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), m_aspectRatio, m_nearZ, m_farZ);
		}
		else
		{
			projRow = XMMatrixOrthographicOffCenterLH(-m_aspectRatio, m_aspectRatio, -1.0f, 1.0f, 0.1f, 10.0f);
		}

		viewProj = (viewRow * projRow).Transpose();
	}

	void SetAspectRatio(float screenWidth, float screenHeight)
	{
		m_aspectRatio = screenWidth / screenHeight;
	}

private:
	void updateViewDir()
	{
		//m_viewDir

		m_rightDir = m_upDir.Cross(m_viewDir);
	}


private:
	Vector3 m_position = Vector3(0.f, 0.f, -2.f);
	Vector3 m_upDir = Vector3(0.f, 1.f, 0.f);
	Vector3 m_viewDir = Vector3(0.f, 0.f, 1.f);
	Vector3 m_rightDir = Vector3(0.f, 1.f, 0.f);
	float m_aspectRatio;
	float m_nearZ = 0.01f;
	float m_farZ = 10.f;

	bool m_usePerspectiveProjection = true;
};

