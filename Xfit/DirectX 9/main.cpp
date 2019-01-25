#include <Windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

HWND hWnd;
HINSTANCE hInstance;

IDirect3D9Ex* d3d;
IDirect3DDevice9Ex* d3dDev;

float rotateY = 0.f;

struct ColorVertex {
	float x, y, z;
	unsigned long color;
};
ColorVertex colorTriVertex[] = { { 2.0f, -2.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255) },
		{ 0.0f, 2.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0) },
		{ -2.0f, -2.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)} };
IDirect3DVertexBuffer9* colorVertexBuffer;

constexpr unsigned long colorVertexFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

LRESULT WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) {
	switch (_msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}
int APIENTRY _tWinMain(HINSTANCE _hInstance, HINSTANCE, LPTSTR,int) {
	hInstance = _hInstance;

	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground =nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszClassName = _T("DirectX 9");
	wc.lpszMenuName = nullptr;
	
	RegisterClass(&wc);

	hWnd = CreateWindow(wc.lpszClassName, _T("DirectX 9 Test"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,CW_USEDEFAULT, 640, 480, nullptr, nullptr, hInstance, nullptr);
	
	Direct3DCreate9Ex(D3D_SDK_VERSION,&d3d);

	D3DPRESENT_PARAMETERS d3dPresentParam;

	ZeroMemory(&d3dPresentParam, sizeof(d3dPresentParam));
	d3dPresentParam.Windowed = true;
	d3dPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPresentParam.hDeviceWindow = hWnd;
	d3dPresentParam.BackBufferWidth = 640;
	d3dPresentParam.BackBufferHeight = 480;
	d3dPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dPresentParam.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dPresentParam.EnableAutoDepthStencil = TRUE;
	
	d3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dPresentParam, nullptr, &d3dDev);
	
	d3dDev->CreateVertexBuffer(3 * sizeof(ColorVertex), 0, colorVertexFVF, D3DPOOL_DEFAULT, &colorVertexBuffer, nullptr);

	void* colorVertexData;
	colorVertexBuffer->Lock(0, 0, &colorVertexData, 0);
	memcpy(colorVertexData, colorTriVertex, sizeof(colorTriVertex));
	colorVertexBuffer->Unlock();

	d3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3dDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	D3DXMATRIX viewMat;
	D3DXMatrixLookAtLH(&viewMat, &D3DXVECTOR3(0.f, 0.f, 5.f), &D3DXVECTOR3(0.f, 0.f, 0.f), &D3DXVECTOR3(0.f, 1.f, 0.f));
	d3dDev->SetTransform(D3DTS_VIEW, &viewMat);

	D3DXMATRIX projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DXToRadian(45.f), 640.f / 480.f, 1.f, 1000.f);
	d3dDev->SetTransform(D3DTS_PROJECTION, &projMat);

	d3dDev->SetFVF(colorVertexFVF);
	d3dDev->SetStreamSource(0, colorVertexBuffer, 0, sizeof(ColorVertex));

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	MSG msg;
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		d3dDev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 100), 1.f, 0);
		d3dDev->Clear(0, nullptr, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);

		d3dDev->BeginScene();

		D3DXMATRIX rotateYMat;
		rotateY += 0.01f;
		D3DXMatrixRotationY(&rotateYMat, rotateY);
		d3dDev->SetTransform(D3DTS_WORLD, &rotateYMat);
		

		d3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		D3DXMATRIX tranMat,worldMat;
		D3DXMatrixTranslation(&tranMat, 1.f, 0.f, -2.f);
		D3DXMatrixMultiply(&worldMat, &rotateYMat, &tranMat);
		d3dDev->SetTransform(D3DTS_WORLD, &worldMat);


		d3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


		d3dDev->EndScene();
		d3dDev->Present(nullptr, nullptr, nullptr, nullptr);
	}
	d3dDev->Release();
	d3d->Release();
	return (int)msg.wParam;
}
