// MyApp2.cpp: 응용 프로그램의 진입점을 정의합니다.
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

LPDIRECT3D9 g_pD3D; //g_ : 글로벌(전역)
LPDIRECT3DDEVICE9 g_pD3DDevice;
D3DCOLOR g_ClearColor = D3DCOLOR_XRGB(0, 0, 255);

//5) 스프라이트 객체 인터페이스 정의, 텍스처 객체 인터페이스 정의
LPD3DXSPRITE g_pSpr[2] = { NULL };
LPDIRECT3DTEXTURE9 g_pTex[2] = { NULL };
D3DXVECTOR3 g_PCPos = D3DXVECTOR3(320, 400, 0);
D3DXVECTOR3 g_PCPos2 = D3DXVECTOR3(300, 200, 0);
D3DXVECTOR3 g_BlockPos = D3DXVECTOR3(100, 0, 0);
D3DXVECTOR3 g_BlockDNSpd = D3DXVECTOR3(0, 10, 0);

float g_SpawnXPos[5] = { 100.f,200.f,300.f,400.f,500.f };

//#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
												//WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
												//WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

												// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool InitDirect3D(HWND hWnd) {
	g_pD3D = NULL;
	g_pD3DDevice = NULL;

	//px오브젝트 생성
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL) { //객체 연결 안됨-> 불가능
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE; //대문자 - 마소가 재정의 함 ㅋㅋ, 변화될 가능성
						   //BOOL : 4바이트 (윈도우는 4바이트에 맞춰 사용함)
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;

	//dx Device 생성
	//어떤 Device 생성?
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, //어떤 그래픽카드? == 기본그래픽카드
		D3DDEVTYPE_HAL, //하드웨어타입
		hWnd, //hWnd 연결
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //폴리곤 프로세싱? == 하드웨어 이용
		&d3dpp, //세부적인 정보 넘기기
		&g_pD3DDevice) == E_FAIL) //디바이스 어디 리턴 //실패 E_FAIL (완전히 실패시 함수 종료)
								  //완전 성공 S_OK //반성공 S_FAIL
	{
		return false;
	}

	return true;
}

void ReleaseDirect3D() {
	if (g_pD3DDevice != NULL) {
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL) { //g_pD3D == NULL이 불가능 하기 때문에
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

	//백버퍼 지우기
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, g_ClearColor, 1.0f, 0);

	D3DXVECTOR3 center = D3DXVECTOR3(50, 50, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		g_pSpr[0]->Begin(D3DXSPRITE_ALPHABLEND);
		g_pSpr[0]->Draw(g_pTex[0], NULL, &center, &g_PCPos, D3DCOLOR_XRGB(255, 255, 255)); //색상변경 - 흰색이면 그대로 출력
		g_pSpr[0]->End();

		g_pSpr[1]->Begin(D3DXSPRITE_ALPHABLEND);
		g_pSpr[1]->Draw(g_pTex[1], NULL, &center, &g_PCPos2, D3DCOLOR_XRGB(255, 255, 255));
		g_pSpr[1]->End();

		g_pD3DDevice->EndScene();

	}

	//백버퍼 전송
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//5-1 스프라이트 객체 생성
LPD3DXSPRITE GetSpriteObject() {
	LPD3DXSPRITE pSprite = NULL;
	//if (D3DXCreateSprite(g_pD3DDevice, &pSprite) == E_FAIL) {}

	if (FAILED(D3DXCreateSprite(g_pD3DDevice, &pSprite))) {
		return NULL;
	}
	return pSprite;
}

//5-2 스프라이트 객체 해제
void ReleaseSpriteObject(LPD3DXSPRITE pSprite) {
	if (pSprite != NULL) { //Nullptr //NULL = 0
		pSprite->Release();
	}
}

//5-3 텍스처 오브젝트 열기
LPDIRECT3DTEXTURE9 LoadTextureFromFile(const TCHAR* filename) {
	LPDIRECT3DTEXTURE9 pTex;
	auto hr = D3DXCreateTextureFromFileEx(g_pD3DDevice, filename, 0, 0, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &pTex);

	if (FAILED(hr)) {
		return NULL;
	}

	return pTex;
}

//5-4 텍스처 오브젝트 해제하기
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

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); //메뉴정보, 타의틀 정보 등등 (rc 파일 참고)
	//LoadStringW(hInstance, IDC_MYAPP2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	HWND hwnd;

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow, hwnd))
	{
		return FALSE;
	}

	InitDirect3D(hwnd);

	g_pTex[0] = LoadTextureFromFile(TEXT("C:\\Major\\player.bmp")); //\\ << 이게 백슬래시 (꼭 bmp로 저장하것)
	g_pTex[1] = LoadTextureFromFile(TEXT("C:\\Major\\block.bmp"));

	g_pSpr[0] = GetSpriteObject();
	g_pSpr[1] = GetSpriteObject();

	// HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYAPP2)); //단축키 만들어서 사용

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	srand(time(NULL));

	// 기본 메시지 루프입니다.
	while (msg.message != WM_QUIT)
		//GetMessage(&msg, nullptr, 0, 0)) // 메시지 없다 -> 멈춤 상태(대기)
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) { 여기 안에 저 밑 두 줄}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { //메시지 없다 -> 다음으로 넘김 (유효시간때 다른 화면으로)
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
	wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_MYAPP2); - 메뉴 사용
	wcex.lpszClassName = TEXT("MyApp2"); //szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& rHwnd)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(TEXT("MyApp2"), TEXT("MyApp2"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	//TEXT("MyApp2") -> szWindowClass(클래스), szTitle(타이틀) 순

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	rHwnd = hWnd; //리턴을 위한 용도 (rHwnd의 내용을 hWnd 내용으로 참조)

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/* //메뉴를 눌러도 아무것도 안 됨
		case WM_COMMAND:
		{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
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
		/* //아무것도 안뜸
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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
// 정보 대화 상자의 메시지 처리기입니다.
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
