#include <windows.h>
#include <GL\glut.h>
#include <iostream>

using std::cin;
using std::cout;

void Initialize()
{
	glClearColor(1, 1, 1, 0);//белый

	//Установить проекцию
	glMatrixMode(GL_PROJECTION);///
	glLoadIdentity();	//Замена стандартной матрицы матрицей идентификаторов
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);    //Инициализация 2D-пространства
}

#define PIXSIZE 10
#define S_WIDTH 1024
#define S_HEIGHT 768

int minX = -10, maxX = 10, minY = -10, maxY = 10;

float getX(int inp) { //Координаты Х
	if (inp > 0) {
		return 0.5 + (1.0 / S_WIDTH) * (inp * PIXSIZE);
	}
	else if (inp < 0) {
		inp *= (-1);
		if (inp > int(inp)) inp += 1.0;
		return  0.5 - (1.0 / S_WIDTH) * (inp * PIXSIZE);
	}
	else return 0.5;

}

float getY(float inp) { //Координаты У
	if (inp >= 0) {
		return 0.5 + (1.0 / S_HEIGHT) * (int(inp) * PIXSIZE);
	}
	else if (inp < 0) {
		inp *= (-1);
		if (inp > int(inp)) inp += 1.0;
		return  0.5 - (1.0 / S_HEIGHT) * (int(inp) * PIXSIZE);
	}
	else return 0.5;
}

void draw_polygon(float px, float py) {//Рисуем квадрат, нижний левый край = (х;у)
	glBegin(GL_POLYGON);
	glVertex2d(getX(px), getY(py));
	glVertex2d(getX(px), getY(py + 1));
	glVertex2d(getX(px + 1), getY(py + 1));
	glVertex2d(getX(px + 1), getY(py));
	glEnd();
	glFlush();
}

void draw_coords() { //Очищение холста и прорисовка системы координат

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.1); //Красный
	glBegin(GL_LINES);
	//ОХ
	glVertex2d(getX(-100), getY(0));
	glVertex2d(getX(100), getY(0));
	//ОY
	glVertex2d(getX(0), getY(-100));
	glVertex2d(getX(0), getY(100));
	glEnd();
	glFlush();

	for (int a = -50; a <= 50; a++) {//Рисуем клеточки
		if (a != 0) {
			glColor3f(0.5, 0.5, 0.5); //Серый
			glBegin(GL_LINES);
			//По оси Y
			glVertex2d(getX(-100), getY(a));
			glVertex2d(getX(100), getY(a));
			//По оси Х
			glVertex2d(getX(a), getY(-100));
			glVertex2d(getX(a), getY(100));
			glEnd();
			glFlush();
		}
	}
}

void drawBR(int x1, int x2, int y1, int y2, int par) {//Прорисовка ЦДА и запись координат в массив
	int px, py, error, lasty = -100;
	int  x, y, deltaerr;
	x = x1;
	y = y1;
	px = abs(x2 - x1);
	py = abs(y2 - y1);
	error = 0;
	if (px >= py) deltaerr = py;
	else deltaerr = px;
	while (((x2 >= x1 && x <= x2) || (x2 < x1 && x >= x2)) && ((y2 >= y1 && y <= y2) || (y2 < y1 && y >= y2))) {
		if (par == 0) glColor3f(0.45, 0.07, 0.7);//пурпурный
		else {
			glColor3f(0.5, 0.9, 0.3);
			if (x >= minX && x < maxX && y >= minY && y < maxY)  glColor3f(0.45, 0.07, 0.7);//пурпурный
		}
		draw_polygon(x, y);
		error = error + deltaerr;
		if (px >= py) {
			if (2 * error >= deltaerr) {
				if (y2 < y1) {
					y--;
				}
				else y++;
				error = error - px;
			}
			if (x2 > x1) x++;
			else if (x2 < x1) x--;
		}
		else {
			if (2 * error >= deltaerr) {
				if (x2 > x1) x++;
				else if (x2 < x1) x--;
				error = error - py;
			}
			if (y2 > y1) y++;
			else if (y2 < y1) y--;
		}
	}
}

void Draw()//Главная функция OpenGL, вызывающая все процессы прорисовки
{
	draw_coords();

	glColor3f(1.0, 0.0, 0.0); //Серый
	glBegin(GL_LINES);

	glVertex2d(getX(minX), getY(minY));
	glVertex2d(getX(minX), getY(maxY));

	glVertex2d(getX(minX), getY(maxY));
	glVertex2d(getX(maxX), getY(maxY));

	glVertex2d(getX(maxX), getY(maxY));
	glVertex2d(getX(maxX), getY(minY));

	glVertex2d(getX(maxX), getY(minY));
	glVertex2d(getX(minX), getY(minY));

	glEnd();
	//glFlush();
	// 
	//L
	int X1 = -11, Y1 = 11,
	X2 = -12, Y2 = 0;
	//рисуем пересекающий
	
	drawBR(X1, X2, Y1, Y2, 0);
	Sleep(750);
	drawBR(X1, X2, Y1, Y2, 1);
	// невидимый
	X1 = -12, Y1 = 0,
	X2 = -7, Y2 = -1;
	drawBR(X1, X2, Y1, Y2, 1);
	Sleep(750);
	//видимый
	//O
	X1 = -3, Y1 = 0,
	X2 = -4, Y2 = 11;
	drawBR(X1, X2, Y1, Y2, 1);

	X1 = -4, Y1 = 11,
	X2 = 3, Y2 = 13;
	drawBR(X1, X2, Y1, Y2, 1);

	X1 = 3, Y1 = 13,
	X2 = 4, Y2 = 2;
	drawBR(X1, X2, Y1, Y2, 1);

	X1 = 4, Y1 = 2,
	X2 = -3, Y2 = 0;
	drawBR(X1, X2, Y1, Y2, 1);

	//L
	X1 = 8, Y1 = 11,
	X2 = 7, Y2 = 0;
	drawBR(X1, X2, Y1, Y2, 0);
	Sleep(750);
	drawBR(X1, X2, Y1, Y2, 1);
	// невидимый
	X1 = 7, Y1 = 0,
		X2 = 12, Y2 = -1;
	drawBR(X1, X2, Y1, Y2, 1);
}

//Войти в главный цикл
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(S_WIDTH, S_HEIGHT);		//Указываем размер окна
	glutInitWindowPosition(100, 100);	        //Позиция окна
	glutCreateWindow("AB-021");			        //Имя окна
	Initialize();						        //Вызов функции Initialize
	glutDisplayFunc(Draw);				        //Вызов функции отрисовки
	glutMainLoop();
	return 0;
}
