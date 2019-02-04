#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <cmath>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color; 
    vec3 Iamb; 
    vec3 Idif;
    vec3 Ispec;
    vec3 v = -(ray.direction);
    color = Ispec = world.ambient_color * world.ambient_intensity * color_ambient;


    //using the equations the professor wrote in class
    for (unsigned i = 0; i < world.lights.size(); i++) { 
        vec3 l = world.lights[i]->position - intersection_point;
        vec3 r = (-l + 2 * dot(l, normal) * normal).normalized();
       
        //Idif = L * R * max(n * l, 0)
	Idif = world.lights[i]->Emitted_Light(l) * color_diffuse * std::max(dot(normal, l.normalized()),0.0);
       // Ispec = L * R * max(r * v, 0)^alpha
        Ispec = world.lights[i]->Emitted_Light(l) * color_specular * std::pow(std::max(dot(r, v),0.0),specular_power);
    	color += Idif + Ispec;
	}

    return color;
    //return intense_amb + intense_dif + intense_spec;
}
