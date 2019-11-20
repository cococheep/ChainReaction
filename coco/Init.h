#include "MATRIX.h"

#include <glut.h>
#include <time.h>

double Lookat[9];//lookat 함수 parameter 값
clock_t start; // 프로그램 시작하면 start time 시작
clock_t start_cam;

void initLookat(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz)
{
	Lookat[0] = eyex;
	Lookat[1] = eyey;
	Lookat[2] = eyez;
	Lookat[3] = centerx;
	Lookat[4] = centery;
	Lookat[5] = centerz;
	Lookat[6] = upx;
	Lookat[7] = upy;
	Lookat[8] = upz;

}

void lighting(float MatColorR, float MatColorG, float MatColorB, float MatOpacity) {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light_pos[] = { 10.0f, 10.0f, 10.0f, 1.0f };
	float light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	float noMat[] = { MatColorR, MatColorG, MatColorB, 1.0f };
	float matAmb[] = { 0.3f, 0.3f, 0.3f, MatOpacity };
	float matDif[] = { 1.0f, 1.0f, 1.0f, MatOpacity };
	float matSpec[] = { 1.0f, 1.0f, 1.0f, MatOpacity };
	float matShininess = 100.0f;

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
}

void Lighting(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING); //광원 활성화
	glEnable(GL_LIGHT0);	//첫번째 광원 활성화
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	float light_pos[] = { 25.0, 50.0, 50.0, 0.0 }; //빛의 위치
	float light_dir[] = { 0, -1, -1 }; //빛의 방향 : 광원의 종류에 따라 필요없을 수도
	float light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f}; //주변 밝기 (rgba)
	float light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //주변 밝기 (rgba)
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //Highlight 광원 (rgba)

	float matShininess = 200;
	float noMat[] = { 0, 0, 0, 1 };
	float matSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); //GL_LIGHT0의 GL_AMBIENT를 light_ambient로 설정
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);	
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void Initialize(int argc, char** argv)
{
	glutInit(&argc, argv);

	// 윈도우 생성
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1228, 690);
	glutCreateWindow("TEAM COCO");

	// depth test
	glEnable(GL_DEPTH_TEST);

	// 배경색 설정
	glClearColor(1, 1, 1, 1.0f);

	glDisable(GL_CULL_FACE);

	//Lighting();

	start = clock(); // time 시작
	start_cam = clock();
}
