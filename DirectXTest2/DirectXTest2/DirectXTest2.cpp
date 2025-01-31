// DirectXTest2.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DirectXTest2.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <memory>

#include "CSprite.h"
#include "CTexture.h"
#include "CEntity.h"
#include "CGameObject.h"
#include "CInput.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib" )

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LPDIRECT3D9 g_pD3D;
LPDIRECT3DDEVICE9 g_pD3DDevice;
D3DCOLOR g_ClearColor = D3DCOLOR_XRGB(0, 0, 255);

bool InitDirect3D(HWND hwnd);
void ReleaseDirect3D();

typedef shared_ptr<CGameObject> GoPtr;
typedef vector<GoPtr> arrayGoPtr;

void Render(GoPtr player,arrayGoPtr& monster, arrayGoPtr& bullet, arrayGoPtr& monstershoot);
void SetMonsterDir(arrayGoPtr& monster, const D3DXVECTOR3& dir);

//shared_ptr<CGameObject> g_Gameplayer;
//CGameObject* g_GameMonster[20];
//CGameObject* g_bullet[20];
//CInput* g_Input;


D3DXVECTOR3 g_monsterDir = { 1.f,0.f,0.f };


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTXTEST2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	HWND hWnd = NULL;

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow, hWnd))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTXTEST2));

	InitDirect3D(hWnd);
	auto g_Input = shared_ptr<CInput>(new CInput(hWnd, hInstance));
	
	//shared_ptr<CGameObject> g_Gameplayer = make_shared<CGameObject>( new CGameObject(g_pD3DDevice, _T("airplane.bmp"), { 320.f,440.f,0.f }));
	shared_ptr<CGameObject> g_Gameplayer = shared_ptr<CGameObject>( new CGameObject(g_pD3DDevice, _T("airplane.bmp"), { 320.f,440.f,0.f }));
	g_Gameplayer->SetSpeed(200.f);
	g_Gameplayer->SetAlive(true);

	vector<shared_ptr<CGameObject>> g_GameMonster;
	vector<shared_ptr<CGameObject>> g_bullet;
	stack<shared_ptr<CGameObject>> g_bulletPool;
	arrayGoPtr g_MonsterShoot;

	for (int i = 0; i < 20; ++i) {
		auto _invader = shared_ptr<CGameObject>(new CGameObject(g_pD3DDevice, _T("monster.bmp")
			, { 32.f + float(i % 5)*64.f,32.f + float(i / 5)*64.f,0.f }));
		_invader->SetDir(g_monsterDir);
		_invader->SetAlive(true);

		g_GameMonster.push_back(_invader);
	}

	for (int i = 0; i < 20; ++i) {
		auto _bullet = shared_ptr<CGameObject>(new CGameObject(g_pD3DDevice, _T("bullet.bmp"), { 0,0,0 }, { 8,16,0 }, { 0,-1,0 }, 200.f));
		g_bullet.push_back(_bullet);
		g_bulletPool.push(g_bullet[i]); //stack에는 쓴 총알만 들어가진다.
	}
	
	for (int i = 0; i < 5; ++i) {
		auto _mbullet = shared_ptr<CGameObject>(new CGameObject(g_pD3DDevice, _T("bullet.bmp"), { 0,0,0 }, { 8,16,0 }, { 0,1,0 }, 100.f));
		g_MonsterShoot.push_back(_mbullet);
	}
	srand((unsigned int)time(NULL));
	
    MSG msg;
	ZeroMemory(&msg, sizeof(msg));

    // 기본 메시지 루프입니다.
    while (msg.message!=WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else {
			g_Input->ReadKeyboard();
			if (g_Input->IsEscapePressed())
				msg.message = WM_QUIT;

			D3DXVECTOR3 keyDir = { 0.f,0.f,0.f };

			g_Input->GetInputDir(keyDir);
			//keyDir.y += g_Input->IsKeyPressed(DIK_UPARROW) ? -1.f : 0.f;
			//keyDir.y += g_Input->IsKeyPressed(DIK_DOWNARROW) ? 1.f : 0.f;

			static DWORD dwPrevTime=GetTickCount();
			DWORD dwCurTime = GetTickCount();
			DWORD dwDt = dwCurTime - dwPrevTime;
			float fDeltaTime = (float)(dwDt) * 0.001f;
			dwPrevTime = dwCurTime;

			g_Gameplayer->SetDir(keyDir);		
			g_Gameplayer->Update(fDeltaTime);

			for (size_t i = 0; i < g_GameMonster.size(); ++i) {
				g_GameMonster[i]->Update(fDeltaTime);
			}

			//for _each(g_GameMonster.begin(), g_GameMonster.end(), [](auto go) {
			//go->Update(fDeltaTime);
			//});
			
			for (size_t i = 0; i < g_bullet.size(); ++i) {
				if(g_bullet[i]->GetAlive())
				g_bullet[i]->Update(fDeltaTime);
			}

			for (size_t i = 0; i < g_MonsterShoot.size(); ++i) {
				if (g_MonsterShoot[i]->GetAlive())
					g_MonsterShoot[i]->Update(fDeltaTime);
			}

			for (size_t i = 0; i < g_MonsterShoot.size(); ++i) {
				if (g_MonsterShoot[i]->GetAlive())
				{
					//적 총알 바운드 처리
					auto pos = g_MonsterShoot[i]->GetPos();
					if (pos.y > 480.f)
						g_MonsterShoot[i]->SetAlive(false);

					//플레이어의 충돌 처리
					auto pcPos = g_Gameplayer->GetPos();

					if (pos.x > pcPos.x - (32.f + 8.f)
						&& pos.x < pcPos.x + (32.f + 8.f)
						&& pos.y < pcPos.y - (32.f + 16.f)
						&& pos.y < pcPos.y + (32.f + 16.f))
					{
						g_MonsterShoot[i]->SetAlive(false);
					}
				}
			}
			

			//몬스터 벽 충돌
			for (size_t i = 0; i < g_GameMonster.size(); ++i)
			{
				auto pos = g_GameMonster[i]->GetPos();
				if (pos.x > 640.f - 32.f && g_GameMonster[i]->GetAlive())
				{
					g_monsterDir = { -1.f,0.f,0.f };
					SetMonsterDir(g_GameMonster,g_monsterDir);
					break;
				}
				else if (pos.x < 32.f&& g_GameMonster[i]->GetAlive())
				{
					g_monsterDir = { 1.f,0.f,0.f };
					SetMonsterDir(g_GameMonster,g_monsterDir);
					break;
				}
			}

			

			if (g_Input->IsSpaceKeyPressed())
			{
				if (!g_bulletPool.empty())
				{
					auto pShoot = g_bulletPool.top();
					auto pos = g_Gameplayer->GetPos(); 
					pShoot->SetPos(pos + D3DXVECTOR3(16, -32, 0));
					pShoot->SetAlive(true);
					g_bulletPool.pop();
				}
				if (!g_bulletPool.empty())
				{
					auto pShoot = g_bulletPool.top();
					auto pos = g_Gameplayer->GetPos();
					pShoot->SetPos(pos + D3DXVECTOR3(-16, -32, 0));
					pShoot->SetAlive(true);
					g_bulletPool.pop();
				}
				
			}

			static float s_ShootTimer = 0.f;
			s_ShootTimer += fDeltaTime;

			//괴물 총알 발사
			if(s_ShootTimer>1.f)
			{
				s_ShootTimer = 0.f;

				for (size_t i = 0; i < g_MonsterShoot.size(); ++i) {
					if (g_MonsterShoot[i]->GetAlive() == false)
					{
						// 1) 생성할 몬스터 인덱스얻기
						int idx;
						int loop = 10;
						do
						{
							idx = rand() % (int)g_GameMonster.size();
							loop--;
						} while (g_GameMonster[idx]->GetAlive() == false && loop > 0);

						if (g_GameMonster[idx]->GetAlive() == true)
						{
							auto pos = g_GameMonster[idx]->GetPos();
							g_MonsterShoot[i]->SetPos(pos + D3DXVECTOR3(16, 32, 0));
							g_MonsterShoot[i]->SetAlive(true);
						}
						break;
					}
				}
			}

			//발사체 처리
			/*for (int n = 0; n < 20; ++n) {
				if (g_bullet[n]->GetAlive() == false &&
					g_Input->IsKeyPressed(DIK_SPACE))
				{
					auto pos = g_Gameplayer->GetPos();
					g_bullet[n]->SetPos(pos + D3DXVECTOR3(0, -32, 0));
					g_bullet[n]->SetAlive(true);
				}
			}*/

			for (size_t n = 0; n < g_bullet.size(); ++n) {
				if (g_bullet[n]->GetAlive()) {
					auto pos = g_bullet[n]->GetPos(); 

					if (pos.y < 0.f)
					{
						g_bullet[n]->SetAlive(false);
						g_bulletPool.push(g_bullet[n]);
					}

					for (size_t i = 0; i < g_GameMonster.size(); ++i)
					{
						if (g_GameMonster[i]->GetAlive()) {

							auto mpos = g_GameMonster[i]->GetPos();

							if (pos.x > mpos.x - (32.f + 8.f)
								&& pos.x < mpos.x + (32.f + 8.f)
								&& pos.y < mpos.y - (32.f + 16.f)
								&& pos.y < mpos.y + (32.f + 16.f))
							{
								g_GameMonster[i]->SetAlive(false);
								g_bullet[n]->SetAlive(false);
								g_bulletPool.push(g_bullet[n]);
								break;
							}
						}
					}
				}
			}

			Render(g_Gameplayer,g_GameMonster,g_bullet,g_MonsterShoot);

		}
    }
	/*for (size_t i = 0; i < g_bullet.size(); ++i)
		 g_bullet[i].reset();*/
	g_bullet.clear();

	/*for (size_t i = 0; i < g_GameMonster.size(); ++i)
		g_GameMonster[i].reset();*/
	g_GameMonster.clear();

	//delete g_Gameplayer; //shared pointer이기 때문에 프로그램이 꺼지면 자동으로 사라짐
	//delete g_Input;
	
	CTexture::ReleaseAll();

	ReleaseDirect3D();
	
    return (int) msg.wParam;
}

void SetMonsterDir(arrayGoPtr& monster, const D3DXVECTOR3& dir)
{
	for (int n = 0; n < 20; ++n)
	{
		monster[n]->SetPos(
			monster[n]->GetPos() + D3DXVECTOR3(0, 10, 0));
		monster[n]->SetDir(dir);
	}
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTXTEST2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DIRECTXTEST2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& hWnd)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
}

bool InitDirect3D(HWND hwnd)
{
	//DX 오브젝트 생성
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
		return false;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;

	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, hwnd
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &d3dpp
		, &g_pD3DDevice) == E_FAIL)
		return false;
}

void ReleaseDirect3D()
{
	if (g_pD3DDevice != NULL)
		g_pD3DDevice->Release();
	if (g_pD3D != NULL)
		g_pD3D->Release();

	g_pD3DDevice = NULL;
	g_pD3D = NULL;
}

void Render(GoPtr player, arrayGoPtr& monster, arrayGoPtr& bullet, arrayGoPtr& monstershoot)
{
	if (g_pD3DDevice == NULL)
		return;
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, g_ClearColor, 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		player->Draw();
		for (size_t i = 0; i < monster.size(); ++i) {
			monster[i]->Draw();

		}
		for (size_t i = 0; i < bullet.size(); ++i)
			if (bullet[i]->GetAlive()) {
				bullet[i]->Draw();
			}

		for (size_t i = 0; i < monstershoot.size(); ++i)
			if (monstershoot[i]->GetAlive()) {
				monstershoot[i]->Draw();
			}
	}
		g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

