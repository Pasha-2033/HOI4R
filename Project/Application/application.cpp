#include "application.h"

#include <DirectXMath.h>			//DirectX::


using namespace std;
HWND win;
winmodule::window* winclass;
dxmodule::directx* dx;
dxmodule::pixelshaderoperator* pso;
dxmodule::vertexshaderoperator* vso;

struct SimpleVertex {
    DirectX::XMFLOAT3 pos;
};
SimpleVertex xyz[]{
        DirectX::XMFLOAT3(0.0f,  5.5f,  5.5f),
        DirectX::XMFLOAT3(5.5f, -5.5f,  5.5f),
        DirectX::XMFLOAT3(-5.5f, -5.5f,  5.5f)
};
ID3D11Buffer* b;

LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        PAINTSTRUCT ps;
        HDC hdc;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}
void render();
int main() {
    winclass = new winmodule::window(WndProc, L"tempclass", L"title");
    winclass->setshowmode(winmodule::windowshowmode::customsize);
    winclass->setW(500);
    winclass->setH(500);
    winclass->setV(true);
    dx = new dxmodule::directx(winclass);
    pso = new dxmodule::pixelshaderoperator;
    vso = new dxmodule::vertexshaderoperator;
    pso->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"pixel shader 0", "PS", "ps_4_0", dx->getdevice());
    vso->addshader((WCHAR*)L"Application/Resources/GFX/FX/tempPixelShader.fx", (WCHAR*)L"vertex shader 0", "VS", "vs_4_0", dx->getdevice());

    dx->getdevicecontext()->IASetInputLayout(vso->getshaderlayout(0));
    dxmodule::createvertexbuffer(dx->getdevice(), &b, xyz);
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    dx->getdevicecontext()->IASetVertexBuffers(0, 1, &b, &stride, &offset);
    dx->getdevicecontext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    dx->getdevicecontext()->VSSetShader(vso->getvertexshader(0), NULL, 0);
    dx->getdevicecontext()->PSSetShader(pso->getpixelshader(0), NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            render();      
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return 0;
}
void render() {
    float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
    dx->getdevicecontext()->ClearRenderTargetView(dx->getrendertargetview(), ClearColor);
    dx->getdevicecontext()->Draw(3, 0);
    dx->getswapchain()->Present(0, 0);
}