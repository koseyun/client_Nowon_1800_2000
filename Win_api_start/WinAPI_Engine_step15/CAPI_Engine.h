#pragma once
#include <windows.h>

#define MAX_LOADSTRING 100

class CTexture;

class CAPI_Engine
{
public:
	CAPI_Engine();
	virtual ~CAPI_Engine(); // ����Ҹ���

	BOOL Create(HINSTANCE hInstance, int nCmdShow);
	MSG Run();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate(float tDeltaTime);

	// ���׸��� ����� �������� ����
	void DrawCircle(float tX, float tY, float tRadius);
	void DrawTexture(float tX, float tY, CTexture* tpTexture);

	// ȭ���� Ŭ�����ϴ� �Լ�
	void Clear(float tR = 1.0f, float tG = 1.0f, float tB = 1.0f);
	void Present();

	float GetDeltaTime() const
	{
		return mDeltaTime;
	}

	// ��������� ��������� �����ϱ� ���ؼ� private�� ���������Ͽ���
private:
	CAPI_Engine(const CAPI_Engine& tEngine) {};
	CAPI_Engine& operator=(const CAPI_Engine& tEngine) {};

protected:
	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);

	// �������� ������ �������� �ش� ����Լ����� static ���� ������
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

public:
	WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
	WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.

	// ��������Լ��� WndProc���� ����ϰ� �����Ƿ� static ���� ������
	static HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.

	// OnUpdate ���� �׸��⸦ ���� 
	HWND mhWnd;
	HDC mhDC;

	CTexture* mpBackBuffer = nullptr;

	// �ð� ��� ������ ���� �����Ӵ� �ð�
	LARGE_INTEGER mSecond;
	LARGE_INTEGER mTime;

	float mDeltaTime = 0.0f;

};
