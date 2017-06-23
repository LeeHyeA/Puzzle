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



// Direct3D9�����Լ� Library
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


// �ΰ��� ��������Ʈ
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

// ��ư �� �ȷ�Ʈ ��������Ʈ
LPDIRECT3DTEXTURE9 sprite_next;
LPDIRECT3DTEXTURE9 sprite_palette;
LPDIRECT3DTEXTURE9 sprite_black_button;
LPDIRECT3DTEXTURE9 sprite_white_button;
LPDIRECT3DTEXTURE9 sprite_blue_button;
LPDIRECT3DTEXTURE9 sprite_skin_button;
LPDIRECT3DTEXTURE9 sprite_yellow_button;
LPDIRECT3DTEXTURE9 sprite_pink_button;

// Ÿ��Ʋ �� Ŭ����ȭ�� ��������Ʈ
LPDIRECT3DTEXTURE9 sprite_title;
LPDIRECT3DTEXTURE9 sprite_start_button;
LPDIRECT3DTEXTURE9 sprite_clear;
LPDIRECT3DTEXTURE9 sprite_replay_button;



void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void title(void);			// Ÿ��Ʋ ȭ�� ���� �Լ�
void init_tile(void);		// Ÿ�� �ʱ�ȭ
void GameLogic(void);		// ���� ���� �� �ؽ�Ʈ ��ư
void Hint(void);			// ���� & ��� ���� ��Ʈ
void ColorHint(void);		// ������ & �ϴ� �÷� ���� ��Ʈ
void GameClear(void);		// Ŭ���� ȭ�� ���� �Լ�
						
int Colorgroup(int color);	// enum���� �����ߴ� �÷����� RGB�÷��� ��ȯ�ϴ� �Լ�


enum { Tile_nomal, Tile_check, Tile_Xmark, Tile_wrong };
// nomal : ����ڰ� �ƹ��� �ൿ�� ������ ���� ����
// check : ����ڰ� üũ�� �� ���� ������ ����
// Xmark : ����ڰ� ����(X)�� ��ŷ�� ����
// wrong : ����ڰ� üũ�� �� ���� ������ ����

enum { Non, Black, White, Blue, Skin, Yellow, Red };
// �÷� �� ����(Non�� ĭ�� �� ���� = �ʱ�ȭ ��)


// ���� ��������
CString Lstr;			// ���� ���� 
CString Tstring[10];	// ��� ����
CString UI;				// ���� ����

CString Time;			// Ŭ����ð�(�̱���)
time_t startTime = 0;	
time_t endTime = 0;

int stage = -1;				// -1 : Ÿ��Ʋ, 20 : ���� Ŭ����
int nowColor = 0;			// ���� ���콺�� ������ �÷� ��	
bool HintControl = false;	// ��Ʈ�� �̹� �׷����ִٸ� true


