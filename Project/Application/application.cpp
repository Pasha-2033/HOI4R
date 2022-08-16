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
/*SimpleVertex vertices[] = { //треугольник
    DirectX::XMFLOAT3(0.0f, 0.625f,  0.5f),
    DirectX::XMFLOAT3(0.6495f, -0.5f, 0.5f),
    DirectX::XMFLOAT3(-0.6495f, -0.5f, 0.5f)
};*/
// Создание буфера вершин (пять углов пирамиды)
SimpleVertex vertices[] =
{	/* координаты X, Y, Z				цвет R, G, B, A					 */
    { DirectX::XMFLOAT3(0.0f,  3.0f,  0.0f), DirectX::XMFLOAT4(2.0f, 2.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(-2.0f,  0.0f, -2.0f), DirectX::XMFLOAT4(0.0f, 2.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(2.0f,  0.0f, -2.0f), DirectX::XMFLOAT4(2.0f, 0.0f, 0.0f, 2.0f) },
    { DirectX::XMFLOAT3(-2.0f,  0.0f,  2.0f), DirectX::XMFLOAT4(0.0f, 2.0f, 2.0f, 2.0f) },
    { DirectX::XMFLOAT3(2.0f,  0.0f,  2.0f), DirectX::XMFLOAT4(2.0f, 0.0f, 2.0f, 2.0f) }
};
WORD indices[] =
{	// индексы массива vertices[], по которым строятся треугольники
    0,2,1,	/* Треугольник 1 = vertices[0], vertices[2], vertices[3] */
    0,3,4,	/* Треугольник 2 = vertices[0], vertices[3], vertices[4] */
    0,1,3,	/* и т. д. */
    0,4,2,

    1,2,3,
    2,4,3,
};
int main() {
    if (FAILED(dxwin->getHR())) return -1;
    
    dxmodule::createvertexbuffer(dxwin->getdx()->getdevice(), &vertexbuffer, vertices);
    dxmodule::createindexbuffer(dxwin->getdx()->getdevice(), &indexbuffer, indices);
    dxmodule::createconstbuffer(dxwin->getdx()->getdevice(), &constbuffer);
    dxwin->getdx()->getdevicecontext()->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
    dxwin->getdx()->getdevicecontext()->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R16_UINT, 0);

    //______________________________________
    //Инициализация матрицы мира
    worldmatrix = DirectX::XMMatrixIdentity();
    // Инициализация матрицы вида
    DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 1.0f, -10.0f, 1200.0f);	// Откуда смотрим
    DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// Куда смотрим
    DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	// Направление верха
    viewmatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);
    //Инициализация матрицы проекции
    projectionmatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, dxwin->getW() / (FLOAT)dxwin->getH(), 0.01f, 100.0f);
    //
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
    dxwin->setW(1000);
    dxwin->setH(500);
    dxwin->setshowmode(winmodule::windowshowmode::customsize);
    dxwin->setV(true);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        render();
    }
	return 0;
}
void render() {
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
    dxwin->getdx()->getdevicecontext()->ClearRenderTargetView(dxwin->getdx()->getrendertargetview(), ClearColor);
    dxwin->getdx()->getdevicecontext()->DrawIndexed(18, 0, 0);
    dxwin->getdx()->getswapchain()->Present(0, 0);
}