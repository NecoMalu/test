#pragma once
#include "../Library/gameObject.h"
#include <assert.h>
#include "config.h"

// �}�E�X��L�{�^�����Ǘ�����N���X
class MouseLButton
{
public:
	MouseLButton();
	~MouseLButton();
	void Update();
	bool IsPush(); // �����Ă܂����H
	bool IsPushDown(); // �������u�Ԃł����H
	bool IsPushUp(); // �������u�Ԃł����H
private:
	bool current; // ���̏��
	bool last; // �O�̃t���[���̏��
};

/// <summary>
/// �Q�[���̔Ֆʂ��Ǘ��A�\�����܂�
/// ���U�A�c�T�}�X�̔Ֆʂł�
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
	/// �ҋ@��Ԃ̏���������
	/// </summary>
	void UpdateIdle();

	void UpdateMove();
	void UpdateDelete();
	void UpdateFall();

	/// <summary>
	/// 3����ł���ʂ������āA
	/// field[][].doErase��1������
	/// </summary>
	/// <returns>��������̂������true</returns>
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
		COLOR color;			// ���̃}�X�̐F
		float x, y;				// �\�����W
		int moveCount;			// �ړ�����
	private:
		int doErase;			// �������
		int eraseCount;			// �����鎞��
		int endX, endY;			//�ړ���̍��W
		float beginX, beginY;	//�ړ��J�n���̍��W
		float rate;				//�ړ�����
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
		S_IDLE = 1, // �ҋ@
		S_MOVE,		// ������
		S_DELETE,	// ����
		S_FALL,		// �������Ƃ���ɏ�̋ʂ𗎂Ƃ�
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