#pragma once
#include "Init.h"
#define PI 3.14159265359
#define grv 9.80665

float realt; // 지금 시간

class Ball;

class Plate
{
public:


	Plate()
		:firstx(0.0f), firsty(0.0f), lastx(0.0f), lasty(0.0f), theta(0.0f), flag(0) {}

	Plate(float firstx, float firsty, float lastx, float lasty, float theta)
		:firstx(firstx), firsty(firsty), lastx(lastx), lasty(lasty), theta(theta), flag(0) {} // 판 생성 첫번째 꼭지점 x,y, 세번째 꼭지점 x,y , 회전각도

	void draw(GLuint);
	void draw();
	float calculateEndx(); // 원하는 각도만큼 회전 후 실제 lastx값
	float calculateEndy(); // 원하는 각도만큼 회전 후 실제 lasty값

	float firstx;
	float firsty;
	float lastx;
	float lasty;
	float theta;
	void rotate(Ball *b);
	int flag; //공이 부딪혔다 -> 1 

};


void Plate::draw(GLuint tex)
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glPushMatrix();
	glTranslatef(firstx, firsty, 0); //3.원위치
	glRotatef(theta, 0.0f, 0.0f, 1.0f); //2. 원하는 만큼 회전
	glTranslatef(-firstx, -firsty, 0); //1.판을 원점으로 가져오기

									   //glColor3f(0.870588, 0.721569, 0.529412);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(firstx, firsty, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lastx, firsty, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lastx, lasty, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(firstx, lasty, 0);
	glEnd();
	glPopMatrix(); // 뒤에 영향 안주게 원상태로
}
void Plate::draw()
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glPushMatrix();
	glTranslatef(firstx, firsty, 0); //3.원위치
	glRotatef(theta, 0.0f, 0.0f, 1.0f); //2. 원하는 만큼 회전
	glTranslatef(-firstx, -firsty, 0); //1.판을 원점으로 가져오기

									   //glColor3f(0.870588, 0.721569, 0.529412);
	glBegin(GL_QUADS);
	glVertex3f(firstx, firsty, 0);
	glVertex3f(lastx, firsty, 0);
	glVertex3f(lastx, lasty, 0);
	glVertex3f(firstx, lasty, 0);
	glEnd();
	glPopMatrix(); // 뒤에 영향 안주게 원상태로
}

float Plate::calculateEndx()
{
	return (cos(theta * PI / 180.0) * (lastx - firstx)) + firstx;
}

float Plate::calculateEndy()
{
	return (sin(theta * PI / 180.0) * (lastx - firstx)) + firsty;
}


class Ball
{
public:
	Ball()
		:x(0.0f), y(0.0f), radius(0.0f), lastvx(0.0f), lastvy(0.0f), vx(0.0f), vy(0.0f), lasttime(0.0f), i(-1) {}

	Ball(float x, float y, float radius)
		:x(x), y(y), radius(radius), lastvx(0.0f), lastvy(0.0f), vx(0.0f), vy(0.0f), lasttime(0.0f), i(-1) {}

	void draw();
	bool BallPlateCollision(Plate p); //판 p랑 충돌하면 true 아니면 false 반환
	void velocity(Plate p, float a);
	bool BallBallCollision(Ball b);

	float x;
	float y;
	float radius;

	float acc; //가속도, m=1이므로 결국 공이 받는 힘
	float accx; //x방향 가속도
	float accy; //y방향 가속도
	float lastacc; //공에 받는 힘이 변하기 전 가속도

	float vx; //x방향 속도
	float vy; //y방향 속도
	float lastvx; //가속도가 변하기 전 x 속도
	float lastvy; //가속도가 변하기 전 y 속도

	float lasttime;
	int i; //충돌하는 판 number

	float p_x; // 판이랑 충돌할때 속도
};

void Ball::draw()
{
	glDisable(GL_TEXTURE_2D);
	Lighting();
	glPushMatrix();
	glTranslatef(x, y, 0); //공 좌표만큼 이동
	glutSolidSphere(radius, 50, 50);
	glPopMatrix(); // 뒤에 영향 안주게 원상태로
	glDisable(GL_LIGHTING);

}

bool Ball::BallPlateCollision(Plate p)
{
	bool x_collision = (p.firstx - 0.5 <= x) && (x <= p.calculateEndx() + (p.firsty - p.lasty)*cos(p.theta*PI / 180.0)); // 공의 x 좌표가 판의 왼쪽끝부터 오른쪽끝 사이. 0.5는 오차 오른쪽
	bool y_collision = y - (((x - p.firstx) / (p.calculateEndx() - p.firstx))*(p.calculateEndy() - p.firsty) + p.firsty) < radius + 0.5; // x -x1 : x2- x1 = y- y1 : y2 - y1
	return (x_collision) && (y_collision);
}


void Ball::velocity(Plate p, float a) //판을 변수로 받는다
{

	if (this->BallPlateCollision(p)) // 공이 판위에 있을 때
	{

		if (p.theta == 0)
		{
			acc = 0;
			accx = acc * cos(-p.theta * PI / 180.0);
			accy = acc * sin(-p.theta * PI / 180.0);

		}
		else if (p.theta < 0) //판 회전방향이 좌상향
		{
			acc = grv * sin(-p.theta * PI / 180.0); //가속도는 g*sin(theta)
			accx = acc * cos(-p.theta * PI / 180.0);
			accy = acc * sin(-p.theta * PI / 180.0);

		}
		else //판 회전방향이 우상향
		{
			acc = grv * sin(p.theta * PI / 180.0);
			accx = acc * -cos(p.theta * PI / 180.0);
			accy = acc * sin(p.theta * PI / 180.0);

		}

	}

	else //공이 판위에 없을 때
	{
		acc = grv; //가속도는 g
		accy = acc;
		accx = 0;
	}

	if (acc < lastacc) // 판에 닿을 때
	{
		lasttime = realt; // 그때 시간 저장
		lastvx = 0; // 그때 속도 저장
		lastvy = 0;
	}
	else if (acc > lastacc) //판에서 떨어질때
	{
		lasttime = realt; // 그때 시간 저장
		lastvx = vx; // 그때 속도 저장
		lastvy = vy;
		i++;
	}

	realt = (float)(clock() - start) / CLOCKS_PER_SEC; //실제 시간 계산
	vx = lastvx + accx * (realt - lasttime) + a; // 항상 등가속도 운동이므로 a = (v1-v2)/(t1-t2) 에서 v1을 구한것. v1: 현재 속도, v2: 가속도가 변하기전 마지막 속도, t1: 현재 시간, t2: 가속도가 변한 시간
	vy = lastvy + accy * (realt - lasttime);


	if (this->BallPlateCollision(p))
	{
		x += 0.1 * vx; //0.01초마다 x 좌표 갱신
		y = (((x - p.firstx) / (p.calculateEndx() - p.firstx))*(p.calculateEndy() - p.firsty) + p.firsty) + radius; // 판위에 있을때는 x값만 계산하고 y는 그 x값에 맞는 위치로
	}
	else
	{
		x += 0.1 * vx; //0.01초마다 x 좌표 갱신
		y -= 0.1 * vy; //0.01초마다 y좌표 갱신
	}

	lastacc = acc; //가속도 매번 저장 - 달라지는지 확인하려고
}

bool Ball::BallBallCollision(Ball b)
{
	return (fabsf((x - b.x)*(x - b.x) + (y - b.y)*(y - b.y) - (radius + b.radius)*(radius + b.radius)) < 10);
}


void Plate::rotate(Ball *b) // 공이 부딪혔을 때 판 회전하게하는 함수
{
	Ball * ball;
	ball = (Ball *)b;

	bool x_collision = false;
	bool y_collision = false;

	if (ball->x < firstx) //공이 판 왼쪽에서 부딪힐 때 
	{
		x_collision = (firstx - ball->x - ball->radius< 0.5);
		y_collision = (lasty < ball->y) && (ball->y < firsty);

		if (flag > 1 && flag < 20)
		{
			if ((firsty - lasty) * cos(theta * PI / 180.0) - (firsty - lasty) + (ball->p_x)*(ball->p_x) / (2.0*grv) > 0.1)// 0.5mv^2 = gmh 에서 h구하는 방식
				theta += 0.5;
		}
		else if (flag > 20 && flag < 37)
			theta -= 0.5;
	}

	else if (ball->x > lastx) // 공이 판 오른쪽에서 부딪힐 때
	{
		x_collision = (ball->x - lastx - ball->radius < 0.5);
		y_collision = (lasty < ball->y) && (ball->y < firsty);


		if (flag > 1 && flag < 20)
		{
			if ((firsty - lasty) * cos(theta * PI / 180.0) - (firsty - lasty) + (ball->p_x)*(ball->p_x) / (2.0*grv) > 0.1) // 0.5mv^2 = gmh 에서 h구하는 방식
				theta -= 0.5;
		}
		else if (flag > 20 && flag < 37)
			theta += 0.5;
	}

	if (x_collision&&y_collision)
	{
		flag++;
		if (flag == 1)
			ball->p_x = ball->vx;
		ball->vx = 0;
		ball->vy = 0;
		ball->lasttime = realt;
	}
}

