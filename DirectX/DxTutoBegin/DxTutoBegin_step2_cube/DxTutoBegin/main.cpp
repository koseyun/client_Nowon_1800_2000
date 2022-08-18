﻿/*
    step2

    최종적으로 육면체을 랜더링해보자

    1. 정점 데이터에 색상 정보 추가

    2. 변환행렬 적용

    3. 육면체 랜더링

        // 정점
        // 인덱스
*/

#include "framework.h"
#include "CDX_Engine.h"
using namespace std;

// 이것은 정점을 나타내는 구조체의 정의이다
// float 데이터 3개로 구성된 벡터이다
// 이 정의는 시스템 메모리에 만든 데이터의 형식을 정의하는 것이다
struct SimpleVertex
{
    XMFLOAT3 Pos; // 정점의 위치 정보 x,y,z

    XMFLOAT4 Color; // 정점의 색상 정보 r,g,b,a
};

// 이것은 Constant Buffer에 사용할 데이터를 나타내는 구조체의 정의이다
// 변환행렬들을 멤버로 갖게 하였다
// 이 정의는 시스템 메모리에 만들 데이터의 형식을 정의하는 것이다
struct CBTransform
{
    XMMATRIX mWorld;        // 월드 변환행렬
    XMMATRIX mView;         // 뷰 변환행렬
    XMMATRIX mProjection;   // 투영 변환행렬
};

// 월드변환 : 로컬좌표계 기준의 정점의 위치를 월드좌표계 기준에 정점의 위치로 변환한다
// 뷰변환 : 월드좌표계 기준의 정점의 위치를 뷰좌표계 기준에 정점의 위치로 변환한다
// 투영변환 : 뷰좌표계 기준의 정점의 위치를 정규뷰 기준에 정점의 위치로 변환하고 근평면에 투영하는 변환이다

class CDXEngine: public CDX_Engine
{
    ID3D11VertexShader* mpVertexShader = nullptr;   // vertex shader 담당 객체
    ID3D11PixelShader* mpPixelShader = nullptr;     // pixel shader 담당 객체

    ID3D11InputLayout* mpVertexLayout = nullptr;    // Input Layout 객체
    // 랜더링에 사용할 자원
    ID3D11Buffer* mpVertexBuffer = nullptr; // 비디오램에 있는 임의의 데이터. 이 예시에서는 삼각형 데이터를 담을 것이다

    // VertexBuffer의 내용을 참고하여 그리기할 정보
    ID3D11Buffer* mpIndexBuffer = nullptr;

    // 랜더링에 사용할 자원
    ID3D11Buffer* mpCBTransform = nullptr; // 변환행렬 정보들을 담을 상수버퍼이다

    // 응용프로그램 차원에서 다룰 변환행렬 변수들
    XMMATRIX mMatWorld;
    XMMATRIX mMatView;
    XMMATRIX mMatProjection;



public:
    CDXEngine() {};
    virtual ~CDXEngine() {};
        
    virtual void OnCreate() override
    {
        CDX_Engine::OnCreate();

        HRESULT hr = S_OK;

        /*
            HLSL : High Level Shader Language : 마이크로소프트사에서 만든 DirectX용 셰이더 언어

            GLSL : OpenGL용 셰이더 언어
            CG : 엔비디아사에서 만든 셰이더 언어        
        */

        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        // 셰이더 소스코드를 컴파일하여 Blob객체에 담는다(담긴 내용은 Byte Code 형식이다)
        hr = CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_4_0", &pVSBlob); // "참고할 파일", "파일 속 함수", "셰이더 버전"
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            //return hr;
        }

        // Create the vertex shader
        // Vertex Shader(mpVertexShader) 객체를 생성한다(Blob객체를 참고하여 만든다)
        hr = mpd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mpVertexShader);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            //return hr;
        }
        
        // SEMANTIC 시맨틱 : 셰이더(랜더링 파이프라인)에서 어떤 용도이냐를 나타내는 꼬리표
        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            // 시맨틱 이름, 시맨틱 인덱스, 타입, 정점버퍼메모리의 슬록인덱스(0~15), 오프셋(몇byte 떨어져있는지), 고급옵션, 고급옵션
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        hr = mpd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &mpVertexLayout);
        pVSBlob->Release();
        /*if (FAILED(hr))
            return hr;*/

        // Set the input layout
        mpImmediateContext->IASetInputLayout(mpVertexLayout);

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        // 셰이더 소스코드를 컴파일하여 Blob객체에 담는다(담긴 내용은 Byte Code 형식이다)
        hr = CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_4_0", &pPSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr,
                L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            //return hr;
        }

        // Create the pixel shader
        // Pixel Shader(mpPixelShader) 객체를 생성한다(Blob객체를 참고하여 만든다)
        hr = mpd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mpPixelShader);
        pPSBlob->Release();
        /*if (FAILED(hr))
            return hr;*/

        // directX에서 기본적인 좌표계의 원점은 화면 중앙에 위치해있고, x축의 양의 방향은 우방, y축 양의 방향은 상방이다
        // 삼각형의 정점 3개의 데이터를 설정
        // 지역변수(시스템 메모리에 있는 것이다)
        /*SimpleVertex vertices[] =
        {
            XMFLOAT3(0.0f, 0.0f, 0.5f), // XMFLOAT3 생성자 호출
            XMFLOAT3(0.0f, 1.0f, 0.5f),
            XMFLOAT3(1.0f, 0.0f, 0.5f),
        };*/

        /*SimpleVertex vertices[] =
        {
            XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), // 정점 하나의 정보
            XMFLOAT3(0.0f, 1.0f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
            XMFLOAT3(1.0f, 0.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
        }; */

        // 정육면체를 이루는 정점 데이터 8개
        SimpleVertex vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        };

        // 주설명
        D3D11_BUFFER_DESC bd = {};
        bd.Usage            = D3D11_USAGE_DEFAULT;      // 버퍼는 기본 용도 버퍼로 사용하겠다
        //bd.ByteWidth        = sizeof(SimpleVertex) * 3; // 정점 세개 크기의 데이터이다
        bd.ByteWidth        = sizeof(SimpleVertex) * 8;
        bd.BindFlags        = D3D11_BIND_VERTEX_BUFFER; // vertex buffer 용도로 사용하겠다
        bd.CPUAccessFlags   = 0;                        // CPU의 접근은 불허한다

        // 부설명
        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem    = vertices;                 // 해당 시스템 메모리에 내용을 넘긴다

        // bd와 InitData를 참고하여 mpVertexBuffer를 생성한다
        // Vertex Buffer는 기하도형을 그리기 위해 필요한 데이터이다
        mpd3dDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
        
        UINT stride = sizeof(SimpleVertex); // 메모리를 해석하는 경계?
        UINT offset = 0;                    // 얼마나 떨어졌는지

        mpImmediateContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
        mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        // 도형을 어떤 방식으로 그릴지 방법을 정하는 것이다
        // 여기서는 정점 3개를 모아서 하나의 삼각형을 구성하는 형태로 랜더링한다

        // Constant Buffer 상수버퍼 생성
        // 주설명
        bd.Usage = D3D11_USAGE_DEFAULT;      // 버퍼는 기본 용도 버퍼로 사용하겠다
        bd.ByteWidth = sizeof(CBTransform) * 1;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // vertex buffer 용도로 사용하겠다
        bd.CPUAccessFlags   = 0;

        // 부설명은 하지 않겠다. 다른 방법으로 갱신한다
        //InitData.pSysMem = ;                 // 해당 시스템 메모리에 내용을 넘긴다

        // 상수버퍼 생성
        mpd3dDevice->CreateBuffer(&bd, nullptr, &mpCBTransform);

        // 월드변환행렬
        mMatWorld = XMMatrixIdentity(); // 단위행렬

        // 뷰변환행렬
        mMatView = XMMatrixIdentity(); // 단위행렬
        // 뷰변환 회전행렬
        XMVECTOR tEye = XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f);   // 카메라의 위치
        XMVECTOR tAt = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);     // 바라보는 지점
        XMVECTOR tUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);     // 상방벡터

        mMatView = XMMatrixLookAtLH(tEye, tAt, tUp);

        // 투영변환행렬
        mMatProjection = XMMatrixIdentity(); // 단위행렬

        RECT rc;
        GetClientRect(mhWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;
        float tAspectRatio = width / (float)height;
        // 시야각, 종횡비, 근평면까지의 거리, 원평면까지의 거리를 기반으로 원근 투영변환 행렬을 만든다
        mMatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, tAspectRatio, 0.01f, 100.0f);

    }

    virtual void OnDestroy() override
    {
        
        if (mpVertexShader) mpVertexShader->Release();
        if (mpPixelShader) mpPixelShader->Release();

        if (mpVertexLayout) mpVertexLayout->Release();

        if (mpVertexBuffer) mpVertexBuffer->Release();

        if (mpCBTransform) mpCBTransform->Release();

        CDX_Engine::OnDestroy();
    }

    virtual void OnUpdate(float tDeltaTime) override
    {
        CDX_Engine::OnUpdate(tDeltaTime);

        static float t = 0.0f;
        t = t + 2.5f * tDeltaTime;
        // 상방축을 회전축으로 회전, 얼마만큼 회전할지는 각도로 주어진다
        // 각도 단위는 라디안
        mMatWorld = XMMatrixRotationY(t);

        // 상수버퍼용 지역변수 선언
        CBTransform cb;
        // 상수버퍼 데이터에 응용프로그램 코드 상에 데이터를 설정

        /*
            DirectX의 응용프로그램 코드(C++)에서는 행벡터를 기준으로 연산한다
            HLSL의 코드에서는 열벡터를 기준으로 연산한다

            그러므로 전치해서 넘겨줘야 한다
        */

        cb.mWorld       = XMMatrixTranspose(mMatWorld);         // 전치
        cb.mView        = XMMatrixTranspose(mMatView);          // 전치
        cb.mProjection  = XMMatrixTranspose(mMatProjection);    // 전치

        // UpdateSubresource : 상수버퍼의 내용을 갱신해주는 함수
        mpImmediateContext->UpdateSubresource(mpCBTransform, 0, nullptr, &cb, 0, 0);
        
        //this->Clear(0.1f, 0.1f, 0.3f);
        this->Clear(Colors::BlueViolet);

        // vertex shader 단계에 vertex shader 객체 설정
        mpImmediateContext->VSSetShader(mpVertexShader, nullptr, 0);
        // 상수버퍼를 Vertex Shader에 설정
        mpImmediateContext->VSSetConstantBuffers(0, 1, &mpCBTransform);

        // pixel shader 단계에 pixel shader 객체 설정
        mpImmediateContext->PSSetShader(mpPixelShader, nullptr, 0);
        // 장치즉시컨텍스트에게 정점버퍼의 내용을 (기하도형) 기반으로 그리라고(랜더링을) 지시한다
        mpImmediateContext->Draw(3, 0);
        // GPU에 보낼 정점의 개수, 보내기 시작할 첫번째 정점의 인덱스
        
        this->Present();
    }

private:
    CDXEngine(const CDXEngine&) {};

    CDXEngine& operator=(const CDXEngine&) {};
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CDXEngine tEngine;
    tEngine.Create(hInstance, nCmdShow);

    MSG msg = { 0 };

    msg = tEngine.Run();

    return (int) msg.wParam;
}