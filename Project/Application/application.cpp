#include "application.h"
#include "GraphicCore/DirectX/include/dxwindow.h"
void render();
dxwindow::dxwindowclass* dxwin = new dxwindow::dxwindowclass(NULL, true);
ID3D11Buffer* vertexbuffer = NULL;
UINT stride = sizeof(SimpleVertex);
UINT offset = 0;
SimpleVertex vertices[] ={
    DirectX::XMFLOAT3(0.0f, 0.625f,  0.5f),
    DirectX::XMFLOAT3(0.6495f, -0.5f, 0.5f),
    DirectX::XMFLOAT3(-0.6495f, -0.5f, 0.5f)
};
int main() {
    if (FAILED(dxwin->getHR())) return -1;

    dxwin->setW(500);
    dxwin->setH(500);

    dxwin->vso->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "VS", "vs_4_0", dxwin->getdx()->getdevice());
    dxwin->pso->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"Pixel Shader 0", "PS", "ps_4_0", dxwin->getdx()->getdevice());
    dxmodule::createvertexbuffer(dxwin->getdx()->getdevice(), &vertexbuffer, vertices);
    dxwin->getdx()->getdevicecontext()->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
    dxwin->getdx()->getdevicecontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    dxwin->applyvertexshader(0);
    dxwin->applypixelshader(0);

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
    dxwin->getdx()->getdevicecontext()->Draw(3, 0);
    dxwin->getdx()->getswapchain()->Present(0, 0);
}