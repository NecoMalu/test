#pragma once
#include "../Library/gameObject.h"
#include <assert.h>
#include "config.h"

// マウスのLボタンを管理するクラス
class MouseLButton
{
public:
	MouseLButton();
	~MouseLButton();
	void Update();
	bool IsPush(); // 押してますか？
	bool IsPushDown(); // 押した瞬間ですか？
	bool IsPushUp(); // 離した瞬間ですか？
private:
	bool current; // 今の状態
	bool last; // 前のフレームの状態
};

/// <summary>
/// ゲームの盤面を管理、表示します
/// 横６、縦５マスの盤面です
/// </summary>
class Stage : public GameObject 
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
private:
	/// <summary>
	/// 待機状態の処理をする
	/// </summary>
	void UpdateIdle();

	void UpdateMove();
	void UpdateDelete();
	void UpdateFall();

	/// <summary>
	/// 3つ並んでいる玉を見つけて、
	/// field[][].doEraseに1を入れる
	/// </summary>
	/// <returns>消えるものがあればtrue</returns>
	bool CheckDelete();

	void PrepareFall();

	enum COLOR 
	{
		RED = 0,
		BLUE,
		YELLOW,
		GREEEN,
		PURPLE,
		HEART,
		MAX
	};
	class BALLINFO 
	{
	public:
		COLOR color;			// このマスの色
		float x, y;				// 表示座標
		int moveCount;			// 移動時間
	private:
		int doErase;			// 消える玉
		int eraseCount;			// 消える時間
		int endX, endY;			//移動後の座標
		float beginX, beginY;	//移動開始時の座標
		float rate;				//移動割合
	public:
		BALLINFO() 
		{
			color = COLOR::RED;
			doErase = 0;
			moveCount = 0;
			eraseCount = 0;
			rate = 1.0f;
		}
		void DeleteBegin();
		void EraseUpdate();
		bool IsEraseEnd();
		int DoErase()
		{
			return doErase;
		}
		int EraseCount()
		{
			return eraseCount;
		}
		void ClearErase()
		{
			doErase = 0;
		}
		void MoveTo(int toX, int toY, int count);
		void MoveUpdate();
		bool MoveFinish()
		{
			return (rate >= 1.0f);
		}
	};	

	int hImage[COLOR::MAX];
	BALLINFO field[HEIGHT][WIDTH];
	int mouseX, mouseY;
	int selectX, selectY;
	int selectColor;

	enum STATE 
	{
		S_IDLE = 1, // 待機
		S_MOVE,		// 動かす
		S_DELETE,	// 消す
		S_FALL,		// 消えたところに上の玉を落とす
	};
	STATE state;

	MouseLButton Lbutton;

	BALLINFO& Cell(int x, int y)
	{
		assert(x >= 0 && x < WIDTH);
		assert(y >= 0 && y < HEIGHT);
		return field[y][x];
	}
};