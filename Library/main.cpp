#include <DxLib.h>
#include "App.h"
#include "../Source/config.h"

#define CoGVersion (2.2)

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

	SetMainWindowText(WINDOW_NAME);
	SetWindowSizeExtendRate(WINDOW_EXTEND);
	ChangeWindowMode(WINDOW_MODE); // Windowモードの場合

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	AppInit();

	int mStartTime = GetNowCount();

	while (true) {
		int cur = GetNowCount();
		if (cur < mStartTime + 16) //120fps対策
			continue;
		mStartTime = cur;

		AppUpdate();
		ClearDrawScreen();
		AppDraw();
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) != 0 )
			break;
		ScreenFlip();


	}
	AppRelease();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}