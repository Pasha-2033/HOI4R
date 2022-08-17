#include "application.h"
#include "GraphicCore/DirectX/include/dxwindow.h"
void render();
dxwindow::dxwindowclass* dxwin = new dxwindow::dxwindowclass(NULL, true);
ID3D11Buffer* vertexbuffer = nullptr;
ID3D11Buffer* indexbuffer = nullptr;
ID3D11Buffer* constbuffer = nullptr;
DirectX::XMMATRIX worldmatrix;
DirectX::XMMATRIX viewmatrix;
DirectX::XMMATRIX projectionmatrix;
UINT stride = sizeof(SimpleVertex);
UINT offset = 0;
static float t = 0.0f;
/*SimpleVertex vertices[] = { //�����������
    DirectX::XMFLOAT3(0.0f, 0.625f,  0.5f),
    DirectX::XMFLOAT3(0.6495f, -0.5f, 0.5f),
    DirectX::XMFLOAT3(-0.6495f, -0.5f, 0.5f)
};*/
// �������� ������ ������ (���� ����� ��������)
SimpleVertex vertices[] =
{	/* ���������� X, Y, Z				���� R, G, B, A					 */
    { DirectX::XMFLOAT3(0.0f,  3.0f,  0.0f), DirectX::XMFLOAT4(2.0f, 2.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(-2.0f,  0.0f, -2.0f), DirectX::XMFLOAT4(0.0f, 2.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(2.0f,  0.0f, -2.0f), DirectX::XMFLOAT4(2.0f, 0.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(-2.0f,  0.0f,  2.0f), DirectX::XMFLOAT4(0.0f, 2.0f, 2.0f, 2.0f) },
    { DirectX::XMFLOAT3(2.0f,  0.0f,  2.0f), DirectX::XMFLOAT4(2.0f, 0.0f, 2.0f, 2.0f) }
};
WORD indices[] =
{	// ������� ������� vertices[], �� ������� �������� ������������
    0,2,1,	/* ����������� 1 = vertices[0], vertices[2], vertices[3] */
    0,3,4,	/* ����������� 2 = vertices[0], vertices[3], vertices[4] */
    0,1,3,	/* � �. �. */
    0,4,2,

    1,2,3,
    2,4,3,
};
UINT vsize = ARRAYSIZE(vertices);
UINT isize = ARRAYSIZE(indices);
int main() {
    if (FAILED(dxwin->getHR())) return -1;
    dxwin->setW(500);
    dxwin->setH(500);
    
    dxmodule::createvertexbuffer(dxwin->getdx()->getdevice(), &vertexbuffer, vertices, vsize);
    dxmodule::createindexbuffer(dxwin->getdx()->getdevice(), &indexbuffer, indices, isize);
    dxmodule::createconstbuffer(dxwin->getdx()->getdevice(), &constbuffer);
    dxwin->getdx()->getdevicecontext()->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
    dxwin->getdx()->getdevicecontext()->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R16_UINT, 0);

    //______________________________________
    //������������� ������� ����
    worldmatrix = DirectX::XMMatrixIdentity();
    // ������������� ������� ����
    DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, -1.0f, -10.0f, 1200.0f);	// ������ �������
    DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	// ���� �������
    DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// ����������� �����
    viewmatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);
    //������������� ������� ��������
    projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, dxwin->getW() / (FLOAT)dxwin->getH(), 0.01f, 100.0f); //to do (������� ���, ����� ���������� ������� ���� �������� ��������� ���� �������)
    //
    worldmatrix = DirectX::XMMatrixRotationY(4.0f);
    dxmodule::constantbufferstruct cb;
    cb.worldmatrix = DirectX::XMMatrixTranspose(worldmatrix);
    cb.viewmatrix = DirectX::XMMatrixTranspose(viewmatrix);
    cb.projectionmatrix = DirectX::XMMatrixTranspose(projectionmatrix);
    dxwin->getdx()->getdevicecontext()->UpdateSubresource(constbuffer, 0, NULL, &cb, 0, 0);
    //______________________________________

    dxwin->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "VS", "vs_4_0", false);
    dxwin->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "PS", "ps_4_0", true);
    dxwin->applyvertexshader(0);
    dxwin->applypixelshader(0);
    dxwin->getdx()->getdevicecontext()->VSSetConstantBuffers(0, 1, &constbuffer);
    dxwin->setshowmode(winmodule::windowshowmode::customsize);
    dxwin->setV(true);
    while (true) {
        render();
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            render();
        }
    }
	return 0;
}
void render() {
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // �������, �������, �����, �����-�����
    dxwin->getdx()->getdevicecontext()->ClearRenderTargetView(dxwin->getdx()->getrendertargetview(), ClearColor);
    //
    // ���������� ����������-�������
    if (dxwin->getdx()->getcur_drivetype() == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)DirectX::XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // ������� ��� �� ��� Y �� ���� t (� ��������)
    worldmatrix = DirectX::XMMatrixRotationY(t);

    // �������� ����������� �����
    // ������� ��������� ��������� � ��������� � ��� �������
    dxmodule::constantbufferstruct cb;
    cb.worldmatrix = DirectX::XMMatrixTranspose(worldmatrix);
    cb.viewmatrix = DirectX::XMMatrixTranspose(viewmatrix);
    cb.projectionmatrix = DirectX::XMMatrixTranspose(projectionmatrix);
    // ��������� ��������� ��������� � ����������� ����� g_pConstantBuffer
    dxwin->getdx()->getdevicecontext()->UpdateSubresource(constbuffer, 0, NULL, &cb, 0, 0);
    //
    dxwin->getdx()->getdevicecontext()->DrawIndexed(18, 0, 0);
    dxwin->getdx()->getswapchain()->Present(0, 0);
}