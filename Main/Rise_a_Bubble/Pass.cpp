#include <iostream>
#include "Scene.h"

static int Cursor = 0;//Cursor:�J�[�\���p
bool Ps_OneShot = false, Ps_Flg = false;// OneShot:���d�����̖h�~ Flg:B�𗣂��ƃV�[�����ς��@
bool PBk_OneShot = false, PBk_Flg = false;	//A�{�^��(�o�b�N)�p�̑��d�����h�~
int p = 0;	//�p�X�̔Ԓn���݂�
bool ErrorPass = FALSE;
char DummyNumber[20];	//�p�X���͎��Ɏg���ϐ�
void Scene::Pass() {

	sound.PlayBGM(sound.stack);

	DrawGraph(0, 0, images.back[14], FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);//������
	DrawFillBox(10, 270, WINDOW_X - 10, 470, 0xaaaaaa);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);// �s����

	SetFontSize(20);
	if (ErrorPass == TRUE) DrawString(50, 70, "20�����̃R�[�h���y�[�X�g���Ă��������B", 0xffffff);

	SetFontSize(30);
	DrawString(50, (WINDOW_Y / 32) * 15, "A", 0xffffff);
	DrawString(145, (WINDOW_Y / 32) * 15, "B", 0xffffff);
	DrawString(240, (WINDOW_Y / 32) * 15, "C", 0xffffff);
	DrawString(335, (WINDOW_Y / 32) * 15, "D", 0xffffff);
	DrawString(50, (WINDOW_Y / 32) * 17, "E", 0xffffff);
	DrawString(145, (WINDOW_Y / 32) * 17, "F", 0xffffff);
	DrawString(240, (WINDOW_Y / 32) * 17, "G", 0xffffff);
	DrawString(335, (WINDOW_Y / 32) * 17, "H", 0xffffff);
	DrawString(50, (WINDOW_Y / 32) * 19, "I", 0xffffff);
	DrawString(145, (WINDOW_Y / 32) * 19, "J", 0xffffff);
	DrawString(240, (WINDOW_Y / 32) * 19, "Back", 0xffffff);
	DrawString(335, (WINDOW_Y / 32) * 19, "Paste", 0xffffff);
	DrawString(335, (WINDOW_Y / 32) * 21, "Create", 0xffffff);

	DrawTriangle(4 * (7 + (Cursor % 4) * 23), 13 * (23 + (Cursor / 4) * 3),
		4 * (7 + (Cursor % 4) * 23), 13 * (25 + (Cursor / 4) * 3),
		4 * (12 + (Cursor % 4) * 23), 13 * (24 + (Cursor / 4) * 3), 0xffff00, TRUE);

	//�J�[�\��
	if (input.Buttons[XINPUT_BUTTON_DPAD_LEFT] && Ps_OneShot == false) {
		((Cursor % 4) == 0) ? Cursor += 3 : Cursor--;
		Ps_OneShot = true;
		sound.PlaySE(sound.choose);
	}
	else if (input.Buttons[XINPUT_BUTTON_DPAD_RIGHT] && Ps_OneShot == false) {
		((Cursor % 4) == 3) ? Cursor -= 3 : Cursor++;
		Ps_OneShot = true;
		sound.PlaySE(sound.choose);
	}
	else if (input.Buttons[XINPUT_BUTTON_DPAD_UP] && Ps_OneShot == false) {
		(Cursor <= 3) ? Cursor += 12 : Cursor -= 4;
		Ps_OneShot = true;
		sound.PlaySE(sound.choose);
	}
	else if (input.Buttons[XINPUT_BUTTON_DPAD_DOWN] && Ps_OneShot == false) {
		(Cursor >= 12) ? Cursor -= 12 : Cursor += 4;
		Ps_OneShot = true;
		sound.PlaySE(sound.choose);
	}
	//��ʑJ�ڏ���
	if (input.Buttons[XINPUT_BUTTON_B] && Ps_OneShot == false) {
		Ps_OneShot = true, Ps_Flg = true;
	}
	else if (!input.Buttons[XINPUT_BUTTON_B] && Ps_Flg == true)
	{
		Difficulty = 0;			//�{���ۑ�������Փx�������
		//LoadNumber = Cursor;	//�J�[�\���ʒu�̔ԍ��������
		if (Cursor <= 9){
			if (p < 20) {
				PassNumber[p] = 'A' + Cursor;
				p++;
				sound.PlaySE(sound.decide);
			}
			else if (p == 20) {
				PassNumber[p] = NULL;
			}
		}
		else if (Cursor == 10)sound.PlaySE(sound.cancel) , Cursor = 0, Before = Changer, Changer = GAMEMODE;
		else if (Cursor == 11) {
			GetClipboardText(DummyNumber);	//�_�~�[�ϐ��ŃN���b�v�{�[�h�̕������������𖞂����Ă��邩���ׂ�
			if (DummyNumber[19] == NULL || DummyNumber[20] != NULL) { //20��������or21�����ȏ�ŃG���[���\��
				ErrorPass = TRUE;
			}
			else {
				ErrorPass = FALSE;
				GetClipboardText(PassNumber);
				p = 20;
			}
		}
		else if (Cursor == 15) CodeRnd_flg = FALSE, Pass_Flg = TRUE, Load_Flg = FALSE, Before = Changer, Changer = GAMEINIT;	//��Փx�I��

		Ps_Flg = false;
	}


	if (input.Buttons[XINPUT_BUTTON_A] && PBk_OneShot == false) {
		PBk_OneShot = true, PBk_Flg = true;
	}
	else if (!input.Buttons[XINPUT_BUTTON_A] && PBk_Flg == true)
	{
		if (p > 0) {
			PassNumber[p - 1] = NULL;
			p--;
		}
		else if (p == 0) {
			Cursor = 0, Before = Changer, Changer = GAMEMODE;
		}
		sound.PlaySE(sound.cancel);
		PBk_OneShot = false, PBk_Flg = false;
	}

	if (Ps_OneShot == true && !(input.Buttons[XINPUT_BUTTON_B]
		|| input.Buttons[XINPUT_BUTTON_DPAD_UP]
		|| input.Buttons[XINPUT_BUTTON_DPAD_DOWN]
		|| input.Buttons[XINPUT_BUTTON_DPAD_LEFT]
		|| input.Buttons[XINPUT_BUTTON_DPAD_RIGHT])) {

		Ps_OneShot = false;
	}
	SetFontSize(32);
	DrawFormatString(20, 150, 0xffffff, "PASS:%s", PassNumber);
}