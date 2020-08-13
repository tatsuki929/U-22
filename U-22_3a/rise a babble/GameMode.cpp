#include "Scene.h"

static int Cursor = 0, OneShot = 0, Flg = 0;//Cursor:�J�[�\���p OneShot:���d�����̖h�~ Flg:B�𗣂��ƃV�[�����ς��@

std::string MODE[]= {"EASY","NORMAL","HARD","EXHARD","CODE","MAKE","Back"};

void Scene::GameMode() {
	DrawString(0, 0, "GameMode", 0xffffff);

	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 21, "EASY", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 22, "NORMAL", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 23, "HARD", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 24, "EXHARD", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 25, "CODE", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 26, "MAKE", 0xffffff);
	DrawString((WINDOW_X / 4), (WINDOW_Y / 32) * 27, "BACK", 0xffffff);

	DrawFormatString((WINDOW_X / 4), (WINDOW_Y / 32) * 30, 0xffffff, "GameMode:%s", MODE[Cursor].c_str());
	DrawTriangle((WINDOW_X / 64) * 14, (WINDOW_Y / 64) * (42 + Cursor * 2),
		(WINDOW_X / 64) * 14, (WINDOW_Y / 64) * (44 + Cursor * 2),
		(WINDOW_X / 64) * 16, (WINDOW_Y / 64) * (43 + Cursor * 2), 0xffff00, TRUE);


#ifdef DEBUG
	for (int i = 0; i < 16; i++) {
		DrawFormatString(50, 200 + (i * 20), 0xffffff, "%d", Input.Buttons[i]);
	}
#endif // DEBUG

	/*����*/

	//�J�[�\��

	if (Input.Buttons[XINPUT_BUTTON_DPAD_UP] && OneShot == 0) {
		(Cursor > 0) ? Cursor-- : Cursor = 6;
		OneShot = 1;
	}
	else if (Input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && OneShot == 0) {
		(Cursor < 6) ? Cursor++ : Cursor = 0;
		OneShot = 1;
	}
	//��ʑJ�ڏ���
	if (Input.Buttons[XINPUT_BUTTON_B] && OneShot == 0) {
		OneShot = 1, Flg = 1;
	}
	else if (!Input.Buttons[XINPUT_BUTTON_B] && Flg == 1)
	{
//		if (Cursor == 0)Before = Changer, Changer = GAMEINIT;
		if (Cursor == 6)Before = Changer, Changer = TITLE;
		else;

		Cursor = 0,	Flg = 0;
	}

	if (OneShot == 1 && !(Input.Buttons[XINPUT_BUTTON_B]
		|| Input.Buttons[XINPUT_BUTTON_DPAD_UP]
		|| Input.Buttons[XINPUT_BUTTON_DPAD_DOWN])) {

		OneShot = 0;
	}

}