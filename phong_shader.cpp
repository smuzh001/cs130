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

    // Use the world's ambient color and intensity to determine ambient
    // light only once
    Iamb = color_ambient * world.ambient_color * world.ambient_intensity;
    // iterate through world lights
    for (unsigned i = 0; i < world.lights.size(); i++) { 
        vec3 l = world.lights[i]->position - intersection_point;
        vec3 r = (-l + 2 * dot(l, normal) * normal).normalized();                             
	Idif += color_diffuse * world.lights[i]->Emitted_Light(l) * std::max(dot(normal, l.normalized()), 0.0);
    
                                                                                              Ispec += color_specular * world.lights[i]->Emitted_Light(l) * std::pow(std::max(dot(r, -(ray.direction)), 0.0), specular_power);
        }
	return Iamb + Idif + Ispec;
                                                                                                                                                                            }
