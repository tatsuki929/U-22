#include "Scene.h"
#include "Enemy.h"
#include <iostream>     // cout
#include <ctime>        // time
#include <cstdlib>      // srand,rand


void Scene::GameInit() {
	//DrawString(0, 0, "Now Roading...", 0xffffff);

	int rx = 0;			//x���̍��W�ʒu
	int ry = 0;			//y���̍��W�ʒu

	// ��Q���̏����ݒ� 
	for (int i = 0; i < IMMOVABLEOBJMAX; i++) {

		myEnemy.Entire_x[i] = rx * (WIDTH / 5) + 45;		//��Q���̍��W�ɓ����O�ɂ��炩���ߎ���Ă������W
		myEnemy.Entire_y[i] = ry * (HEIGHT / 2) / 5 + 50;		//��Q���̍��W�ɓ����O�ɂ��炩���ߎ���Ă������W

		rx += 1;	//x���W

		if (i % 5 == 4) {	//5�z�u����邲�Ƃ�x���W�����ɖ߂�y���W��1�i������
			rx = 0;
			ry += 1;
		}
	}

	// ��Q���̏����ݒ� 
	for (int m = 0; m < MAPMAX; m++) {
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {

			myEnemy.g_immovableobj[m][i].r = 30.0f;	//��Q���̉~�̔��a
			myEnemy.g_immovableobj[m][i].setflg = false;	//��Q����z�u���邩�̃t���O��S��false��
			myEnemy.g_immovableobj[m][i].flg = FALSE;
		}

		// �G�̏����ݒ�  
		for (int i = 0; i < ENEMYMAX; i++) {
			myEnemy.g_enemy[m][i].sx = 50;
			myEnemy.g_enemy[m][i].sy = 50;
			myEnemy.g_enemy[m][i].flg = FALSE;
			myEnemy.g_enemy[m][i].move = FALSE;
		}
	}

	for (int c = 0; c < BULLET_MAX; c++)bullet[c].c_flg = FALSE;

	player.x = WINDOW_X / 2;		//Player���W�̏�����
	player.y = WINDOW_Y / 4 * 3;	//Player���W�̏�����
	player.size = 30;				//Player�T�C�Y�̏�����
	player.max_speed = 6;			//Player�̍ő�X�s�[�h�̏�����
	player.scl = (WINDOW_Y - player.y);		//Scroll�̏�����
	for (int i = 0; i < 4; i++) {
		Vec[i].Inertia = 0;			//�����̏�����
		Vec[i].De_Flg = TRUE;		//�����t���O�̏�����
	}
	srand((unsigned)time(NULL));	//�����Ń����_���̏����l�����߂�
	for (int p = 0; p < MAPMAX; p++) {
		myEnemy.Pattern[p] = GetRand(2);	//0�`3�̃����_���Ȓl
	}

	for (int moji = 0; moji < 3; moji++) {
		CodeOrigin[moji] = moji*moji;		
	}

	//WaitTimer(300);
	Before = Changer, Changer = GAMEMAIN;		//�V�[���̐؂�ւ�
}

void Scene::GameMain() {

	ScrollMap();
	DrawPlayer();
	Bound();
	PlayerMove();
	CreateBubble();
	FireBubble();
	FloatBubble();
	myEnemy.CreateImmovableObj();
	myEnemy.DrawImmovableObj();
	myEnemy.MoveEnemy();
	HitCheck();
	CreateCode();

#ifdef DEBUG
	DrawFormatString(0, 0, 0xff0000, "%d", input.ThumbLY);
	DrawFormatString(0, 15, 0xff0000, "%d", input.ThumbLX);
	DrawFormatString(0, 30, 0x0000ff, "%2.2f", player.x);
	DrawFormatString(0, 75, 0x00ff00, "%f", player.angle);
	DrawFormatString(0, 90, 0x00ff00, "%f", StickX);
	DrawFormatString(0, 105, 0x00ff00, "%f", StickY);
	DrawFormatString(0, 120, 0xff0000, "%2.2f", player.scl);
	DrawFormatString(0, 135, 0xff0000, "%2.2f", player.scl);


	DrawFormatString(player.x - 3, player.y - 50 - 3, 0xff0000, "%2.2f", Vec[UP].Inertia);
	DrawFormatString(player.x - 3, player.y - 60 - 3, 0x0000ff, "%d", Vec[UP].De_Flg);
	DrawFormatString(player.x - 3, player.y + 50 - 3, 0xff0000, "%2.2f", Vec[DOWN].Inertia);
	DrawFormatString(player.x - 3, player.y + 60 - 3, 0x0000ff, "%d", Vec[DOWN].De_Flg);
	DrawFormatString(player.x - 50 - 3, player.y - 3, 0xff0000, "%2.2f", Vec[LEFT].Inertia);
	DrawFormatString(player.x - 60 - 3, player.y - 3, 0x0000ff, "%d", Vec[LEFT].De_Flg);
	DrawFormatString(player.x + 50 - 3, player.y - 3, 0xff0000, "%2.2f", Vec[RIGHT].Inertia);
	DrawFormatString(player.x + 60 - 3, player.y - 3, 0x0000ff, "%d", Vec[RIGHT].De_Flg);

	DrawFormatString(0, 165, 0x0000ff, "%d", myEnemy.Entire_x[0]);
	DrawFormatString(0, 180, 0x0000ff, "%d", myEnemy.Entire_x[1]);
#endif // DEBUG

}

int Scene::LoadImages() {
	if ((images.muzzle = LoadGraph("Images/muzzle.png")) == -1) return -1;		//���ˌ����̉摜
	if ((images.player = LoadGraph("Images/player_b.png")) == -1) return -1;	//Player�摜
	if ((images.bubble = LoadGraph("Images/bubble.png")) == -1) return -1;		//����ڂ�e�̉摜
	for (int i = 0; i < 10; i++) {
		if ((images.back[i] = LoadGraph("Images/stick.png")) == -1) return -1;	//�w�i�摜
	}
	if ((ImmovableObj = LoadGraph("Images/Player__.png")) == -1) return -1;	//���������Q���摜�̓ǂݍ���
	if ((enemy = LoadGraph("Images/bubble.png")) == -1) return -1;	//���������Q���摜�̓ǂݍ���
}

int Scene::Cnt(int n) {		//�^����ꂽ���l��1�𑫂��ĕԂ�
	int i = n;
	i++;
	return i;
}

void Scene::ScrollMap() {	//�w�i����
	for (int i = 0; i < 10; i++) {
		DrawGraph(0, 0 - (player.scl - (WINDOW_Y*-i)) + (WINDOW_Y / 4), images.back[i], FALSE);
	}
}

void Scene::DrawPlayer() {	//Player�̕`��
	Angle();
	DrawRotaGraph(player.x, player.y, 1, 0, images.player, TRUE);
	DrawRotaGraph(player.x, player.y, 1, player.angle, images.muzzle, TRUE);
}

void Scene::Angle() {		//Player�̌��������߂�
	if (input.ThumbLX >= DEADZONE || input.ThumbLX <= -DEADZONE || input.ThumbLY >= DEADZONE || input.ThumbLY <= -DEADZONE) {
		StickX = (input.ThumbLX / 3.2767);		
		StickY = (input.ThumbLY / 3.2767);		
	}
	float rad = atan2(StickX, StickY);
	player.angle = rad;
}

void Scene::Bound() {		//Player�̕ǂł̔��ˏ���
	if (player.x - player.size <= 0) {					//Player�����̕ǂɓ��������Ƃ��̏���
		player.x = player.size;
		Vec[RIGHT].Inertia = (Vec[LEFT].Inertia - Vec[RIGHT].Inertia);
		Vec[LEFT].Inertia = 0;

		if (Vec[UP].Inertia != 0) {
			Vec[DOWN].Inertia = (Vec[UP].Inertia - Vec[DOWN].Inertia);
			Vec[UP].Inertia = 0;
		}
		else if (Vec[DOWN].Inertia != 0)
		{
			Vec[UP].Inertia = (Vec[DOWN].Inertia - Vec[UP].Inertia);
			Vec[DOWN].Inertia = 0;
		}
	}
	else if (player.x + player.size >= WINDOW_X) {		//Player���E�̕ǂɓ��������Ƃ��̏���
		player.x = WINDOW_X - player.size;
		Vec[LEFT].Inertia = (Vec[RIGHT].Inertia - Vec[LEFT].Inertia);
		Vec[RIGHT].Inertia = 0;

		if (Vec[UP].Inertia != 0) {
			Vec[DOWN].Inertia = (Vec[UP].Inertia - Vec[DOWN].Inertia);
			Vec[UP].Inertia = 0;
		}
		else if (Vec[DOWN].Inertia != 0)
		{
			Vec[UP].Inertia = (Vec[DOWN].Inertia - Vec[UP].Inertia);
			Vec[DOWN].Inertia = 0;
		}
	}
}

void Scene::PlayerMove() {		//Player�̈ړ�����
	//���X�e�B�b�N���f�b�h�]�[�����Ȃ�����t���O��FALSE
	if (input.ThumbLX <= DEADZONE) {
		Vec[RIGHT  ].Add_Flg = FALSE;
		Vec[RIGHT].De_Flg = TRUE;
	}
	if (input.ThumbLX >= -DEADZONE) {
		Vec[LEFT].Add_Flg = FALSE;
		Vec[LEFT].De_Flg = TRUE;
	}
	if (input.ThumbLY <= DEADZONE) {
		Vec[UP].Add_Flg = FALSE;
		Vec[UP].De_Flg = TRUE;
	}
	if (input.ThumbLY >= -DEADZONE) {
		Vec[DOWN].Add_Flg = FALSE;
		Vec[DOWN].De_Flg = TRUE;
	}

	//���X�e�B�b�N���f�b�h�]�[���O�Ȃ�ړ����A�����ĉ����A�e�����ւ̈ړ��A�����t���O���I��
	if (input.ThumbLY >= DEADZONE) {
		player.y -= Vec[UP].Inertia;
		player.scl -= Vec[UP].Inertia;
		for (int m = 0; m < MAPMAX; m++) {
			for (int i = 0; i < IMMOVABLEOBJMAX; i++) {
				myEnemy.g_immovableobj[m][i].y += Vec[UP].Inertia;
			}
			for (int e = 0; e < ENEMYMAX; e++) {
				myEnemy.g_enemy[m][e].my += Vec[UP].Inertia;
			}
		}
		Vec[UP].Add_Flg = TRUE;
		Vec[UP].De_Flg = FALSE;
	}
	if (input.ThumbLX >= DEADZONE) {
		player.x += Vec[RIGHT].Inertia;
		Vec[RIGHT].Add_Flg = TRUE;
		Vec[RIGHT].De_Flg = FALSE;
	}
	if (input.ThumbLY <= -DEADZONE) {
		player.y += Vec[DOWN].Inertia;
		player.scl += Vec[DOWN].Inertia;
		for (int m = 0; m < MAPMAX; m++) {
			for (int i = 0; i < IMMOVABLEOBJMAX; i++) {
				myEnemy.g_immovableobj[m][i].y -= Vec[DOWN].Inertia;
			}
			for (int e = 0; e < ENEMYMAX; e++) {
				myEnemy.g_enemy[m][e].my -= Vec[DOWN].Inertia;
			}
		}
		Vec[DOWN].Add_Flg = TRUE;
		Vec[DOWN].De_Flg = FALSE;
	}
	if (input.ThumbLX <= -DEADZONE) {
		player.x -= Vec[LEFT].Inertia;
		Vec[LEFT].Add_Flg = TRUE;
		Vec[LEFT].De_Flg = FALSE;
	}

	for (int i = UP; i < VEC_SIZE; i++) {		//�S�������ʂ̏���
		if (Vec[i].Add_Flg == TRUE) {		//�����t���O���I���ɂȂ��Ă�������Ԋu���Ƃɉ����x�𑝉�
			Vec[i].Add_Cnt = Cnt(Vec[i].Add_Cnt);

			if (Vec[i].Add_Cnt % 3 == 0) {
				Vec[i].Inertia += 0.15f;
			}
		}
		else if (Vec[i].Add_Flg == FALSE) {	//�����t���O���I�t�Ȃ�J�E���g���[���ɂ���
			Vec[i].Add_Cnt = 0;
		}

		if (Vec[i].Inertia > player.max_speed)Vec[i].Inertia = player.max_speed;	//�ő�X�s�[�h�𒴂��Ă���ő�X�s�[�h�ɂ���


		if (Vec[i].De_Flg == TRUE) {		//�����t���O���I���ɂȂ��Ă�����Ԋu���Ƃɉ����x������
			Vec[i].De_Cnt = Cnt(Vec[i].De_Cnt);
			if (Vec[i].De_Cnt % 5 == 0) {
				Vec[i].Inertia -= 0.2f;
			}
		}
		if (Vec[i].De_Flg == FALSE) {		//�����t���O���I�t�Ȃ�J�E���g���[���ɂ���
			Vec[i].De_Cnt = 0;
		}
		if (Vec[i].Inertia < 0) {			//�����x���[���������Ȃ��悤�ɂ���
			Vec[i].Inertia = 0;
		}

		if (Vec[i].Add_Flg == FALSE) {		//�����t���O���I�t�ł����̕����ɐi�ޏ���
			if (i == UP) {
				player.y -= Vec[UP].Inertia;
				player.scl -= Vec[UP].Inertia;
				for (int m = 0; m < MAPMAX; m++) {
					for (int i = 0; i < IMMOVABLEOBJMAX; i++) {
						myEnemy.g_immovableobj[m][i].y += Vec[UP].Inertia;
					}
					for (int e = 0; e < ENEMYMAX; e++) {
						myEnemy.g_enemy[m][e].my += Vec[UP].Inertia;
					}
				}

			}
			if (i == DOWN) {
				player.y += Vec[DOWN].Inertia;
				player.scl += Vec[DOWN].Inertia;
				for (int m = 0; m < MAPMAX; m++) {
					for (int i = 0; i < IMMOVABLEOBJMAX; i++) {
						myEnemy.g_immovableobj[m][i].y -= Vec[DOWN].Inertia;
					}
					for (int e = 0; e < ENEMYMAX; e++) {
						myEnemy.g_enemy[m][e].my -= Vec[DOWN].Inertia;
					}
				}
			}
			if (i == RIGHT)player.x += Vec[RIGHT].Inertia;
			if (i == LEFT)player.x -= Vec[LEFT].Inertia;

		}

		if (Vec[i].Inertia == 0.00f) {	//�����x���[���Ȃ猸������߂�
			Vec[i].De_Flg = FALSE;
		}

		if (player.y <= (WINDOW_Y / 5) * 3) {	//Player�̈ړ��͈͂̐���
			player.y = (WINDOW_Y / 5) * 3;
		}
		if (player.y >= (WINDOW_Y / 5) * 4) {
			player.y = (WINDOW_Y / 5) * 4;
		}

	}
}

void Scene::CreateBubble() {		//����ڂ�e�̐���
	static int i = 0;
	static int m = 0;
	if (input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] == TRUE || input.Buttons[XINPUT_BUTTON_B] == TRUE) {		//RB��������B�Œe�𐶐�
		if (i < BULLET_MAX) {
			if (bullet[i].c_flg == FALSE) {
				bullet[i].x = player.x;
				bullet[i].y = player.y;
				bullet[i].c_flg = TRUE;
				bullet[i].angle = player.angle - 1.5f;
			}
			if (m % (BULLET_MAX / 2) == 0)i++;
			m = Cnt(m);
		}
		if (i >= BULLET_MAX)i = 0;
		if (m == BULLET_MAX)m = 0;
#ifdef DEBUG
		DrawString(WINDOW_X - 55, 0, "GetKey", 0x00ff00);
		DrawFormatString(WINDOW_X - 20, 15, 0x00ff00, "%d", m);
		DrawFormatString(WINDOW_X - 20, 30, 0x00ff00, "%d", i);
		DrawFormatString(WINDOW_X - 20, 45, 0x00ff00, "%d", bullet[i].c_flg);
#endif // DEBUG
	}
	else if (input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] == FALSE || input.Buttons[XINPUT_BUTTON_B] == FALSE) {
		m = 0;
	}
}

void Scene::FireBubble() {		//�������ꂽ����ڂ�e�𔭎�
	for (int i = 0; i < BULLET_MAX; i++) {		//��������Ă��Ȃ��ꍇ�z��̏���������
		if (bullet[i].c_flg == FALSE && bullet[i].m_flg == FALSE) {
			bullet[i].x = 0;
			bullet[i].y = 0;
			bullet[i].angle = 0;
			bullet[i].speed = 0;
			bullet[i].time = 0;
		}

		if (bullet[i].c_flg == TRUE) {		//��������Ă�����`��
			//DrawCircle(bullet[i].x, bullet[i].y, BULLET_SIZE, 0x00ff00, TRUE);
			DrawRotaGraph(bullet[i].x, bullet[i].y, 1, 0, images.bubble, TRUE);
#ifdef DEBUG
			DrawFormatString(bullet[i].x - 5, bullet[i].y - 5, 0x000000, "%d", i);
#endif // DEBUG
			bullet[i].m_flg = TRUE;
		}

		if (bullet[i].m_flg == TRUE) {		//�����A�`�悪���ꂽ����ڂ�e��Player�̌����ɔ���
			bullet[i].x += cos(bullet[i].angle)*BULLET_SPEED;
			bullet[i].y += sin(bullet[i].angle)*BULLET_SPEED;
			if (bullet[i].x > WINDOW_X + BULLET_SIZE || bullet[i].y > WINDOW_Y + BULLET_SIZE
				|| bullet[i].x < 0 - BULLET_SIZE || bullet[i].y < 0 - BULLET_SIZE) {	//����ڂ�e����ʊO�ɍs���Ə���
				bullet[i].c_flg = FALSE;
				bullet[i].m_flg = FALSE;
			}
		}
	}
}

void Scene::FloatBubble()		//�����쎞��Player���ӂ�ӂ�Ɠ�������
{
	static int f = 0;
	static int x = 0, y = 0;
	f = Cnt(f);
	if ((Vec->Add_Flg == FALSE && Vec->De_Flg == FALSE) && f % 6 == 0) {
		player.x += infinity[x][y];
		y++;
		player.y += infinity[x][y];
		x++;
		y--;
		if (x == INFINITY_X)x = 0;
	}
}

void Scene::HitCheck(void)
{

	//	�v���C���[�Ə�Q���̓����蔻��
	for (int m = 0; m < MAPMAX; m++) {
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {		//�v���C���[�Ɠ�������I�u�W�F�N�g(�~�Ɖ~)
			hit_x[i] = player.x - myEnemy.g_immovableobj[m][i].x;	//�v���C���[�Ə�Q����x���W�̍�
			hit_y[i] = player.y - myEnemy.g_immovableobj[m][i].y;	//�v���C���[�Ə�Q����y���W�̍�
			hit_r[i] = sqrt(hit_x[i] * hit_x[i] + hit_y[i] * hit_y[i]);	//�v���C���[�Ə�Q���̉~�̔��a�̘a

			if (hit_r[i] <= player.size + myEnemy.g_immovableobj[m][i].r)		//�������Ă��邩����
			{
				DrawString(100, HEIGHT - 20, "��Q���ƃq�b�g", White);
				/*if (Vec[UP].Inertia != 0) {
					Vec[DOWN].Inertia = Vec[UP].Inertia;
					player.y -= Vec[DOWN].Inertia;
					Vec[UP].Inertia = 0;
				}
				else if (Vec[DOWN].Inertia != 0) {
					Vec[UP].Inertia = Vec[DOWN].Inertia;
					player.y += Vec[UP].Inertia;
					Vec[DOWN].Inertia = 0;
				}
				if (Vec[RIGHT].Inertia != 0) {
					Vec[LEFT].Inertia = Vec[RIGHT].Inertia;
					player.x -= Vec[LEFT].Inertia;
					Vec[RIGHT].Inertia = 0;
				}
				else if (Vec[LEFT].Inertia != 0) {
					Vec[RIGHT].Inertia = Vec[LEFT].Inertia;
					player.x += Vec[RIGHT].Inertia;
					Vec[LEFT].Inertia = 0;
				}*/

				/*if (Vec[UP].Inertia != 0)Vec[UP].Inertia = 0;
				if (Vec[DOWN].Inertia != 0)Vec[DOWN].Inertia = 0;
				if (Vec[RIGHT].Inertia != 0)Vec[RIGHT].Inertia = 0;
				if (Vec[LEFT].Inertia != 0)Vec[LEFT].Inertia = 0;*/

				Changer = GAMEINIT;;
			}
		}

		//�v���C���[�ƓG�̓����蔻��
		for (int e = 0; e < ENEMYMAX; e++) {	//�~�Ǝl�p
			if ((DistanceSqrf(myEnemy.g_enemy[m][e].mx, (myEnemy.g_enemy[m][e].mx + myEnemy.g_enemy[m][e].sx), myEnemy.g_enemy[m][e].my, (myEnemy.g_enemy[m][e].my + myEnemy.g_enemy[m][e].sy), player.x, player.y, player.size) == true)) {
				DrawString(100, HEIGHT - 40, "�G�ƃq�b�g", White);
			}
		}

		//����ڂ�e�Ƃ̓����蔻��
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {		//�v���C���[�Ɠ�������I�u�W�F�N�g(�~�Ɖ~)
			for (int b = 0; b < BULLET_MAX; b++) {
				hit_x[i] = bullet[b].x - myEnemy.g_immovableobj[m][i].x;	//�v���C���[�Ə�Q����x���W�̍�
				hit_y[i] = bullet[b].y - myEnemy.g_immovableobj[m][i].y;	//�v���C���[�Ə�Q����y���W�̍�
				hit_r[i] = sqrt(hit_x[i] * hit_x[i] + hit_y[i] * hit_y[i]);	//�v���C���[�Ə�Q���̉~�̔��a�̘a

				if (hit_r[i] <= BULLET_SIZE + myEnemy.g_immovableobj[m][i].r)		//�������Ă��邩����
				{
					DrawString(100, HEIGHT - 20, "��Q���Ƀq�b�g", White);
					myEnemy.g_immovableobj[m][i].x += cos(bullet[b].angle)*BULLET_SPEED;
					myEnemy.g_immovableobj[m][i].y += sin(bullet[b].angle)*BULLET_SPEED;
					bullet[b].c_flg = FALSE;
					bullet[b].m_flg = FALSE;
				}
			}
		}
	}

}

float Scene::DistanceSqrf(float L, float R, float T, float B, float x, float y, float r)
{
	if (L - r > x || R + r < x || T - r > y || B + r < y) {//��`�̗̈攻��1
		return false;
	}
	if (L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < r * r)) {//����̓����蔻��
		return false;
	}
	if (R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < r * r)) {//�E��̓����蔻��
		return false;
	}
	if (L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < r * r)) {//�����̓����蔻��
		return false;
	}
	if (R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < r * r)) {//�E���̓����蔻��
		return false;
	}
	return true;//���ׂĂ̏������O�ꂽ�Ƃ��ɓ������Ă���
}

void Scene::CreateCode() {		//�}�b�v�R�[�h�̐���
	if (CodeRnd_flg == TRUE) {
		for (int co = 0; co < MAPMAX; co++) {
			DrawFormatString(0 + co * 10, 150, 0xff0000, "%c\n", Code[co]);
		}

		for (int m = 0; m < MAPMAX; m++) {
			Code[m] = 'A' + myEnemy.Pattern[m];
			if (m == MAPMAX - 1)Code[MAPMAX] = '\n';
		}
	}
	if (CodeRnd_flg == FALSE) {

	}
}