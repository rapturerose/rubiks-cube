#include <glut.h>
#include <string.h>
#include <stdio.h>
int height = 500;
int wight = 700;
int layer = 0; //  не особо понимаю зачем это нужно 
//----------------------------------
//			Все используемые классы
//----------------------------------
class point {
public:
	double x, y, z;
	point(double px, double py, double pz) {
		x = px;
		y = py;
		z = pz;
	}
};
class color {
public:
	float r, g, b;// RED, GREEN, BLUE
	

	color() {
		r = 0;
		g = 0;
		b = 0;
	}

	color(float ir, float ig, float ib) {
		r = ir;
		g = ig;
		b = ib;
	}
};
color ListOfColors[8] = { 
	color(1.0, 0, 0), // Красный
	color(0.7, 0.5, 0), // Оранжевый
	color(1.0 ,1.0, 0), // желтый
	color(0, 1.0, 0), // зеленый
	color(0, 0, 1.0), // синий
	color(1.0, 0, 1.0), // фиолетовый
	color(1.0, 1.0, 1.0),// белый
	color(0, 0, 0)};// черный

class button {
public:
	float x, y;// верхний левый угол
	float weight, height;
	color bg, fg;
	char* text;

	// забиваем параметры кнопки
	button(float px, float py, float iw, float ih, color background, color foreground, char* disptext) {
		x = px;
		y = py;
		weight = iw;
		height = ih;
		bg = background;
		fg = foreground;
		text = disptext;
	}

	void draw() 
	{
		glColor3f(0.1, 0.7, 0.1);
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x, y - height);
		glVertex2f(x + weight, y - height);
		glVertex2f(x + weight, y);
		glEnd();
		// Напечатаем текст на кнопке
		glPushMatrix();
		glTranslatef(x , y - (height/2), 0.1);
		glScalef(1/ 3500.0, 1 / 3500.0, 0);
		glLineWidth(2);
		glColor3f(0,0,0);
		for (int i = 0; i < strlen(text); i++) 
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text[i]);
		glPopMatrix();
	}

	bool Push(float px, float py) {
		if (px > x && px < x + weight && py > y - height && py< y)
			return true;
		return false;
	}
};

class Cube
{
	/*
	0 - Перед -  front
	1 - Зад - back
	2 - Left
	3 - Right
	4 - Up
	5 - Down
	*/
public:
	int faces[6][3][3];// цвета элементов. 6 сторон 3*3
	// функция инициализирует кубик (собранный)
	Cube(){
		for (int k = 0; k < 6; k++)
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					faces[k][i][j] = k;
		// k - Параметр, указывающий какая это сторона 
		// i , j - просто номера в двухмерном массиве стороны 
	}

	 // проверка не совсем точная 
	bool IsSolved() 
	{
		for (int k = 0; k < 6; k++)
		{
			int color = faces[k][0][0];
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (faces[k][i][j] != color)
						return false;
		}
		return true;
	}
	// ----------------------
	//		Все повороты 
	// ----------------------
	void front_anticlock() {

		int t[3];
		// поворот всех ребер
		for (int i = 0; i < 3; i++)
		{
			t[i] = faces[4][2 - layer][2 - i];
			faces[4][2 - layer][2 - i] = faces[3][2 - i][layer];
			faces[3][2 - i][layer] = faces[5][layer][i];
			faces[5][layer][i] = faces[2][i][2 - layer];
			faces[2][i][2 - layer] = t[i];
		}

		if (layer == 0) // перемешение самой передней грани
		{
			t[0] = faces[0][0][2];
			faces[0][0][2] = faces[0][2][2];
			faces[0][2][2] = faces[0][2][0];
			faces[0][2][0] = faces[0][0][0];
			faces[0][0][0] = t[0];
		}
	}

	void front_clock() {

		int t[3];
		for(int i=0; i<3; i++)
		{
			t[i] = faces[4][2 - layer][i];
			faces[4][2 - layer][i] = faces[2][2 - i][2 - layer];
			faces[2][2 - i][2 - layer] = faces[5][layer][2 - i];
			faces[5][layer][2 - i] = faces[3][i][layer];
			faces[3][i][layer] = t[i];
		}

		if (layer == 0) {
			t[0] = faces[0][0][0];
			faces[0][0][0] = faces[0][2][0];
			faces[0][2][0] = faces[0][2][2];
			faces[0][2][2] = faces[0][0][2];
			faces[0][0][2] = t[0];
		}
	}

	void back_anticlock() {
		int t[3];

		for(int i=0; i<3; i++) 
		{
			t[i] = faces[4][layer][i];
			faces[4][layer][i] = faces[2][2 - i][layer];
			faces[2][2 - i][layer] = faces[5][2 - layer][2 - i];
			faces[5][2 - layer][2 - i] = faces[3][i][2 - layer];
			faces[3][i][2 - layer] = t[i];
		}

		if (layer == 0) {
			
			t[0] = faces[1][0][2];
			faces[1][0][2] = faces[1][2][2];
			faces[1][2][2] = faces[1][2][0];
			faces[1][2][0] = faces[1][0][0];
			faces[1][0][0] = t[0];
		}
	}

	void back_clock() {
		int t[3];

		for (int i = 0; i < 3;i++) 
		{
			t[i] = faces[4][layer][2 - i];
			faces[4][layer][2 - i] = faces[3][2 - i][2 - layer];
			faces[3][2 - i][2 - layer] = faces[5][2 - layer][i];
			faces[5][2 - layer][i] = faces[2][i][layer];
			faces[2][i][layer] = t[i];
		}

		if (layer == 0) {
			
			t[0] = faces[1][0][0];
			faces[1][0][0] = faces[1][2][0];
			faces[1][2][0] = faces[1][2][2];
			faces[1][2][2] = faces[1][0][2];
			faces[1][0][2] = t[0];
		}
	}

	void left_anticlock() {
		int t[3];
		for (int i=0; i<3; i++)
		{
			t[i] = faces[4][2 - i][layer];
			faces[4][2 - i][layer] = faces[0][2 - i][layer];
			faces[0][2 - i][layer] = faces[5][2 - i][layer];
			faces[5][2 - i][layer] = faces[1][i][2 - layer];
			faces[1][i][2 - layer] = t[i];
		}

		if (layer == 0) {
		
			t[0] = faces[2][0][2 - 0];
			faces[2][0][2] = faces[2][2][2];
			faces[2][2][2] = faces[2][2][0];
			faces[2][2][0] = faces[2][0][0];
			faces[2][0][0] = t[0];
		}
	}

	void left_clock() {
		int t[3];

		for (int i=0; i<3; i++)
		{
			t[i] = faces[4][i][layer];
			faces[4][i][layer] = faces[1][2 - i][2 - layer];
			faces[1][2 - i][2 - layer] = faces[5][i][layer];
			faces[5][i][layer] = faces[0][i][layer];
			faces[0][i][layer] = t[i];
		}

		if (layer == 0) {
			t[0] = faces[2][0][0];
			faces[2][0][0] = faces[2][2][0];
			faces[2][2][0] = faces[2][2][2];
			faces[2][2][2] = faces[2][0][2];
			faces[2][0][2] = t[0];
		}
	}

	void right_anticlock() {

		int t[3];

		for(int i=0; i<3; i++)
		{
			t[i] = faces[4][i][2 - layer];
			faces[4][i][2 - layer] = faces[1][2 - i][layer];
			faces[1][2 - i][layer] = faces[5][i][2 - layer];
			faces[5][i][2 - layer] = faces[0][i][2 - layer];
			faces[0][i][2 - layer] = t[i];
		}

		if (layer == 0) {
			t[0] = faces[3][0][2];
			faces[3][0][2] = faces[3][2][2];
			faces[3][2][2] = faces[3][2][0];
			faces[3][2][0] = faces[3][0][0];
			faces[3][0][0] = t[0];
		}
	}
	
	void right_clock() {

		int t[3];

		for (int i = 0; i < 3; i++) 
		{
			t[i] = faces[4][2 - i][2 - layer];
			faces[4][2 - i][2 - layer] = faces[0][2 - i][2 - layer];
			faces[0][2 - i][2 - layer] = faces[5][2 - i][2 - layer];
			faces[5][2 - i][2 - layer] = faces[1][i][layer];
			faces[1][i][layer] = t[i];
		}

		if (layer == 0) 
		{
			t[0] = faces[3][0][0];
			faces[3][0][0] = faces[3][2][0];
			faces[3][2][0] = faces[3][2][2];
			faces[3][2][2] = faces[3][0][2];
			faces[3][0][2] = t[0];
		}
	}

	void up_anticlock() {

		int t[3];
		for(int i=0; i<3; i++)
		{
			t[i] = faces[1][layer][i];
			faces[1][layer][i] = faces[3][layer][i];
			faces[3][layer][i] = faces[0][layer][i];
			faces[0][layer][i] = faces[2][layer][i];
			faces[2][layer][i] = t[i];
		}

		if (layer == 0) {
		
				t[0] = faces[4][0][2];
				faces[4][0][2] = faces[4][2][2];
				faces[4][2][2] = faces[4][2][0];
				faces[4][2][0] = faces[4][0][0];
				faces[4][0][0] = t[0];
		}
	}

	void up_clock() {

		int t[3];

		for (int i = 0; i < 3; i++) {
			t[i] = faces[1][layer][2 - i];
			faces[1][layer][2 - i] = faces[2][layer][2 - i];
			faces[2][layer][2 - i] = faces[0][layer][2 - i];
			faces[0][layer][2 - i] = faces[3][layer][2 - i];
			faces[3][layer][2 - i] = t[i];
		}

		if (layer == 0) {		
			t[0] = faces[4][0][0];
			faces[4][0][0] = faces[4][2][0];
			faces[4][2][0] = faces[4][2][2];
			faces[4][2][2] = faces[4][0][2];
			faces[4][0][2] = t[0];
		}
	}

	void down_anticlock() {

		int t[3];

		for(int i=0; i<3;i++)
		{
			t[i] = faces[0][2 - layer][2- i];
			faces[0][2 - layer][2 - i] = faces[3][2 - layer][2 - i];
			faces[3][2 - layer][2 - i] = faces[1][2 - layer][2 - i];
			faces[1][2 - layer][2 - i] = faces[2][2 - layer][2 - i];
			faces[2][2 - layer][2 - i] = t[i];
		}

		if (layer == 0) {
					t[0] = faces[5][0][2];
					faces[5][0][2] = faces[5][2][2];
					faces[5][2][2] = faces[5][2][0];
					faces[5][2][0] = faces[5][0][0];
					faces[5][0][0] = t[0];
				
		}
	}

	void down_clock() {

		int t[3];

		for (int i = 0; i < 3; i++)
		{
			t[i] = faces[0][2 - layer][i];
			faces[0][2 - layer][i] = faces[2][2 - layer][i];
			faces[2][2 - layer][i] = faces[1][2 - layer][i];
			faces[1][2 - layer][i] = faces[3][2 - layer][i];
			faces[3][2 - layer][i] = t[i];
		}
		if (layer == 0) {
			t[0] = faces[5][0][0];
			faces[5][0][0] = faces[5][2][0];
			faces[5][2][0] = faces[5][2][2];
			faces[5][2][2] = faces[5][0][2];
			faces[5][0][2] = t[0];
		}
	}
};

bool ismousepressed=false;

//void buildRubiksCube() {
//
//	double big_szie = 2.4;
//	double small_size = big_szie / 3;
//	double intercube_spacing = small_size * 0.05;
//
//	double start = big_szie / 2 + intercube_spacing;
//
//	for (double z = start; z>-start; z -= small_size + intercube_spacing) {
//		for (double y = start; y>-start; y -= small_size + intercube_spacing) {
//			for (double x = -start; x<start; x += small_size + intercube_spacing) {
//
//				int cx = (int)round((x + big_szie / 2) / (small_size + intercube_spacing));
//				int cy = (int)round((-y + big_szie / 2) / (small_size + intercube_spacing));
//				int cz = (int)round((-z + big_szie / 2) / (small_size + intercube_spacing));
//
//				glPushMatrix();
//				glTranslatef(8, 0, 0);
//				glMultMatrixd(getRotationMatrix(cubesRotation[cz][cy][cx]));
//				glTranslatef(-8, 0, 0);
//				float v[8][3] = {
//					{ x + 8, y, z },
//				{ x + small_size + 8, y, z },
//				{ x + small_size + 8, y - small_size, z },
//				{ x + 8, y - small_size, z },
//				{ x + 8, y, z - small_size },
//				{ x + small_size + 8, y, z - small_size },
//				{ x + small_size + 8, y - small_size, z - small_size },
//				{ x + 8, y - small_size, z - small_size }
//				};
//
//				drawCube(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], cx, cy, cz);
//				glPopMatrix();
//			}
//		}
//	}
//
//
//}

void printText(float x, float y, char* text, float size, void *font = GLUT_STROKE_ROMAN, float stroke = 2.0) {

	glPushMatrix();// сохраняем текущие координаты 
	glTranslatef(x, y, 0); // перемешаем оси координат от начального положения
	glScalef(size / 800.0, size / 800.0, 0);// команда масштабирования
	glLineWidth(stroke);// устанавливаем ширину линии
	glColor3f(1.0,1.0,1.0);// выбираем цвет

	for (int i = 0; i<strlen(text); ++i)
		glutStrokeCharacter(font, text[i]);
	glPopMatrix();// возвращаемся к сохраненным координатам
}
void mouse(int button, int state, int x, int y)
{
	float glx = (x - (float)wight/ 2) / wight;
	float gly = ((float)height/2 - y) / height;
	printf("Mouse click at %f\n, %f\n", glx, gly);
}

void display() {
	// очистка буфера цвета и  глубины, попросту очистка экрана
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнуление трансформации
	glLoadIdentity();
	
	printText(-0.9, -0.9, "CUBIC RUBIC", 1);
	button start = button(-0.9, 0.5, 0.2, 0.11, ListOfColors[1], ListOfColors[2], "Button");
	start.draw();
	/*buildRubiksCube();*/
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) 
{
	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(wight, height);
	glutCreateWindow("КУБИК РУБИК");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);

	glLoadIdentity();
	// основной цикл
	glutMainLoop();
	return 1;
}
