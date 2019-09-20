// MyApp2.cpp: ���� ���α׷��� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MyApp2.h"

#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

LPDIRECT3D9 g_pD3D; //g_ : �۷ι�(����)
LPDIRECT3DDEVICE9 g_pD3DDevice;
D3DCOLOR g_ClearColor = D3DCOLOR_XRGB(0, 0, 255);

//5) ��������Ʈ ��ü �������̽� ����, �ؽ�ó ��ü �������̽� ����
LPD3DXSPRITE g_pSpr[2] = { NULL };
LPDIRECT3DTEXTURE9 g_pTex[2] = { NULL };
D3DXVECTOR3 g_PCPos = D3DXVECTOR3(320, 400, 0);
D3DXVECTOR3 g_PCPos2 = D3DXVECTOR3(300, 200, 0);
D3DXVECTOR3 g_BlockPos = D3DXVECTOR3(100, 0, 0);
D3DXVECTOR3 g_BlockDNSpd = D3DXVECTOR3(0, 10, 0);

float g_SpawnXPos[5] = { 100.f,200.f,300.f,400.f,500.f };

//#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
												//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
												//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

												// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool InitDirect3D(HWND hWnd) {
	g_pD3D = NULL;
	g_pD3DDevice = NULL;

	//px������Ʈ ����
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) { //��ü ���� �ȵ�-> �Ұ���
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE; //�빮�� - ���Ұ� ������ �� ����, ��ȭ�� ���ɼ�
						   //BOOL : 4����Ʈ (������� 4����Ʈ�� ���� �����)
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;

	//dx Device ����
	//� Device ����?
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, //� �׷���ī��? == �⺻�׷���ī��
		D3DDEVTYPE_HAL, //�ϵ����Ÿ��
		hWnd, //hWnd ����
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //������ ���μ���? == �ϵ���� �̿�
		&d3dpp, //�������� ���� �ѱ��
		&g_pD3DDevice) == E_FAIL) //����̽� ��� ���� //���� E_FAIL (������ ���н� �Լ� ����)
								  //���� ���� S_OK //�ݼ��� S_FAIL
	{
		return false;
	}

	return true;
}

void ReleaseDirect3D() {
	if (g_pD3DDevice != NULL) {
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL) { //g_pD3D == NULL�� �Ұ��� �ϱ� ������
		g_pD3D->Release();
	}
}

void Render() {

	g_PCPos2 = g_PCPos2 + D3DXVECTOR3(0, 10, 0);

	if (g_PCPos2.y == 480) {
		g_PCPos2 = D3DXVECTOR3(rand() % 500 + 50, -50, 0);
	}

	if (g_PCPos == g_PCPos2) {
		g_PCPos2 = D3DXVECTOR3(0, 0, 0);
	}

	if (g_pD3DDevice == NULL) {
		return;
	}

	//����� �����
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, g_ClearColor, 1.0f, 0);

	D3DXVECTOR3 center = D3DXVECTOR3(50, 50, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		g_pSpr[0]->Begin(D3DXSPRITE_ALPHABLEND);
		g_pSpr[0]->Draw(g_pTex[0], NULL, &center, &g_PCPos, D3DCOLOR_XRGB(255, 255, 255)); //���󺯰� - ����̸� �״�� ���
		g_pSpr[0]->End();

		g_pSpr[1]->Begin(D3DXSPRITE_ALPHABLEND);
		g_pSpr[1]->Draw(g_pTex[1], NULL, &center, &g_PCPos2, D3DCOLOR_XRGB(255, 255, 255));
		g_pSpr[1]->End();

		g_pD3DDevice->EndScene();

	}

	//����� ����
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//5-1 ��������Ʈ ��ü ����
LPD3DXSPRITE GetSpriteObject() {
	LPD3DXSPRITE pSprite = NULL;
	//if (D3DXCreateSprite(g_pD3DDevice, &pSprite) == E_FAIL) {}

	if (FAILED(D3DXCreateSprite(g_pD3DDevice, &pSprite))) {
		return NULL;
	}
	return pSprite;
}

//5-2 ��������Ʈ ��ü ����
void ReleaseSpriteObject(LPD3DXSPRITE pSprite) {
	if (pSprite != NULL) { //Nullptr //NULL = 0
		pSprite->Release();
	}
}

//5-3 �ؽ�ó ������Ʈ ����
LPDIRECT3DTEXTURE9 LoadTextureFromFile(const TCHAR* filename) {
	LPDIRECT3DTEXTURE9 pTex;
	auto hr = D3DXCreateTextureFromFileEx(g_pD3DDevice, filename, 0, 0, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &pTex);

	if (FAILED(hr)) {
		return NULL;
	}

	return pTex;
}

//5-4 �ؽ�ó ������Ʈ �����ϱ�
void ReleaseTexture(LPDIRECT3DTEXTURE9 pTex) {
	if (pTex != NULL) {
		pTex->Release();
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); //�޴�����, Ÿ��Ʋ ���� ��� (rc ���� ����)
	//LoadStringW(hInstance, IDC_MYAPP2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	HWND hwnd;

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow, hwnd))
	{
		return FALSE;
	}

	InitDirect3D(hwnd);

	g_pTex[0] = LoadTextureFromFile(TEXT("C:\\Major\\player.bmp")); //\\ << �̰� �齽���� (�� bmp�� �����ϰ�)
	g_pTex[1] = LoadTextureFromFile(TEXT("C:\\Major\\block.bmp"));

	g_pSpr[0] = GetSpriteObject();
	g_pSpr[1] = GetSpriteObject();

	// HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYAPP2)); //����Ű ���� ���

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	srand(time(NULL));

	// �⺻ �޽��� �����Դϴ�.
	while (msg.message != WM_QUIT)
		//GetMessage(&msg, nullptr, 0, 0)) // �޽��� ���� -> ���� ����(���)
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) { ���� �ȿ� �� �� �� ��}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { //�޽��� ���� -> �������� �ѱ� (��ȿ�ð��� �ٸ� ȭ������)
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			static auto level = 0;
			g_BlockPos += g_BlockDNSpd;
			
			if (g_BlockPos.y > 480) {
				g_BlockPos = D3DXVECTOR3(g_SpawnXPos[rand() % 5], 0, 0);
				g_BlockDNSpd += D3DXVECTOR3(0, 10, 0);
				level++;
				if (level > 4) {
					level = 0;
					g_BlockDNSpd = D3DXVECTOR3(0, 10, 0);
				}
			}
			Render();
		}
	}

	//static auto level = 0;
	//g_BlockPos += g_BlockDNSpd;

	//if (g_BlockPos.y > 480) {
	//	g_BlockPos = D3DXVECTOR3(g_SpawnXPos[rand() % 5], 0, 0);
	//	g_BlockDNSpd += D3DXVECTOR3(0, 10, 0);
	//	level++;

	//	if (level > 4) {
	//		level = 0;
	//		g_BlockDNSpd = D3DXVECTOR3(0, 10, 0);
	//	}
	//}

	ReleaseSpriteObject(g_pSpr[0]);
	ReleaseSpriteObject(g_pSpr[1]);
	ReleaseTexture(g_pTex[0]);
	ReleaseTexture(g_pTex[1]);

	ReleaseDirect3D();

	return (int)msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYAPP2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_MYAPP2); - �޴� ���
	wcex.lpszClassName = TEXT("MyApp2"); //szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& rHwnd)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	HWND hWnd = CreateWindowW(TEXT("MyApp2"), TEXT("MyApp2"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	//TEXT("MyApp2") -> szWindowClass(Ŭ����), szTitle(Ÿ��Ʋ) ��

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	rHwnd = hWnd; //������ ���� �뵵 (rHwnd�� ������ hWnd �������� ����)

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/* //�޴��� ������ �ƹ��͵� �� ��
		case WM_COMMAND:
		{
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
		case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		break;
		*/
		/* //�ƹ��͵� �ȶ�
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		}
		break;
		*/
	case WM_KEYDOWN:
	{
		if (wParam == VK_F1) {
			g_ClearColor = D3DCOLOR_XRGB(255, 0, 0);
		}
		else if (wParam == VK_F2) {
			g_ClearColor = D3DCOLOR_XRGB(0, 0, 255);
		}
		else if (wParam == VK_LEFT) {
			g_PCPos = g_PCPos - D3DXVECTOR3(10, 0, 0);
		}
		else if (wParam == VK_RIGHT) {
			g_PCPos = g_PCPos + D3DXVECTOR3(10, 0, 0);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
UNREFERENCED_PARAMETER(lParam);
switch (message)
{
case WM_INITDIALOG:
return (INT_PTR)TRUE;

case WM_COMMAND:
if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
{
EndDialog(hDlg, LOWORD(wParam));
return (INT_PTR)TRUE;
}
break;
}
return (INT_PTR)FALSE;
} */
