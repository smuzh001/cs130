#include "sphere.h"
#include "ray.h"
#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
   // TODO;
	Hit SphereHit{NULL, 0, -1};

	double a = ray.direction.magnitude_squared(); 
	double b = 2 * dot(( ray.endpoint - this->center ) ,ray.direction);
	double c = ((ray.endpoint - center).magnitude_squared()) - (radius * radius);
	double d = (b*b) - (4 * a * c);
	//checks for non complex numbers
	if (d < 0){
		return SphereHit;
	}  	
	double t1 = (-b + sqrt(d)) / (2 * a);
	double t2 = (-b - sqrt(d)) / (2 * a);
	if(t1 < t2 && t1 >= small_t){
		SphereHit.object = this;
		SphereHit.dist = t1;
		
	}
	else if(t2 <= t1 && t2 >= small_t){
		SphereHit.object = this;
		SphereHit.dist = t2;
	}
	return SphereHit;


}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
   // TODO; // compute the normal direction
	//according to equation in book.
    	normal = (point - this->center).normalized();
	return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
