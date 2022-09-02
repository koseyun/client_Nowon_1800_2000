#include "CD3DApi.h"

void CD3DApi::Clear(float tR, float tG, float tB)
{
    // XM directX Math
    XMVECTORF32 tColor;
    tColor.f[0] = tR;
    tColor.f[1] = tG;
    tColor.f[2] = tB;

    // render target view�� �ش� �������� Ŭ����
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    // ���̽��ٽǹ��� Ŭ����, ���̰��� �ִ� 1�� ����ȭ
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void CD3DApi::Clear(XMVECTORF32 tColor)
{
    mpImmediateContext->ClearRenderTargetView(mpRenderTargetView, tColor);

    // ���̽��ٽǹ��� Ŭ����, ���̰��� �ִ� 1�� ����ȭ
    mpImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3DApi::Present()
{
    // ������ ���� ���ɵ��� ������ �����Ѵ�
    // ��������� OM���� ����ͷ� �ѷ�����
    mpSwapChain->Present(0, 0);
}

HRESULT CD3DApi::Create(HWND thWnd)
{

    // ������ �ڵ��� �Ѱܹ޴´�
    mhWnd = thWnd;

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(mhWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        // �׷��� ī�� ��ġ���� ������ ó��
        D3D_DRIVER_TYPE_HARDWARE,
        // ���� ������ ����Ʈ���� ������ ó��, Hardware���� ������ ������ ó���� �̷������ ���ϴ� ��� �ּ��� ������� ����Ʈ���������� ������ ó��
        D3D_DRIVER_TYPE_WARP,
        // CPU���� ������ ����Ʈ���������� ������ ó��, �ſ� ����, ���߿�(�տ� �ܰ��� ����̹����� ��� �����ص� �̰����� ����)
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // ��ó ���� : DirectX ���� ���� �����ϴ� ������ �������
    // �׷��� ī�忡�� �����ϴ� DirectX �������
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1, // DirectX 11.1 ����
        D3D_FEATURE_LEVEL_11_0, // DirectX 11 ����
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels); // ARRAYSIZE �ش� �迭�� ũ�⸦ ��� ��ũ�� �Լ�

    // ����̹� ���� ������ŭ ��ȸ
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        mDriverType = driverTypes[driverTypeIndex];
        // ����̹� Ÿ�԰� ��ó���� ���� ������� ����̽��� ����̽� ���ؽ�Ʈ(�׷��� ī�� ��ü)�� �����
        hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);

        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &mpd3dDevice, &mFeatureLevel, &mpImmediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    // DXGI : DiretX Graphic Infra
    // DirectX�� �����ϴ� �׷��� ó���� ���� �Ϲ����� '����� �Ǵ� ����'
    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)

    // COM : ���̳ʸ� ������ ȣȯ ������ ��ǰ
    // DirectX�� COM ����� ������� ������� �ִ�
    // __uuidof ������ COM ��ü�� �ĺ��ڸ� ��� �������̴�

    // C++���� �����ϴ� ����ȯ ������
    // static_cast<T> : ���� ����ȯ
    // dynamic_cast<T> : ���� ����ȯ
    // const_cast<T> : ��� �Ӽ��� �����ϴ� ����ȯ 
    // reinterpret_cast<T> C++�� ����ȯ ������ �� �ϳ�. ��� Ÿ���̶� ����ȯ �����ϴ�

    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = mpd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr; // �׷��� ó����ġ(�׷��� ī��)�� �߻�ȭ�س��� ��ü, DXGI���� �ٷ�� ����
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
    if (dxgiFactory2)
    {
        // DirectX 11.1 or later
        hr = mpd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mpd3dDevice1));
        if (SUCCEEDED(hr))
        {
            (void)mpImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mpImmediateContext1));
        }

        // ����ü�� ������ ���� ���� ���� �ۼ�
        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;                       // ������ �ʺ�
        sd.Height = height;                     // ������ ����
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32��Ʈ ���� ������ ���
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;                     // back buffer�� ����

        // ����ü�� ��ü ����
        hr = dxgiFactory2->CreateSwapChainForHwnd(mpd3dDevice, mhWnd, &sd, nullptr, nullptr, &mpSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = mpSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mpSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = mhWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(mpd3dDevice, &sd, &mpSwapChain);
    }

    // Ǯ��ũ�� ��ȯ ����Ű Alt+Enter�� ������� �ʰڴ�
    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(mhWnd, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    // back buffer�� Render Target View�� �����
    // ���⼭�� View��� ������ DirectX���� �̹��� �����͸� �� �� �ִ� ������ �޸𸮸� ��Ī�ϴ� ����̴�
    hr = mpd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mpRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    // OM : Output Merger�� ���Ӹ�
    // OutPut Merger�� Render Target View�� �����Ѵ�
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, nullptr);


    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���̹��� 24, ���ٽ� 8 -> 32,  [0, 1]�� ������ ����ȭ�Ǿ��ִ�.
    descDepth.SampleDesc.Count = 1; // ���ø��� �ؽ��ĸ� �ٷ� �� ����
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // ����/���ٽ� ���ۿ��̴�
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    // DirectX11���� ����/���ٽ� ���۴� �ؽ��� ���ҽ��� �� ������ ����Ѵ�
    mpd3dDevice->CreateTexture2D(&descDepth, nullptr, &mpDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    // ����/���ٽ� �並 �����
    mpd3dDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);

    // ����Ÿ�ٺ信 ���̽��ٽ� �� ����
    mpImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

    // Setup the viewport
    // ����Ʈ�� �����Ѵ�
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mpImmediateContext->RSSetViewports(1, &vp); // RS Render State

    return S_OK;
}

void CD3DApi::Destroy()
{
    // ������ ���µ��� Ŭ����
    if (mpImmediateContext) mpImmediateContext->ClearState();

    // ��ü���� ������(����)
    if (mpRenderTargetView) mpRenderTargetView->Release();
    if (mpSwapChain1) mpSwapChain1->Release();
    if (mpSwapChain) mpSwapChain->Release();
    if (mpImmediateContext1) mpImmediateContext1->Release();
    if (mpImmediateContext) mpImmediateContext->Release();
    if (mpd3dDevice1) mpd3dDevice1->Release();
    if (mpd3dDevice) mpd3dDevice->Release();
}

HRESULT CD3DApi::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    // ���̴� �ҽ��ڵ� ������ ������ ���ִ� �Լ�
    // Shader SourceCode File-- > ������-- > ����Ʈ �ڵ� ByteCode(�߰� �����)
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}