#include "primitive.h"
#include <iostream>

	Material::Material(glm::vec3 color)
	{
		this->color = color;
		this->reflection = 0;
	}

	Material::Material(glm::vec3 color, float reflection)
	{
		this->color = color;
		this->reflection = reflection;
	}

	Material::Material(const Material& x)
	{
		color = x.color;
		this->reflection=x.reflection;
		//std::cout << "here" << std::endl;
	}

	Material::Material()
	{

		//std::cout << "here2" << std::endl;
	}

	Primitive::Primitive(Material material)
	{
		this->material = material;
	}

	Primitive::Primitive()
	{

	}


	Box::Box(glm::vec3 edges, glm::mat4 transformer, Material material) :
		Primitive(material)
	{
		this->edges = edges;
		this->transformer = glm::inverse(transformer);
		// this->material = material;
	}

	Box::Box(const Box &x)
	{
		this->edges = x.edges;
		this->transformer = x.transformer;
		this->material = x.material;
	}	


	float Box::dist(glm::vec3 p)
	{
		glm::vec4 v(p, 1.0f);
		glm::vec3 v3(transformer*v);
		return glm::length(glm::max(glm::abs(v3)-edges,0.0f));
	}

	Sphere::Sphere(float radius, glm::mat4 transformer, Material material) :
		Primitive(material)
	{
		this->radius = radius;
		this->transformer = glm::inverse(transformer);
		//this->material = material;
	}

	Sphere::Sphere(const Sphere &x)
	{
		this->radius = x.radius;
		this->transformer = x.transformer;
		this->material = x.material;
	}	

	float Sphere::dist(glm::vec3 p)
	{
		glm::vec4 v(p, 1.0f);
		glm::vec3 v3(transformer*v);

		return glm::length(v3)-radius;
	}

	TriangularPrism::TriangularPrism(glm::vec3 height, glm::mat4 transformer, Material material) :
		Primitive(material)
	{
		this->height = height;
		this->transformer = glm::inverse(transformer);
		//this->material = material;
	}

	float TriangularPrism::dist(glm::vec3 p)
	{
		glm::vec4 v(p, 1.0f);
		glm::vec3 v3(transformer*v);
		glm::vec3 q = glm::abs(v3);
    		return glm::max(q.z-height.y,glm::max(q.x*0.866025f+v3.y*0.5f,-v3.y)-height.x*0.5f);
	}


	Mandelbulb::Mandelbulb(float bailout, float power, glm::mat4 transformer, Material material, int iterations=10):
		Primitive(material)
	{
		this->bailout = bailout;
		this->power = power;
		this->transformer = glm::inverse(transformer);
		this->iterations = iterations;
		//this->material = material;
	}

	float Mandelbulb::dist(glm::vec3 p)
	{
		glm::vec4 v(p, 1.0f);
		glm::vec3 pos(transformer*v);
		glm::vec3 z = pos ;
		float dr = 1.0;
		float r = 0.0;
		for (int i = 0; i < iterations ; i++) {
			r = glm::length(z) + 0.0001;
			if (r>bailout) break;
			
			// convert to polar coordinates
			float theta = acos(z.z/r);
			float phi = atan(z.y/z.x);
			dr =  pow( r, power-1.0)*power*dr + 1.0;
			
			// scale and rotate the point
			float zr = pow( r,power);
			theta = theta*power;
			phi = phi*power;
			
			// convert back to cartesian coordinates
			z = zr*glm::vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
			z+=pos;
		}
		return 0.5*log(r)*r/dr;
	}


	Tetrahedron::Tetrahedron(float scale, glm::mat4 transformer, Material material, int iterations):
		Primitive(material)
	{
		this->scale=scale;
		this->transformer = glm::inverse(transformer);
		this->iterations = iterations;
		//this->material = material;
	}


	float Tetrahedron::dist(glm::vec3 p)
	{
		glm::vec4 v(p, 1.0f);
		glm::vec3 z(transformer*v);
		glm::vec3 a1 = glm::vec3(1,1,1);
		glm::vec3 a2 = glm::vec3(-1,-1,1);
		glm::vec3 a3 = glm::vec3(1,-1,-1);
		glm::vec3 a4 = glm::vec3(-1,1,-1);
		glm::vec3 c;
		int n = 0;
		float dist, d;
		while (n < iterations) {
			 c = a1; dist = glm::length(z-a1);
		        d = glm::length(z-a2); if (d < dist) { c = a2; dist=d; }
			 d = glm::length(z-a3); if (d < dist) { c = a3; dist=d; }
			 d = glm::length(z-a4); if (d < dist) { c = a4; dist=d; }
			z = scale*z-c*(scale-1.0f);
			n++;
		}

		return glm::length(z) * pow(scale, float(-n));
	}




	float Mengersponge::sdBox(glm::vec3 p, glm::vec3 b)
	{
		glm::vec3 d = glm::abs(p) - b;
  		return std::min(std::max(d.x,std::max(d.y,d.z)),0.0f) + glm::length(glm::max(d,0.0f));
	}


	float Mengersponge::sdCross(glm::vec3 p)
	{
	  float da = sdBox(p,glm::vec3(std::numeric_limits<float>::infinity(),1.0f,1.0f));
	  float db = sdBox(glm::vec3(p.y, p.z, p.x),glm::vec3(1.0f,std::numeric_limits<float>::infinity(),1.0f));
	  float dc = sdBox(glm::vec3(p.z, p.x, p.y),glm::vec3(1.0f,1.0f,std::numeric_limits<float>::infinity()));
	  return glm::min(da,glm::min(db,dc));
	}

	Mengersponge::Mengersponge( glm::mat4 transformer, Material material, int iterations):
		Primitive(material)
	{
		this->transformer = glm::inverse(transformer);
		//this->material = material;
		this->iterations = iterations;	
	}

	float Mengersponge::dist(glm::vec3 p)
	{
	   float d = sdBox(p,glm::vec3(1.0f));

	   float s = 1.0f;
	   for( int m=0; m<3; m++ )
	   {
	      glm::vec3 a = glm::mod( p*s, 2.0f )-1.0f;
	      s *= 3.0f;
	      glm::vec3 r = 1.0f - 3.0f*glm::abs(a);

	      float c = sdCross(r)/s;
	      d = std::max(d,-c);
	   }

	   return d;
	}


	float Light::dist(glm::vec3 p)
	{
		return glm::length(pos-p);
	}

	Light::Light(glm::vec3 pos, float power = 1.0f)
	{
		this->pos = pos;
		this->power = power;
	}

	Eye::Eye(glm::vec3 pos, glm::vec3 at)
	{
		this->pos = pos;
		this->at = at;
	}
