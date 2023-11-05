#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <wrl.h> // ComPtr
#include "Camera.h"

namespace hlab
{

	using Microsoft::WRL::ComPtr;
	using std::vector;
	using std::wstring;

	// ��� �������� ���������� ����� ��ɵ��� ������ �ִ�
	// �θ� Ŭ����
	class AppBase
	{
	public:
		AppBase();
		virtual ~AppBase();
		int Run();
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	protected: // ��� ���� Ŭ���������� ���� ����
		virtual bool Initialize();
		virtual void UpdateGUI() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		bool InitMainWindow();
		bool InitDirect3D();
		bool InitGUI();

		float GetAspectRatio() const;
		// Convenience overrides for handling mouse input.
		virtual void OnMouseDown(WPARAM btnState, int x, int y) {};
		virtual void OnMouseUp(WPARAM btnState, int x, int y) {};
		virtual void OnMouseMove(WPARAM btnState, int x, int y) {};
	private:


	public:
		// ���� �̸� ���̴� ��Ģ�� VS DX11/12 �⺻ ���ø��� �����ϴ�.
		// �ٸ� ���� �̸��� ���̱� ���� d3d�� �����߽��ϴ�.
		// ��: m_d3dDevice -> m_device
		int m_screenWidth; // �������� ���� ȭ���� �ػ�
		int m_screenHeight;
		HWND m_mainWindow;
		Camera m_camera;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;

		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11RasterizerState> m_rasterizerSate;

		// Depth buffer ����
		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		D3D11_VIEWPORT m_screenViewport;
	};
} // namespace hlab