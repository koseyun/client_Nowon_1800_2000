#pragma once
#include "framework.h"

class CD3DApi
{
private:
	// D3D : Direct3D
	// 드라이버 : 하드웨어에 밀착된 소프트웨어
	D3D_DRIVER_TYPE         mDriverType = D3D_DRIVER_TYPE_NULL; // 드라이버 프로그램의 종류
	D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0; // FeatureLevel : 임의의 장치가 지원하는 DirectX의 기능집합 수준

	// 디바이스(장치) : 그래픽 처리장치(그래픽 카드)를 추상화 해놓은 객체 : 멀티스레딩(명령문을 동시처리하는 기능)을 고려하지 않은 부분을 담당
	ID3D11Device* mpd3dDevice = nullptr;
	ID3D11Device1* mpd3dDevice1 = nullptr;

	// 디바이스 컨텍스트(장치 컨텍스트) : 그래픽 처리장치(그래픽 카드)를 추상화 해놓은 객체
	// : 멀티스레딩(명령문을 동시처리하는 기능)을 고려한 부분을 담당. 디바이스 개념과 분리되어 있다
	ID3D11DeviceContext* mpImmediateContext = nullptr;
	ID3D11DeviceContext1* mpImmediateContext1 = nullptr;

	// 스왑체인 : 더블버퍼링 구조가 존재하고 이 두 버퍼가 서로 교환되는 구조의 추상화된 객체이다
	// window api때 수동으로 작성해 본 더블 버퍼 구조는 버퍼 두개가 존재하고(back buffer, front buffer)
	// back buffer에서 front buffer로 이미지 데이터를 복사하는 형태로 작동했었다면
	// 여기 DirectX에서 제공하는 더블버퍼는 좀더 진화된 개념으로써 이 두 버퍼가 서로 교환된다
	// 이러한 동작이 마치 사슬처럼 보여 SwapChain이라 명명되었다
	IDXGISwapChain* mpSwapChain = nullptr;
	IDXGISwapChain1* mpSwapChain1 = nullptr;

	// 랜더 타켓 뷰 객체는 back buffer(후면버퍼)를 이미지 데이터를 랜더링 할 수 있는 형태로 추상화 해놓은 객체이다
	ID3D11RenderTargetView* mpRenderTargetView = nullptr;

	// depth
	// 깊이버퍼 : 깊이값을 기억시켜두는 메모리. 앞뒤 판정용 데이터이다
	// 스텐실버퍼 : 임의의 효과를 만들기 위한 메모리
	// DirectX에서는 이 두 버퍼를 하나의 리소스에 담아 사용한다. 깊이버퍼 24 + 스텐실버퍼 8 = 32
	// 그래서 이름이 DepthStencil 이다
	ID3D11Texture2D* mpDepthStencil = nullptr;
	ID3D11DepthStencilView* mpDepthStencilView = nullptr;

	// 윈도우 핸들
	HWND mhWnd;

public:
	HRESULT Create(HWND thWnd);
	void Destroy();

	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Clear(XMVECTORF32 tColor);
	void Present();

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	ID3D11Device* GetD3DDevice() const
	{
		return mpd3dDevice;
	}

	ID3D11DeviceContext* GetImmediateContext() const
	{
		return mpImmediateContext;
	}
};
