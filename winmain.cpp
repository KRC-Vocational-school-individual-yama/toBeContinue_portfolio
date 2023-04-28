#include "DxLib.h"
#include <string>
#include<assert.h>
//#include"staffroll.h"
//#include"staffroll_sita.h"
//#include"kieru.h"
//#include"feedinout.h"
//#include"matome.h"
#include"saiyoo.h"
#include"to_be_continue.h"
#include"epilogue.h"

enum GameStat
{
	title,
	sentaku,
	sentakuend,
	staffroll,
	staffrollend,
	epiroll,
	epirollend,
	BattleStart,
	BattleStartDisp,
	PlayerSelect,
	PlayerSelectEnd,
	MagicSelect,
	MagicSelectEnd,
	PlayerAtack,
	AtackEnd,
	EnemyJudge,
	EnemySelect,
	EnemyAtack,
	EnemyAtackDisp,
	PlayerJudge,
	GameOver,
	GameOverEnd,
	Result,
	End
};

enum MagicId
{
	Maya,
	Photosho,
	Unity,
	VisualStudio,
	MagicMax
};
enum MagicType
{
	Atack,//攻撃魔法
	Heal,//回復魔法
	Buff,//強化魔法
};

//魔法構造体
struct MagicData
{
	std::string name = "";
	std::string message = "";
	double power = 0;
	int cost = 0;
	MagicType type = Atack;
	bool enable = false;//有効フラグ
};

enum EnemyId
{
	Tachiki,
	Sakuma,
	Yamada,
	Shibata,
	Ueda,
	EnemyMax
};
//敵構造体
struct Enemy
{
	std::string name = "";//名前
	int hp = 0;
	int mp = 0;
	int ap = 0;
	int graphic = 0;//画像
	MagicData magic[1];//敵の魔法は最大一種類
};
enum CharacterId
{
	Chino,
	ChinoKen,
	CharacterMax
};
//プレイヤー構造体
struct Player
{
	std::string name = "";
	int hp = 0;
	int mp = 0;
	int ap = 0;
	MagicData magic[3];//魔法は３種類
};

const int PartyMax = 2;//パーティ2人
bool keystop = false;
int GameStatus = 0;
int SelectNo = 0;//選択肢
int haikeiimg[3] = {};
int haikeicount = 0;

struct staff {
	std::string Text = "";
	bool flag = false;
	int count = 0;
};

staff messe[25] = {};
int titleimg = -1;
int countm = 0;


MagicData MagicTable[MagicMax];//魔法情報
Enemy EnemyTable[EnemyMax];//敵情報
Player CharacterTable[CharacterMax];

Enemy BattleEnemy;//戦闘中の敵
Player Party[PartyMax];//パーティ(一人)
std::string MessageText = "";

const int MenuMax = 4;//メニュー最大数
std::string SelectText[MenuMax] = { "","","","" };

//何人目のキャラまで行ったか
int SelectPlayerNo = 0;

//表示数文字数
int text_count;
//wstringをstringへ変換
std::string WStringToString(std::wstring oWString)
{
	// wstring → SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);
	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];
	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);
	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);
	// バッファの破棄
	delete[] cpMultiByte;
	// 変換結果を返す
	return(oRet);
}
std::wstring StringToWString(std::string oString)
{
	//  SJIS→wstring
	std::wstring ws;
	int iBufferSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, oString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpMultiByte = new wchar_t[iBufferSize];
	// wstring → SJIS
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpMultiByte
		, iBufferSize);
	// wstringの生成
	std::wstring oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);
	// バッファの破棄
	delete[] cpMultiByte;
	// 変換結果を返す
	return(oRet);
}

//最初に1回呼ばれる処理
void Init()
{
	epiinit();
	tobecontinueinit();
	//StaffrollInit();
	//Staffroll_Sita_Init();
	//KieruInit();
	//FeedInit();
	//MatomeInit();
	saiyooinit();
	titleimg = LoadGraph("gazo-\\yabas.png");
	haikeiimg[0] = LoadGraph("gazo-\\良き光差し込む.png");
	//assert(haikeiimg[0] > 0);
	haikeiimg[1]=LoadGraph("gazo-\\怖っ.png");
	assert(haikeiimg[1] > 0);
	haikeiimg[2]=LoadGraph("");


	//魔法情報を初期化
	MagicTable[Maya].name = "マヤ";
	MagicTable[Maya].message = "ポリゴンショック！！激しい光の点滅に注意してください";
	MagicTable[Maya].cost = 10;
	MagicTable[Maya].power = 50;
	MagicTable[Maya].type = Atack;
	MagicTable[Maya].enable = true;

	//魔法情報を初期化
	MagicTable[Photosho].name = "フォトショ";
	MagicTable[Photosho].message = "傷口がレタッチされていく…";
	MagicTable[Photosho].cost = 10;
	MagicTable[Photosho].power = 50;
	MagicTable[Photosho].type = Heal;
	MagicTable[Photosho].enable = true;

	//魔法情報を初期化
	MagicTable[Unity].name = "ユニティ";
	MagicTable[Unity].message = "攻撃効率がアップした！";
	MagicTable[Unity].cost = 10;
	MagicTable[Unity].power = 1.5;
	MagicTable[Unity].type = Buff;
	MagicTable[Unity].enable = true;

	//魔法情報を初期化
	MagicTable[VisualStudio].name = "ヴィジュアルスタジオ";
	MagicTable[VisualStudio].message = "バグが発生した！！";
	MagicTable[VisualStudio].cost = 10;
	MagicTable[VisualStudio].power = 0.5;
	MagicTable[VisualStudio].type = Buff;
	MagicTable[VisualStudio].enable = true;

	//敵情報を初期化
	EnemyTable[Tachiki].name = "立木ティーチャー";
	EnemyTable[Tachiki].hp = 100;
	EnemyTable[Tachiki].ap = 10;
	EnemyTable[Tachiki].mp = 100;
	EnemyTable[Tachiki].magic[0] = MagicTable[Maya];
	EnemyTable[Tachiki].graphic = LoadGraph("gazo-\\monban_00_green.png");

	EnemyTable[Sakuma].name = "佐久間ティーチャー";
	EnemyTable[Sakuma].hp = 120;
	EnemyTable[Sakuma].ap = 30;
	EnemyTable[Sakuma].mp = 10;
	EnemyTable[Sakuma].magic[0] = MagicTable[Unity];
	EnemyTable[Sakuma].graphic = LoadGraph("gazo-\\monban_01_yellorred.png");

	EnemyTable[Yamada].name = "山田ティーチャー";
	EnemyTable[Yamada].hp = 80;
	EnemyTable[Yamada].ap = 20;
	EnemyTable[Yamada].mp = 80;
	EnemyTable[Yamada].magic[0] = MagicTable[Unity];
	EnemyTable[Yamada].graphic = LoadGraph("gazo-\\monban_00_kousai.png");

	EnemyTable[Shibata].name = "柴田ティーチャー";
	EnemyTable[Shibata].hp = 80;
	EnemyTable[Shibata].ap = 5;
	EnemyTable[Shibata].mp = 150;
	EnemyTable[Shibata].magic[0] = MagicTable[Photosho];
	EnemyTable[Shibata].graphic = LoadGraph("gazo-\\monban_01_kousai.png");

	EnemyTable[Ueda].name = "植田ティーチャー";
	EnemyTable[Ueda].hp = 50;
	EnemyTable[Ueda].ap = 50;
	EnemyTable[Ueda].mp = 10;
	EnemyTable[Ueda].magic[0] = MagicTable[Photosho];
	EnemyTable[Ueda].graphic = LoadGraph("ueda.png");

	//キャラクターテーブル初期化
	CharacterTable[Chino].name = "千野";
	CharacterTable[Chino].hp = 80;
	CharacterTable[Chino].ap = 200000;
	CharacterTable[Chino].mp = 50;
	CharacterTable[Chino].magic[0]= MagicTable[Maya];
	CharacterTable[Chino].magic[1] = MagicTable[VisualStudio];
	CharacterTable[Chino].magic[2] = MagicTable[Photosho];

	CharacterTable[ChinoKen].name = "千野謙";
	CharacterTable[ChinoKen].hp = 90;
	CharacterTable[ChinoKen].ap = 1000000;
	CharacterTable[ChinoKen].mp = 40;
	CharacterTable[ChinoKen].magic[0] = MagicTable[Maya];
	CharacterTable[ChinoKen].magic[1] = MagicTable[VisualStudio];
	CharacterTable[ChinoKen].magic[2] = MagicTable[Photosho];

}


//毎フレーム呼ばれる処理

void Update()
{
	if (GameStatus == staffroll) {
		MessageText = "";
		//StaffrollUpdete();
		//Staffroll_Sita_Updete();
		//KieruUpdete();
		//FeedUpdete();
		//MatomeUpdete();
		saiyooupdete();
		tobecontinueupdete();
	}
	if (GameStatus == epiroll) {
		MessageText = "";
		epiupdete();
	}
	if (CheckHitKey(KEY_INPUT_RETURN) == 1 && keystop == false) {
		
		GameStatus++;
		text_count = 0;//表示文字数をリセット

		if (GameStatus >= End) {
			GameStatus = sentaku;
		}

		if (GameStatus == sentaku) {
			SelectText[0] = "バトルへ";
			SelectText[1] = "プロローグ";
			SelectText[2] = "エピローグ";
			SelectText[3] = "スタッフロール";

			MessageText = "ここは選択の間";
		}
		if (GameStatus == sentakuend) {
			if (SelectNo == 0) {
				GameStatus = BattleStart;
			}
			if (SelectNo == 1) {
				GameStatus = staffroll;
			}
			if (SelectNo == 2) {
				GameStatus = epiroll;
			}
			if (SelectNo == 3) {
				GameStatus = staffroll;
			}
			SelectNo = 0;
			countm = 0;
		}
		
		if (GameStatus == staffrollend) {
			countm = 0;
			GameStatus = sentaku;
		}
		if (GameStatus == epirollend) {
			GameStatus = sentaku;
		}
		if (GameStatus == EnemyJudge)
		{
			if (BattleEnemy.hp <= 0) {
				BattleEnemy.graphic = -1;
				MessageText = BattleEnemy.name + "を倒した";
				GameStatus = Result;//倒した
			}else{

				if (SelectPlayerNo >= PartyMax - 1) {
					GameStatus = EnemySelect;
				}
				else {
					SelectPlayerNo++;
					GameStatus = PlayerSelect;
				}
			}
		}

		if (GameStatus == PlayerJudge)
		{
			int deathcount = 0;
			//現状は一人だが増やすことも可能
			for (int i = 0; i < PartyMax; i++)
			{
				if (Party[i].hp <= 0) {
					deathcount++;//死亡人数
				}
			}
			//全滅したらゲームオーバー
			if (deathcount >= PartyMax) {
				MessageText = Party[0].name + "は死んでしまった。";
				GameStatus = GameOver;
			}else {
				GameStatus = PlayerSelect;
			}
		}

		if (GameStatus == GameOverEnd) {
			GameStatus = BattleStart;
		}



		if (GameStatus == PlayerSelectEnd) {
			if (SelectNo == 0) {
				//攻撃
				GameStatus = PlayerAtack;
			}
			if (SelectNo == 1) {
				//魔法
				SelectText[0] = Party[0].magic[0].name;
				SelectText[1] = Party[0].magic[1].name;
				SelectText[2] = Party[0].magic[2].name;
				SelectText[3] = "戻る";
				MessageText = "魔法を選択してください";
				GameStatus = MagicSelect;
			}
			SelectNo = 0;
		}

		if (GameStatus == PlayerAtack) {
			BattleEnemy.hp -= Party[SelectPlayerNo].ap;
			MessageText = Party[SelectPlayerNo].name + "の攻撃！\n" + BattleEnemy.name + "に" + std::to_string(Party[SelectPlayerNo].ap) + "のダメージ！\n残りHP" + std::to_string(BattleEnemy.hp);
			GameStatus=AtackEnd;
		}

		if (GameStatus == MagicSelectEnd) {
			if (SelectNo == 3) {
				GameStatus = PlayerSelect;
			}
			else if (SelectNo < 3) {
				if (Party[SelectPlayerNo].magic[SelectNo].cost > Party[SelectPlayerNo].mp) {
					MessageText = "MPが足りない！";
				}
				else {
					MessageText = Party[SelectPlayerNo].name + "は" + Party[SelectPlayerNo].magic[SelectNo].name + "を唱えた！\n" + Party[0].magic[SelectNo].message + "\n残りMP" + std::to_string(Party[SelectPlayerNo].mp) + "\n";

					if (Party[SelectPlayerNo].magic[SelectNo].type == Atack) {
						BattleEnemy.hp -= (int)Party[0].magic[SelectNo].power;
						MessageText = MessageText + BattleEnemy.name + "に" + std::to_string((int)Party[0].magic[SelectNo].power) + "のダメージ！\n残りHP" + std::to_string(BattleEnemy.hp);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == Heal) {
						Party[SelectPlayerNo].hp += (int)Party[SelectPlayerNo].magic[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "のHPが" + std::to_string((int)Party[SelectPlayerNo].magic[SelectNo].power) + "回復！\n残りHP" + std::to_string(Party[SelectPlayerNo].hp);
					}
					if (Party[SelectPlayerNo].magic[SelectNo].type == Buff) {
						Party[SelectPlayerNo].ap *= Party[SelectPlayerNo].magic[SelectNo].power;
						MessageText = MessageText + Party[SelectPlayerNo].name + "の攻撃力が" + std::to_string(Party[SelectPlayerNo].magic[SelectNo].power) + "倍になった！\n攻撃力" + std::to_string(Party[SelectPlayerNo].ap);
					}
					Party[SelectPlayerNo].mp -= Party[SelectPlayerNo].magic[SelectNo].cost;
				}

				GameStatus = AtackEnd;

			}
			SelectNo = 0;
		}

		if (GameStatus == PlayerSelect) {
			MessageText = "コマンドを選択してください";
			SelectText[0] = "戦う";
			SelectText[1] = "魔法";
			SelectText[2] = "";
			SelectText[3] = "";
		}

		if (GameStatus == EnemySelect) {
			int r = 0;
			if (BattleEnemy.magic[0].cost <= BattleEnemy.mp) {
				r = GetRand(1);
			}

			int target = GetRand(PartyMax-1);
			if (r == 0) {
				//通常攻撃
				Party[0].hp -= BattleEnemy.ap;
				MessageText = BattleEnemy.name + "の攻撃！\n" + Party[target].name + "に" + std::to_string(BattleEnemy.ap) + "のダメージ！\n残りHP" + std::to_string(Party[target].hp);
			}
			if (r == 1) {
				//魔法攻撃
				MessageText = BattleEnemy.name + "は" + BattleEnemy.magic[0].name + "を唱えた！\n" + BattleEnemy.magic[0].message + "\n";
				if (BattleEnemy.magic[0].type == Atack) {
					Party[0].hp -= (int)BattleEnemy.magic[0].power;
					MessageText = MessageText + Party[0].name + "に" + std::to_string((int)BattleEnemy.magic[0].power) + "のダメージ！\n残りHP" + std::to_string(Party[target].hp);
				}
				if (BattleEnemy.magic[0].type == Heal) {
					BattleEnemy.hp += (int)BattleEnemy.magic[0].power;
					MessageText = MessageText + BattleEnemy.name + "のHPが" + std::to_string((int)BattleEnemy.magic[0].power) + "回復！\n残りHP" + std::to_string(BattleEnemy.hp);
				}
				if (BattleEnemy.magic[0].type == Buff) {
					BattleEnemy.ap *= BattleEnemy.magic[0].power;
					MessageText = MessageText + BattleEnemy.name + "の攻撃力が" + std::to_string(BattleEnemy.magic[0].power) + "倍になった！\n攻撃力" + std::to_string(BattleEnemy.ap);
				}
				BattleEnemy.mp -= BattleEnemy.magic[0].cost;
			}
			GameStatus = EnemyAtackDisp;
			SelectPlayerNo = 0;
		}


		keystop = true;//キー停止
	}
	if (CheckHitKey(KEY_INPUT_UP) == 1 && keystop == false) {
		if(SelectNo > 0) SelectNo--;
		keystop = true;//キー停止
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1 && keystop == false) {
		if(GameStatus == PlayerSelect && SelectNo < 1) SelectNo++;
		if(GameStatus == MagicSelect && SelectNo < 3) SelectNo++;
		if (GameStatus == sentaku && SelectNo < 3) SelectNo++;
		keystop = true;//キー停止
	}

	//キー停止処理解除
	if (CheckHitKey(KEY_INPUT_RETURN) == 0 &&
		CheckHitKey(KEY_INPUT_UP) == 0 &&
		CheckHitKey(KEY_INPUT_DOWN)==0)
	{
		keystop = false;
	}


	if (GameStatus == BattleStart){
		//パーティメンバーは千野と千野謙
		Party[0] = CharacterTable[Chino];
		Party[1] = CharacterTable[ChinoKen];


		//画像の抽出
		haikeicount = GetRand(1);
		//敵の抽選
		int r = GetRand(3/*EnemyMax - 1*/);
		BattleEnemy = EnemyTable[r];
		MessageText = BattleEnemy.name + "が現れた";
		GameStatus = BattleStartDisp;
	}


}
void Draw()
{
	if (GameStatus != sentaku&&GameStatus!=staffroll) {
		DrawGraph(0, 0, haikeiimg[haikeicount], true);
	}
	if (GameStatus == title) {
		DrawGraph(0, 0, titleimg, true);
	}
	
	
	//敵表示
	DrawGraph(150, 10, BattleEnemy.graphic, true);
	if (GameStatus == staffroll) {
		DrawGraph(0,0,LoadGraph("gazo-\\yabas.png"),true);
		//StaffrollDraw();
		//Staffroll_Sita_Draw();
		//KieruDraw();
		//FeedDraw();
		//MatomeDraw();
		saiyoodraw();
		tobecontinuedraw();
	}
	if (GameStatus == epiroll) {
		epidraw();
	}
	//メッセージ表示

	text_count++;
	//メッセージウィンドウにテキストを表示する
	std::wstring message = StringToWString(MessageText);
	std::string disp_message = WStringToString(message.substr(0, text_count / 2));
	DrawFormatString(240, 440, GetColor(255, 255, 255), disp_message.c_str());

	//メニュー表示
	if (GameStatus == PlayerSelect || GameStatus == MagicSelect||GameStatus==sentaku)
	{
		//選択メニュー表示
		for (int i = 0; i < MenuMax; i++) {
			DrawFormatString(60, 440 + i * 30, GetColor(255, 255, 255), SelectText[i].c_str());
		}

		//カーソル表示
		DrawFormatString(30, 440 + SelectNo * 30, GetColor(255, 255, 255), "⇒");
	}
}

void Delete()
{
	for (int i = 0; i < EnemyMax; i++) {
		DeleteGraph(EnemyTable[i].graphic);
	}
	for (int i = 0; i < haikeicount; i++) {
		DeleteGraph(haikeiimg[i]);
	}
	//StaffrollRelece();
	//Staffroll_Sita_Relece();
	//KieruRelece();
	//FeedRelece();
	//MatomeRelece();
	saiyoorelece();
	tobecontinuerelece();
	epirelece();
}
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);	//ウィンドウモードにする
	SetGraphMode(800, 600, 32);	//ウィンドウサイズを設定する
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面を描画対象へ

	Init();//初期化
	//メイン処理
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		Update();//更新処理
		Draw();//描画処理

		ScreenFlip();		//裏画面と表画面の入替
		ClearDrawScreen();	//裏画面の描画を全て消去
	}
	Delete();
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}