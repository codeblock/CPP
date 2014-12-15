//
//  game-eat-the-food.cpp
//  CPP
//
//  Created by bermann on 2014. 12. 13..
//  Copyright (c) 2014년 bermann. All rights reserved.
//
// http://kin.naver.com/qna/detail.nhn?d1id=1&dirId=1040101&docId=212917976

#include <iostream>
#include <cstdio> // getchar
#include <cstdlib> // rand, srand
#include <ctime> // time
#include <list>

#define MAX(a, b) ((a) > (b) ? (a) : (b)) // instead #include <algorythm>
#define MIN(a, b) ((a) < (b) ? (a) : (b)) // instead #include <algorythm>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#	define CLEAR_SCREEN() system("clear")
#	define DASHBAR "━━"
#else
#	define CLEAR_SCREEN() system("cls")
#	define DASHBAR "━"
#endif

#define SPACE "　"
#define HUMAN "Ｈ"
#define MONSTER "Ｍ"
#define FOOD "＠"

using namespace std;

class GameObject // 추상 클래스
{
protected:
	int distance; // 한 번 이동 거리
	int x, y; // 현재 위치
public:
	GameObject(int startX, int startY, int distance) { // 초기 위치와 이동거리 설정
		this->x = startX; this->y = startY;
		this->distance = distance;
	}
	virtual ~GameObject() {}; // 가상 소멸자
	
	virtual void move() = 0; // 이동한 후 새로운 위치로 x, y 변경
	virtual char *getShape() = 0; // 객체의 모양을 나타내는 문자: [Human: 'H', Monster: 'M' Food: '@']
	
	int getX() { return x; }
	int getY() { return y; }
	bool collide(GameObject *p) { // 이 객체가 객체 p와 충돌했으면 true 리턴
		if (this->x == p->getX() && this->y == p->getY()) {
			return true;
		} else {
			return false;
		}
	}
};

class Human : public GameObject
{
private:
	int x_pre, y_pre;
public:
	Human(int x, int y, int d) : GameObject(x, y, d) {}
	void move() { this->x = this->x_pre; this->y = this->y_pre; }
	char *getShape() { return HUMAN; }
	void setX(int p_arg, int p_min, int p_max) { this->x_pre = MIN(p_max, MAX(p_arg, p_min)); }
	void setY(int p_arg, int p_min, int p_max) { this->y_pre = MIN(p_max, MAX(p_arg, p_min)); }
	int getDistance() { return this->distance; }
	~Human() {
		//
	}
};

class Monster : public GameObject
{
private:
	int x_pre, y_pre;
public:
	Monster(int x, int y, int d) : GameObject(x, y, d) {}
	void move() { this->x = this->x_pre; this->y = this->y_pre; }
	char *getShape() { return MONSTER; }
	void setX(int p_arg, int p_min, int p_max) { this->x_pre = MIN(p_max, MAX(p_arg, p_min)); }
	void setY(int p_arg, int p_min, int p_max) { this->y_pre = MIN(p_max, MAX(p_arg, p_min)); }
	int getDistance() { return this->distance; }
	~Monster() {
		//
	}
};

class Food : public GameObject
{
private:
	int x_pre, y_pre;
	int movin_able;
	int movin_cnt;
	int cycle_cnt;
	int *cycle;
public:
	Food(int x, int y, int d) : GameObject(x, y, d) {
		this->movin_able = 2; // 반복되는 주기당 움직일 수 있는 회수
		this->movin_cnt = 0; // 움직임 시도의 회수
		this->cycle_cnt = 5; // 반복되는 주기 및 배열의 크기
		this->cycle = new int[this->cycle_cnt];
	}
	void move() { this->x = this->x_pre; this->y = this->y_pre; }
	char *getShape() { return FOOD; }
	void setX(int p_arg, int p_min, int p_max, bool init) { this->x_pre = MIN(p_max, MAX(p_arg, p_min)); }
	void setY(int p_arg, int p_min, int p_max, bool init) { this->y_pre = MIN(p_max, MAX(p_arg, p_min)); }
	void setX(int p_arg, int p_min, int p_max) {
		if (this->isMovable()) {
			this->x_pre = MIN(p_max, MAX(p_arg, p_min));
		}
	}
	void setY(int p_arg, int p_min, int p_max) {
		if (this->isMovable()) {
			this->y_pre = MIN(p_max, MAX(p_arg, p_min));
		}
	}
	int getDistance() { return this->distance; }
	bool isMovable() {
		bool rtn = false;
		
		if (this->movin_cnt == 0 || this->movin_cnt == this->cycle_cnt) {
			this->resetRepeat();
			this->movin_cnt = 0;
		}
		if (this->cycle[this->movin_cnt] == 1) {
			rtn = true;
		}
		this->movin_cnt++;
		
		return rtn;
	}
	void resetRepeat() {
		int i, r, r_cnt = 0;
		
		for (i = 0; i < this->cycle_cnt; i++) { this->cycle[i] = 0; } // initialize the values
		while (r_cnt < this->movin_able) { // 랜덤이므로 움직임 표시플래그가(=1) 모자를 수 있음
			for (i = 0; i < this->cycle_cnt; i++) {
				if (r_cnt < this->movin_able && this->cycle[i] == 0) {
					r = rand() % 2;
					if (r == 1) { r_cnt++; }
					this->cycle[i] = r; // 0 or 1
				}
			}
		}
	}
	void status() {
		int i;
		
		cout << this->movin_cnt << " : ";
		for (i = 0; i < this->cycle_cnt; i++) {
			cout << (this->cycle[i] == 1 ? "●" : "○");
		}
	}
	~Food() {
		delete[] this->cycle;
	}
};

class Game
{
private:
	int col, row;
	list<GameObject *> *li;
	GameObject *human, *monster, *food;
	enum _ctrl {QUIT = 'q', RETRY = 'r', LEFT = 'a', DOWN = 's', UP = 'd', RIGHT = 'f'};
	
	void shuffle() {
		int x, y;
		list<GameObject *>::iterator iter, iter_in;
		
		for (iter = this->li->begin(); iter != this->li->end(); ++iter) {
			x = rand() % this->col;
			y = rand() % this->row;
			       if (*iter == this->human) {
				((Human *)*iter)->setX(x, 0, this->col - 1);
				((Human *)*iter)->setY(y, 0, this->row - 1);
			} else if (*iter == this->monster) {
				((Monster *)*iter)->setX(x, 0, this->col - 1);
				((Monster *)*iter)->setY(y, 0, this->row - 1);
			} else if (*iter == this->food) {
				((Food *)*iter)->setX(x, 0, this->col - 1, 1);
				((Food *)*iter)->setY(y, 0, this->row - 1, 1);
			}
			(*iter)->move();
			for (iter_in = this->li->begin(); iter_in != this->li->end(); ++iter_in) {
				if (iter != iter_in) {
					while ((*iter)->collide(*iter_in)) {
						x = rand() % this->col;
						y = rand() % this->row;
						       if (*iter == this->human) {
							((Human *)*iter)->setX(x, 0, this->col - 1);
							((Human *)*iter)->setY(y, 0, this->row - 1);
						} else if (*iter == this->monster) {
							((Monster *)*iter)->setX(x, 0, this->col - 1);
							((Monster *)*iter)->setY(y, 0, this->row - 1);
						} else if (*iter == this->food) {
							((Food *)*iter)->setX(x, 0, this->col - 1, 1);
							((Food *)*iter)->setY(y, 0, this->row - 1, 1);
						}
						(*iter)->move();
					}
				}
			}
			//cout << (*iter)->getShape();
			//cout << " - x: " << (*iter)->getX() << ", y: " << (*iter)->getY();
			//cout << endl;
		}
	}
	int init() {
		int rtn = 0;
		srand((unsigned)time(NULL));
		
		this->li = new list<GameObject *>;
		
		this->human   = new Human(-1, -1, 1);
		this->monster = new Monster(-1, -1, 2);
		this->food    = new Food(-1, -1, 1);
		
		this->li->push_back(this->human);
		this->li->push_back(this->monster);
		this->li->push_back(this->food);
		
		this->shuffle();
		
		return rtn;
	}
	void map() {
		int i, j;
		int rows = this->row * 2 + 1;
		int cols = this->col * 2 + 1;
		
		for (i = 0; i < rows; i++) {
			for (j = 0; j < cols; j++) {
				if (i == 0) {
					if (j == 0)       { cout << "┏"; }
					else if (j == cols - 1) { cout << "┓"; }
					else if (j % 2 == 0)   { cout << "┳"; }
				}
				else if (i == rows - 1) {
					if (j == 0)       { cout << "┗"; }
					else if (j == cols - 1) { cout << "┛"; }
					else if (j % 2 == 0)   { cout << "┻"; }
				}
				else {
					if (i % 2 == 0) {
						if (j == 0)       { cout << "┣"; }
						else if (j == cols - 1) { cout << "┫"; }
						else if (j % 2 == 0)   { cout << "╋"; }
					}
					else {
						if (j % 2 == 0) { cout << "┃"; }
						//else if (j == 1) { cout << HUMAN; }
						//else if (j == 3) { cout << MONSTER; }
						//else if (j == 5) { cout << FOOD; }
						//else { cout << SPACE; }
						else {
							if (this->human->getY() * 2 + 1 == i && this->human->getX() * 2 + 1 == j) {
								cout << HUMAN;
							} else if (this->monster->getY() * 2 + 1 == i && this->monster->getX() * 2 + 1 == j) {
								cout << MONSTER;
							} else if (this->food->getY() * 2 + 1 == i && this->food->getX() * 2 + 1 == j) {
								cout << FOOD;
							} else {
								cout << SPACE;
							}
						}
					}
				}
				if (i % 2 == 0 && j % 2 != 0) { cout << DASHBAR; }
			}
			cout << endl;
		}
	}
	int menu() {
		int rtn;
		bool ending = false;
		
		cout << "move - {a: left s: down d: up f: right}" << endl;
		this->status();
		cout << "quit - q, retry - r" << endl;
		if (this->food->collide(this->monster) || this->monster->collide(this->human)) {
			cout << "<<실패>>" << endl;
			ending = true;
		} else if (this->food->collide(this->human)) {
			cout << "<<성공>>" << endl;
			ending = true;
		}
		rtn = getchar();
		
		if (ending == true) {
			rtn = RETRY;
		}
		
		return rtn;
	}
	void status() {
		cout << "stat - ";
		cout << "H: {" << this->human->getX() << ", " << this->human->getY() << "}, ";
		cout << "M: {" << this->monster->getX() << ", " << this->monster->getY() << "}, ";
		
		cout << "@: {" << this->food->getX() << ", " << this->food->getY();
		cout << "}(";
		((Food *)this->food)->status();
		cout << ")";
		
		cout << endl;
	}
	void control(int input) {
		int dt; // DisTance
		int dir_monster;
		int dir_food;
		
		if (input == LEFT || input == DOWN || input == UP || input == RIGHT) {
			dt = ((Human *)this->human)->getDistance();
			switch (input) {
				case LEFT:
					((Human *)this->human)->setX(this->human->getX() - dt, 0, this->col - 1);
					break;
				case DOWN:
					((Human *)this->human)->setY(this->human->getY() + dt, 0, this->row - 1);
					break;
				case UP:
					((Human *)this->human)->setY(this->human->getY() - dt, 0, this->row - 1);
					break;
				case RIGHT:
					((Human *)this->human)->setX(this->human->getX() + dt, 0, this->col - 1);
					break;
			}
			
			dir_monster = rand() % 4;
			dt = ((Monster *)this->monster)->getDistance();
			switch (dir_monster) {
				case 0: // LEFT
					((Monster *)this->monster)->setX(this->monster->getX() - dt, 0, this->col - 1);
					break;
				case 1: // DOWN
					((Monster *)this->monster)->setY(this->monster->getY() + dt, 0, this->row - 1);
					break;
				case 2: // UP
					((Monster *)this->monster)->setY(this->monster->getY() - dt, 0, this->row - 1);
					break;
				case 3: // RIGHT
					((Monster *)this->monster)->setX(this->monster->getX() + dt, 0, this->col - 1);
					break;
			}
			
			dir_food = rand() % 4;
			dt = ((Food *)this->food)->getDistance();
			switch (dir_food) {
				case 0: // LEFT
					((Food *)this->food)->setX(this->food->getX() - dt, 0, this->col - 1);
					break;
				case 1: // DOWN
					((Food *)this->food)->setY(this->food->getY() + dt, 0, this->row - 1);
					break;
				case 2: // UP
					((Food *)this->food)->setY(this->food->getY() - dt, 0, this->row - 1);
					break;
				case 3: // RIGHT
					((Food *)this->food)->setX(this->food->getX() + dt, 0, this->col - 1);
					break;
			}
			
			this->human->move();
			this->monster->move();
			this->food->move();
		}
	}
public:
	Game() {
		this->col = 10;
		this->row = 20;
	}
	Game(int col, int row) {
		this->col = col;
		this->row = row;
	}
	~Game() {
		delete this->li;
		delete this->human;
		delete this->monster;
		delete this->food;
	}
	void restart() {
		int col, row;
		
		col = this->col;
		row = this->row;
		
		delete this;
		
		Game *game = new Game(col, row);
		game->start();
	}
	void start() {
		int input;
		
		this->init();
		do {
			CLEAR_SCREEN();
			this->map();
			input = this->menu();
			if (input == QUIT) {
				cout << "are you quit? (y or n)";
				getchar(); // clear buffer (remove enter)
				if (getchar() != (int)'y') {
					input = 0;
				}
			} else if (input == RETRY) {
				cout << "are you restart or quit? (r or Anykey)";
				getchar(); // clear buffer (remove enter)
				if (getchar() == (int)'r') {
					this->restart();
				} else {
					input = QUIT;
				}
			}// else {
			//	getchar(); // clear buffer (remove enter)
			//}
			this->control(input);
		} while (input != QUIT);
	}
};

int main(int argc, const char *argv[]) {
	Game *game = new Game(10, 10);
	game->start();
	
	delete game;
	game = NULL;
	
	return 0;
}

