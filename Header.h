#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <atlstr.h>
#include <time.h>
#include <mmsystem.h>



// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// Define 
#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT 900
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define X_SIZE 10
#define Y_SIZE 10



// Direct3D9관련함수 Library
#pragma comment(lib, "winmm.lib")               
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")



// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface

ID3DXFont* Font;
ID3DXFont* UIFont;
ID3DXSprite* FontSprite;


// 인게임 스프라이트
LPDIRECT3DTEXTURE9 sprite;   
LPDIRECT3DTEXTURE9 sprite_empty_tile;
LPDIRECT3DTEXTURE9 sprite_Xmark_tile;
LPDIRECT3DTEXTURE9 sprite_black_tile;
LPDIRECT3DTEXTURE9 sprite_white_tile;
LPDIRECT3DTEXTURE9 sprite_blue_tile;
LPDIRECT3DTEXTURE9 sprite_skin_tile;
LPDIRECT3DTEXTURE9 sprite_yellow_tile;
LPDIRECT3DTEXTURE9 sprite_pink_tile;
LPDIRECT3DTEXTURE9 sprite_width_line;
LPDIRECT3DTEXTURE9 sprite_height_line;

// 버튼 및 팔레트 스프라이트
LPDIRECT3DTEXTURE9 sprite_next;
LPDIRECT3DTEXTURE9 sprite_palette;
LPDIRECT3DTEXTURE9 sprite_black_button;
LPDIRECT3DTEXTURE9 sprite_white_button;
LPDIRECT3DTEXTURE9 sprite_blue_button;
LPDIRECT3DTEXTURE9 sprite_skin_button;
LPDIRECT3DTEXTURE9 sprite_yellow_button;
LPDIRECT3DTEXTURE9 sprite_pink_button;

// 타이틀 및 클리어화면 스프라이트
LPDIRECT3DTEXTURE9 sprite_title;
LPDIRECT3DTEXTURE9 sprite_start_button;
LPDIRECT3DTEXTURE9 sprite_clear;
LPDIRECT3DTEXTURE9 sprite_replay_button;



void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void title(void);			// 타이틀 화면 관련 함수
void init_tile(void);		// 타일 초기화
void GameLogic(void);		// 게임 로직 및 넥스트 버튼
void Hint(void);			// 왼쪽 & 상단 숫자 힌트
void ColorHint(void);		// 오른쪽 & 하단 컬러 숫자 힌트
void GameClear(void);		// 클리어 화면 관련 함수
						
int Colorgroup(int color);	// enum으로 정의했던 컬러값을 RGB컬러로 반환하는 함수


enum { Tile_nomal, Tile_check, Tile_Xmark, Tile_wrong };
// nomal : 사용자가 아무런 행동을 취하지 않은 상태
// check : 사용자가 체크를 한 것이 정답인 상태
// Xmark : 사용자가 오답(X)을 마킹한 상태
// wrong : 사용자가 체크를 한 것이 오답인 상태

enum { Non, Black, White, Blue, Skin, Yellow, Red };
// 컬러 값 상태(Non은 칸이 빈 상태 = 초기화 값)


// 사용된 전역변수
CString Lstr;			// 왼쪽 숫자 
CString Tstring[10];	// 상단 숫자
CString UI;				// 진행 상태

CString Time;			// 클리어시간(미구현)
time_t startTime = 0;	
time_t endTime = 0;

int stage = -1;				// -1 : 타이틀, 20 : 게임 클리어
int nowColor = 0;			// 현재 마우스가 가지는 컬러 값	
bool HintControl = false;	// 힌트가 이미 그려져있다면 true


