#include <iostream>
#include "Triangle.h"

using namespace Objects;

Triangle::Triangle() {
	a = glm::vec3();
	b = glm::vec3();
	c = glm::vec3();

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Triangle::Triangle(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c) {
	a = _a;
	b = _b;
	c = _c;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Triangle::Triangle(const Triangle &other) {
	a = other.a;
	b = other.b;
	c = other.c;

	pigment = other.pigment;
	finish = other.finish;

	ModelMatrix = other.ModelMatrix;
	InverseMatrix = other.InverseMatrix;

	boundingBox = other.boundingBox;

	isLight = other.isLight;
}

std::string Triangle::type() {
	return "Triangle";
}

float Triangle::getFirstCollision(Ray &ray) {
	// NEED TO OPTIMIZE
	
	float t, gamma, beta, alpha, determinant_a = 0.f;
	glm::mat4 m_a, m_gamma, m_beta, m_t;

	// Compute T
	m_a = glm::mat3(0.f);
	m_a[0][0] = a.x - b.x;	m_a[0][1] = a.x - c.x;  m_a[0][2] = ray.d.x;
	m_a[1][0] = a.y - b.y; 	m_a[1][1] = a.y - c.y;	m_a[1][2] = ray.d.y;
	m_a[2][0] = a.z - b.z;	m_a[2][1] = a.z - c.z;	m_a[2][2] = ray.d.z;

	m_gamma = glm::mat3(0.f);
	m_gamma[0][0] = a.x - b.x;	m_gamma[0][1] = a.x - ray.origin.x;  m_gamma[0][2] = ray.d.x;
	m_gamma[1][0] = a.y - b.y;	m_gamma[1][1] = a.y - ray.origin.y;  m_gamma[1][2] = ray.d.y;
	m_gamma[2][0] = a.z - b.z;  m_gamma[2][1] = a.z - ray.origin.z;  m_gamma[2][2] = ray.d.z;

	m_beta = glm::mat3(0.f);
	m_beta[0][0] = a.x - ray.origin.x;  m_beta[0][1] = a.x - c.x;  m_beta[0][2] = ray.d.x;
	m_beta[1][0] = a.y - ray.origin.y;  m_beta[1][1] = a.y - c.y;  m_beta[1][2] = ray.d.y;
	m_beta[2][0] = a.z - ray.origin.z;  m_beta[2][1] = a.z - c.z;  m_beta[2][2] = ray.d.z;

	m_t = glm::mat3(0.f);
	m_t[0][0] = a.x - b.x;  m_t[0][1] = a.x - c.x;  m_t[0][2] = a.x - ray.origin.x;
	m_t[1][0] = a.y - b.y;  m_t[1][1] = a.y - c.y;  m_t[1][2] = a.y - ray.origin.y;
	m_t[2][0] = a.z - b.z;  m_t[2][1] = a.z - c.z;  m_t[2][2] = a.z - ray.origin.z;


	determinant_a = glm::determinant(m_a);
	gamma = glm::determinant(m_gamma) / determinant_a;
	beta = glm::determinant(m_beta) / determinant_a;
	t = glm::determinant(m_t) / determinant_a;

	if (t <= 0) {
		return -1;
	}
	// Check gamma
	if (gamma < 0 || gamma > 1) {
		return -1;
	}
	// Check beta
	if (beta < 0 || beta > 1) {
		return -1;
	}
	// Check alpha
	alpha = 1 - (gamma + beta);
	if (alpha < 0 || alpha > 1) {
		return -1;
	}

	return t;
}

glm::vec3 Triangle::getNormal(glm::vec3 point) {
	glm::vec3 normal = glm::vec3(0.f);

	glm::vec3 U = b - a;
	glm::vec3 V = c - a;

	normal.x = (U.y * V.z) - (U.z * V.y);
	normal.y = (U.z * V.x) - (U.x * V.z);
	normal.z = (U.x * V.y) - (U.y * V.x); 

	return glm::normalize(normal);
}

void Triangle::calculateBoundingBox() {
	boundingBox = BoundingBox(a);
	boundingBox.addPoint(b);
	boundingBox.addPoint(c);

	boundingBox.rotate(ModelMatrix);
}

void Triangle::setPosition(glm::vec3 position) {}

glm::vec3 Triangle::getPosition() {return glm::vec3();}