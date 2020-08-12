#include "Scene.h"
#include "Enemy.h"
#include <iostream>     // cout
#include <ctime>        // time
#include <cstdlib>      // srand,rand


void Scene::GameInit() {
	//DrawString(0, 0, "Now Roading...", 0xffffff);

	int rx = 0;			//x軸の座標位置
	int ry = 0;			//y軸の座標位置

	// 障害物の初期設定 
	for (int i = 0; i < IMMOVABLEOBJMAX; i++) {

		myEnemy.Entire_x[i] = rx * (WIDTH / 5) + 45;		//障害物の座標に入れる前にあらかじめ取っておく座標
		myEnemy.Entire_y[i] = ry * (HEIGHT / 2) / 5 + 50;		//障害物の座標に入れる前にあらかじめ取っておく座標

		rx += 1;	//x座標

		if (i % 5 == 4) {	//5個配置されるごとにx座標を左に戻しy座標を1段下げる
			rx = 0;
			ry += 1;
		}
	}

	// 障害物の初期設定 
	for (int m = 0; m < MAPMAX; m++) {
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {

			myEnemy.g_immovableobj[m][i].r = 30.0f;	//障害物の円の半径
			myEnemy.g_immovableobj[m][i].setflg = false;	//障害物を配置するかのフラグを全てfalseに
			myEnemy.g_immovableobj[m][i].flg = FALSE;
		}

		// 敵の初期設定  
		for (int i = 0; i < ENEMYMAX; i++) {
			myEnemy.g_enemy[m][i].sx = 50;
			myEnemy.g_enemy[m][i].sy = 50;
			myEnemy.g_enemy[m][i].flg = FALSE;
			myEnemy.g_enemy[m][i].move = FALSE;
		}
	}

	for (int c = 0; c < BULLET_MAX; c++)bullet[c].c_flg = FALSE;

	player.x = WINDOW_X / 2;		//Player座標の初期化
	player.y = WINDOW_Y / 4 * 3;	//Player座標の初期化
	player.size = 30;				//Playerサイズの初期化
	player.max_speed = 6;			//Playerの最大スピードの初期化
	player.scl = (WINDOW_Y - player.y);		//Scrollの初期化
	for (int i = 0; i < 4; i++) {
		Vec[i].Inertia = 0;			//慣性の初期化
		Vec[i].De_Flg = TRUE;		//減速フラグの初期化
	}
	srand((unsigned)time(NULL));	//時刻でランダムの初期値を決める
	for (int p = 0; p < MAPMAX; p++) {
		myEnemy.Pattern[p] = GetRand(2);	//0〜3のランダムな値
	}

	for (int moji = 0; moji < 3; moji++) {
		CodeOrigin[moji] = moji*moji;		
	}

	//WaitTimer(300);
	Before = Changer, Changer = GAMEMAIN;		//シーンの切り替え
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
	if ((images.muzzle = LoadGraph("Images/muzzle.png")) == -1) return -1;		//発射向きの画像
	if ((images.player = LoadGraph("Images/player_b.png")) == -1) return -1;	//Player画像
	if ((images.bubble = LoadGraph("Images/bubble.png")) == -1) return -1;		//しゃぼん弾の画像
	for (int i = 0; i < 10; i++) {
		if ((images.back[i] = LoadGraph("Images/stick.png")) == -1) return -1;	//背景画像
	}
	if ((ImmovableObj = LoadGraph("Images/Player__.png")) == -1) return -1;	//動かせる障害物画像の読み込み
	if ((enemy = LoadGraph("Images/bubble.png")) == -1) return -1;	//動かせる障害物画像の読み込み
}

int Scene::Cnt(int n) {		//与えられた数値に1を足して返す
	int i = n;
	i++;
	return i;
}

void Scene::ScrollMap() {	//背景処理
	for (int i = 0; i < 10; i++) {
		DrawGraph(0, 0 - (player.scl - (WINDOW_Y*-i)) + (WINDOW_Y / 4), images.back[i], FALSE);
	}
}

void Scene::DrawPlayer() {	//Playerの描画
	Angle();
	DrawRotaGraph(player.x, player.y, 1, 0, images.player, TRUE);
	DrawRotaGraph(player.x, player.y, 1, player.angle, images.muzzle, TRUE);
}

void Scene::Angle() {		//Playerの向きを求める
	if (input.ThumbLX >= DEADZONE || input.ThumbLX <= -DEADZONE || input.ThumbLY >= DEADZONE || input.ThumbLY <= -DEADZONE) {
		StickX = (input.ThumbLX / 3.2767);		
		StickY = (input.ThumbLY / 3.2767);		
	}
	float rad = atan2(StickX, StickY);
	player.angle = rad;
}

void Scene::Bound() {		//Playerの壁での反射処理
	if (player.x - player.size <= 0) {					//Playerが左の壁に当たったときの処理
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
	else if (player.x + player.size >= WINDOW_X) {		//Playerが右の壁に当たったときの処理
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

void Scene::PlayerMove() {		//Playerの移動処理
	//左スティックがデッドゾーン内なら加速フラグをFALSE
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

	//左スティックがデッドゾーン外なら移動し、そして加速、各方向への移動、加速フラグをオン
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

	for (int i = UP; i < VEC_SIZE; i++) {		//全方向共通の処理
		if (Vec[i].Add_Flg == TRUE) {		//加速フラグがオンになっていたら一定間隔ごとに加速度を増加
			Vec[i].Add_Cnt = Cnt(Vec[i].Add_Cnt);

			if (Vec[i].Add_Cnt % 3 == 0) {
				Vec[i].Inertia += 0.15f;
			}
		}
		else if (Vec[i].Add_Flg == FALSE) {	//加速フラグがオフならカウントをゼロにする
			Vec[i].Add_Cnt = 0;
		}

		if (Vec[i].Inertia > player.max_speed)Vec[i].Inertia = player.max_speed;	//最大スピードを超えてたら最大スピードにする


		if (Vec[i].De_Flg == TRUE) {		//減速フラグがオンになってたら一定間隔ごとに加速度を減少
			Vec[i].De_Cnt = Cnt(Vec[i].De_Cnt);
			if (Vec[i].De_Cnt % 5 == 0) {
				Vec[i].Inertia -= 0.2f;
			}
		}
		if (Vec[i].De_Flg == FALSE) {		//減速フラグがオフならカウントをゼロにする
			Vec[i].De_Cnt = 0;
		}
		if (Vec[i].Inertia < 0) {			//加速度がゼロを下回らないようにする
			Vec[i].Inertia = 0;
		}

		if (Vec[i].Add_Flg == FALSE) {		//加速フラグがオフでもその方向に進む処理
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

		if (Vec[i].Inertia == 0.00f) {	//加速度がゼロなら減速をやめる
			Vec[i].De_Flg = FALSE;
		}

		if (player.y <= (WINDOW_Y / 5) * 3) {	//Playerの移動範囲の制限
			player.y = (WINDOW_Y / 5) * 3;
		}
		if (player.y >= (WINDOW_Y / 5) * 4) {
			player.y = (WINDOW_Y / 5) * 4;
		}

	}
}

void Scene::CreateBubble() {		//しゃぼん弾の生成
	static int i = 0;
	static int m = 0;
	if (input.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] == TRUE || input.Buttons[XINPUT_BUTTON_B] == TRUE) {		//RBもしくはBで弾を生成
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

void Scene::FireBubble() {		//生成されたしゃぼん弾を発射
	for (int i = 0; i < BULLET_MAX; i++) {		//生成されていない場合配列の情報を初期化
		if (bullet[i].c_flg == FALSE && bullet[i].m_flg == FALSE) {
			bullet[i].x = 0;
			bullet[i].y = 0;
			bullet[i].angle = 0;
			bullet[i].speed = 0;
			bullet[i].time = 0;
		}

		if (bullet[i].c_flg == TRUE) {		//生成されていたら描画
			//DrawCircle(bullet[i].x, bullet[i].y, BULLET_SIZE, 0x00ff00, TRUE);
			DrawRotaGraph(bullet[i].x, bullet[i].y, 1, 0, images.bubble, TRUE);
#ifdef DEBUG
			DrawFormatString(bullet[i].x - 5, bullet[i].y - 5, 0x000000, "%d", i);
#endif // DEBUG
			bullet[i].m_flg = TRUE;
		}

		if (bullet[i].m_flg == TRUE) {		//生成、描画がされたしゃぼん弾をPlayerの向きに発射
			bullet[i].x += cos(bullet[i].angle)*BULLET_SPEED;
			bullet[i].y += sin(bullet[i].angle)*BULLET_SPEED;
			if (bullet[i].x > WINDOW_X + BULLET_SIZE || bullet[i].y > WINDOW_Y + BULLET_SIZE
				|| bullet[i].x < 0 - BULLET_SIZE || bullet[i].y < 0 - BULLET_SIZE) {	//しゃぼん弾が画面外に行くと消滅
				bullet[i].c_flg = FALSE;
				bullet[i].m_flg = FALSE;
			}
		}
	}
}

void Scene::FloatBubble()		//無操作時にPlayerがふわふわと動く処理
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

	//	プレイヤーと障害物の当たり判定
	for (int m = 0; m < MAPMAX; m++) {
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {		//プレイヤーと動かせるオブジェクト(円と円)
			hit_x[i] = player.x - myEnemy.g_immovableobj[m][i].x;	//プレイヤーと障害物のx座標の差
			hit_y[i] = player.y - myEnemy.g_immovableobj[m][i].y;	//プレイヤーと障害物のy座標の差
			hit_r[i] = sqrt(hit_x[i] * hit_x[i] + hit_y[i] * hit_y[i]);	//プレイヤーと障害物の円の半径の和

			if (hit_r[i] <= player.size + myEnemy.g_immovableobj[m][i].r)		//当たっているか判定
			{
				DrawString(100, HEIGHT - 20, "障害物とヒット", White);
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

		//プレイヤーと敵の当たり判定
		for (int e = 0; e < ENEMYMAX; e++) {	//円と四角
			if ((DistanceSqrf(myEnemy.g_enemy[m][e].mx, (myEnemy.g_enemy[m][e].mx + myEnemy.g_enemy[m][e].sx), myEnemy.g_enemy[m][e].my, (myEnemy.g_enemy[m][e].my + myEnemy.g_enemy[m][e].sy), player.x, player.y, player.size) == true)) {
				DrawString(100, HEIGHT - 40, "敵とヒット", White);
			}
		}

		//しゃぼん弾との当たり判定
		for (int i = 0; i < IMMOVABLEOBJMAX; i++) {		//プレイヤーと動かせるオブジェクト(円と円)
			for (int b = 0; b < BULLET_MAX; b++) {
				hit_x[i] = bullet[b].x - myEnemy.g_immovableobj[m][i].x;	//プレイヤーと障害物のx座標の差
				hit_y[i] = bullet[b].y - myEnemy.g_immovableobj[m][i].y;	//プレイヤーと障害物のy座標の差
				hit_r[i] = sqrt(hit_x[i] * hit_x[i] + hit_y[i] * hit_y[i]);	//プレイヤーと障害物の円の半径の和

				if (hit_r[i] <= BULLET_SIZE + myEnemy.g_immovableobj[m][i].r)		//当たっているか判定
				{
					DrawString(100, HEIGHT - 20, "障害物にヒット", White);
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
	if (L - r > x || R + r < x || T - r > y || B + r < y) {//矩形の領域判定1
		return false;
	}
	if (L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < r * r)) {//左上の当たり判定
		return false;
	}
	if (R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < r * r)) {//右上の当たり判定
		return false;
	}
	if (L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < r * r)) {//左下の当たり判定
		return false;
	}
	if (R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < r * r)) {//右下の当たり判定
		return false;
	}
	return true;//すべての条件が外れたときに当たっている
}

void Scene::CreateCode() {		//マップコードの生成
	if (CodeRnd_flg == TRUE) {
		for (int co = 0; co < MAPMAX; co++) {
			DrawFormatString(0 + co * 10, 150, 0xff0000, "%c\n", Code[co]);
		}

		for (int m = 0; m < MAPMAX; m++) {
			Code[m] = 'A' + Pattern[m];
			if (m == MAPMAX - 1)Code[MAPMAX] = '\n';
		}
	}
	if (CodeRnd_flg == FALSE) {

	}
}