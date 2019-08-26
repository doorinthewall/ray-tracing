#include "primitive.h"
#include "raything.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include "matrix.h"
#include "io.h"

using namespace std;

int main()
{

	//glm::vec3 sm =  2.f * glm::vec3(0.1, 0.1, 0.1);

	//return 0;

	Material gold(glm::vec3(255,215,0), 0.8f);
	Material steel(glm::vec3(224, 223, 219), 0.8f);
	Material sky(glm::vec3(51, 184, 182), 0.01f);
	Material copper(glm::vec3(184, 82, 51), 0.3f);

	Material gravel(glm::vec3(136, 221, 191), 0.f);

	glm::mat4 trans(glm::translate(glm::mat4(1.0f),glm::vec3(0,0,20)));

	


	Sphere rr(5.f, trans, steel);

	Sphere ar1(1.f, glm::translate(glm::mat4(1.0f),glm::vec3(5.f,3.4f,18.f)), steel);

	//Sphere ar2(6.f, glm::translate(glm::mat4(1.0f),glm::vec3(3.4f,3.f,18.f)), steel);


	Sphere rr1(5.f, glm::translate(glm::mat4(1.0f),glm::vec3(5,5,30)), gold);

	Box bb(glm::vec3(0.1f,1,5), glm::translate(glm::mat4(1.0f),glm::vec3(10.6f,5.f,30)), steel);

	Box bb1(glm::vec3(1000,1000,0.5), glm::translate(glm::mat4(1.0f),glm::vec3(0.f,0.f,100)), sky);

	TriangularPrism pr(glm::vec3(2,5,10), glm::translate(glm::mat4(1.0f),glm::vec3(10.6f,-3.f,23)), copper);

	Mandelbulb mb(5.f, 8.f, glm::mat4(1.0f), gravel, 10);


	Tetrahedron tet(2.f, glm::mat4(1.0f), gravel, 15);

	Mengersponge men(glm::mat4(1.0f), gravel, 3);
	
	

	std::vector<Primitive*> stage; // сцена


	stage.push_back(&rr);
	stage.push_back(&rr1);

	stage.push_back(&bb);

	stage.push_back(&pr);


	stage.push_back(&bb1);



	stage.push_back(&ar1);


	//stage.push_back(&mb);

	//stage.push_back(&tet);

	//stage.push_back(&men);

//end



	//stage.push_back(&ar2);

	//stage.push_back(&bb);

	//cout << "------" << endl;

	//Primitive* a = &rr;

	//cout << a->material.color[0] << a->material.color[1] << a->material.color[2] << endl;
	//cout << (*stage[0]).material.color[0] << (*stage[0]).material.color[1] << (*stage[0]).material.color[2] << endl;
	//cout << rr.material.color[0] << rr.material.color[1] << rr.material.color[2] << endl;

	//return 0;

	Light light1(glm::vec3(-20.f,-20.f,5.f), 1.5f), light2(glm::vec3(0,0, 5.f), 0.3f);

	vector<Light> lights;
	lights.push_back(light1);
	lights.push_back(light2);

	//cout << bb.dist(light2.pos) << endl;


	glm::vec3 eyepos(4.f,0.5f,5.f);
	Eye eye(eyepos, glm::vec3(5,5,50));//наблюдатель

	//glm::vec3 eyepos(0.f,-1.0f, -3.f);
	//Eye eye(eyepos, glm::vec3(0,0, 0));//наблюдатель

/*
	Ray test = Ray(eyepos, glm::vec3(0,0,1) - eyepos);
	Hit hh = RaySceneIntersection(test, stage, 10, 0.001f);
	glm::vec3 hit_po = test.pos + test.dir*hh.t;
	std::cout << hit_po.x << " " << hit_po.y <<  " " << hit_po.z << std::endl;*/

	//return 0;

	lights.push_back(Light(eyepos, 0.1f));

	lights.push_back(Light(glm::vec3(0,0,80), 10));


	lights.push_back(Light(glm::vec3(20,20,80), 1.5));

	lights.push_back(Light(glm::vec3(-20,20,80), 1.5));


	lights.push_back(Light(glm::vec3(-50,40,80), 1.5));
	
	lights.push_back(Light(glm::vec3(11,2,25), 0.1));

	cout << stage.size() << " " << stage[0]->dist(eyepos) << endl;
	//cout << bb.dist(eye.pos) << endl;


	Image result = InitRay(eye, stage, lights);

	save_image(result, "resorigin.bmp");
	
	return 0;
}
