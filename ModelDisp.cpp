#include <stdio.h>
#include <miffy/gl/glutility.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <float.h>
#include <Windows.h>
#include <assert.h>
#define _USE_MATH_DEFINES 

#include <math.h>//クォータニオンに必要
#include <miffy/math/vec3.h>
#include <miffy/math/matrix.h>
#include <miffy/math/quaternion.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


using namespace std;
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp-vc120-mt.lib")

bool quaternionflag = false;
const int WINWIDTH = 600;
const int WINHEIGHT = 600;
float background[3] = { 0.0, 0.0, 0.0 };//背景色
float myrotate[3] = { 0.0, 0.0, 0.0 };
float translate_speed = 0.01;
float zoom_speed = 0.2;
float eyePoint[3] = { 0.0, 0.0, 1.0 };
miffy::quat<float> current_quaternion(1.0, 0.0, 0.0, 0.0);
miffy::quat<float> target_quaternion;
int previous_x;
int previous_y;
int mouse_down[2];

//回転マトリックス
miffy::mat4<float> rotate;

//平行移動
float trans[3] = { 0, 0, -4.0 };//最初のズーム位置を決める
float area = 1.0;
float offset = -0.5;
float offsetz = 10;
float aspectratio = 1.0;//y/xデータの縦横比
const string INPUTFILE = "bun_zipper_res4.ply";

//debug情報
//回転行列
miffy::mat4<float> rotation_matrix;
aiMesh* assimp_mesh;
aiVector3D* assimp_verts;
Assimp::Importer importer;
const aiScene* assimp_scene;


void InitGeometry(){
	assimp_scene = importer.ReadFile(INPUTFILE, 0);
	if (!assimp_scene){ printf("load error"); }
	printf("have camera?%d\n", assimp_scene->HasCameras());
	printf("have mesh?%d\n", assimp_scene->HasMeshes());
	printf("num of mesh:%d\n", assimp_scene->mNumMeshes);
	aiMesh* assimp_mesh = assimp_scene->mMeshes[0];
	printf("have born?%d\n", assimp_mesh->HasBones());
	printf("%d\n", assimp_mesh->HasFaces());
	printf("%d\n", assimp_mesh->mNumFaces);
	printf("%d\n", assimp_mesh->mFaces[0].mNumIndices);//3角系なら0,1,2..となっている
	printf("normal:%d\n", assimp_mesh->HasNormals());
	printf("%d\n", assimp_mesh->HasPositions());
	printf("%d\n", assimp_mesh->mNumVertices);//÷3して丁度みたいだ。
	assimp_verts = assimp_mesh->mVertices;

}
void InitCamera(){
	current_quaternion.toMat4(rotation_matrix.m);
}
void Destroy(){
}
void idle(){
	glutPostRedisplay();
}
void reshape(int w, int h)
{
	//射影変換行列
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (float)w / (float)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void Init(){
	InitGeometry();
	InitCamera();
}
void display(void)
{
	glClearColor(background[0], background[1], background[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();/* clear the matrix */
	gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//zが上であるが、そうするのが難しい。
	glPushMatrix();
	glTranslatef(trans[0], trans[1], trans[2]);//平行移動
	glMultMatrixf(rotation_matrix.m);//クォータニオンによる回転
	// DrawCoordinate(0.5);

	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i<assimp_mesh->mNumFaces; i++){
		glVertex3f(
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[0]].x,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[0]].y,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[0]].z);
		glVertex3f(
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[1]].x,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[1]].y,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[1]].z);
		glVertex3f(
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[2]].x,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[2]].y,
			assimp_mesh->mVertices[assimp_mesh->mFaces[i].mIndices[2]].z);
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();

}
void key(unsigned char key, int x, int y) {
	//printf("key=%d\n",key);
	switch (key){
	case '+':
		trans[2] += zoom_speed;
		break;
	case '-':
		trans[2] -= zoom_speed;
		break;
	}
	printf("zoom%f\n", trans[2]);
	glutPostRedisplay();
}
void wheel(int button, int direction, int x, int y)
{
	if (direction>0){
		trans[2] -= zoom_speed;
	}
	else{
		trans[2] += zoom_speed;
	}
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON){//平行移動

		quaternionflag = false;
		if (state == GLUT_DOWN){
			mouse_down[0] = x;
			mouse_down[1] = y;
			//printf("down:%d,%d\n",x,y);
		}
		else if (state == GLUT_UP){
			trans[0] += translate_speed*(x - mouse_down[0]);
			trans[1] += -translate_speed*(y - mouse_down[1]);//yは上下逆
			//printf("up:%d,%d\n",x,y);
		}

	}
	else
	if (button == GLUT_LEFT_BUTTON){
		quaternionflag = true;
		switch (state){
		case GLUT_DOWN://Quaternion:マウスボタンを押した位置を記憶 
			previous_x = x;
			previous_y = y;
		case GLUT_UP://Quaternion:姿勢を保存
			current_quaternion = target_quaternion;

			break;
		default:
			break;
		}

	}
	glutPostRedisplay();

}

void move(int x, int y)
{
	if (quaternionflag){
		//移動量を計算 画面の中で何％ぐらい動いたか？
		float dx = (x - previous_x) * 1.0 / WINWIDTH;
		float dy = (y - previous_y) * 1.0 / WINHEIGHT;

		//クォータニオンの長さ
		float length = sqrt(dx * dx + dy * dy);

		if (length != 0.0) {
			//M_PIは適当な換算係数　piにしておくと、画面いっぱい動かした時にちょうど一回転になる
			float radian = length * M_PI;
			float theta = sin(radian) / length;
			miffy::quat<float> after(cos(radian), dy * theta, dx * theta, 0.0);//回転後の姿勢
			target_quaternion = after * current_quaternion;
			target_quaternion.toMat4(rotation_matrix.m);

		}
	}
	else{//平行移動
		trans[0] += translate_speed*(x - mouse_down[0]);
		trans[1] += -translate_speed*(y - mouse_down[1]);//yは上下逆
		mouse_down[0] = x;
		mouse_down[1] = y;
	}
}
int main(int argc, char **argv)
{
	printf("end");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glutMainLoopから抜け出してから何か表示するためのもの
	printf("end");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(WINWIDTH, WINHEIGHT);
	printf("end");
	glutInitWindowPosition(0, 0);
	printf("end");
	glutCreateWindow("miffy program");
	glewInit();
	Init();
	printf("end");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutMouseWheelFunc(wheel);
	glutMainLoop();

	printf("end");
	exit(0);
	return 0;
}