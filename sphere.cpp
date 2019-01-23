#include "sphere.h"
#include "ray.h"
#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
   // TODO;
	double a = dot(ray.direction, ray.direction);
	double b = 2 * dot(( ray.endpoint - this->center ) ,ray.direction);
	double c = dot((ray.endpoint - this->center), (ray.endpoint - this->center)) - (this->radius *this->radius);
	double d = (b*b)- (4 * a * c);
	
	if (d < 0){
		return {NULL, 0, 0};
	}  	
	double t1 = ((-1)*b + pow(d , 0.5)) / (2 * a);
	double t2 = ((-1)*b + pow(d, 0.5)) / (2 * a);
	if(t1 < t2 && t1 >= small_t){
		return {this, t1, part};
	}
	else if(t2 <= t1 && t2 >= small_t){
		return {this, t2, part};
	}
	else{
    		return {NULL,0,0};
	}

}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
   // TODO; // compute the normal direction
	//according to equation in book.
    	normal = (point - this->center) / radius;
	return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
