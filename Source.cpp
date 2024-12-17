#include<gl/glut.h>
#include<vector>
#include<cstdlib>
#include<ctime>

const int gridWidth = 40;
const int gridHeight = 40;
int snakeDirection = 0;
bool gameOver = false;

std::vector<std::pair<int, int>> snake = { {10,10}, {10,11}, {10,12} };
std::pair<int, int> food;

void placeFood() {
	food.first = rand() % gridWidth;
	food.second = rand() % gridHeight;
}

void init() {
	srand(time(0));
	placeFood();
}

void drawGrid() {
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_LINES);
	for (int i = 0; i <= gridWidth; ++i) {
		glVertex2f(i, 0);
		glVertex2f(i, gridHeight);
	}
	for (int j = 0; j <= gridWidth; ++j) {
		glVertex2f(0, j);
		glVertex2f(gridWidth, j);
	}
	glEnd();
}

void drawRect(int x, int y) {
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

void drawSnake() {
	glColor3f(0, 1, 0);//Green
	for (auto s : snake) {
		drawRect(s.first, s.second);
	}
}

void drawFood() {
	glColor3f(1, 0, 0); // Red
	drawRect(food.first, food.second);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawGrid();
	drawSnake();
	drawFood();
	glutSwapBuffers();
}

void updateSnake() {
	std::pair<int, int> newHead = snake.front();
	if (snakeDirection == 0) newHead.first--;
	if (snakeDirection == 1) newHead.second++;
	if (snakeDirection == 2) newHead.first++;
	if (snakeDirection == 3) newHead.second--;

	if (newHead.first < 0 || newHead.second < 0 || newHead.first >= gridWidth || newHead.second >= gridHeight) {
		gameOver = true;
		return;
	}
	for (auto s : snake) {
		if (newHead == s) {
			gameOver = true;
			return;
		}
	}

	snake.insert(snake.begin(), newHead);

	if (newHead == food) {
		placeFood();
	}
	else {
		snake.pop_back(); //Remove tail
	}
}

void timer(int) {
	if (!gameOver) {
		updateSnake();
		glutPostRedisplay();
		glutTimerFunc(100, timer, 0);
	}
	else {
		printf("Game Over! Your score : %u\n", snake.size());
		exit(0);
	}
}

void keyboard(int key, int, int) {
	if (key == GLUT_KEY_LEFT && snakeDirection != 2) snakeDirection = 0;
	if (key == GLUT_KEY_UP && snakeDirection != 3) snakeDirection = 1;
	if (key == GLUT_KEY_RIGHT && snakeDirection != 0) snakeDirection = 2; \
		if (key == GLUT_KEY_DOWN && snakeDirection != 1) snakeDirection = 3;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OpenGL Snake Game");

	gluOrtho2D(0, gridWidth, 0, gridHeight);

	init();
	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}