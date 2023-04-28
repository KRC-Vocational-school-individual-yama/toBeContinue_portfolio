#include<DxLib.h>
#include<string>
#include"saiyoo.h"

void saiyooreset();

namespace {

	const int mozimax = 20;
	const int aida = 30;

	bool reseflag = false;

	struct Blend {
		bool flag = false;
		int count = 0;
		int one = 0;
	};

	struct Mozi {
		std::string text = "";
		bool flag = false;
		int count = 0;
		int x = 0;
		int y = 0;
		Blend blend;
	};

	Mozi mozi[mozimax];


}
void saiyooinit() {

	mozi[0].text="Licht";
	mozi[1].text="�N���W�b�g���[��";
	mozi[2].text="";
	mozi[3].text="�쐬�S��";
	mozi[4].text="�v�����i�[�F�ɓ��g��\n�v���O���}�[�F��������\n�f�U�C�i�[�F�R�{�D��";
	mozi[5].text="";
	mozi[6].text="�o��L�����N�^�[";
	mozi[7].text="������\n���炸��ƃL�����̖��O�������Ă����Ƃ��H";
	mozi[8].text="���Ƃ͊��z�Ƃ��H";
	mozi[9].text="";
	mozi[10].text = "�v���O���}�̂������|�C���g";
	mozi[11].text = "��肭�s�������Ă����Ɋ��z�ł���������\n�y����������Ȃ��H��";
	mozi[12].text = "";
	mozi[13].text = "text�̃����o�ϐ��ɓ��������s�̐���";
	mozi[14].text = "�P\n2\n3\n4\n5\n6";
	mozi[15].text = "���ő�ł��B����ȏ���s����̂Ȃ�΁A�ϐ���ς��Ă��������B";
	mozi[16].text = "��s�̍ő啶������29�����ɂȂ��Ă���܂��B(�S�đS�p�̏ꍇ)\n���̔��������h���̗ǂ������Ǝv����B";
	mozi[17].text = "";//�������牺�̃����o�ϐ��ɂ͕��������Ȃ��̂����\�D��
	mozi[18].text="";//End�����󂯂Ă�ƌ��h�����ǂ������B
	mozi[19].text = "�@�v���C���Ă�������\n���肪�Ƃ��������܂����B";

	
	
	saiyooreset();
}
void saiyooreset() {
	for (int i = 0; i < mozimax; i++) {
		mozi[i].count = 0;
		mozi[i].flag = false;
		mozi[i].x = 300;
		mozi[i].y = 600;
		mozi[i].blend.one = 1;
		mozi[i].blend.flag = false;
		mozi[i].blend.count = 0;
	}
	

	mozi[0].blend.flag = false;
	mozi[0].blend.count = 0;
	mozi[0].x = 280;
	mozi[0].y = 250;

	mozi[mozimax-1].blend.flag = false;
	mozi[mozimax-1].blend.count = 255;
	mozi[mozimax - 1].y = 620;
	mozi[mozimax - 1].x = 130;
	reseflag = false;
}


void scroll(int num) {

	if (CheckHitKey(KEY_INPUT_S) == 1 && mozi[0].y <= 0) {
		mozi[num].count++;
		if (mozi[num].flag == true) {
			mozi[num].y -= 5;
		}
	}
	if (CheckHitKey(KEY_INPUT_W) == 1) {
		mozi[num].y -= 5;
	}
	if (CheckHitKey(KEY_INPUT_T) == 1) {
		mozi[num].y=-2000;
	}
}

void saiyooupdete() {
	


	mozi[0].blend.count += mozi[0].blend.one * 3;
	if (mozi[0].blend.count > 255) {
		mozi[0].blend.flag = true;
	}
	if (mozi[0].blend.flag == true) {
		mozi[0].count++;
		if (mozi[0].count >= 30) {
			mozi[0].flag = true;
		}
		if (mozi[0].flag == true) {
			mozi[0].y -= 3;
		}
	}

	if (mozi[0].y <= 0) {
		for (int i = 1; i < mozimax - 1; i++) {
			scroll(i);
			mozi[i].count++;
			if (mozi[i].count >= -30 + i * aida) {
				mozi[i].flag = true; 
			}
			if (mozi[i].flag == true) {
				mozi[i].y -= 3;
			}
		}
	}

	if (mozi[mozimax - 2].y <= 0-((mozimax-2)*aida)) {
		mozi[mozimax - 1].count++;
		if (mozi[mozimax - 1].count >= -30 + mozimax - 1 * aida) {
			mozi[mozimax - 1].flag = true;

			if (mozi[mozimax - 1].y <= 280) {
				mozi[mozimax - 1].blend.count -= mozi[mozimax - 1].blend.one * 2;
			}
			else {
				mozi[mozimax - 1].y -= 3;
			}
		}
		if (mozi[mozimax - 1].blend.count <= -150) {
			reseflag = true;
		}
	}

	bool set = CheckHitKey(KEY_INPUT_RETURN);
	bool res = false;
	if (mozi[mozimax - 1].y <= -50 + -((mozimax - 1) * aida)) {
		set = true;

	}

	//�������̓y�[�W���ڂ�����
	//�������Z�b�g������
	if (set == true || reseflag == true) {
		saiyooreset();
	}

}
void saiyoodraw() {

	//DrawGraph(0, 0, LoadGraph("gazo-\\yabas.png"), true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mozi[0].blend.count);
	SetFontSize(100);
	DrawFormatString(mozi[0].x, mozi[0].y, GetColor(255, 255, 255), mozi[0].text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
	
	for (int i = 1; i < mozimax-1; i++) {
		DrawFormatString(mozi[i].x, mozi[i].y + i * aida, GetColor(255, 255, 255), mozi[i].text.c_str());
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mozi[mozimax - 1].blend.count);
	SetFontSize(50);
	DrawFormatString(mozi[mozimax-1].x, mozi[mozimax-1].y + mozimax-1 * aida, GetColor(255, 255, 255), mozi[mozimax-1].text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 
	SetFontSize(16);
	//DrawFormatString(100,100,GetColor(0,0,0),"%d",mozi[1].y);
}
void saiyoorelece() {

}