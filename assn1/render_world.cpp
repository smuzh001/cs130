#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <iostream>

#include <limits>
extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<double>::max();
    Hit curr_Hit = {NULL, 0, 0};

    for (auto obj : objects) {
        Hit temp = obj->Intersection(ray, -1);
        if((temp.object && temp.dist >= small_t) && temp.dist < min_t) {
        min_t = temp.dist;
   	curr_Hit = temp;
   	}
   }
   return curr_Hit;
}

/**/// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO; // set up the initial view ray here
   //ray(endpoint , direction) remember that that direction gets normalized by the ctor() of ray 
   Ray ray(camera.position, camera.World_Position(pixel_index) - ray.endpoint);
   // 	ray.endpoint = camera.position;
   //	ray.direction = (camera.World_Position(pixel_index) - ray.endpoint).normalized();
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index, Pixel_Color(color));
}
/**/
void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection

/**/
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{   vec3 color;
	Hit Closest_Hit = {NULL,0,0}; 
	Closest_Hit = this->Closest_Intersection(ray);
	vec3 intersection_Point = ray.Point(Closest_Hit.dist);
	if(Closest_Hit.object != NULL){
	color = Closest_Hit.object->material_shader->Shade_Surface(ray, intersection_Point, Closest_Hit.object->Normal(intersection_Point, Closest_Hit.part), recursion_depth); 
	}
	else{	//all we need is the color
		color = this->background_shader->Shade_Surface(ray, color, color, recursion_depth);
	}
   // TODO; // determine the color here
    return color;
}
/**/
void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
