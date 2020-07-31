//#include "DxLib.h"
#include "scene.h"
#include <cmath>
#include <math.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("Rise a babble");			// �^�C�g����ݒ�
	ChangeWindowMode(TRUE);						// �E�B���h�E���[�h�ŋN��
	SetGraphMode(WINDOW_X, WINDOW_Y, COLOR_BIT);
	SetDrawScreen(DX_SCREEN_BACK);				// �`����ʂ𗠂ɂ���
	if (DxLib_Init() == -1)return -1;

	Scene S;//�V�[���N���X���Ăяo��

	 
	while (ProcessMessage() == 0 && S.Changer != 99) {
		
		 
		S.getKeyInput();
		ClearDrawScreen(); // ��ʂ̏�����
		switch (S.Changer)
		{
		case 0:S.Title(); break;//�^�C�g���@�@
		case 1:S.GameInit(); break;//�Q�[���O�̏�����
		case 2:S.GameMain(); break;//�Q�[��
		case 3:S.Result(); break;//���U���g
		case 4:S.Option(); break;//�I�v�V����
		case 5:S.Ending(); break;//�I��

		default:goto finish; break;//�����܂ł����}����
		}
		ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f 

	}
	finish://goto����̉��}����
	DxLib_End();				// DX ���C�u�����g�p�̏I������
	return 0;					// �\�t�g�̏I��
}