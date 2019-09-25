#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <comdef.h>

// d3d11을 사용하기위한 lib를 연결합니다.
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

	// 이 플래그는 색상 표현 순서가 다른 표면(Surface)에 대한 지원을 추가합니다.
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	
	// Direct3D 11 API 장치 개체 및 현재 컨택스트를 생성합니다.
	Microsoft::WRL::ComPtr<ID3D11Device>		device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	D3D_FEATURE_LEVEL featureLevel;

	/*
	
	D3D11CreateDevice()

		IDXGIAdapter* pAdapter
			- 디바이스를 생성하는데 필요한 비디오 어뎁터 포인터이다.
			- IDXGIFactory1::EnumAdapters를 통해 열거된 첫번째 어댑터인
			기본 어댑터를 사용하기 위해선 NULL을 전달하십시오.

			Check!
			- DXGI 1.0 (IDXGIFactory)과 DXGI 1.1 (IDXGIFactor1)을 같은 application에서 
			혼합하여 사용하지 마십시오.
			- IDXGIFactor 나 IDXGIFactory1을 사용하되 둘을 동시에 사용하지 마십시오.

		D3D_DRIVER_TYPE DriverType
			- 생성할 드라이버 타입을 말하는 D3D_DRIVER_TYPE 입니다.
		
		HMODULE Software
			- 소프트웨어 레스터라이저를 구현하는 DLL 핸들입니다.
			- 만약 드라이버 타입이 D3D_DRIVER_TYPE_SOFRWARE라면 "Software"는
			  절대 NULL이어서는 안됩니다.
			- LoadLibray, LoadLibraryEx,혹 GetModuleHandle을 통해 핸들을 얻으십시오.

		UINT Flags
			- 활성화할 런타임 레이어들 ( https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag 을 통해 확인)
			- 값들을 비트단위로 OR 계산 가능합니다.

		const D3D_FEATURE_LEVEL* pFeatureLevels
			- 생성하려는 feature level의 순서를 결정하는 D3D_FEATURE_LEVEL의 배열에 대한 포인터입니다.
			- pFeatureLevel을 NULL로 설정한다면, 이 함수는 아래의 feature level 배열을 사용합니다.
			
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			Check!
			- 만약 Direct3D 11.1 runtime이 컴퓨터에 존재하고 pFeatureLevels가 NULL이라면
			  이 함수는 D3D_FEATURE_LEVEL_11_1 디바이스를 생성할 수 없을것 입니다.
			- D3D_FEATURE_LEVEL_11_1 디바이스를 생성하기 위해선, D3D_FEATURE_LEVEL 배열에 
			  D3D_FEATURE_LEVEL_11_1을 명시적으로 포함하여 제공해야 할 것입니다.
			- 만약 D3D_FEATURE_LEVEL_11_1을 D3D_FEATURE_LEVEL 배열에 포함하여 제공하였을때
			  컴퓨터에 Direct3D 11.1 runtime이 설치되어 있지 않다면, 이 함수는 E_INVALIDARG
			  오류와 함께 즉시 실패할 것 입니다.

		UINT FeatureLevels
			- pFeatureLevels 배열의 원소 개수 입니다.

		UINT SDKVersion
			- SDK 버전입니다. D3D11_SDK_VERSION을 사용하십시오.

		ID3D11Device** ppDevice
			- 생성된 장치를 나타내는 ID3D11Device 포인터의 주소값을 반환합니다.
			- 만약 이 매개 변수가 NULL일 경우 ID3D11Device는 반환되지 않을것 입니다.

		D3D_FEATURE_LEVEL* pFeatureLevel
			- 성공하면 성공한 pFeatureLevels 배열에서 첫 번째 D3D_FEATURE_LEVEL을 반환합니다.
			- 지원되는 Feature Level을 결정할 필요가 없는 경우 입력으로 NULL을 제공하십시오.
	
		ID3D11DeviceContext** ppImmediateContext
			- 디바이스 컨텍스트를 나타내는 ID3D11DeviceContext 포인터의 주소값을 반환합니다.
			- 만약 이 매개 변수가 NULL일 경우, ID3D11DeviceContext는 반환되지 않습니다.
	*/
	HRESULT hr = S_OK;
	hr = D3D11CreateDevice(
		nullptr,					// 기본 어뎁터를 사용하려면 nullptr를 명시합니다.
		D3D_DRIVER_TYPE_HARDWARE,	// 하드웨어 그래픽 드라이버를 이용해 디바이스를 생성합니다.
		0,							// 드라이버가 D3D_DRIVER_TYPE_SOFTWARE가 아닌 이상 0이어야 합니다.
		deviceFlags,				// 디버그와 Direct2D 호환성 플래그를 설정합니다.
		levels,						// 이 앱이 지원할 수 있는 feature level 리스트 입니다.
		ARRAYSIZE(levels),			// 위 list의 크기
		D3D11_SDK_VERSION,			// 윈도우 스토어 앱의 경우 항상 d3D11_SDK_VERSION으로 세팅하십시오.
		&device,
		&featureLevel,
		&context);

	if (FAILED(hr))
	{
	
		/*
		https://stackoverflow.com/questions/7008047/is-there-a-way-to-get-the-string-representation-of-hresult-value-using-win-api
		hresult 에러 메세지 문자열로 읽기
		*/
		_com_error err(hr);
		LPCWSTR errMsg = err.ErrorMessage();
		MessageBox(GetActiveWindow(), errMsg, L"DirectX init error",MB_OK);
	}

	// Direct3D API device와 immediate context를 포인터에 담습니다.
	device.As(_ppd3dDevice);
	context.As(_ppd3dDeviceContext);
}
