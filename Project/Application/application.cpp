#include "application.h"
#include "GraphicCore/DirectX/include/dxwindow.h"
void render();
void rotate();
dxwindow::dxwindowclass* dxwin = new dxwindow::dxwindowclass(NULL, true, true);
UINT stride = sizeof(SimpleVertex);
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
    dxwin->setzfar(100.0f);
    
    dxmodule::createvertexbuffer(dxwin->getdx()->getdevice(), dxwin->vertexbuffer, vertices, vsize);
    dxmodule::createindexbuffer(dxwin->getdx()->getdevice(), dxwin->indexbuffer, indices, isize);
    dxmodule::createconstbuffer(dxwin->getdx()->getdevice(), dxwin->constbuffer);
    dxwin->getdx()->getdevicecontext()->IASetVertexBuffers(0, 1, &dxwin->vertexbuffer, &stride, &dxwindow::offset);
    dxwin->getdx()->getdevicecontext()->IASetIndexBuffer(dxwin->indexbuffer, DXGI_FORMAT_R16_UINT, 0);

    //______________________________________
    dxmodule::constantbufferstruct cb;
    cb.worldmatrix = DirectX::XMMatrixTranspose(dxwin->worldmatrix);
    cb.viewmatrix = DirectX::XMMatrixTranspose(dxwin->viewmatrix);
    cb.projectionmatrix = DirectX::XMMatrixTranspose(dxwin->projectionmatrix);
    dxwin->getdx()->getdevicecontext()->UpdateSubresource(dxwin->constbuffer, 0, NULL, &cb, 0, 0);
    //______________________________________

    dxwin->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "VS", "vs_4_0", false);
    dxwin->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "PS", "ps_4_0", true);
    dxwin->applyvertexshader(0);
    dxwin->applypixelshader(0);
    dxwin->getdx()->getdevicecontext()->VSSetConstantBuffers(0, 1, &dxwin->constbuffer);
    dxwin->setW(1000);
    dxwin->setH(500);
    dxwin->setshowmode(winmodule::windowshowmode::customsize);
    dxwin->setV(true);

    MSG msg = { 0 };
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            rotate(); // �������� ������� ����
            render();
        }
    }
	return 0;
}
void render() {
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // �������, �������, �����, �����-�����
    dxwin->getdx()->getdevicecontext()->ClearRenderTargetView(dxwin->getdx()->getrendertargetview(), ClearColor);
    dxwin->getdx()->getdevicecontext()->ClearDepthStencilView(dxwin->getdx()->getzbuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    dxwin->getdx()->getdevicecontext()->DrawIndexed(18, 0, 0);
    dxwin->getdx()->getswapchain()->Present(0, 0);
}
void rotate() {
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
    dxwin->worldmatrix = DirectX::XMMatrixRotationY(t);

    // ������� ������ ��� ������� set<NN>_rotation
    // ��������� �������� ���������� 2�� �����, � �� ������ �������, � �� ������ �����, Z �� ������ ������
    //dxwin->getcamera()->setY_rotation(0.0001f + dxwin->getcamera()->getY_rotation());
    //dxwin->getcamera()->setZ_rotation(0.0001f + dxwin->getcamera()->getZ_rotation());
    dxwin->getcamera()->setX_rotation(0.00001f + dxwin->getcamera()->getX_rotation());
    dxwin->viewmatrix = dxwin->getcamera()->getviewpoint();

    // �������� ����������� �����
    // ������� ��������� ��������� � ��������� � ��� �������
    dxmodule::constantbufferstruct cb;
    cb.worldmatrix = DirectX::XMMatrixTranspose(dxwin->worldmatrix);
    cb.viewmatrix = DirectX::XMMatrixTranspose(dxwin->viewmatrix);
    cb.projectionmatrix = DirectX::XMMatrixTranspose(dxwin->projectionmatrix);
    // ��������� ��������� ��������� � ����������� ����� g_pConstantBuffer
    dxwin->getdx()->getdevicecontext()->UpdateSubresource(dxwin->constbuffer, 0, NULL, &cb, 0, 0);
    //
}