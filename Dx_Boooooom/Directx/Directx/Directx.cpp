// Directx.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
/*
	https://docs.microsoft.com/ko-kr/windows/win32/direct3dgetstarted/work-with-dxgi
	-참고
*/

#include "stdafx.h"
#include "Directx.h"
#include "DirectX11.h"
#include <string>

LPCWSTR WINDOW_CLASS_NAME = L"DirectX Boooooom!";
HWND g_WndHandle = NULL;
HINSTANCE g_hInstance;
const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

// DX...
Microsoft::WRL::ComPtr<ID3D11Device>		g_pd3dDevice		= nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext = nullptr;
//

//funcs
void Update();
void Render();
void Init();
LRESULT _stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR _stdcall SysDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//....

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	if(hInstance == NULL)
		hInstance = (HINSTANCE)GetModuleHandle(NULL);

	g_hInstance = hInstance;

	WNDCLASS wndClass;
	ZeroMemory(&wndClass,sizeof(WNDCLASS));

	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDC_DIRECTX);
	wndClass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wndClass))
	{
		DWORD dwError = GetLastError();
		if(dwError != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(dwError);
	}

	RECT rect;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	SetRect(&rect, 0,0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	AdjustWindowRect(
		&rect,
		WS_OVERLAPPEDWINDOW,
		false
	);

	g_WndHandle = CreateWindow(
		WINDOW_CLASS_NAME,
		_T("Booom!"),
		WS_OVERLAPPEDWINDOW,
		x,y,
		(rect.right -rect.left), (rect.bottom - rect.top),
		NULL,
		NULL,
		g_hInstance,
		NULL
	);

	if (g_WndHandle == NULL)
	{
		DWORD dwError = GetLastError();
		return HRESULT_FROM_WIN32(dwError);
	}

	ShowWindow(g_WndHandle, nCmdShow);

	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{

		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}

	return 0;
}

LRESULT _stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			UnregisterClass(
				WINDOW_CLASS_NAME,
				g_hInstance
			);
			return 0;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_INFO_SYSTEM:
				{
					DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_SYSTEM),NULL, SysDlgProc);
				}
			default:
				break;
			}
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		break;
	}
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

INT_PTR _stdcall SysDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			std::wstring s = std::to_wstring(g_pd3dDevice->GetFeatureLevel());
			SetDlgItemText(hWnd,IDC_FEATURE_LEVEL, s.c_str());
			break;
		}
		case WM_CLOSE:
		{
			EndDialog(hWnd,	NULL);
			break;
		}
	default:
		return 0;
	}
	return 0;
}

// Main Logics

void Init()
{
	InitDX(&g_pd3dDevice, &g_pd3dDeviceContext);

}

void Update()
{

}

void Render()
{

}