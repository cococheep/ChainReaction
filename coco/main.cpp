#pragma warning(disable:4819)

#include "VECTOR.h"
#include "Face.h"
#include "Mesh.h"
#include "object.h"
#include <stdlib.h>


#include <GL.H>
#include <glu_.h>
#include <glut.h>
#include <GLAUX.H>
#include <glext.h>
#include <GLU.H>
#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <windows.h>




using namespace std;
#define meshnum 1


Plate p[30];
Ball b[20];
int coll;
float cam[2];
int mv;

float realt_cam;
float theta[4];
GLuint tex[4];   // 6���� �ؽ��ĸ� ����ϹǷ� 6���� cell�� ���� �迭 ����


AUX_RGBImageRec *LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;
	if (!filename) return NULL;

	hFile = fopen(filename, "r");
	if (hFile) {
		fclose(hFile);
		return auxDIBImageLoad(filename);
	}

	return NULL;
}

void InitGL(GLvoid) {
	AUX_RGBImageRec *texRec[4]; // 6���� �ؽ��� ���� �ҽ��� ������ �ϱ⿡ 6���� cell�� ���� �迭 ����
	memset(texRec, 0, sizeof(void *) * 4);   // �޸� ���� 0���� ����� �ڵ�

	if ((texRec[0] = LoadBMPFile((char *)"./image_plate.bmp")) &&        // �Ǵ�� text
		(texRec[1] = LoadBMPFile((char *)"./image_background.bmp")) &&   // tex[1]�� ��� text img8
		(texRec[2] = LoadBMPFile((char *)"./image_box.bmp")) &&         // tex[2]�� ���� �����Ǵ� �� 
		(texRec[3] = LoadBMPFile((char *)"./image_finish.bmp"))) {      // ������ text
		glGenTextures(4, tex); // ��� �ؽ��� ���� �ҽ� ����, ���� ù��° �ؽ��� ���� �ҽ��� �ĺ����� �ּҰ�
		for (int i = 0; i<4; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);   // �ؽ��� 2����,������ ������ �ؽ��� ������ �ĺ���
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 1��������,2��������
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGB,
				texRec[i]->sizeX,
				texRec[i]->sizeY,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				texRec[i]->data);
		}
	}

	for (int i = 0; i<4; i++) {
		if (texRec[i])
		{
			if (texRec[i]->data) free(texRec[i]->data); // �ʿ�ġ �ʴ� �Ҵ�� �޸� ���� �ڵ�
			free(texRec[i]);
		}
	}

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void draw_Ground() { // ��� texture mapping
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 11; j++) {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-900 + 100 * i, 500 - 100 * j, -5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-800 + 100 * i, 500 - 100 * j, -5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-800 + 100 * i, 400 - 100 * j, -5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-900 + 100 * i, 400 - 100 * j, -5);
			glEnd();
		}
	}
}





float rx(float x, float y)
{
	return cos(theta[0] * PI / 180.0)*(x + 41) - sin(theta[0] * PI / 180.0)*(y + 450) - 41;
}

float ry(float x, float y)
{
	return sin(theta[0] * PI / 180.0)*(x + 41) + cos(theta[0] * PI / 180.0)*(y + 450) - 450;
}

void Mouse(int mouse_event, int state, int x, int y)
{
	glutPostRedisplay();
}

void Motion(int x, int y)
{
	glutPostRedisplay();
}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunc, 1);
}



void RenderMesh()
{

	draw_Ground();


	float sint = sin(-theta[0] * PI / 180.0);
	float cost = cos(-theta[0] * PI / 180.0);

	p[5] = Plate(-41, -450, 139, -460, theta[0]); //���� ��

												  //������ �ٱ���
	p[10] = Plate(rx(113, -450) + 30 * sint, ry(113, -450) + 30 * cost, rx(113, -450) + 30 * sint + 5, ry(113, -450) + 30 * cost - 30, theta[0]);
	p[11] = Plate(rx(134, -450) + 30 * sint, ry(134, -450) + 30 * cost, rx(134, -450) + 30 * sint + 5, ry(134, -450) + 30 * cost - 30, theta[0]);

	//������ �浹���� ��
	p[12] = Plate(-41 + 30 * sint, -450 + 30 * cost, -41 + 30 * sint + 180, -450 + 30 * cost - 2, theta[0]);
	p[13] = Plate(-41 + 20 * sint, -450 + 20 * cost, -41 + 20 * sint + 180, -450 + 20 * cost - 2, theta[0]);
	p[14] = Plate(-41 + 10 * sint, -450 + 10 * cost, -41 + 10 * sint + 180, -450 + 10 * cost - 2, theta[0]);

	//���� �ٱ���
	p[15] = Plate(-41 + 30 * sint, -450 + 30 * cost, -41 + 30 * sint + 5, -450 + 30 * cost - 30, theta[0]);
	p[16] = Plate(rx(-20, -450) + 30 * sint, ry(-20, -450) + 30 * cost, rx(-20, -450) + 30 * sint + 5, ry(-20, -450) + 30 * cost - 30, theta[0]);

	p[25] = Plate(590 + mv, -350, 740 + mv, -360, 0); //���� ��

													  //Lighting();
	p[0].draw(tex[0]);
	p[1].draw(tex[0]);
	p[2].draw(tex[0]);
	p[3].draw(tex[0]);
	p[4].draw(tex[0]);
	p[5].draw(tex[0]);

	p[8].draw(tex[0]);

	p[10].draw(tex[0]);
	p[11].draw(tex[0]);
	p[15].draw(tex[0]);
	p[16].draw(tex[0]);

	p[6].draw(tex[0]);
	p[19].draw(tex[0]);
	p[21].draw(tex[0]);
	p[25].draw(tex[0]);


	glColor3f(1, 1, 1);
	p[26].draw(tex[3]);

	glColor3f(0, 0, 1);
	p[18].draw();
	p[20].draw();

	glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
	glTranslatef(0, 0, 20);
	p[9].draw(tex[2]);
	p[24].draw(tex[2]);
	p[27].draw(tex[2]);
	glTranslatef(0, 0, -20);

	glColor3f(1.0, 1, 1);
	p[17].draw(tex[0]);
	p[7].draw(tex[0]);
	glColor3f(0.7, 0.7, 0.7);
	b[5].draw(); // ū��
	b[6].draw(); //���۰�

	printf("%f\n",b[1].vx);

	if (b[5].BallBallCollision(b[6]))  //ó�� ���� ���̶� ū���̶� �浹
	{
		coll = 1;
	}

	if (!b[6].BallPlateCollision(p[7]))
		b[6].velocity(p[7], 9);

	if (coll == 0) // �浹�� �
	{
		b[6].velocity(p[7], 4.5);
		b[5].velocity(p[7], 0);
	}
	else //�浹 �� �
	{
		if (b[5].x < -300)
		{
			b[6].velocity(p[7], 0); // �浹 �� ���� �� ����
			b[5].velocity(p[7], 2); // �浹 �� ū �� �

		}
		else
		{
			if (b[5].y < 120)
			{
				coll++;
				if (coll == 2)
				{
					start = clock();
				}

				b[5].vy = 0;          // ū �� ���ھȿ� ���� ����
				b[5].vx = 0;

				b[0].velocity(p[b[0].i], 0);
				b[1].velocity(p[b[1].i], 0);   //ù��° �豸�� �

				if (b[2].i != 5) {
					b[2].velocity(p[b[2].i], 0);   //�ι�° �豸�� �
				}
				else {
					b[2].velocity(p[14], 0);   // ������ ������ ���̶��� collision ���ϰ� ���̶� ��
				}

				if (b[3].i != 5) {
					b[3].velocity(p[b[3].i], 0);   // ����° �豸��
				}
				else {
					b[3].velocity(p[13], 0);

				}

				if (b[4].i != 5) {
					b[4].velocity(p[b[4].i], 0);   // �׹�° �豸��
				}
				else {
					if (b[4].BallPlateCollision(p[12]))
					{
						//theta[0] -= 1;         //�� ����̱�
						b[4].velocity(p[12], 5);
						b[4].y += 5;
					}
					else
						b[4].velocity(p[12], 0);
				}

				for (int j = 1; j < 4; j++)
				{
					if (b[j].y < 100)
						b[j].draw();         // �豸������ ���� �Ʒ������� ���̱� ������
				}
				if (b[4].y < 100) {
					glColor3f(1, 0, 0);
					b[4].draw();
				}
				glColor3f(0.8, 0.8, 0.8);
				if (b[1].y < -180)            // �豸������ ������� �ؿ� ���� ������ �豸���� ����
				{
					b[7].draw();
					if (b[7].y > -400)
						b[7].velocity(p[8], 0);
					else
						b[7].velocity(p[5], 0);
				}
				if (b[2].y < -180)
				{
					b[8].draw();
					if (b[8].y > -400)
						b[8].velocity(p[8], 0);
					else
						b[8].velocity(p[14], 0);
				}

				if (b[3].y < -180)
				{
					b[9].draw();
					if (b[9].y > -400)
						b[9].velocity(p[8], 0);
					else
						b[9].velocity(p[13], 0);
				}

				glColor3f(0.7, 0.7, 0.7);
				b[10].draw(); // ball �׸���
				b[11].draw();
				b[12].draw();
				b[13].draw();
				b[14].draw();
				b[15].draw();
				b[16].draw();
				b[17].draw();

				//ƼŰŸī ����
				if (b[11].x > 400)
					b[11].velocity(p[18], 0);
				else
					b[11].velocity(p[19], 0);

				if (b[12].x < 300)
					b[12].velocity(p[20], 0);
				else
					b[12].velocity(p[21], 0);

				p[18].rotate(&b[4]);
				p[20].rotate(&b[11]);

				if (b[12].x > 590)
					mv += 1;
				b[13].velocity(p[25], 0);
				b[14].velocity(p[25], 0);
				b[15].velocity(p[25], 0);
				b[16].velocity(p[25], 0);
				b[17].velocity(p[25], 0);
			}
			else
				b[5].velocity(p[17], 0); //ū�� ���� ��������
		}

	}

	realt_cam = (float)(clock() - start_cam) / CLOCKS_PER_SEC;
	initLookat(-600 + cam[1], 300 + cam[0], 400, -600 + cam[1], 300 + cam[0], 0, 0.0f, 1.0f, 0.0f);
	if (realt_cam < 8.5) {
		cam[1] += 0.78;
		cam[0] -= 0.05;
	}
	else if (realt_cam < 22)
		cam[0] -= 0.5;
	else if (realt_cam > 22 && realt_cam < 25);
	else if (realt_cam < 33)
		cam[1] += 0.7;
	else if (realt_cam < 42);
	else if (realt_cam < 45)
		cam[1] += 0.5;
}

void Rendering(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Lookat[0], Lookat[1], Lookat[2], Lookat[3], Lookat[4], Lookat[5], Lookat[6], Lookat[7], Lookat[8]);

	RenderMesh();

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// ����Ʈ ����
	glViewport(0, 0, w, h);

	// ���� ���� ���
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 1500);

	// �𵨺� ��Ʈ���� �ʱ�ȭ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	glutPostRedisplay();
}

void EventHandlingAndLoop()
{
	glutKeyboardFunc(Keyboard);  // Ű���� �Է� ó�� Callback �Լ� ���
	glutDisplayFunc(Rendering);  // ��ȯ�� ���� ���� Rendering Callback �Լ� ���
	glutReshapeFunc(Reshape);    // ������ â ũ�Ⱑ �ٲ������ ȣ��Ǵ� Callback �Լ� ���
	glutMouseFunc(Mouse);        // ���콺 Ŭ�� ó�� Callback �Լ� ���
	glutMotionFunc(Motion);      // ���콺 ������ ó�� Callback �Լ� ���
	glutTimerFunc(100, TimerFunc, 1);
	glutMainLoop(); // ��ϵ� callback �Լ��� �ݺ��Ͽ� ȣ��
}


int main(int argc, char** argv)
{
	Initialize(argc, argv);
	InitGL();

	initLookat(0, 0, 1200, 0, 0, 0, 0.0f, 1.0f, 0.0f);// lookat �Լ� ������ ����

	for (int i = 0; i < 10; i++)
		coll = 0;
	for (int j = 0; j < 4; j++)
		theta[j] = 0;
	mv = 0;


	p[0] = Plate(0.0, 30.0, 100.0, 20.0, -30); // �� ����
	p[1] = Plate(50.0, -100, 250, -110, 30);
	p[2] = Plate(-50.0, -100.0, 50, -110, -30);
	p[3] = Plate(50.0, -200, 150, -210, 30);
	p[4] = Plate(-50.0, -200, 50, -210, -30);

	p[9] = Plate(-30, 70, 40, 140, 0);  //�� ������
	p[17] = Plate(-300, 320, -100, 310, -27); //ū�� �������� ��
	p[7] = Plate(-500, 320, -300, 310, 0); // ó�� ����
	p[8] = Plate(100.0, -250, 200, -260, 30); // ��Ī ��

	//���� �豸����
	b[0] = Ball(0, 50, 5.0); //�Ⱦ��µ� �־���ϴ� ����
	b[1] = Ball(0, 150, 5.0);
	b[2] = Ball(0, 300, 5.0);
	b[3] = Ball(0, 600, 5.0);
	b[4] = Ball(0, 1000, 5.0);

	b[5] = Ball(-350, 335, 15.0); //ū ��
	b[6] = Ball(-670, 250, 5.0);  //���� ��
	b[6].lastvy = -15;
	b[6].lastacc = grv;

	// ������ �豸����
	b[7] = Ball(180, -180, 5.0);
	b[8] = Ball(180, -180, 5.0);
	b[9] = Ball(180, -180, 5.0);

	// ƽ��ƽ�� �� ����
	//b[10] = Ball(0 + 300, 10 - 400, 5.0);
	b[11] = Ball(102.5 + 300, 60 - 400, 5.0);
	b[12] = Ball(-4.5 + 300, 110 - 400, 5.0);

	// ƽ��ƽ�� �� ����
	p[6] = Plate(-150.0 + 300, 0.0 - 450, 110.0 + 300, -10.0 - 450, -10);
	p[19] = Plate(0 + 300, 30 - 400, 100 + 300, 20 - 400, 10);
	p[21] = Plate(0 + 300, 90 - 400, 300 + 300, 80 - 400, -10);
	p[18] = Plate(100 + 300, 50 - 400, 105 + 300, -100 - 400, theta[1]);
	p[20] = Plate(-7 + 300, 95 - 400, -2 + 300, 15 - 400, theta[2]);

	p[24] = Plate(-700, 230, -650, 210, 50); //����

	//���� 
	b[13] = Ball(605, -335, 15.0);
	b[14] = Ball(635, -335, 15.0);
	b[15] = Ball(665, -335, 15.0);
	b[16] = Ball(695, -335, 15.0);
	b[17] = Ball(725, -335, 15.0);
	p[26] = Plate(590, -320, 740, -350, 0);

	//�ڽ� �ϳ�
	p[27] = Plate(160, -145, 200, -185, 0);

	EventHandlingAndLoop();

	return 0;
}

