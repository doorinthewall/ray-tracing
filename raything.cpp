#include "raything.h"
#include "primitive.h"
#include <iostream>
#include <cmath>
#define MINDIST 0.0005f
#define FOG glm::vec3(32.f,0,32.f)
#define MAXITER 500
#define OCC_STEP 0.008f

Ray::Ray(glm::vec3 pos, glm::vec3 dir)
{
	this->pos = pos;
	this->dir = glm::normalize(dir); //normalize
	this->dist = 0.f;
}

Ray::Ray(glm::vec3 pos, glm::vec3 dir, float dist)
{
	this->pos = pos;
	this->dir = glm::normalize(dir); //normalize
	this->dist = dist;
}

Ray::Ray(const Ray& x)
{
	pos = x.pos;
	dir = x.dir;
	dist = x.dist;
}



Hit::Hit(bool exist, glm::vec3 normal, Material material, float t, int id)
{
	this->exist = exist;
	this->normal = normal;
	this->material = material;
	this->t = t;
	this->id = id;
}

Hit::Hit(bool exist)
{
	this->exist = exist;
}

Hit::Hit(const Hit& x)
{
	exist = x.exist;
	if(exist)
	{
		normal = x.normal;
		material = x.material;
		t = x.t;
		id = x.id;
	}
}


glm::vec3 EstimateNormal(glm::vec3 z, float eps, Primitive* surface)
{
	glm::vec3 z1 = z + glm::vec3(eps, 0, 0);
	glm::vec3 z2 = z - glm::vec3(eps, 0, 0);
	glm::vec3 z3 = z + glm::vec3(0, eps, 0);
	glm::vec3 z4 = z - glm::vec3(0, eps, 0);
	glm::vec3 z5 = z + glm::vec3(0, 0, eps);
	glm::vec3 z6 = z - glm::vec3(0, 0, eps);
	float dx = surface->dist(z1) - surface->dist(z2);
	float dy = surface->dist(z3) - surface->dist(z4);
	float dz = surface->dist(z5) - surface->dist(z6);
	return glm::normalize(glm::vec3(dx, dy, dz) / (2.0f*eps));
}




Hit RaySceneIntersection(Ray ray, std::vector<Primitive*> stage, int maxiter=MAXITER, float mindist=MINDIST)
{
	glm::vec3 point = ray.pos;
	float dist = 0;
	for(int i=0; i<maxiter; i++)
	{
		float min = -1.0f;
		int min_id = 0;
		for(int j = 0; j < stage.size(); j++)
		{
			if(min < 0.0f or min > stage[j]->dist(point))
			{
				min_id = j;
				min = stage[j]->dist(point);
			}
		}
		dist += min;
		point += ray.dir * min;

		if(min <= mindist)
		{	
			point = ray.pos + ray.dir * dist;
			glm::vec3 normal = EstimateNormal(point , 0.0001f ,stage[min_id]);
			return Hit(true, normal, stage[min_id]->material, dist, min_id);
		}

		//std::cout << min << std::endl;

		//std::cout << i << std::endl;

	}
	return Hit(false);
}



float Shade(glm::vec3 hit_point, glm::vec3 normal, glm::vec3 light_point)
{
	glm::vec3 dir = glm::normalize(light_point - hit_point);
	float dist = glm::length(light_point - hit_point);
	return glm::dot(dir, normal) / pow(dist, 2);
}




bool Visible(glm::vec3 hit_point, Light light, std::vector<Primitive*> stage, int id, int maxiter=MAXITER, float mindist=MINDIST)
//переписать идя из света в точку
{
	/*stage.erase(stage.begin() + id);
	if(stage.empty())
	{

		return true;
	}


	glm::vec3 dir = glm::normalize(light.pos - hit_point);
	float dist_limit = light.dist(hit_point);
	float dist = 0;
	glm::vec3 point = hit_point;
	for(int i=0; i<maxiter; i++)
	{

		float min = -1.0f;

		for(int j = 0; j < stage.size(); j++)
		{
			if(min < 0.0f or min > stage[j]->dist(point))
				min = stage[j]->dist(point);
		}

		dist += min;
		point += dir * min;

		if(min <= mindist)
	
			return false;	
		
		if(dist >= dist_limit or (dist_limit - dist) < mindist)
			return true;
	}
	return true;*/

	glm::vec3 dir = glm::normalize(hit_point - light.pos);
	float dist_limit = light.dist(hit_point);
	float dist = 0;
	glm::vec3 point = light.pos;
	for(int i=0; i<maxiter; i++)
	{
		float min = -1.0f;
		for(int j = 0; j < stage.size(); j++)
		{
			if(min < 0.0f or min > stage[j]->dist(point))
				min = stage[j]->dist(point);
		}

		point += dir * min;
		dist += min;


		if(min <= mindist)
		{
			if(min + 0.005f < glm::length(hit_point - point))
				return false;

			return true;	
		}

		if(dist >= dist_limit or (dist_limit - dist) < mindist)
			return true;
	}


}


Ray reflect(const Ray& ray, const Hit& hit)
{
	glm::vec3 pos = ray.pos + ray.dir*hit.t;
	glm::vec3 dir = ray.dir + 2.f*((-hit.normal)*ray.dir)*hit.normal;
	return Ray(pos, dir, hit.t + ray.dist);
}


glm::vec3  RayTrace(const Ray& ray, std::vector<Primitive*> stage, std::vector<Light> lights)
{
	 glm::vec3  color(0,0,0);
	 Hit hit = RaySceneIntersection(ray, stage);
	 if (!hit.exist)
	 	return FOG;
	 glm::vec3  hit_point = ray.pos + ray.dir*hit.t;

	 //std::cout << hit.material.color[0] << hit.material.color[1] << hit.material.color[2] << std::endl;

	 
	 for(int i=0;i < lights.size();i++)
	 {
	 	float shade =  Shade(hit_point, hit.normal, lights[i].pos);
	 	if(shade > 0)
	 	{
		 	if(Visible(hit_point, lights[i], stage, hit.id))
		 	{
		 		
		 		
		 		//std::cout << shade << std::endl;
		 		color += 100* lights[i].power *shade*hit.material.color; 
		 	}
		}
	 }


	// std::cout << ray.dist << std::endl; 

	 
	 if (hit.material.reflection > 0 and ray.dist < 100.f)
	 {
	 	 //std::cout << "here" << std::endl;
		 Ray reflRay = reflect(ray, hit);
		 float min = -1.0f;
		 for (int i = 0; i < stage.size(); i++) {
		 	if (i == hit.id)
		 		continue;
		 	float dist = stage[i]->dist(hit_point);
		 	if(min < 0 or min > dist)
		 		min = dist;
		 }
		 reflRay.pos += reflRay.dir * min;
		 reflRay.dist += min;

		 color += hit.material.reflection*RayTrace(reflRay, stage, lights);
	 }
	 
	 /*
	 if (hit.material.refraction > 0)
	 {
		 Ray refrRay = refract(ray, hit);
		 color += hit.material.refraction*RayTrace(refrRay);
	 }
	 */
	 float coef = hit.exist ? 1.f/(exp(hit.t * 0.005f)) : 0.f;
	 color = coef * color + (1-coef)*FOG;

	 coef = 0.f;
	 float norm = 0.f;
	 for(int i = 1; i <= 10; i++)
	 {
	 	float gamma = 0.9;
	 	glm::vec3 point(hit_point + hit.normal*OCC_STEP);
	 	float min = i * OCC_STEP;
		for(int j = 0; j < stage.size(); j++)
		{
			
			if(stage[j]->dist(point) > 0 and min > stage[j]->dist(point))
			{
				min = stage[j]->dist(point);
			}	
		
		}
		coef += pow(gamma, (i-1))*(i * OCC_STEP - min);
		norm += pow(gamma, (i-1))*i * OCC_STEP ;
	 }
	 return color * (1-coef);
}


Image InitRay(Eye eye, std::vector<Primitive*> stage, std::vector<Light> lights)
{
	int v_side = 1000;
	int h_side = 1000;
	float offset = 2.5f;
	

	float vstep = 0.003;
	float hstep = 0.003;
	Matrix<glm::vec3> result((uint)v_side, (uint)h_side);
	glm::mat4 trans(inverse(glm::lookAt(eye.pos, eye.at, glm::vec3(0,1,0))));//матрица перехода из базиса камеры,
	// камера направлена по -Z  https://alexander-bobkov.ru/opengl/tutorials/tutorial3


	#pragma omp parallel for collapse(2) schedule(dynamic, 512)
	for(int i = 0; i < h_side; i++)
	{
		for(int j = 0; j < v_side; j++)
		{
			

			glm::vec4 aid4(1000*hstep*(i-h_side/2)/h_side,1000*vstep*(j-v_side/2)/v_side, -offset, 0.0f);

			/*


			for(int k =0; k<4; k++)
			{
				std::cout << aid4[k] << " ";
			}
 			*/
			//std::cout << std::endl;

			glm::vec3 point(trans*aid4);
			/*
			for(int k =0; k<3; k++)
			{
				std::cout << point[k] << " ";
				//std::cout << 
			}
 
			std::cout << std::endl;

			*/
			Ray ray(eye.pos, point);
			glm::vec3 color = glm::min(glm::max(RayTrace(ray, stage, lights), 0.f), 255.f);

			result(i,j) = color;

		}
	}

	Image final((uint)v_side/2,(uint)h_side/2);

	for(int i = 0; i < h_side-1; i+=2)
	{
		for(int j = 0; j < v_side-1; j+=2)
		{
			glm::vec3 color = (result(i,j) + result(i+1,j+1) + result(i+1,j) + result(i,j+1));
			final(i/2, j/2) = std::make_tuple(color[0]/4, color[1]/4, color[2]/4);
		}
	}


	return final;
}