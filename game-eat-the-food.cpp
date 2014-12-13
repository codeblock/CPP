//
//  game-eat-the-food.cpp
//  CPP
//
//  Created by bermann on 2014. 12. 13..
//  Copyright (c) 2014년 bermann. All rights reserved.
//
// http://kin.naver.com/qna/detail.nhn?d1id=1&dirId=1040101&docId=212917976

#include <iostream>
#include <cstdlib> // rand, srand
#include <ctime> // time
#include <list>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#	define CLEAR_SCREEN system("clear")
#	define DASHBAR "━━"
#else
#	define CLEAR_SCREEN system("cls")
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
	
	virtual void move(int, int) = 0; // 이동한 후 새로운 위치로 x, y 변경
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
public:
	Human(int x, int y, int d) : GameObject(x, y, d) {}
	void move(int x, int y) { this->x = x; this->y = y; }
	char *getShape() { return HUMAN; }
	~Human() {
		//
	}
};

class Monster : public GameObject
{
public:
	Monster(int x, int y, int d) : GameObject(x, y, d) {}
	void move(int x, int y) { this->x = x; this->y = y; }
	char *getShape() { return MONSTER; }
	~Monster() {
		//
	}
};

class Food : public GameObject
{
public:
	Food(int x, int y, int d) : GameObject(x, y, d) {}
	void move(int x, int y) { this->x = x; this->y = y; }
	char *getShape() { return FOOD; }
	~Food() {
		//
	}
};

class Game
{
private:
	int col, row;
	GameObject *human, *monster, *food;
	list<GameObject *> *li;
	
	void shuffle() {
		int x, y;
		list<GameObject *>::iterator iter, iter_in;
		
		for (iter = this->li->begin(); iter != this->li->end(); ++iter) {
			x = rand() % this->col;
			y = rand() % this->row;
			(*iter)->move(x, y);
			for (iter_in = this->li->begin(); iter_in != this->li->end(); ++iter_in) {
				if (iter != iter_in) {
					while ((*iter)->collide(*iter_in)) {
						x = rand() % this->col;
						y = rand() % this->row;
						(*iter_in)->move(x, y);
					}
				}
			}
			cout << (*iter)->getShape();
			cout << " - x: " << (*iter)->getX() << ", y: " << (*iter)->getY();
			cout << endl;
		}
	}
	int init() {
		int rtn = 0;
		this->li = new list<GameObject *>;
		
		srand((unsigned)time(NULL));
		
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
	}
	void start() {
		this->init();
		this->map();
	}
};

int main(int argc, const char *argv[]) {
	Game *game = new Game();//10, 10);
	game->start();
	
	//cout << rand() % 5;
	
	return 0;
}

