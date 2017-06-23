// include the basic windows header files and the Direct3D header file
#include "Header.h"
#include "Class.h"
#include "Stage.h"


// 클래스 함수 초기화
void entity::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
	status = Tile_nomal;
	color = 0;
}

void colorStr::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

void colorStr::init()
{
	str = "";
	colorNum = 0;
}

//객체 생성 
entity tile[10][10];

colorStr Rstr[10][10];
colorStr Bstr[10][10];


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	// 주석된 부분을 풀고 windowed = false 하면 전체화면
	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Color Nemo Nemo Logic",
		WS_EX_TOPMOST | WS_POPUP /*WS_OVERLAPPEDWINDOW*/, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);
	init_tile();

	// enter the main loop:
	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		render_frame();

		if (stage == -1)		// 타이틀 화면
			title();

		else if (0 <= stage && stage < 20)	
		{
			GameLogic();

			if (!HintControl)
			{
				Hint();
				ColorHint();
				HintControl = true;	// 힌트는 스테이지가 바뀔때마다 한번만 그려줌
			}
		}

		else if (stage == 20)	// 클리어 화면
			GameClear();


		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);


		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	// 이부분 FALSE시 전체화면
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object
										  //init_graphics();

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"Panel3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite



	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"empty.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_empty_tile);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"widthLine.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_width_line);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"HeightLine.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_height_line);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"nextButton.png",    // the file name
		150,    // default width
		150,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_next);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"buttonBG.png",    // the file name
		100,    // default width
		557,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_palette);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"blackButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_black_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"whiteButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_white_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"blueButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_blue_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"skinButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_skin_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"yellowButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_yellow_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"pinkButton.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_pink_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"X.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_Xmark_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"black.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_black_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"white.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_white_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"blue.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_blue_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"skin.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_skin_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"yellow.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_yellow_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"pink.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_pink_tile);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"title.png",    // the file name
		1920,    // default width
		1080,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 255, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_title);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"start.png",    // the file name
		606,    // default width
		260,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 255, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_start_button);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"clear.png",    // the file name
		1920,    // default width
		1080,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 255, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_clear);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"replayButton.png",    // the file name
		150,    // default width
		150,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_replay_button);    // load to sprite

	D3DXCreateFont(d3ddev,
		32,
		0,
		FW_BOLD,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"나눔고딕체",
		&Font);

	D3DXCreateFont(d3ddev,
		60,
		0,
		FW_BOLD,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"나눔고딕체",
		&UIFont);

	D3DXCreateSprite(d3ddev, &FontSprite);
	return;
}


// 타일을 64픽셀 간격으로 배열
void init_tile()	
{
	for (int i = 0; i<X_SIZE; i++)
	{
		for (int j = 0; j<Y_SIZE; j++)
		{
			tile[i][j].init(i * 64.0f, j * 64.0f);
		}
	}
}

// 인게임 로직 (넥스트 버튼, 컬러 팔레트)
void GameLogic()
{

	POINT p;						// 마우스 입력
	int Check[10][10] = { 0 };		// next확인용 배열

	if (KEY_DOWN(VK_LBUTTON))
	{
		MessageBeep(0);
		GetCursorPos(&p); //get current mouse position

		int x_index = (p.x - 639) / 64;
		int y_index = (p.y - 219) / 64;

		// 팔레트 클릭시 현재 색상값 저장
		if (200 <= p.x && p.x <= 264)
		{
			for (int i = 0; i < 6; i++)
			{
				if (255 + i * 90 <= p.y && p.x <= 319 + i * 90)
					nowColor = i + 1;
			}
		}

		// Next 버튼
		if (1730 <= p.x && p.x <= 1880 && 880 <= p.y && p.y <= 1030 && stage < 20)
		{
			bool nextChk = false;

			if (!nextChk)
				nextChk = true;


			for (int i = 0; i < X_SIZE; i++)
			{
				for (int j = 0; j < Y_SIZE; j++)
				{
					if (Game[stage][j][i] > 0)
					{
						if (tile[i][j].status == Tile_wrong || tile[i][j].color == 0)
							nextChk = false;
					}
				}
			}

			if (nextChk)
			{
				stage++;
				init_tile();
				HintControl = false;
				nextChk = false;
			}

			if (stage == 20)	// 20스테이지 일때, 축하 빵빠레울림
			{
				endTime = clock();
				PlaySound(NULL, NULL, NULL);
				PlaySound(L"clear.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
		}



		// 컬러값 저장 및 타일 상태 변경
		if (0 <= x_index && x_index < 10 && 0 <= y_index && y_index < 10)
		{
			tile[x_index][y_index].color = nowColor;

			if (tile[x_index][y_index].color == Game[stage][y_index][x_index])
				tile[x_index][y_index].status = Tile_check;

			else
				tile[x_index][y_index].status = Tile_wrong;

		}

	}

	// X 마킹
	if (KEY_DOWN(VK_RBUTTON))
	{
		MessageBeep(0);
		GetCursorPos(&p); //get current mouse position

		int x_index = (p.x - 639) / 64;
		int y_index = (p.y - 219) / 64;

		if (0 <= x_index && x_index < 10 && 0 <= y_index && y_index < 10)
		{
			tile[x_index][y_index].status = Tile_Xmark;
			tile[x_index][y_index].color = Non;
		}
	}
}

// 타이틀 버튼 클릭 및 BGM 재생
void title()
{
	POINT p;

	if (KEY_DOWN(VK_LBUTTON))
	{
		GetCursorPos(&p); //get current mouse position

		if (662 < p.x && p.x < 1268 && 745 < p.y && p.y < 1005)
		{
			stage = 0;
			startTime = clock();
			PlaySound(L"bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

	}
}

void Hint()
{
	int Lcount = 0;		// 왼쪽 힌트 로직 카운트
	int Tcount = 0;		// 상단 힌트 로직 카운트

	int LPresent = 0;	// 왼쪽 현재 스테이지의 컬러값
	int TPresent = 0;	// 상단 현재 스테이지의 컬러값
	char number[10];

	// CString 초기화
	Lstr = "";
	for (int k = 0; k < X_SIZE; k++)
	{
		Tstring[k] = "";
	}
	UI = "";

	// 힌트
	for (int i = 0; i < Y_SIZE; i++)
	{
		for (int j = 0; j < X_SIZE; j++)
		{

			// 현재 스테이지의 컬러값
			LPresent = Game[stage][i][j];
			TPresent = Game[stage][j][i];

			// Left
			if (LPresent != 0)	
			{
				Lcount++;


				if (Lcount == 10)
				{
					sprintf_s(number, "%d", Lcount);

					Lstr += number;
					Lstr += "  ";

					Lcount = 0;
				}

				if (j == 9 && Lcount != 0)
				{
					sprintf_s(number, "%d", Lcount);

					Lstr += number;
					Lstr += "  ";

					Lcount = 0;
				}
			}

			else
			{
				if (Lcount != 0)
				{
					sprintf_s(number, "%d", Lcount);

					Lstr += number;
					Lstr += "  ";

					Lcount = 0;
				}
			}



			// Top
			if (TPresent != 0)
			{
				Tcount++;

				if (Tcount == 10)
				{
					sprintf_s(number, "%d", Tcount);

					Tstring[i] += number;
					Tstring[i] += "\n";

					Tcount = 0;
				}

				if (j == 9 && Tcount != 0)
				{
					sprintf_s(number, "%d", Tcount);

					Tstring[i] += number;
					Tstring[i] += "\n";

					Tcount = 0;
				}
			}

			else
			{
				if (Tcount != 0)
				{
					sprintf_s(number, "%d", Tcount);

					Tstring[i] += number;
					Tstring[i] += "\n";

					Tcount = 0;
				}
			}

		}
		Lstr += "\n\n";
	}

	UI += "STAGE : ";
	sprintf_s(number, "%d", stage + 1);
	UI += number;
	UI += " / 20";
}

void ColorHint()
{
	int Rcount = 0;		// 왼쪽 힌트 로직 카운트
	int Bcount = 0;		// 상단 힌트 로직 카운트

	int RPresent = 0;	// 왼쪽 현재 스테이지의 컬러값
	int BPresent = 0;	// 상단 현재 스테이지의 컬러값

	int Rbefore = 0;	// 왼쪽 이전 칸의 컬러값
	int Bbefore = 0;	// 상단 이전 칸의 컬러값

	char number[10];

	// 힌트
	for (int i = 0; i < Y_SIZE; i++)
	{
		for (int j = 0; j < X_SIZE; j++)
		{
			// CString 초기화
			Rstr[i][j].init();
			Bstr[i][j].init();

			// present 값 대입
			RPresent = Game[stage][i][j];
			BPresent = Game[stage][j][i];

			// before 값 대입
			if (j > 0)
			{
				Rbefore = Game[stage][i][j - 1];
				Bbefore = Game[stage][j - 1][i];
			}

			else
			{
				Rbefore = 0;
				Bbefore = 0;
			}

			// right
			if (RPresent != 0)
			{
				if (Rbefore == 0 || RPresent == Rbefore)
				{
					Rcount++;
					Rstr[i][j].colorNum = RPresent;
				}

				if (Rbefore != 0 && RPresent != Rbefore)
				{
					if (Rcount != 0)
					{
						sprintf_s(number, "%d", Rcount);

						Rstr[i][j - 1].str += number;
						Rstr[i][j - 1].str += "  ";

						Rstr[i][j - 1].colorNum = Rbefore;
					}

					Rcount = 0;
					Rcount++;
				}

				if (Rcount == 10)
				{
					sprintf_s(number, "%d", Rcount);

					Rstr[i][j].str += number;
					Rstr[i][j].str += "  ";

					Rstr[i][j].colorNum = RPresent;

					Rcount = 0;
				}

				if (j == 9 && Rcount != 0)
				{
					sprintf_s(number, "%d", Rcount);

					Rstr[i][j].str += number;
					Rstr[i][j].str += "  ";

					Rstr[i][j].colorNum = RPresent;

					Rcount = 0;
				}
			}

			else
			{
				if (Rcount != 0)
				{
					sprintf_s(number, "%d", Rcount);

					Rstr[i][j].str += number;
					Rstr[i][j].str += "  ";

					Rstr[i][j].colorNum = Rbefore;

					Rcount = 0;
				}
			}



			// Bottom
			if (BPresent != 0)
			{
				if (Bbefore == 0 || BPresent == Bbefore)
				{
					Bcount++;
					Bstr[i][j].colorNum = BPresent;

				}
				if (Bbefore != 0 && BPresent != Bbefore)
				{
					sprintf_s(number, "%d", Bcount);

					Bstr[i][j].str += number;
					Bstr[i][j].str += "\n";

					Bstr[i][j].colorNum = Bbefore;

					Bcount = 0;
					Bcount++;
				}

				if (Bcount == 10)
				{
					sprintf_s(number, "%d", Bcount);

					Bstr[i][j].str += number;
					Bstr[i][j].str += "\n";

					Bstr[i][j].colorNum = BPresent;

					Bcount = 0;
				}

				if (j == 9 && Bcount != 0)
				{
					sprintf_s(number, "%d", Bcount);

					Bstr[i][j].str += number;
					Bstr[i][j].str += "\n";

					Bstr[i][j].colorNum = Game[stage][j][i];

					Bcount = 0;
				}
			}

			else
			{
				if (Bcount != 0)
				{
					sprintf_s(number, "%d", Bcount);

					Bstr[i][j].str += number;
					Bstr[i][j].str += "\n";

					Bstr[i][j].colorNum = Bbefore;

					Bcount = 0;
				}
			}

		}
		// 오른쪽 힌트의 경우 한줄이 다 끝나면 줄띄움
		Rstr[i][9].str += "\n\n";
	}
}

// enum으로 정의했던 컬러값을 RGB컬러로 반환하는 함수
int Colorgroup(int color)
{
	D3DCOLOR fontColor;

	switch (color)
	{
	case 1:
		fontColor = D3DCOLOR_XRGB(2, 4, 3);
		break;
	case 2:
		fontColor = D3DCOLOR_XRGB(255, 255, 255);
		break;
	case 3:
		fontColor = D3DCOLOR_XRGB(53, 147, 231);
		break;
	case 4:
		fontColor = D3DCOLOR_XRGB(238, 189, 172);
		break;
	case 5:
		fontColor = D3DCOLOR_XRGB(238, 203, 57);
		break;
	case 6:
		fontColor = D3DCOLOR_XRGB(239, 115, 175);
		break;
	default:
		fontColor = D3DCOLOR_XRGB(0, 0, 0);
		break;
	}

	return fontColor;
}

// 클리어 화면 관련 함수 (리플레이버튼)
void GameClear()
{
	POINT p;
	char number[20];
	int sum;
	float gap;

	if (KEY_DOWN(VK_LBUTTON))
	{
		GetCursorPos(&p); //get current mouse position

		if (1730 <= p.x && p.x <= 1880 && 460 <= p.y && p.y <= 710 && stage == 20)
		{
			stage = -1;
			PlaySound(NULL, NULL, NULL);
		}
	}

	// 미구현된 타이머
	//Time += "Time : ";
	//gap = endTime - startTime / (CLOCKS_PER_SEC);
	//sprintf_s(number, "%2.f", gap / 60);
	//Time += number;
	//Time += "분 ";
	//sprintf_s(number, "%2.f", (int)gap%60);
	//Time += number;
	//Time += "초";
	//Time += "12분 45초";
}

// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(226, 233, 226), 1.0f, 0);
	d3ddev->BeginScene();					 // begins the 3D scene
	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    //  begin sprite drawing with transparency			 
	d3ddev->SetFVF(CUSTOMFVF);				 // select which vertex format we are using

	
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position;
	
	if (stage == -1)
	{
		// 타이틀 이미지
		RECT title;
		SetRect(&title, 0, 0, 1920, 1080);
		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		d3dspt->Draw(sprite_title, &title, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 스타트 버튼
		RECT start;
		SetRect(&start, 0, 0, 606, 260);
		position = D3DXVECTOR3(662.0f, 745.0f, 0.0f);
		d3dspt->Draw(sprite_start_button, &title, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 인게임 중일때만 
	if (0 <= stage && stage < 20)
	{
		RECT part;
		SetRect(&part, 0, 0, 64, 64);

		for (int i = 0; i < X_SIZE; i++)
		{
			for (int j = 0; j < Y_SIZE; j++)
			{
				position = D3DXVECTOR3(tile[i][j].x_pos + 640, tile[i][j].y_pos + 200, 0.0f);    // position at 50, 50 with no depth
				switch (tile[i][j].status)
				{
				case Tile_nomal:
					d3dspt->Draw(sprite_empty_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
					break;

				case Tile_Xmark:
					d3dspt->Draw(sprite_Xmark_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
					break;

					// 컬러 타일 출력
				default:
					switch (tile[i][j].color)
					{
					case 1:
						d3dspt->Draw(sprite_black_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					case 2:
						d3dspt->Draw(sprite_white_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					case 3:
						d3dspt->Draw(sprite_blue_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					case 4:
						d3dspt->Draw(sprite_skin_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					case 5:
						d3dspt->Draw(sprite_yellow_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					case 6:
						d3dspt->Draw(sprite_pink_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;

					default:
						d3dspt->Draw(sprite_empty_tile, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
						break;
					}
					break;
				}
			}
		}

		// 가로 구분선
		RECT part1;
		SetRect(&part1, 0, 0, 640, 3);

		// 세로 구분선
		RECT part2;
		SetRect(&part2, 0, 0, 3, 640);
		for (int k = 0; k < 3; k++)
		{
			position = D3DXVECTOR3(640.0f, k * 320.0f + 200.0f, 0.0f);
			d3dspt->Draw(sprite_width_line, &part1, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

			position = D3DXVECTOR3(320.0f * k + 640.0f, 200.0f, 0.0f);
			d3dspt->Draw(sprite_width_line, &part2, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (Font)
		{
			// 왼쪽 힌트
			RECT leftPart;
			SetRect(&leftPart, 0, 220, 620, 900);
			Font->DrawText(NULL, Lstr, -1, &leftPart, DT_RIGHT, D3DCOLOR_XRGB(0, 0, 0));

			// 상단 힌트
			RECT topPart;
			SetRect(&topPart, 660, 0, 1280, 190);
			for (int i = 0; i < X_SIZE; i++)
			{
				topPart.left = 660 + 64 * i;
				Font->DrawText(NULL, Tstring[i], -1, &topPart, DT_BOTTOM, D3DCOLOR_XRGB(0, 0, 0));

			}

			// 오른쪽 컬러 힌트
			RECT rightPart;
			SetRect(&rightPart, 1305, 220, 1925, 900);
			for (int i = 0; i < X_SIZE; i++)
			{
				for (int j = 0; j < Y_SIZE; j++)
				{
					if (Rstr[i][j].str != "")
					{
						rightPart.top = 220 + 64 * i;

						Font->DrawText(NULL, Rstr[i][j].str, -1, &rightPart, DT_LEFT, Colorgroup(Rstr[i][j].colorNum));

						rightPart.left = rightPart.left + 32;
					}
				}
				rightPart.left = 1305;
			}


			// 하단 컬러 힌트
			RECT bottomPart;
			SetRect(&bottomPart, 660, 855, 1280, 1065);

			for (int i = 0; i < X_SIZE; i++)
			{
				bottomPart.left = 660 + 64 * i;
				bottomPart.top = 855;
				for (int j = 0; j < Y_SIZE; j++)
				{

					if (Bstr[i][j].str != "")
					{
						Font->DrawText(NULL, Bstr[i][j].str, -1, &bottomPart, DT_TOP, Colorgroup(Bstr[i][j].colorNum));
						bottomPart.top = bottomPart.top + 32;

					}
				}
			}
		}

		if (UIFont)
		{
			// Stage UI
			RECT text;
			SetRect(&text, 0, 50, 1870, 500);
			UIFont->DrawText(NULL, UI, -1, &text, DT_RIGHT, D3DCOLOR_XRGB(72, 118, 66));
		}

		// next버튼
		RECT nextPart;
		SetRect(&nextPart, 0, 0, 150, 150);
		position = D3DXVECTOR3(1730, 880, 0.0f);
		d3dspt->Draw(sprite_next, &nextPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		// palette
		RECT palettePart;
		SetRect(&palettePart, 0, 0, 100, 557);
		position = D3DXVECTOR3(180, 237, 0.0f);
		d3dspt->Draw(sprite_palette, &palettePart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		// color버튼
		RECT colorPart;
		SetRect(&colorPart, 0, 0, 64, 64);
		position = D3DXVECTOR3(200, 255, 0.0f);
		d3dspt->Draw(sprite_black_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		position = D3DXVECTOR3(200, 345, 0.0f);
		d3dspt->Draw(sprite_white_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		position = D3DXVECTOR3(200, 435, 0.0f);
		d3dspt->Draw(sprite_blue_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		position = D3DXVECTOR3(200, 525, 0.0f);
		d3dspt->Draw(sprite_skin_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		position = D3DXVECTOR3(200, 615, 0.0f);
		d3dspt->Draw(sprite_yellow_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		position = D3DXVECTOR3(200, 705, 0.0f);
		d3dspt->Draw(sprite_pink_button, &colorPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

	}


	if (stage == 20)
	{
		// 클리어 이미지
		RECT clear;
		SetRect(&clear, 0, 0, 1920, 1080);
		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		d3dspt->Draw(sprite_clear, &clear, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 리플레이 버튼
		RECT replayPart;
		SetRect(&replayPart, 0, 0, 150, 150);
		position = D3DXVECTOR3(1730, 460, 0.0f);
		d3dspt->Draw(sprite_replay_button, &replayPart, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 미구현된 Timer
		//if (FontSprite)
		//{
		//	FontSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		//}
		//RECT Timetext;
		//SetRect(&Timetext, 200, 200, 800, 800);
		//UIFont->DrawText(FontSprite, L"안나수미나라", -1, &Timetext, DT_NOCLIP, D3DCOLOR_XRGB(72, 118, 66));
		//FontSprite->End();
	}


	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_empty_tile->Release();
	sprite_width_line->Release();
	sprite_height_line->Release();
	Font->Release();
	UIFont->Release();
	FontSprite->Release();

	sprite_next->Release();
	sprite_palette->Release();
	sprite_black_button->Release();
	sprite_white_button->Release();
	sprite_blue_button->Release();
	sprite_skin_button->Release();
	sprite_yellow_button->Release();
	sprite_pink_button->Release();

	sprite_Xmark_tile->Release();
	sprite_black_tile->Release();
	sprite_white_tile->Release();
	sprite_blue_tile->Release();
	sprite_skin_tile->Release();
	sprite_yellow_tile->Release();
	sprite_pink_tile->Release();

	sprite_title->Release();
	sprite_start_button->Release();
	sprite_clear->Release();
	sprite_replay_button->Release();


	return;
}
