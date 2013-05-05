// Hari Ganesan
// Actor class

#include "Elements.hpp"

class Stage;

class Actor {
protected:
	int x;
	int y;
	int w;
	int h;

public:
	std::string id;

	Actor(std::string d) : x(-1), y(-1) {id = d;};
};

class DynamicActor : public Actor {
public:
	bool isJumping;
	int jumpCount;

	DynamicActor(std::string d) : Actor(d), isJumping(false), jumpCount(0) {};
	bool moveLeft();
	bool moveRight();
	bool moveLeft(int x);
	bool moveRight(int x);
	void setX(int newX) {x = newX;};
	void setY(int newY) {y = newY;};
	int getX() {return x;};
	int getY() {return y;};
	int getW() {return w;};
	int getH() {return h;};

};

class Ghost : public DynamicActor {
public:
	bool moveUp();
	bool moveDown();
	Ghost(std::string d, int newX, int newY, int newW, int newH) : DynamicActor(d) {
		x = newX;
		y = newY;
		w = newW;
		h = newH;
	}
};

class Character : public DynamicActor {
public:
	bool isGhost;
	bool isPossessible;
	bool possessed;
	bool tracking;
	int invincible; // loops invisible
	int pillCount;
	Ghost *ghost;
	int hitPoints;
	int maxHitPoints;
	Stage *s;
	int attack;


	Character(Stage *newS, std::string d, int hP, int newX, int newY, int nW, int nH) 
			: DynamicActor(d), isGhost(false), possessed(false), invincible(0)
			,	pillCount(5), attack(1) {
		hitPoints = hP;
		maxHitPoints = hP;
		s = newS;
		x = newX;
		y = newY;
		w = nW;
		h = nH;
	};
	bool jump();
	int jumpFn(double initialV, int timeElapsed);
	Ghost *createGhost(int newX, int newY);
	void destroyGhost();
	void takeDamage(int damage);
};

class Stage {
public:
	Character *mc; // main character
	Ghost *mcG; // main character as ghost
	Character *mcP; // enemy/object mc possesses
	Character *enemies[MAX_ENEMY_COUNT]; // all other characters
	int gravity;


	Stage(int g) {gravity = g;};
	void popUp(std::string msg);
	bool fullOverlap(DynamicActor *c1, DynamicActor *c2);
};