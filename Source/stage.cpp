#include "stage.h"
#include <assert.h>
#include "config.h"

Stage::Stage()
{
	for (int i=0; i<COLOR::MAX; i++) 
	{
		std::string fname = "data/textures/ball";
		fname += std::to_string(i);
		fname += ".png";
		hImage[i] = LoadGraph(fname.c_str());
		assert(hImage[i] > 0);
	}

	for (int y=0; y<HEIGHT; y++) 
	{
		for (int x=0; x<WIDTH; x++) 
		{
			BALLINFO& b = field[y][x];
			b.color = (COLOR)GetRand(COLOR::MAX - 1);
			b.x = x * IMAGE_SIZE;
			b.y = y * IMAGE_SIZE;
		}
	}
	state = STATE::S_IDLE;
}

Stage::~Stage()
{
	for (int i = 0; i < COLOR::MAX; i++)
	{
		if (hImage[i] > 0)
		{	
			DeleteGraph(hImage[i]);
		}
	}
}

void Stage::Update()
{
	Lbutton.Update();

	switch (state) 
	{
	case STATE::S_IDLE:
		UpdateIdle();
		break;
	case STATE::S_MOVE:
		UpdateMove();
		break;
	case STATE::S_DELETE:
		UpdateDelete();
		break;
	case STATE::S_FALL:
		UpdateFall();
		break;
	default:
		assert(false);
	};

	// 玉の座標を補正する
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			field[y][x].MoveUpdate();
		}
	}
}

void Stage::Draw()
{
	for (int y = 0; y < HEIGHT;y++) 
	{
		for (int x=0; x<WIDTH; x++) 
		{
			int color = field[y][x].color;
			if (x == selectX && y == selectY) 
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawGraph(POSITION_X + field[y][x].x, POSITION_Y + field[y][x].y, hImage[color], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else if (field[y][x].EraseCount() %2==1)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawGraph(POSITION_X + field[y][x].x, POSITION_Y + field[y][x].y, hImage[color], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			} 		
			else 
			{
				DrawGraph(POSITION_X + field[y][x].x, POSITION_Y + field[y][x].y, hImage[color], TRUE);
			}
			DrawFormatString(POSITION_X + x * IMAGE_SIZE, POSITION_Y + y * IMAGE_SIZE,
				0xffffff, "%d", field[y][x].DoErase());
		}
	}
	// 触った玉を表示
	if (selectColor >= 0) 
	{
		DrawRotaGraph(mouseX, mouseY, 1.1, 0, hImage[selectColor], TRUE);
	}
}

void Stage::UpdateIdle()
{
	selectColor = -1;
	// ボタンを押した瞬間で、玉を捕まえたら、
	// select変数を代入して、S_MOVEへ
	if (Lbutton.IsPushDown()) 
	{
		GetMousePoint(&mouseX, &mouseY);
		if (mouseX - POSITION_X >= 0)
			selectX = (mouseX - POSITION_X) / IMAGE_SIZE;
		else
			selectX = -1;
		if (mouseY >= POSITION_Y)
			selectY = (mouseY - POSITION_Y) / IMAGE_SIZE;
		else
			selectY = -1;
		if (selectX >= 0 && selectX < WIDTH 
			&& selectY >= 0 && selectY < HEIGHT)
		{
			selectColor = field[selectY][selectX].color;
			state = STATE::S_MOVE;
		} 
		else
			selectColor = -1;
	}
}

void Stage::UpdateMove()
{
	// 隣のマスに移動したら、玉を入れ替える
	int lastX = selectX; // 前回の場所を保存
	int lastY = selectY;
	GetMousePoint(&mouseX, &mouseY);
	if (mouseX - POSITION_X >= 0)
	{
		selectX = (mouseX - POSITION_X) / IMAGE_SIZE;
		if (selectX >= WIDTH)
			selectX = WIDTH - 1;
	}
	else
	{
		selectX = 0;
	}
	if (mouseY >= POSITION_Y)
	{
		selectY = (mouseY - POSITION_Y) / IMAGE_SIZE;
		if (selectY >= HEIGHT)
			selectY = HEIGHT - 1;
	}
	else
	{
		selectY = 0;
	}
	if (lastX != selectX || lastY != selectY) 
	{
		// 移動したので、玉を入れ替える
		auto tmp = field[lastY][lastX];
		field[lastY][lastX] = field[selectY][selectX];
		field[selectY][selectX] = tmp;
		field[lastY][lastX].MoveTo(lastX * IMAGE_SIZE, lastY * IMAGE_SIZE, SPEED);
		field[selectY][selectX].MoveTo(selectX * IMAGE_SIZE, selectY * IMAGE_SIZE, SPEED);
	}
	// ボタンを離したら（時間経過したら）S_DELETEへ
	if (Lbutton.IsPushUp())
	{
		if (CheckDelete())
		{
			state = STATE::S_DELETE;
		}
		else 
		{
			state = STATE::S_IDLE;
		}
	}
}

void Stage::UpdateDelete()
{
	// すべての玉で
	// eraseCountが0以外であれば、eraseCount--をする
	for (int y = 0; y < HEIGHT; y++) 
	{
		for (int x = 0; x < WIDTH; x++)
		{
			field[y][x].EraseUpdate();
		}
	 }
	// すべてのeraseCountが0になったら、S_FALLへ
	// eraseCount!=0が1つでもあればダメ
	bool r = true;
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++) 
		{
			if (field[y][x].EraseCount() > 0) 
			{
				r = false;
			}
		}
	}
	if (r) 
	{
		PrepareFall();
		state = STATE::S_FALL;
	}
}

void Stage::UpdateFall()
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (!field[y][x].MoveFinish())
			{
				return;
			}
		}
	}
	// 全部落ちるのを待つ
	// 全部落ちたら、CheckDelete()を呼んで、
	// trueならS_DELETE, falseならS_IDLE
	if (CheckDelete())
	{
		state = STATE::S_DELETE;
	}
	else
	{
		state = STATE::S_IDLE;
	}
}

bool Stage::CheckDelete()
{
	for (int y=0; y<HEIGHT; y++) 
	{
		for (int x = 0; x < WIDTH; x++) 
		{
			field[y][x].ClearErase();
		}
	}
	// 3つ並んだものを探す
	bool ret = false;
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (x<WIDTH-2 && field[y][x].color == field[y][x+1].color && field[y][x].color == field[y][x+2].color) 
			{
				field[y][x].DeleteBegin();
				field[y][x+1].DeleteBegin();
				field[y][x+2].DeleteBegin();
				ret = true;
			}
			if (y < HEIGHT - 2 && field[y][x].color == field[y+1][x].color && field[y][x].color == field[y+2][x].color) 
			{
				field[y][x].DeleteBegin();
				field[y + 1][x].DeleteBegin();
				field[y + 2][x].DeleteBegin();
				ret = true;
			}
		}
	}
	return ret;
}

void Stage::PrepareFall()
{
	for (int x = 0; x < WIDTH; x++) 
	{
		int erased = 0;
		for (int y = HEIGHT - 1; y >= 0; y--) 
		{
			if (field[y][x].DoErase() == 0)
			{
				field[y + erased][x] = field[y][x];
				field[y + erased][x].moveCount = 10 * erased;
				field[y + erased][x].MoveTo(x * IMAGE_SIZE, (y + erased) * IMAGE_SIZE, 10 * erased);
			}
			else
			{
				erased++;
			}
		}
		for (int e = 0; e < erased; e++)
		{
			field[e][x].color = (COLOR)GetRand(COLOR::MAX - 1);
			field[e][x].x = x * IMAGE_SIZE;
			field[e][x].y = (e - erased) * IMAGE_SIZE;
			field[e][x].MoveTo(x * IMAGE_SIZE, e * IMAGE_SIZE, 10 * erased);
		}
	}
}

MouseLButton::MouseLButton() : 
		current(false), last(false)
{
}

MouseLButton::~MouseLButton()
{
}

void MouseLButton::Update()
{
	last = current;
	current = (GetMouseInput() & MOUSE_INPUT_LEFT);
}

bool MouseLButton::IsPush()
{
	return current;
}

bool MouseLButton::IsPushDown()
{
	return (!last && current);
}

bool MouseLButton::IsPushUp()
{
	return (last && !current);
}

void Stage::BALLINFO::DeleteBegin()
{
	doErase = 1;
	eraseCount = POSITION_X;
}

void Stage::BALLINFO::EraseUpdate()
{
	if (eraseCount > 0);
	eraseCount--;
}

bool Stage::BALLINFO::IsEraseEnd()
{
	return (eraseCount <= 0);
}

void Stage::BALLINFO::MoveTo(int toX, int toY, int count)
{
	beginX = x;
	beginY = y;
	endX = toX;
	endY = toY;
	moveCount = count;
	rate = 0.0f;
}

void Stage::BALLINFO::MoveUpdate()
{
	if(rate < 1.0f)
	{
		rate += 1.0f / moveCount;
		if (rate > 1.0)
		{
			rate = 1.0f;
		}
		x = (endX - beginX) * rate + beginX;
		y = (endY - beginY) * rate + beginY;
	}
}
