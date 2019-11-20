#pragma once
#include "Init.h"
#define PI 3.14159265359
#define grv 9.80665

float realt; // ���� �ð�

class Ball;

class Plate
{
public:


	Plate()
		:firstx(0.0f), firsty(0.0f), lastx(0.0f), lasty(0.0f), theta(0.0f), flag(0) {}

	Plate(float firstx, float firsty, float lastx, float lasty, float theta)
		:firstx(firstx), firsty(firsty), lastx(lastx), lasty(lasty), theta(theta), flag(0) {} // �� ���� ù��° ������ x,y, ����° ������ x,y , ȸ������

	void draw(GLuint);
	void draw();
	float calculateEndx(); // ���ϴ� ������ŭ ȸ�� �� ���� lastx��
	float calculateEndy(); // ���ϴ� ������ŭ ȸ�� �� ���� lasty��

	float firstx;
	float firsty;
	float lastx;
	float lasty;
	float theta;
	void rotate(Ball *b);
	int flag; //���� �ε����� -> 1 

};


void Plate::draw(GLuint tex)
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glPushMatrix();
	glTranslatef(firstx, firsty, 0); //3.����ġ
	glRotatef(theta, 0.0f, 0.0f, 1.0f); //2. ���ϴ� ��ŭ ȸ��
	glTranslatef(-firstx, -firsty, 0); //1.���� �������� ��������

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
	glPopMatrix(); // �ڿ� ���� ���ְ� �����·�
}
void Plate::draw()
{
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	glPushMatrix();
	glTranslatef(firstx, firsty, 0); //3.����ġ
	glRotatef(theta, 0.0f, 0.0f, 1.0f); //2. ���ϴ� ��ŭ ȸ��
	glTranslatef(-firstx, -firsty, 0); //1.���� �������� ��������

									   //glColor3f(0.870588, 0.721569, 0.529412);
	glBegin(GL_QUADS);
	glVertex3f(firstx, firsty, 0);
	glVertex3f(lastx, firsty, 0);
	glVertex3f(lastx, lasty, 0);
	glVertex3f(firstx, lasty, 0);
	glEnd();
	glPopMatrix(); // �ڿ� ���� ���ְ� �����·�
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
	bool BallPlateCollision(Plate p); //�� p�� �浹�ϸ� true �ƴϸ� false ��ȯ
	void velocity(Plate p, float a);
	bool BallBallCollision(Ball b);

	float x;
	float y;
	float radius;

	float acc; //���ӵ�, m=1�̹Ƿ� �ᱹ ���� �޴� ��
	float accx; //x���� ���ӵ�
	float accy; //y���� ���ӵ�
	float lastacc; //���� �޴� ���� ���ϱ� �� ���ӵ�

	float vx; //x���� �ӵ�
	float vy; //y���� �ӵ�
	float lastvx; //���ӵ��� ���ϱ� �� x �ӵ�
	float lastvy; //���ӵ��� ���ϱ� �� y �ӵ�

	float lasttime;
	int i; //�浹�ϴ� �� number

	float p_x; // ���̶� �浹�Ҷ� �ӵ�
};

void Ball::draw()
{
	glDisable(GL_TEXTURE_2D);
	Lighting();
	glPushMatrix();
	glTranslatef(x, y, 0); //�� ��ǥ��ŭ �̵�
	glutSolidSphere(radius, 50, 50);
	glPopMatrix(); // �ڿ� ���� ���ְ� �����·�
	glDisable(GL_LIGHTING);

}

bool Ball::BallPlateCollision(Plate p)
{
	bool x_collision = (p.firstx - 0.5 <= x) && (x <= p.calculateEndx() + (p.firsty - p.lasty)*cos(p.theta*PI / 180.0)); // ���� x ��ǥ�� ���� ���ʳ����� �����ʳ� ����. 0.5�� ���� ������
	bool y_collision = y - (((x - p.firstx) / (p.calculateEndx() - p.firstx))*(p.calculateEndy() - p.firsty) + p.firsty) < radius + 0.5; // x -x1 : x2- x1 = y- y1 : y2 - y1
	return (x_collision) && (y_collision);
}


void Ball::velocity(Plate p, float a) //���� ������ �޴´�
{

	if (this->BallPlateCollision(p)) // ���� ������ ���� ��
	{

		if (p.theta == 0)
		{
			acc = 0;
			accx = acc * cos(-p.theta * PI / 180.0);
			accy = acc * sin(-p.theta * PI / 180.0);

		}
		else if (p.theta < 0) //�� ȸ�������� �»���
		{
			acc = grv * sin(-p.theta * PI / 180.0); //���ӵ��� g*sin(theta)
			accx = acc * cos(-p.theta * PI / 180.0);
			accy = acc * sin(-p.theta * PI / 180.0);

		}
		else //�� ȸ�������� �����
		{
			acc = grv * sin(p.theta * PI / 180.0);
			accx = acc * -cos(p.theta * PI / 180.0);
			accy = acc * sin(p.theta * PI / 180.0);

		}

	}

	else //���� ������ ���� ��
	{
		acc = grv; //���ӵ��� g
		accy = acc;
		accx = 0;
	}

	if (acc < lastacc) // �ǿ� ���� ��
	{
		lasttime = realt; // �׶� �ð� ����
		lastvx = 0; // �׶� �ӵ� ����
		lastvy = 0;
	}
	else if (acc > lastacc) //�ǿ��� ��������
	{
		lasttime = realt; // �׶� �ð� ����
		lastvx = vx; // �׶� �ӵ� ����
		lastvy = vy;
		i++;
	}

	realt = (float)(clock() - start) / CLOCKS_PER_SEC; //���� �ð� ���
	vx = lastvx + accx * (realt - lasttime) + a; // �׻� ��ӵ� ��̹Ƿ� a = (v1-v2)/(t1-t2) ���� v1�� ���Ѱ�. v1: ���� �ӵ�, v2: ���ӵ��� ���ϱ��� ������ �ӵ�, t1: ���� �ð�, t2: ���ӵ��� ���� �ð�
	vy = lastvy + accy * (realt - lasttime);


	if (this->BallPlateCollision(p))
	{
		x += 0.1 * vx; //0.01�ʸ��� x ��ǥ ����
		y = (((x - p.firstx) / (p.calculateEndx() - p.firstx))*(p.calculateEndy() - p.firsty) + p.firsty) + radius; // ������ �������� x���� ����ϰ� y�� �� x���� �´� ��ġ��
	}
	else
	{
		x += 0.1 * vx; //0.01�ʸ��� x ��ǥ ����
		y -= 0.1 * vy; //0.01�ʸ��� y��ǥ ����
	}

	lastacc = acc; //���ӵ� �Ź� ���� - �޶������� Ȯ���Ϸ���
}

bool Ball::BallBallCollision(Ball b)
{
	return (fabsf((x - b.x)*(x - b.x) + (y - b.y)*(y - b.y) - (radius + b.radius)*(radius + b.radius)) < 10);
}


void Plate::rotate(Ball *b) // ���� �ε����� �� �� ȸ���ϰ��ϴ� �Լ�
{
	Ball * ball;
	ball = (Ball *)b;

	bool x_collision = false;
	bool y_collision = false;

	if (ball->x < firstx) //���� �� ���ʿ��� �ε��� �� 
	{
		x_collision = (firstx - ball->x - ball->radius< 0.5);
		y_collision = (lasty < ball->y) && (ball->y < firsty);

		if (flag > 1 && flag < 20)
		{
			if ((firsty - lasty) * cos(theta * PI / 180.0) - (firsty - lasty) + (ball->p_x)*(ball->p_x) / (2.0*grv) > 0.1)// 0.5mv^2 = gmh ���� h���ϴ� ���
				theta += 0.5;
		}
		else if (flag > 20 && flag < 37)
			theta -= 0.5;
	}

	else if (ball->x > lastx) // ���� �� �����ʿ��� �ε��� ��
	{
		x_collision = (ball->x - lastx - ball->radius < 0.5);
		y_collision = (lasty < ball->y) && (ball->y < firsty);


		if (flag > 1 && flag < 20)
		{
			if ((firsty - lasty) * cos(theta * PI / 180.0) - (firsty - lasty) + (ball->p_x)*(ball->p_x) / (2.0*grv) > 0.1) // 0.5mv^2 = gmh ���� h���ϴ� ���
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

