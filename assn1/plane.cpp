#include "plane.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{	//by solving t = ((x0 -e) * n) / (u * n) you need to make sure u * n doesnt equal 0
    	Hit PlaneHit{NULL,0,-1};

	double UdotN = dot(ray.direction, normal);
	//the ray must not be perpendicular to the plane, otherwise it is not a hit.
	if(!UdotN){
		return PlaneHit;	
	}
	double t = (dot((this->x1 - ray.endpoint),this->normal) / dot(ray.direction, this->normal));
	//if t is less than the threshold of a valid hit, then we don't consider it
	if(t < small_t){
		return PlaneHit;
	}
	else{
		PlaneHit.object = this;
		PlaneHit.dist = t;
		return PlaneHit;
	}
   //TODO;
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}
