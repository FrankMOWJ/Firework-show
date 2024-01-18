#ifndef PARTICLE_H
#define PARTICLE_H
#include"../utils/ColourSystems.h"
#include"../utils/CoordSystems.h"

class Particle {
public:
	Vector3 Position;	// ����λ��
	float Age;	//���ӵ�ǰ����
	float MaxAge;	//�����������

	float Scale;	//���ӵ�ǰ��С
	float EndScale;	//���մ�С

	// ���ӱ���Ҳ��Ҫ��ɫ����Ϊû�б�ըǰҪ��Ⱦ��β
	RGBA Colour;	//��ɫ
	RGBA EndColour;

	Vector3 MoveVector;		// �ٶ�������Ŀǰֻ��x,yƽ����ٶȷ���
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
