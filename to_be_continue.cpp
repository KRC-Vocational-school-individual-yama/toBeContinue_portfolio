#include<DxLib.h>
#include<string>
#include"to_be_continue.h"
#include<assert.h>

namespace {

	const int mozimax = 1;

	int tobewhite = 0;
	int tobeblack = 0;
	int musicimg = -1;
	int musicflag = 0;
	int sepiaimg = -1;
	bool weitflag = false;

	class Font {
	public:
		std::string text = "";
		bool flag = false;
		int x = 0;
		int y = 0;
		int count = 0;

		int move(int speed);

	};
	Font zi[mozimax];
}
int Font::move(int speed) {
	
	int ans = zi[0].x += speed;
	return ans;
};

void tobecontinueinit() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	tobewhite = GetColor(255, 255, 255);
	tobeblack = GetColor(0, 0, 0);
	musicimg = LoadSoundMem("To-Be-Continued-.mp3");
	musicflag = 0;
	sepiaimg = LoadGraph("gazo-\\sepia.png");
	assert(sepiaimg > 0);

	zi[0].text = "Å@To Be Continue";

	for (int i = 0; i < mozimax; i++) {
		zi[i].flag = false;
		zi[i].x = 770;
		zi[i].y = 380;
		zi[i].count = 0;
	}
	
	weitflag = false;

	StopSoundMem(musicimg);
}
void hitkey() {

	if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
		tobecontinueinit();
	}
	if ((CheckHitKey(KEY_INPUT_NUMPAD4)|CheckHitKey(KEY_INPUT_4)) == 1 && musicflag == 0) {
		musicflag = 1;
		PlaySoundMem(musicimg, DX_PLAYTYPE_BACK);
	}
	if (CheckHitKey(KEY_INPUT_NUMPAD0) == 1 && musicflag == 1) {
		musicflag = 0;
		
	}

}
void tobecontinueupdete() {
	hitkey();
	if ((CheckHitKey(KEY_INPUT_NUMPAD4) | CheckHitKey(KEY_INPUT_4)) == 1) {
		zi[0].flag = true;
	}
	if (zi[0].flag == true) {
		zi[0].count++;
	}
	if (zi[0].count >= 4 * 60) {

		if (zi[0].x <= -35) {
			SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
			weitflag = true;
			
			if (weitflag == true) {
				WaitTimer(6000);
				
				tobecontinueinit();
				weitflag = false;
			}
		
		}
		else {
			zi[0].move(1 * -70);
		}

	}

}

void tobecontinuedraw() {

	if (zi[0].x <= -70) {
		DrawGraph(0,0,sepiaimg,true);
	}

	DrawTriangle(100+zi[0].x, 116+zi[0].y,  75+zi[0].x, 148+zi[0].y,  100+zi[0].x, 180+zi[0].y, tobeblack, true);
	DrawTriangle(100+zi[0].x, 116+zi[0].y,  75+zi[0].x, 148+zi[0].y,  100+zi[0].x, 180+zi[0].y, tobewhite, false);
	      DrawBox(90+zi[0].x, 133+zi[0].y, 270+zi[0].x, 164+zi[0].y, tobeblack, true);
		DrawLine(100+zi[0].x, 133+zi[0].y, 270+zi[0].x, 133+zi[0].y, tobewhite);
		DrawLine(100+zi[0].x, 163+zi[0].y, 270+zi[0].x, 163+zi[0].y, tobewhite);
		DrawLine(270+zi[0].x, 133+zi[0].y, 270+zi[0].x, 163+zi[0].y, tobewhite);
	DrawTriangle(275+zi[0].x, 133+zi[0].y, 275+zi[0].x, 164+zi[0].y, 290+zi[0].x, 164+zi[0].y, tobeblack, true);
	DrawTriangle(275+zi[0].x, 133+zi[0].y, 275+zi[0].x, 164+zi[0].y, 290+zi[0].x, 164+zi[0].y, tobewhite, false);
	DrawTriangle(280+zi[0].x, 133+zi[0].y, 295+zi[0].x, 133+zi[0].y, 295+zi[0].x, 164+zi[0].y, tobeblack, true);
	DrawTriangle(280+zi[0].x, 133+zi[0].y, 295+zi[0].x, 133+zi[0].y, 295+zi[0].x, 164+zi[0].y, tobewhite, false);
	DrawTriangle(300+zi[0].x, 133+zi[0].y, 300+zi[0].x, 164+zi[0].y, 315+zi[0].x, 164+zi[0].y, tobeblack, true);
	DrawTriangle(300+zi[0].x, 133+zi[0].y, 300+zi[0].x, 164+zi[0].y, 315+zi[0].x, 164+zi[0].y, tobewhite, false);

	SetFontSize(18);
	DrawFormatString(100 + zi[0].x, 140 + zi[0].y, tobewhite, zi[0].text.c_str());
	SetFontSize(16);

	DrawFormatString(200, 100, tobewhite, "%d", zi[0].x);
}

void tobecontinuerelece() {
	DeleteGraph(sepiaimg);

}