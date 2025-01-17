#pragma once
#include "framework.h"
#include "SVector2D.h"
#include "CObjectAPI.h"
#include "CUnit.h"
#include"CCollisionMgr.h"
// api_colorkey
#pragma comment(lib, "msimg32.lib")
#define MAX_LOADSTRING 100

class CTexture;

// 32bit 색상 구조체
struct ApiPixel
{
	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;
	uint8_t a= 255;
};

class CAPI_Engine
{
public:
	CAPI_Engine();
	virtual ~CAPI_Engine();

private:
	CAPI_Engine(const CAPI_Engine&) {};

	CAPI_Engine& operator=(const CAPI_Engine&) {};

public:
	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);

	void DrawCircle(float tX, float tY, float tRadius);

	// api_colorkey
	void DrawTexture(float tX, float tY, CTexture* tpTex, COLORREF tColorKey = RGB(255,255,255));

	// api_ani_spritesheet_i
	void DrawTexturePartial(float tX, float tY, CTexture* tpTex,int tRow, int tCol, int tIndex, COLORREF tColorKey = RGB(255, 255, 255));

	void Clear(float tR = 0.0f, float tG = 0.0f, float tB = 0.0f);
	void Present();
	
	template<typename T>
	T* CreatePrefab(CTexture* tpTex, float tAnchorX, float tAnchorY, SVector2D tPosition)
	{
	    // 프리팹들을 만들어둔다 (원본객체)
		T* tpPrefab = new T();
		
		tpPrefab->Create(this);

		tpPrefab->SetTexture(tpTex);
		tpPrefab->SetAnchorPoint(tAnchorX, tAnchorY);
		tpPrefab->SetPosition(tPosition);

		tpPrefab->SetIsActive(true);

		return tpPrefab;
	}
	void DestroyPrefab(CUnit*& tPrefab)
	{
	    if (nullptr != tPrefab)
		{
			tPrefab->Destroy();

			delete tPrefab;
			tPrefab = nullptr;
		}
	}

	template<typename T>
	T* InstantObject(CUnit* tpPrefab)
	{
		T* tpUnit = nullptr;

		tpUnit = static_cast<T*>(tpPrefab->clone());
		//tpUnit->AddRef();
		//tpUnit->Release();

		CCollisionMgr::GetInstance()->AddUnit(tpUnit);

		return tpUnit;
	}

	template<typename T>
	void DestroyObject(T*& tUnit)
	{
		if (nullptr != tUnit)
		{
			//CCollisionMgr::GetInstance()->DeleteUnit(tUnit);

			tUnit->Release();
			tUnit = nullptr;
		}
	}
	
	float GetDeltaTime() const
	{
		return mDeltaTime;
	}

	HINSTANCE GetHInst() const
	{
		return hInst;
	}
	HDC GetHDC() const
	{
		return mhDC;
	}

	int ScreenWidth()
	{
		return 800;
	}
	int ScreenHeight()
	{
		return 600;
	}

	// 삼각형 그리기 함수: 2D 평면에 그리므로 (x,y) 두 개의 좌표성분으로 구성
	// 점 3개
	// x_0, y_0
	// x_1, y_1
	// x_2, y_2
	// 
	// 색상
	// tR, tG, tB  각각의 수치는 [0,1] 로 정규화되었다고 가정한다
	// 선으로만 삼각형 그리기
	void DrawTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2, float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	// 면으로 삼각형 그리기 
	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2, float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);

	void FillTriangle(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2, ApiPixel tPixel);

protected:
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);

	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	//static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

protected:
	WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
	WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

	static HINSTANCE hInst;                                // 현재 인스턴스입니다.

	HWND mhWnd;
	HDC mhDC;

	CTexture* mpBackBuffer = nullptr;

private:
	LARGE_INTEGER mSecond;
	LARGE_INTEGER mTime;

	float mDeltaTime;
};
