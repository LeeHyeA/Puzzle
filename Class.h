#pragma once
#include <atlstr.h>
class entity {

public:
	float x_pos;
	float y_pos;
	int status;		// Ÿ�ϻ���
	int color;		// �÷���

	void init(float x, float y);
};

class colorStr {

public:
	float x_pos;
	float y_pos;
	CString str;
	int colorNum;
	int sumNum;

	void init(float x, float y);
	void init();
};
