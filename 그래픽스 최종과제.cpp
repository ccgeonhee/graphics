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
// ȭ�� ũ��
int Width, Height;

// �ڵ��� �̵� �� ��ġ
float C_MovePosX = 0.0f, C_MovePosZ = 0.0f;
// -Z ������ �ٶ󺸵��� -90�� ȸ�� �� ���¿��� ����
float C_RotateAngle = -90.0f;
float angle = 0.0f;

// �ڵ��� �ӵ�
float C_Speed = 0.0f;
// �ڵ��� ����
bool C_Accel = false;
void CarMoverTimer(int Value);

// ���� ����
void MyLightInit()
{
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };	//���� �ֺ��ݻ� (R, G, B, A) : ���� �����

	//0�� ���� Ư�� �⺻ 3����
	GLfloat light0_ambient[] = { 0.5, 0.5, 0.4, 1.0 };		// �ֺ���(R, G, B, A)
	GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };		// ������(R, G, B, A)
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };		// ��鱤(R, G, B, A)

	glShadeModel(GL_SMOOTH);	//���� ���̵� -> ���� ����ѹ��, �������ϸ� �÷� ���̵�
	glEnable(GL_DEPTH_TEST);	//���� ���� Ȱ��ȭ

	glEnable(GL_LIGHTING);		//���� Ȱ��ȭ

	//*
	glEnable(GL_LIGHT0);		//0�� ���� Ȱ��ȭ		-> ��ü
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);	//0�� ���� Ư���Ҵ�, �տ��� ������ ���� ���� ����
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	//*/



	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);		//�����ֺ��ݻ� 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);		//�������� -> ���������� ����? å�� ���� ����
}
void SetMyColorLigint(float r, float g, float b) //��ü Ư��
{
	//��ü Ư�� -> ��ü�� �� // �򰥸��� �ʰ� �ϱ����ؼ� ������ ���� ������� �θ� ��
	// => ���鿡 ���ؼ� �󸶸�ŭ ������ �� ������ ������ ���ִ°��̴�
	GLfloat material_ambient[] = { r, g, b, 1.0 };		// �ֺ���
	GLfloat material_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };		// ������ => ������ ����
	GLfloat material_specular[] = { 0.5, 1.0, 1.0, 1.0 };	// ��鱤
	GLfloat material_shininess[] = { 25.0 };

	//*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);     //��ü Ư���Ҵ�
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

// �ڵ��� ��ġ �ʱ�ȭ
void CarInitialize()
{
	C_MovePosX = 0.0f, C_MovePosZ = 0.0f;
	C_RotateAngle = -90.0f;
	angle = 0.0f;
	C_Speed = 0.0f;

	C_Accel = false;

	glutTimerFunc(30, CarMoverTimer, 1);
}
// ����� ������ ī�޶� �������� ������ �ؾ��Ѵ�.
void Init() {
	glClearColor(0.529412, 0.807843, 0.980392, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ������Ʈ ���
void DrawStreet(int front) {
	// ���̽��� �Ǵ� �� ���.
	glPushMatrix();
		SetMyColorLigint(0.3, 1.0, 0.3);
		glBegin(GL_QUADS);
			glVertex3f(100.0 * front, 0.0, 60.0 * front);
			glVertex3f(100.0 * front, 0.0, -100.0 * front);
			glVertex3f(-100.0 * front, 0.0, -100.0 * front);
			glVertex3f(-100.0 * front, 0.0, 60.0 * front);
		glEnd();
	glPopMatrix();

	// ���� ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// �� �׸���
			glVertex3f(-98.0 * front, 0.01, 60.0 * front);
			glVertex3f(-98.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// ���� �ι�° ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// �� �׸���
				glVertex3f(-68.0 * front, 0.01, 60.0 * front);
				glVertex3f(-68.0 * front, 0.01, -100.0 * front);
				glVertex3f(-70.0 * front, 0.01, -100.0 * front);
				glVertex3f(-70.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// ��� ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// �� �׸���
				glVertex3f(1.0 * front, 0.01, 60.0 * front);
				glVertex3f(1.0 * front, 0.01, -100.0 * front);
				glVertex3f(-1.0 * front, 0.01, -100.0 * front);
				glVertex3f(-1.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// ������ ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// �� �׸���
			glVertex3f(100.0 * front, 0.01, 60.0 * front);
			glVertex3f(100.0 * front, 0.01, -100.0 * front);
			glVertex3f(98.0 * front, 0.01, -100.0 * front);
			glVertex3f(98.0 * front, 0.01, 60.0 * front);
		glEnd();
	glPopMatrix();

	// �Ʒ� ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// �� �׸���
				glVertex3f(100.0 * front, 0.01, 60.0 * front);
				glVertex3f(100.0 * front, 0.01, 58.0 * front);
				glVertex3f(-100.0 * front, 0.01, 58.0 * front);
				glVertex3f(-100.0 * front, 0.01, 60.0 * front);
			glEnd();
	glPopMatrix();

	// ��� ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);					// �� �׸���
				glVertex3f(100.0 * front, 0.01, 1.0 * front);
				glVertex3f(100.0 * front, 0.01, -1.0 * front);
				glVertex3f(-100.0 * front, 0.01, -1.0 * front);
				glVertex3f(-100.0 * front, 0.01, 1.0 * front);
		glEnd();
	glPopMatrix;
	// �� ���η� ū��
	glPushMatrix();
		SetMyColorLigint(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);					// �� �׸���
			glVertex3f(100.0 * front, 0.01, -98.0 * front);
			glVertex3f(100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -100.0 * front);
			glVertex3f(-100.0 * front, 0.01, -98.0 * front);
		glEnd();
	glPopMatrix();
}
void DrawApartTypeA(int front)
{
	// ����Ʈ �������ing --------------------------------------------
	glPushMatrix();
		SetMyColorLigint(0.8, 0.8, 0.8);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(5.0, 8.0, 5.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawApartTypeB(int front)
{
	// ����Ʈ �������ing --------------------------------------------
	glPushMatrix();
		SetMyColorLigint(0.5, 0.5, 0.5);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(10.0, 15.0, 10.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawApartTypeC(int front)
{
	// ����Ʈ �������ing --------------------------------------------
	// �ǹ�
	glPushMatrix();
		SetMyColorLigint(1.0, 1.0, 1.0);
		glTranslatef(0.0 * front, 0.0, 0.0);
		glScalef(15.0, 6.0, 15.0);
		glutSolidCube(1.0);
	glPopMatrix();
}
void DrawCity(int front) {
	// �� ���� ����
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 3; j++)
		{
			glPushMatrix();
				glTranslatef((-94.0 + (j * 10.0)) * front, 0.0, -94.0 + (i * 10.0));
				DrawApartTypeA(front);
			glPopMatrix();
		}

	// �� ��� ����
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
				glTranslatef((-60.0 + (j * 17.0)) * front, 0.0, -90.0 + (i * 16.0));
				DrawApartTypeB(front);
			glPopMatrix();
		}

	// �� ������ ����

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

	// �Ʒ� ��� ����
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
// 4���� ����Ʈ ���� ������ �׸��� �׸�
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

// ����Ʈ 4���� ������ �����
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW); //CCW = �ݽð����
	glEnable(GL_CULL_FACE); //�Ѹ鸸 �׸�.
	glCullFace(GL_BACK);  //�ĸ� ����
	glEnable(GL_DEPTH_TEST); //DePTH_TEST �����ϰ���.

	GLfloat LightPosition0[] = { 0.0, 5.0, 0.0, 1.0 };	//0�� ������ġ
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);	//��ġ�� ����

	
	glBegin(GL_QUADS);					// �� �׸���
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

	glViewport(0, 140, 200, 280); //3    			-> leftSideMirrorView ���̵�̷��� �ڸ� ��.
	glPushMatrix();
		gluPerspective(30, (GLdouble)200 / (GLdouble)280, 1.0, 100.0); //����, ��Ⱦ��, near,far
		gluLookAt((C_MovePosX) * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle - 25) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle - 25) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();

	glViewport(1080, 140, 200, 280); //3    			-> RightSideMirrorView ���̵�̷��� �ڸ� ��.
	glPushMatrix();
		gluPerspective(30, (GLdouble)200 / (GLdouble)280, 1.0, 100.0); //����, ��Ⱦ��, near, far
		gluLookAt((C_MovePosX) * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle + 25) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle + 25) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();
	glPopMatrix();

	glViewport(200, 0, 880, 460); //3    			-> FrontView
	glPushMatrix();
		gluPerspective(30, (GLdouble)560 / (GLdouble)460, 1.0, 100.0); //����, ��Ⱦ��, near,far
		gluLookAt(C_MovePosX + cos((C_RotateAngle + angle) * pi / 180.0) * 15.0, 3.0, C_MovePosZ - sin((C_RotateAngle + angle) * pi / 180.0) * 15.0,					// ī�޶���ġ
		C_MovePosX, 0.0, C_MovePosZ,			// �ٶ󺸴� ��� ��ġ
		0.0, 1.0, 0.0);							// up ����
		DrawScene(1);
	glPopMatrix();

	glViewport(420, 470, 400, 160); //3    			-> BackView �ڸ� ��.
	glPushMatrix();
		gluPerspective(30, (GLdouble)400 / (GLdouble)160, 1.0, 100.0); //����, ��Ⱦ��, near,far
		gluLookAt(C_MovePosX * -1, 0.5, C_MovePosZ,
		(C_MovePosX + cos((C_RotateAngle + angle) * pi / 180.0)) * -1, 0.4, C_MovePosZ - sin((C_RotateAngle + angle) * pi / 180.0),
		0.0, 1.0, 0.0);
		DrawScene(-1);
	glPopMatrix();

	glutSwapBuffers();
}

// ȭ��ǥ �Է�
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

// ������ â ����� ȣ��
void MyReshape(int w, int h) {
	Width = w;   Height = h;
}

void CarMoverTimer(int Value) {
	if (C_Accel) //�Ǽ� ������
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

	printf("�ڵ��� ��ġ : %f, %f, %f\n", C_MovePosX, 0.3, C_MovePosZ);

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

	// ���� ����
	Init();

	// ���� ����
	MyLightInit();

	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(SpecialInput);

	glutReshapeFunc(MyReshape);

	glutIdleFunc(MyIdle);
	glutTimerFunc(30, CarMoverTimer, 1);

	glutMainLoop();
	return 0;
}
