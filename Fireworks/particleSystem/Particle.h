#ifndef PARTICLE_H
#define PARTICLE_H
#include"../utils/ColourSystems.h"
#include"../utils/CoordSystems.h"

class Particle {
public:
	Vector3 Position;	// 粒子位置
	float Age;	//粒子当前寿命
	float MaxAge;	//粒子最大寿命

	float Scale;	//粒子当前大小
	float EndScale;	//最终大小

	// 粒子本身也需要颜色，因为没有爆炸前要渲染拖尾
	RGBA Colour;	//颜色
	RGBA EndColour;

	Vector3 MoveVector;		// 速度向量，目前只有x,y平面的速度分量
	bool IsFake;

private:
	float _startScale;
	RGBA _startColour;

public:
	Particle();
	~Particle();
	void update(float dt);
};

#endif // !PARTICLE_H
