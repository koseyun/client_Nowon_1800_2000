#include "CUIBitmap.h"
#include "CDX_Engine.h"
#include "apibuffer.h"
#include "CTextureApi.h"

CUIBitmap::CUIBitmap()
{
    mpVertexBuffer = nullptr;
    mpIndexBuffer = nullptr;

    mCountVertex = 0;
    mCountIndex = 0;

    mpTexture = nullptr;

    mpEngine = nullptr;
}
CUIBitmap::~CUIBitmap()
{

}

void CUIBitmap::Create(ID3D11Device* tpDevice, LPCWSTR tpFileName, int tScreenWidth, int tScreenHeight, int tBitmapWidth, int tBitmapHeight)
{
    mScreenWidth = tScreenWidth;
    mScreenHeight = tScreenHeight;
    mBitmapWidth = tBitmapWidth;
    mBitmapHeight = tBitmapHeight;

    mPrePosX = -1;
    mPrePosY = -1;

    CreateBuffers(tpDevice);

    mpTexture = new CTextureApi();
    mpTexture->Create(tpDevice, tpFileName);
}
void CUIBitmap::Destroy()
{
    if (mpTexture)
    {
        mpTexture->Destroy();

        delete mpTexture;
        mpTexture = nullptr;
    }

    ShutdownBuffers();
}
void CUIBitmap::Render(int tPosX, int tPosY)
{
    UpdateBuffers(mpEngine->GetImmediateContext(), tPosX, tPosY);

    RenderBuffers(mpEngine->GetImmediateContext());
}

void CUIBitmap::CreateBuffers(ID3D11Device* tpDevice)
{
    SimpleVertex* tpVertexs = nullptr;
    WORD* tpIndexs = nullptr;

    D3D11_BUFFER_DESC bd = {};
    D3D11_SUBRESOURCE_DATA InitData = {};

    // ���Ϸκ��� ������ ������ �״�� ������ ����
    // ���� ����, �ε��� ������ ������
    mCountVertex = 6;
    mCountIndex = 6;

    // ���޸𸮿� �޸� Ȯ���ϰ� �ʱ�ȭ
    tpVertexs = new SimpleVertex[mCountVertex];
    memset(tpVertexs, 0, sizeof(SimpleVertex) * mCountVertex);

    tpIndexs = new WORD[mCountIndex];
    for (int ti = 0; ti < mCountIndex; ++ti)
    {
        tpIndexs[ti] = ti;
    }

    // �ּ���
    bd.Usage = D3D11_USAGE_DYNAMIC; // ���� �뵵�� ����ϰڴ�(���� �߿� ���ۿ� �����Ͽ� �������� ���� ��ִ´�)

    bd.ByteWidth = sizeof(SimpleVertex) * mCountVertex;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //�������� ���� ��ִ´�
    // �μ���
    InitData.pSysMem = tpVertexs;

    tpDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);

    // �ּ���
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * mCountIndex;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    // �μ���
    InitData.pSysMem = tpIndexs;

    tpDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);

    // �ý��� �޸𸮿� ���� �����ʹ� ���� �ʿ�����Ƿ� �����Ѵ�
    if (tpIndexs)
    {
        delete[] tpIndexs;
        tpIndexs = nullptr;
    }

    if (tpVertexs)
    {
        delete[] tpVertexs;
        tpVertexs = nullptr;
    }
}
void CUIBitmap::ShutdownBuffers()
{
    if (mpIndexBuffer)
    {
        mpIndexBuffer->Release();
        mpIndexBuffer = nullptr;
    }

    if (mpVertexBuffer)
    {
        mpVertexBuffer->Release();
        mpVertexBuffer = nullptr;
    }
}
void CUIBitmap::RenderBuffers(ID3D11DeviceContext* tpDeviceContext)
{
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    // IA�� VB, IB, topology����
    tpDeviceContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
    tpDeviceContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    tpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �ε��� ���� ������ �̿��Ͽ� ������
    //tpDeviceContext->DrawIndexed(mCountIndex, 0, 0);
}

bool CUIBitmap::UpdateBuffers(ID3D11DeviceContext* tpImmediateContext, int tPosX, int tPosY)
{
    // �簢�� ���� ����
    float left;
    float right;
    float top;
    float bottom;

    // vertex buffer�� ����� �����Ϳ� (�ý��� �޸�)
    SimpleVertex* tpVertices = nullptr;

    // vertex buffer ���ٿ� ����
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    SimpleVertex* verticesPtr = nullptr;

    // �����ġ ������ ���ٸ� map, unmap�� �̿��� vertex buffer ������ ���� �ʴ´�
    // <- �׷����ϸ� ���� ȿ������ ������ �����ϴ�(������ �پ��Ƿ�)
    if ((mPrePosX == tPosX) && (mPrePosY == tPosY))
    {
        return true;
    }

    mPrePosX = tPosX;
    mPrePosY = tPosY;

    left = (mScreenWidth / 2.0f) * (-1.0f) + (float)tPosX;
    right = left + (float)mBitmapWidth;

    top = (mScreenHeight / 2.0f) - (float)tPosY;
    bottom = top - (float)mBitmapHeight;

    tpVertices = new SimpleVertex[mCountVertex];
    // �ʱ�ȭ
    memset(tpVertices, 0, sizeof(SimpleVertex) * mCountVertex);

    // normal �����ʹ� �������� �ʾҴ�

    // ù��° �ﰢ��
    tpVertices[0].Pos = XMFLOAT3(left, top, 0.0f);
    tpVertices[0].Tex = XMFLOAT2(0.0f, 0.0f);

    tpVertices[1].Pos = XMFLOAT3(right, bottom, 0.0f);
    tpVertices[1].Tex = XMFLOAT2(1.0f, 1.0f);

    tpVertices[2].Pos = XMFLOAT3(left, bottom, 0.0f);
    tpVertices[2].Tex = XMFLOAT2(0.0f, 1.0f);
    // �ι�° �ﰢ��
    tpVertices[3].Pos = XMFLOAT3(left, top, 0.0f);
    tpVertices[3].Tex = XMFLOAT2(0.0f, 0.0f);

    tpVertices[4].Pos = XMFLOAT3(right, top, 0.0f);
    tpVertices[4].Tex = XMFLOAT2(1.0f, 0.0f);

    tpVertices[5].Pos = XMFLOAT3(right, bottom, 0.0f);
    tpVertices[5].Tex = XMFLOAT2(1.0f, 1.0f);

    // �ǽð����� vertex buffer�� ���� ������ ������ ��ִ´�

    // Map, Unmap ¦�� ����߸� �Ѵ�
    // Map�� vertex buffer�� CPU ������ ���� �۾��� �ϱ� ���� �ٸ� �۾��鿡 ���ؼ��� ����� �ϴ� ���̴�
    // �� �Լ��� ȣ���ϸ� mappedResource�� �����͸� ��ֱ� ���� ������ �������� �������ش�
    // vertex buffer�� ���� ������� ��´�
    tpImmediateContext->Map(mpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        // void*(�������� ����)�� ���� vertex buffer�� ������ �츮�� �ٷ�� ������ �������� ����ȯ
        verticesPtr = (SimpleVertex*)mappedResource.pData;
        // �����ؼ� ��ִ´�
        memcpy(verticesPtr, (void*)tpVertices, sizeof(SimpleVertex) * mCountVertex);

    // vertex buffer�� ���� ������� �����ش�
    tpImmediateContext->Unmap(mpVertexBuffer, 0);

    if (tpVertices)
    {
        delete[] tpVertices;
        tpVertices = nullptr;
    }

    return true;
}

ID3D11ShaderResourceView* CUIBitmap::GetTexture()
{
    return mpTexture->GetTexture();
}