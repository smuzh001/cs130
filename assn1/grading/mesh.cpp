#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>
#include "plane.h"

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
	Hit Tri_Hit {NULL, 0, 0};
	if(part >= 0){
		if(Intersect_Triangle(ray, part, Tri_Hit.dist)){
			Tri_Hit.object = this;
			Tri_Hit.part = part;
		}
	}
	else{	
		//iterate through all triangles on our mesh, return the triangle with the smallest distance to intersection.
		Tri_Hit.dist = std::numeric_limits<double>::max(); 
		for(unsigned i = 0; i < triangles.size(); i++){
			double temp;
			if(Intersect_Triangle(ray, i, temp))
				if(temp < Tri_Hit.dist){
					Tri_Hit.object = this;
					Tri_Hit.dist = temp;
					Tri_Hit.part = i;
				}
		}
	}
	
   

// TODO;
    return Tri_Hit;
}

// Compute the normal direction for the triangle with index part.
/**/
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
	vec3 A = vertices[triangles[part][0]];
	vec3 B = vertices[triangles[part][1]];
	vec3 C = vertices[triangles[part][2]];
	vec3 N = cross((A-B),(A-C)).normalized();
	return N;
//    TODO;

}
/**/

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
//    TODO
	vec3 A = vertices[triangles[tri][0]];
	vec3 B = vertices[triangles[tri][1]];
	vec3 C = vertices[triangles[tri][2]];

	Plane p(A, Normal(B,tri));
	Hit tri_hit = p.Intersection(ray, tri);
	//quick check that the ray is not parallel to the triangle
	if(!tri_hit.object){
		return false;
	}
	vec3 Intersection_Point = ray.Point(dist);
	//using the eq givin in class y = Beta(v) + Gamma(w) - ut
	vec3 u = ray.direction;
	vec3 v = B - A;
	vec3 w = C - A;
	vec3 y = Intersection_Point - A;
	double denominator = dot(cross(u,v),w);
	// u X v * w == w X u * v == v X w * u
	//so since they are all equal, we can reuse one for solving beta plus check
	//that denominator != 0
	if(!denominator)
		return false;

	double Gamma = dot(cross(u,v), y) / denominator;
	double Beta = dot(cross(w,u), y) / denominator;
	double Alpha = 1 - Beta - Gamma;

	if(Alpha > -weight_tol && Beta > -weight_tol && Gamma > -weight_tol){
		dist = tri_hit.dist;
		return true;
	}


    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
