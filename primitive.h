#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <map>
#include<algorithm>

struct Material
{
	glm::vec3 color;
	float reflection;
	Material(glm::vec3 color);
	Material(glm::vec3 color, float reflection);
	Material(const Material& x);
	Material();
};


class Primitive
{
public:
	Material material;
	virtual float dist(glm::vec3 p) = 0;
	Primitive(Material material);
	Primitive();
};

class Box: public Primitive
{
public:
	glm::vec3 edges;
	Material material;
	glm::mat4 transformer;

	Box(glm::vec3 edges, glm::mat4 transformer, Material material);
	Box(const Box &x);
	float dist(glm::vec3 p);
};

class Sphere:public Primitive
{
public:
	float radius;
	glm::mat4 transformer;
	Material material;
	Sphere(float radius, glm::mat4 transformer, Material material);
	Sphere(const Sphere &x);
	float dist(glm::vec3 p);
};

class TriangularPrism:public Primitive
{
public:
	glm::vec3 height;
	glm::mat4 transformer;
	Material material;
	TriangularPrism(glm::vec3 height, glm::mat4 transformer, Material material);
	float dist(glm::vec3 p);
};

class Mandelbulb:public Primitive
{
	public:
	float bailout;
	float power;
	glm::mat4 transformer;
	Material material;
	int iterations;
	Mandelbulb(float bailout, float power, glm::mat4 transformer, Material material, int iterations);
	float dist(glm::vec3 p);
};

class Tetrahedron:public Primitive
{
	public:
	float scale;
	glm::mat4 transformer;
	Material material;
	int iterations;
	Tetrahedron(float scale, glm::mat4 transformer, Material material, int iterations);
	float dist(glm::vec3 p);
};



class Mengersponge:public Primitive
{
public:
	int iterations;
	float sdBox(glm::vec3 p, glm::vec3 b);
	float sdCross(glm::vec3 p);
	Material material;
	glm::mat4 transformer;
	Mengersponge( glm::mat4 transformer, Material material, int iterations);
	float dist(glm::vec3 p);
};



class Light
{
public:
	glm::vec3 pos;
	float power;
	Light(glm::vec3 pos, float power);
	float dist(glm::vec3 p);

};


class Eye
{
public:
	glm::vec3 pos;
	glm::vec3 at;
	Eye(glm::vec3 pos, glm::vec3 at);
};