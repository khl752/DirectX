#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <comdef.h>

// d3d11�� ����ϱ����� lib�� �����մϴ�.
#pragma comment(lib, "d3d11.lib")


void InitDX(_Out_ Microsoft::WRL::ComPtr<ID3D11Device> *_ppd3dDevice, 
			_Out_ Microsoft::WRL::ComPtr<ID3D11DeviceContext> *_ppd3dDeviceContext)
{
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};

	// �� �÷��״� ���� ǥ�� ������ �ٸ� ǥ��(Surface)�� ���� ������ �߰��մϴ�.
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	
	// Direct3D 11 API ��ġ ��ü �� ���� ���ý�Ʈ�� �����մϴ�.
	Microsoft::WRL::ComPtr<ID3D11Device>		device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	D3D_FEATURE_LEVEL featureLevel;

	/*
	
	D3D11CreateDevice()

		IDXGIAdapter* pAdapter
			- ����̽��� �����ϴµ� �ʿ��� ���� ��� �������̴�.
			- IDXGIFactory1::EnumAdapters�� ���� ���ŵ� ù��° �������
			�⺻ ����͸� ����ϱ� ���ؼ� NULL�� �����Ͻʽÿ�.

			Check!
			- DXGI 1.0 (IDXGIFactory)�� DXGI 1.1 (IDXGIFactor1)�� ���� application���� 
			ȥ���Ͽ� ������� ���ʽÿ�.
			- IDXGIFactor �� IDXGIFactory1�� ����ϵ� ���� ���ÿ� ������� ���ʽÿ�.

		D3D_DRIVER_TYPE DriverType
			- ������ ����̹� Ÿ���� ���ϴ� D3D_DRIVER_TYPE �Դϴ�.
		
		HMODULE Software
			- ����Ʈ���� �����Ͷ������� �����ϴ� DLL �ڵ��Դϴ�.
			- ���� ����̹� Ÿ���� D3D_DRIVER_TYPE_SOFRWARE��� "Software"��
			  ���� NULL�̾�� �ȵ˴ϴ�.
			- LoadLibray, LoadLibraryEx,Ȥ GetModuleHandle�� ���� �ڵ��� �����ʽÿ�.

		UINT Flags
			- Ȱ��ȭ�� ��Ÿ�� ���̾�� ( https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag �� ���� Ȯ��)
			- ������ ��Ʈ������ OR ��� �����մϴ�.

		const D3D_FEATURE_LEVEL* pFeatureLevels
			- �����Ϸ��� feature level�� ������ �����ϴ� D3D_FEATURE_LEVEL�� �迭�� ���� �������Դϴ�.
			- pFeatureLevel�� NULL�� �����Ѵٸ�, �� �Լ��� �Ʒ��� feature level �迭�� ����մϴ�.
			
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			Check!
			- ���� Direct3D 11.1 runtime�� ��ǻ�Ϳ� �����ϰ� pFeatureLevels�� NULL�̶��
			  �� �Լ��� D3D_FEATURE_LEVEL_11_1 ����̽��� ������ �� ������ �Դϴ�.
			- D3D_FEATURE_LEVEL_11_1 ����̽��� �����ϱ� ���ؼ�, D3D_FEATURE_LEVEL �迭�� 
			  D3D_FEATURE_LEVEL_11_1�� ��������� �����Ͽ� �����ؾ� �� ���Դϴ�.
			- ���� D3D_FEATURE_LEVEL_11_1�� D3D_FEATURE_LEVEL �迭�� �����Ͽ� �����Ͽ�����
			  ��ǻ�Ϳ� Direct3D 11.1 runtime�� ��ġ�Ǿ� ���� �ʴٸ�, �� �Լ��� E_INVALIDARG
			  ������ �Բ� ��� ������ �� �Դϴ�.

		UINT FeatureLevels
			- pFeatureLevels �迭�� ���� ���� �Դϴ�.

		UINT SDKVersion
			- SDK �����Դϴ�. D3D11_SDK_VERSION�� ����Ͻʽÿ�.

		ID3D11Device** ppDevice
			- ������ ��ġ�� ��Ÿ���� ID3D11Device �������� �ּҰ��� ��ȯ�մϴ�.
			- ���� �� �Ű� ������ NULL�� ��� ID3D11Device�� ��ȯ���� ������ �Դϴ�.

		D3D_FEATURE_LEVEL* pFeatureLevel
			- �����ϸ� ������ pFeatureLevels �迭���� ù ��° D3D_FEATURE_LEVEL�� ��ȯ�մϴ�.
			- �����Ǵ� Feature Level�� ������ �ʿ䰡 ���� ��� �Է����� NULL�� �����Ͻʽÿ�.
	
		ID3D11DeviceContext** ppImmediateContext
			- ����̽� ���ؽ�Ʈ�� ��Ÿ���� ID3D11DeviceContext �������� �ּҰ��� ��ȯ�մϴ�.
			- ���� �� �Ű� ������ NULL�� ���, ID3D11DeviceContext�� ��ȯ���� �ʽ��ϴ�.
	*/
	HRESULT hr = S_OK;
	hr = D3D11CreateDevice(
		nullptr,					// �⺻ ��͸� ����Ϸ��� nullptr�� ����մϴ�.
		D3D_DRIVER_TYPE_HARDWARE,	// �ϵ���� �׷��� ����̹��� �̿��� ����̽��� �����մϴ�.
		0,							// ����̹��� D3D_DRIVER_TYPE_SOFTWARE�� �ƴ� �̻� 0�̾�� �մϴ�.
		deviceFlags,				// ����׿� Direct2D ȣȯ�� �÷��׸� �����մϴ�.
		levels,						// �� ���� ������ �� �ִ� feature level ����Ʈ �Դϴ�.
		ARRAYSIZE(levels),			// �� list�� ũ��
		D3D11_SDK_VERSION,			// ������ ����� ���� ��� �׻� d3D11_SDK_VERSION���� �����Ͻʽÿ�.
		&device,
		&featureLevel,
		&context);

	if (FAILED(hr))
	{
	
		/*
		https://stackoverflow.com/questions/7008047/is-there-a-way-to-get-the-string-representation-of-hresult-value-using-win-api
		hresult ���� �޼��� ���ڿ��� �б�
		*/
		_com_error err(hr);
		LPCWSTR errMsg = err.ErrorMessage();
		MessageBox(GetActiveWindow(), errMsg, L"DirectX init error",MB_OK);
	}

	// Direct3D API device�� immediate context�� �����Ϳ� ����ϴ�.
	device.As(_ppd3dDevice);
	context.As(_ppd3dDeviceContext);
}
