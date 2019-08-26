#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "primitive.h"

#include "io.h"
#include "matrix.h"


class Ray
{
public:
	glm::vec3 pos;
	glm::vec3 dir;
	float dist;
	Ray(glm::vec3 pos, glm::vec3 dir);
	Ray(glm::vec3 pos, glm::vec3 dir, float dist);
	Ray(const Ray& x);
};



class Hit
{
public:
	bool exist;
	glm::vec3 normal;
	Material material;
	float t;
	int id;
	Hit(bool exist,	glm::vec3 normal, Material material, float t, int id);
	Hit(bool exist);
	Hit(const Hit& x);
};





Image InitRay(Eye eye, std::vector<Primitive*> stage, std::vector<Light> lights);

glm::vec3 RayTrace(const Ray& ray);

Hit RaySceneIntersection(Ray ray, std::vector<Primitive*> stage, int maxiter, float mindist);

glm::vec3 EstimateNormal(glm::vec3 z, float eps, Primitive* surface);

float Shade(glm::vec3 hit_point, glm::vec3 normal, glm::vec3 light_point);

bool Visible(glm::vec3 hit_point, Light light, std::vector<Primitive*> stage, int id, int maxiter, float mindist);