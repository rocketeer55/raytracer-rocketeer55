#include <iostream>
#include "Triangle.h"

using namespace Objects;

Triangle::Triangle() {
	a = glm::vec3();
	b = glm::vec3();
	c = glm::vec3();

	InverseMatrix = glm::mat4(1.f);
}

Triangle::Triangle(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c) {
	a = _a;
	b = _b;
	c = _c;

	InverseMatrix = glm::mat4(1.f);
}

std::string Triangle::type() {
	return "Triangle";
}

void Triangle::print() {
	std::cout << "- Type: Triangle" << std::endl;
	std::cout << "- Point A: {" << a.x << " " << a.y << " " << a.z << "}" << std::endl;
	std::cout << "- Point B: {" << b.x << " " << b.y << " " << b.z << "}" << std::endl;
	std::cout << "- Point C: {" << c.x << " " << c.y << " " << c.z << "}" << std::endl;
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
	std::cout << "- Material " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
}

float Triangle::getFirstCollision(Ray* ray) {
	// NEED TO OPTIMIZE
	
	float t, gamma, beta, alpha, determinant_a = 0.f;
	glm::mat4 m_a, m_gamma, m_beta, m_t;

	// Compute T
	m_a = glm::mat3(0.f);
	m_a[0][0] = a.x - b.x;	m_a[0][1] = a.x - c.x;  m_a[0][2] = ray->d.x;
	m_a[1][0] = a.y - b.y; 	m_a[1][1] = a.y - c.y;	m_a[1][2] = ray->d.y;
	m_a[2][0] = a.z - b.z;	m_a[2][1] = a.z - c.z;	m_a[2][2] = ray->d.z;

	m_gamma = glm::mat3(0.f);
	m_gamma[0][0] = a.x - b.x;	m_gamma[0][1] = a.x - ray->origin.x;  m_gamma[0][2] = ray->d.x;
	m_gamma[1][0] = a.y - b.y;	m_gamma[1][1] = a.y - ray->origin.y;  m_gamma[1][2] = ray->d.y;
	m_gamma[2][0] = a.z - b.z;  m_gamma[2][1] = a.z - ray->origin.z;  m_gamma[2][2] = ray->d.z;

	m_beta = glm::mat3(0.f);
	m_beta[0][0] = a.x - ray->origin.x;  m_beta[0][1] = a.x - c.x;  m_beta[0][2] = ray->d.x;
	m_beta[1][0] = a.y - ray->origin.y;  m_beta[1][1] = a.y - c.y;  m_beta[1][2] = ray->d.y;
	m_beta[2][0] = a.z - ray->origin.z;  m_beta[2][1] = a.z - c.z;  m_beta[2][2] = ray->d.z;

	m_t = glm::mat3(0.f);
	m_t[0][0] = a.x - b.x;  m_t[0][1] = a.x - c.x;  m_t[0][2] = a.x - ray->origin.x;
	m_t[1][0] = a.y - b.y;  m_t[1][1] = a.y - c.y;  m_t[1][2] = a.y - ray->origin.y;
	m_t[2][0] = a.z - b.z;  m_t[2][1] = a.z - c.z;  m_t[2][2] = a.z - ray->origin.z;


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
	BoundingBox* box = new BoundingBox(a);
	box->addPoint(b);
	box->addPoint(c);

	box->rotate(ModelMatrix);

	boundingBox = box;
}