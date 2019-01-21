#include "sphere.h"
#include "ray.h"
#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    TODO;
	double a = dot(ray.direction, ray.direction);
	double b = 2 * dot(( ray.endpoint - this->center ) ,ray.direction);
	double c = dot((ray.endpoint - this->center), (ray.endpoint - this->center)) - (this->radius *this->radius);
	if ( (b*b)-(4 * a * c) >= 0){  	
	double t = ((-1)*b + pow((b*b) - (4 * a * c) , 0.5)) / (2 * a);
	return {this, t, part};
	}
	else{
    		return {0,0,0};
	}

}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    TODO; // compute the normal direction
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
