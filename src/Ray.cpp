#include <iostream>
#include <iomanip>
#include "Ray.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Objects;

Ray::Ray() {
	d = glm::vec3(0.f);
	origin = glm::vec3(0.f);
	x = y = width = height = 0;
}

Ray::Ray(int _x, int _y, int _width, int _height, int q, int r, int s, Camera* camera) {
	float U, V, W;
	glm::vec3 u, v, w;

	x = _x;
	y = _y;
	width = _width;
	height = _height;

	origin = camera->location;

	U = -0.5f + (s * x + q + 0.5f) / (float)(width * s);
	V = -0.5f + (s * y + r + 0.5f) / (float)(height * s);
	W = -1.f;

	u = camera->right;
	v = camera->up;

	glm::vec3 l = camera->look_at - camera->location;
	l = glm::normalize(l);
	w = l * -1.f;

	glm::vec3 D = u * U + v * V + w * W;
	d = glm::normalize(D);
}

Ray::Ray(glm::vec3 _origin, glm::vec3 _destination) {
	origin = _origin;

	d = normalize(_destination - _origin);
}