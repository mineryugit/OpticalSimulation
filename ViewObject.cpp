#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "Mouse.h"
#include "OBJLoader.h"
using namespace std;

int WindowPositionX = 100;
int WindowPositionY = 100;
int WindowWidth = 512;
int WindowHeight = 512;
char WindowTitle[] = "Display OBJ file";
GLfloat lightColor[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[4] = { 0.0, 100.0, 0.0, 0.0 };
ViewCamera camera(5.0);
OBJMesh mesh;
bool wireframe_flag = false;

//
//　prototype
//
void Initialize();
void Display();
void Idle();
void Shutdown();
void Reshape(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void PassiveMotion(int x, int y);
void SetLighting();

//----------------------------------------------------------------------------------------------------
//　　main
//　　Desc : メインエントリポイント
//----------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if (argc != 3){
		std::cout << "Usage: <projection surface (.obj)> <optical path(.bin)>" << std::endl;
		//return 0;
	}

	std::cout << argc << endl;
	glutInit(&argc, argv);
	glutInitWindowPosition(WindowPositionX, WindowPositionY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(WindowTitle);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	Initialize(argv[1]);

	glutMainLoop();

	Shutdown();

	return 0;
}


//----------------------------------------------------------------------------------------------------
//　　SetLighting
//　　Desc : ライティング
//----------------------------------------------------------------------------------------------------
void SetLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
}

//----------------------------------------------------------------------------------------------------
//　　Initialize
//　　Desc : 初期化処理
//----------------------------------------------------------------------------------------------------
void Initialize(char* filename)
{
	//　塗りつぶし色
	glClearColor(0.3, 0.3, 1.0, 1.0);

	//　深度テスト有効
	glEnable(GL_DEPTH_TEST);

	//　スムーズシェイディング
	glShadeModel(GL_SMOOTH);

	//　ライティング
	SetLighting();

	//　メッシュファイルの読み込み(sample:"./../../data/model/dosei.obj")
	mesh.Load(filename);

	//　メッシュファイルの情報を表示
	mesh.Information();
}

//---------------------------------------------------------------------------------------------------
//　　Idle
//　　Desc : アイドリング時の処理
//---------------------------------------------------------------------------------------------------
void Idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------------
//　　Reshape
//　　Desc : サイズ変更
//---------------------------------------------------------------------------------------------------
void Reshape(int x, int y)
{
	WindowWidth = x;
	WindowHeight = y;
	if (WindowWidth < 1) WindowWidth = 1;
	if (WindowHeight < 1) WindowHeight = 1;
}


//---------------------------------------------------------------------------------------------------
//　　Display
//　　Desc : ウィンドウへの描画
//---------------------------------------------------------------------------------------------------
void Display()
{
	//　
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)WindowWidth / (double)WindowHeight, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	///
	glPushMatrix();

	//　視点の描画
	camera.Set();

	//　ライティング
	SetLighting();

	//　メッシュを描画
	//if (wireframe_flag) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh.Render(1.0);
	if (wireframe_flag) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	///
	glPopMatrix();

	/*
	//　補助軸の描画
	glPushMatrix();
	camera.RenderSubAxis(WindowWidth, WindowHeight);
	glPopMatrix();
	*/


	//　ダブルバッファ
	glutSwapBuffers();
}

//---------------------------------------------------------------------------------------------------
//　　Mouse
//　　Desc : マウス処理
//---------------------------------------------------------------------------------------------------
void Mouse(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//--------------------------------------------------------------------------------------------------
//　　Motion
//　　Desc : マウスドラッグ時
//--------------------------------------------------------------------------------------------------
void Motion(int x, int y)
{
	camera.MouseMotion(x, y);
}

//--------------------------------------------------------------------------------------------------
//　　PassiveMotion
//　　Desc : マウス移動時
//--------------------------------------------------------------------------------------------------
void PassiveMotion(int x, int y)
{
}

//--------------------------------------------------------------------------------------------------
//　　Keyboard
//　　Desc : キーボード処理
//--------------------------------------------------------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '\033':
		exit(0);
		break;

	case 'w':
		wireframe_flag ? wireframe_flag = false : wireframe_flag = true;
		cout << "Wire Frame : " << wireframe_flag << endl;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------
//　　Special
//　　Desc : 特殊キー処理
//--------------------------------------------------------------------------------------------------
void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		break;

	case GLUT_KEY_F2:
		break;

	case GLUT_KEY_F3:
		break;

	case GLUT_KEY_F4:
		break;

	case GLUT_KEY_F5:
		break;

	case GLUT_KEY_F6:
		break;

	case GLUT_KEY_F7:
		break;

	case GLUT_KEY_F8:
		break;

	case GLUT_KEY_F9:
		break;

	case GLUT_KEY_F10:
		break;

	case GLUT_KEY_F11:
		break;

	case GLUT_KEY_F12:
		break;

	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;

	case GLUT_KEY_PAGE_UP:
		break;

	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_HOME:
		break;

	case GLUT_KEY_END:
		break;

	case GLUT_KEY_INSERT:
		break;
	}
}

//----------------------------------------------------------------------------------------------------
//　　Shutdown
//　　Desc : 後片付け
//----------------------------------------------------------------------------------------------------
void Shutdown()
{
	//　メッシュファイルの後片付け
	mesh.Release();
}