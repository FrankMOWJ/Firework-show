#ifndef CoordSystemsH
#define CoordSystemsH

#include <math.h>
#include <time.h>
#include <random>

struct Vector3 {

	float x, y, z;
	Vector3() {
		x = y = z = 0;
	}

	Vector3(float inX, float inY, float inZ) {
		x = inX;
		y = inY;
		z = inZ;
	}
	void normalise() {
		float length = sqrt((x * x) + (y * y) + (z * z));
		x /= length;
		y /= length;
		z /= length;
	}
	void random(float max) {

		x = (float)rand() / ((float)RAND_MAX / max);
		y = (float)rand() / ((float)RAND_MAX / max);
		z = (float)rand() / ((float)RAND_MAX / max);
	}
};

Vector3 operator + (const Vector3 v1, const Vector3 v2);
Vector3 operator - (const Vector3 v1, const Vector3 v2);
Vector3 operator / (const Vector3 v1, float amount);
Vector3 operator * (const Vector3 v1, float amount);
Vector3 operator * (const Vector3 v1, const Vector3 v2);

#endif