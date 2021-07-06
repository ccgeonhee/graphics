#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
double pi = 3.1415926535897932384626433832795028841;
// 화면 크기
int Width, Height;

// 자동차 이동 한 위치
float C_MovePosX = 0.0f, C_MovePosZ = 0.0f;
// -Z 방향을 바라보도록 -90도 회전 한 상태에서 시작
float C_RotateAngle = -90.0f;
float angle = 0.0f;

// 자동차 속도
float C_Speed = 0.0f;
// 자동차 엑셀
bool C_Accel = false;
void CarMoverTimer(int Value);

// 조명 설정
void MyLightInit()
{
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };	//전역 주변반사 (R, G, B, A) : 현재 백색광

	//0번 광원 특성 기본 3가지
	GLfloat light0_ambient[] = { 0.5, 0.5, 0.4, 1.0 };		// 주변광(R, G, B, A)
	GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };		// 직접광(R, G, B, A)
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };		// 경면광(R, G, B, A)

	glShadeModel(GL_SMOOTH);	//구로 셰이딩 -> 보간 사용한방법, 설정안하면 플랫 셰이딩
	glEnable(GL_DEPTH_TEST);	//깊이 버퍼 활성화

	glEnable(GL_LIGHTING);		//조명 활성화

	//*
	glEnable(GL_LIGHT0);		//0번 광원 활성화		-> 전체
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);	//0번 광원 특성할당, 앞에서 지정해 놓은 값을 지정
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	//*/



	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);		//전역주변반사 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);		//근접시점 -> 가까운곳에서 본다? 책에 설명 있음
}
void SetMyColorLigint(float r, float g, float b) //물체 특성
{
	//물체 특성 -> 물체의 색 // 헷갈리지 않게 하기위해선 조명의 색을 백색으로 두면 됌
	// => 빛들에 대해서 얼마만큼 반응을 할 것인지 결정을 해주는것이다
	GLfloat material_ambient[] = { r, g, b, 1.0 };		// 주변광
	GLfloat material_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };		// 직접광 => 반응할 정도
	GLfloat material_specular[] = { 0.5, 1.0, 1.0, 1.0 };	// 경면광
	GLfloat material_shininess[] = { 25.0 };

	//*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);     //물체 특성할당
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

// 자동차 위치 초기화
void CarInitialize()
{
	C_MovePosX = 0.0f, C_MovePosZ = 0.0f;
	C_RotateAngle = -90.0f;
	angle = 0.0f;
	C_Speed = 0.0f;

	C_Accel = false;

	glutTimerFunc(30, CarMoverTimer, 1);
}
// 무대는 무조건 카메라를 기준으로 생각을 해야한다.
void Init() {
	glClearColor(0.529412, 0.807843, 0.980392, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// 오브젝트 출력
void DrawStreet(int front) {
	// 베이스가 되는 판 출력.
	glPushMatrix();
		SetMyColorLigint(0.3, 1.0, 0.3);
		glBegin(GL_QUADS);
			glVertex3f(100.0 * front, 0.0, 60.0 * front);
			glVertex3f(100.0 * front, 0.0, -100.0 * front);
			glVertex3f(-100.0 * front, 0.0, -100.0 * front);
			glVertex3f(-100.0 * front, 0.0, 60.0 * front);
		glEnd();
	glPopMatrix();

	// 왼쪽 세로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// 길 그리기
			glVertex3f(-98.0 * front, 0.01, 60.0 * front);
			glVertex3f(-98.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// 왼쪽 두번째 세로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// 길 그리기
				glVertex3f(-68.0 * front, 0.01, 60.0 * front);
				glVertex3f(-68.0 * front, 0.01, -100.0 * front);
				glVertex3f(-70.0 * front, 0.01, -100.0 * front);
				glVertex3f(-70.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// 가운데 세로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// 길 그리기
				glVertex3f(1.0 * front, 0.01, 60.0 * front);
				glVertex3f(1.0 * front, 0.01, -100.0 * front);
				glVertex3f(-1.0 * front, 0.01, -100.0 * front);
				glVertex3f(-1.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// 오른쪽 세로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// 길 그리기
			glVertex3f(100.0 * front, 0.01, 60.0 * front);
			glVertex3f(100.0 * front, 0.01, -100.0 * front);
			glVertex3f(98.0 * front, 0.01, -100.0 * front);
			glVertex3f(98.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// 아래 가로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// 길 그리기
				glVertex3f(100.0 * front, 0.01, 60.0 * front);
				glVertex3f(100.0 * front, 0.01, 58.0 * front);
				glVertex3f(-100.0 * front, 0.01, 58.0 * front);
				glVertex3f(-100.0 * front, 0.01, 60.0 * front);
			glEnd();
	glPopMatrix();

	// 가운데 가로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// 길 그리기
				glVertex3f(100.0 * front, 0.01, 1.0 * front);
				glVertex3f(100.0 * front, 0.01, -1.0 * front);
				glVertex3f(-100.0 * front, 0.01, -1.0 * front);
				glVertex3f(-100.0 * front, 0.01, 1.0 * front);
		glEnd();
	glPopMatrix;
	// 위 가로로 큰길
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// 길 그리기
			glVertex3f(100.0 * front, 0.01, -98.0 * front);
			glVertex3f(100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -98.0 * front);
		glEnd();
	glPopMatrix();
}
void DrawApartTypeA(int front)
{
	// 아파트 만드는중ing --------------------------------------------
	glPushMatrix();
		SetMyColorLigint(0.8, 0.8, 0.8);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(5.0, 8.0, 5.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawApartTypeB(int front)
{
	// 아파트 만드는중ing --------------------------------------------
	glPushMatrix();
		SetMyColorLigint(0.5, 0.5, 0.5);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(10.0, 15.0, 10.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawApartTypeC(int front)
{
	// 아파트 만드는중ing --------------------------------------------
	// 건물
	glPushMatrix();
		SetMyColorLigint(1.0, 1.0, 1.0);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(15.0, 6.0, 15.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawCity(int front) {
	// 위 왼쪽 동네
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 3; j++)
		{
			glPushMatrix();
				glTranslatef((-94.0 + (j * 10.0)) * front, 0.0, -94.0 + (i * 10.0));
				DrawApartTypeA(front);
			glPopMatrix();
		}

	// 위 가운데 동네
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
				glTranslatef((-60.0 + (j * 17.0)) * front, 0.0, -90.0 + (i * 16.0));
				DrawApartTypeB(front);
			glPopMatrix();
		}

	// 위 오른쪽 동네

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			glPushMatrix();
				glTranslatef((10.0 + (j * 19.5)) * front, 0.0, -90.0 + (i * 20.0));
				DrawApartTypeC(front);
			glPopMatrix();
		}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 1; j++)
		{
			glPushMatrix();
				glTranslatef((-92.0 + (j * 15.0)) * front, 0.0, 8.0 + (i * 14.0));
				DrawApartTypeB(front);
			glPopMatrix();
		}

	// 아래 가운데 동네
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			glPushMatrix();
				glTranslatef((-56.0 + (j * 21.5)) * front, 0.0, 9.0 + (i * 19.5));
				DrawApartTypeC(front);
			glPopMatrix();
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 7; j++)
		{
			glPushMatrix();
				glTranslatef((6.0 + (j * 9.5)) * front, 0.0, 6.0 + (i * 9.5));
				DrawApartTypeA(front);
			glPopMatrix();
		}

}
// 4개의 뷰포트 전부 동일한 그림을 그림
void DrawScene(int front) {
	DrawStreet(front);
	DrawCity(front);

	glPushMatrix();
		glColor3f(0.3, 0.3, 0.3);
		SetMyColorLigint(0.3, 0.3, 0.3);
		glTranslated(C_MovePosX * front, 0.3, C_MovePosZ);
		glRotated(C_RotateAngle + angle, 0.0, 1.0, 0.0);
		glScalef(2.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.2, 0.2, 0.2);
		glTranslated(C_MovePosX * front, 0.5, C_MovePosZ);
		glRotated(C_RotateAngle + angle, 0.0, 1.0, 0.0);
		glScalef(1.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
}

// 뷰포트 4개로 나눠서 출력함
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW); //CCW = 반시계방향
	glEnable(GL_CULL_FACE); //겉면만 그림.
	glCullFace(GL_BACK);  //후면 제거
	glEnable(GL_DEPTH_TEST); //DePTH_TEST 가능하게함.

	GLfloat LightPosition0[] = { 0.0, 5.0, 0.0, 1.0 };	//0번 광원위치
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);	//위치성 광원

	
	glBegin(GL_QUADS);					// 길 그리기
		glVertex3f(1.0, 1.0, -0.1);
		glVertex3f(-1.0, 1.0, -0.1);
		glVertex3f(-1.0, -1.0, -0.1);
		glVertex3f(1.0, -1.0, -0.1);
	glEnd();
	
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslated(0.0, -0.5, 0.0);
		glEnd();
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glViewport(0, 140, 200, 280); //3    			-> leftSideMirrorView 사이드미러는 뒤를 봄.
	glPushMatrix();
		gluPerspective(30, (GLdouble)200 / (GLdouble)280, 1.0, 100.0); //각도, 종횡비, near,far
		gluLookAt((C_MovePosX) * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle - 25) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle - 25) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();

	glViewport(1080, 140, 200, 280); //3    			-> RightSideMirrorView 사이드미러는 뒤를 봄.
	glPushMatrix();
		gluPerspective(30, (GLdouble)200 / (GLdouble)280, 1.0, 100.0); //각도, 종횡비, near, far
		gluLookAt((C_MovePosX) * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle + 25) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle + 25) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();
	glPopMatrix();

	glViewport(200, 0, 880, 460); //3    			-> FrontView
	glPushMatrix();
		gluPerspective(30, (GLdouble)560 / (GLdouble)460, 1.0, 100.0); //각도, 종횡비, near,far
		gluLookAt(C_MovePosX + cos((C_RotateAngle + angle) * pi / 180.0) * 15.0, 3.0, C_MovePosZ - sin((C_RotateAngle + angle) * pi / 180.0) * 15.0,					// 카메라위치
		C_MovePosX, 0.0, C_MovePosZ,			// 바라보는 대상 위치
		0.0, 1.0, 0.0);							// up 벡터
		DrawScene(1);
	glPopMatrix();

	glViewport(420, 470, 400, 160); //3    			-> BackView 뒤를 봄.
	glPushMatrix();
		gluPerspective(30, (GLdouble)400 / (GLdouble)160, 1.0, 100.0); //각도, 종횡비, near,far
		gluLookAt(C_MovePosX * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();

	glutSwapBuffers();
}

// 화살표 입력
void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		C_Accel = false;
		C_Speed += 0.01f;
		break;
	case GLUT_KEY_UP:
		C_Accel = true;
		C_Speed -= 0.003f;
		break;
	case GLUT_KEY_RIGHT:
		angle -= 2.0f;
		break;
	case GLUT_KEY_LEFT:
		angle += 2.0f;
		break;
	}
	glutPostRedisplay();
}

// 윈도우 창 변경시 호출
void MyReshape(int w, int h) {
	Width = w;   Height = h;
}

void CarMoverTimer(int Value) {
	if (C_Accel) //악셀 밟으면
		C_Speed -= 0.001f;

	if (C_MovePosX <= -100.0f)
		C_MovePosX = 99.0f;
	else if (C_MovePosX >= 100.0f)
		C_MovePosX = -99.0f;

	if (C_MovePosZ <= -100.0f)
		C_MovePosZ = 59.0f;
	else if (C_MovePosZ >= 60.0f)
		C_MovePosZ = -99.0f;

	C_MovePosX += C_Speed * cos((C_RotateAngle + angle) * pi / 180.0);
	C_MovePosZ -= C_Speed * sin((C_RotateAngle + angle) * pi / 180.0);

	printf("자동차 위치 : %f, %f, %f\n", C_MovePosX, 0.3, C_MovePosZ);

	glutPostRedisplay();

	glutTimerFunc(30, CarMoverTimer, 1);

}
void MyIdle() {
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	Width = 1280; Height = 720;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2017E7005 KimGeonHee Simulator");

	// 무대 설정
	Init();

	// 조명 생성
	MyLightInit();

	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(SpecialInput);

	glutReshapeFunc(MyReshape);

	glutIdleFunc(MyIdle);
	glutTimerFunc(30, CarMoverTimer, 1);

	glutMainLoop();
	return 0;
}
