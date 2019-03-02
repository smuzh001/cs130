#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
	//initialize our color to surface color from phong.
	color = shader->Shade_Surface(ray,intersection_point, normal, recursion_depth);
	vec3 v = ray.direction; 
	//solve for r from drawing model	
	vec3 r = v + 2 * dot((-1.0)*v,normal) * normal;
	Ray reflection_ray(intersection_point, r);
	//color = (1-k) * color + k * cast_ray(reflected ray, recursion_depth -1)
	//Note: Cast_Ray calls shade_surface so this is a recursive call, you need to set a base case to end the recursion which
	//is given as recursion_depth_limit in our world pointer.
	if(recursion_depth != world.recursion_depth_limit){
		return color = ((1-reflectivity) * color) + reflectivity * world.Cast_Ray(reflection_ray, recursion_depth + 1);
}//    TODO; // determine the color
	//because this is still a reflective shader there is still the (1-k) component being multiplied by our component.
    	else return (1-reflectivity) * color;
}
