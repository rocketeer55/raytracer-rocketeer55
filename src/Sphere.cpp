#include <iostream>
#include "Sphere.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace Objects;

Sphere::Sphere() {
	center = glm::vec3();
	radius = 0.f;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Sphere::Sphere(glm::vec3 _center, float _radius) {
	center = _center;
	radius = _radius;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Sphere::Sphere(const Sphere &other) {
	center = other.center;
	radius = other.radius;

	pigment = other.pigment;
	finish = other.finish;

	ModelMatrix = other.ModelMatrix;
	InverseMatrix = other.InverseMatrix;

	boundingBox = other.boundingBox;

	isLight = other.isLight;
}

std::string Sphere::type() {
	return "Sphere";
}

float Sphere::getFirstCollision(Ray &ray) {
	float a, b, c, discriminant, t;

	a = glm::dot(ray.d, ray.d);
	b = glm::dot(2.f * ray.d, ray.origin - center);
	c = glm::dot(ray.origin - center, ray.origin - center) - (radius * radius);

	discriminant = (b * b) - (4 * a * c);
	if (discriminant == 0) {
		// One collision
		t = -b / (2 * a);
		if (t >= 0) {
			return t;
		}
		return -1;
	}
	else if (discriminant > 0) {
		// Two solutions
		t = (-b - sqrt(discriminant)) / (2 * a);
		if (t < 0) {
			t = (-b + sqrt(discriminant)) / (2 * a);
			if (t < 0) {
				return -1;
			}
			return t;
		}
		return t;
	}
	// No solutions

	return -1;
}

glm::vec3 Sphere::getNormal(glm::vec3 point) {
	return glm::normalize(point - center);
}

void Sphere::calculateBoundingBox() {
	boundingBox = BoundingBox(center - glm::vec3(radius));
	boundingBox.addPoint(center + glm::vec3(radius));

	boundingBox.rotate(ModelMatrix);
}

void Sphere::setPosition(glm::vec3 position) {
	center = position;
}

glm::vec3 Sphere::getPosition() {
	return center;
}