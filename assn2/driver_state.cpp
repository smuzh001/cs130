#include "driver_state.h"
#include <cstring>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_color=0;
    state.image_depth=0;
	state.image_color = new pixel[width * height]; 
	//set each pixel in our render to be black
	for(int i = 0; i < width * height; i++){
		state.image_color[i] = make_pixel(0,0,0);
	}


    //std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;
}

// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
data_geometry* Geometry;
data_vertex dv;
int index = 0;
int NumberOfTriangles = 0;

	switch(type){
		case render_type::triangle :
			Geometry = new data_geometry[3];
			NumberOfTriangles = state.num_vertices / 3;
			for(int j = 0; j > NumberOfTriangles; j++){
				for(int i = 0; i < 3; i++){
					//vertex_data * floats_per_vertex = size of 1 vertex entry in array
					//you are just setting each pointer to point to the specific start of each new vertex 
					Geometry[i].data = state.vertex_data + index;
					//handles the vertex shader which sets the vec4 position for each vertex in our data_geometry
					dv.data = Geometry[i].data;
					state.vertex_shader(dv, Geometry[i], state.uniform_data);
					//you increment your index by the size of floats per vertex so the next data_geometry will point to the next vertex
					index += state.floats_per_vertex;
				}	
				rasterize_triangle(state,(const data_geometry**)&Geometry);
				//rasterize_triangle(state,(const)&Geometry);

			}
			delete[] Geometry;
			break;
		case render_type::indexed:
			break;
		case render_type::fan:
			break;
		case render_type::strip:
 			break;
		default: printf("invalid type\n");
		
	}

// std::cout<<"TODO: implement rendering."<<std::endl;
}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry* in[3],int face)
{
    if(face==6)
    {
        rasterize_triangle(state, in);
        return;
    }
    std::cout<<"TODO: implement clipping. (The current code passes the triangle through without clipping them.)"<<std::endl;
    clip_triangle(state,in,face+1);
}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry* in[3])
{	//equations given to us from class
	int x[3],y[3];
	float k0[3], k1[3], k2[3], ABC, alpha, beta, gamma, w, h;
	
	w = state.image_width / 2.0;
	h = state.image_height / 2.0;

	//i = xprime = w/2 + (w/2 -1/2)
	//j = yprime = h/2 + (h/2 - 1/2)
	for(int k = 0; k < 3; k++){
		x[k] = (int)(w * in[k]->gl_Position[0] + (w - 0.5));
		y[k] = (int)(h * in[k]->gl_Position[1] + (h - 0.5));	

	}
	//with i and j we can plug in to solve for alpha
	//using area of the triangle formula that is given to us
	//area(ABC) = 0.5 * ((bxcy - bycx) - (axcy - cxay) - (bxcy - cxby))
	ABC = 0.5 * ((x[1] * y[2] - x[2]*y[1]) - (x[0] * y[2] - x[2] * y[0]) - (x[1] * y[2] - x[2] * y[1]) );
	
	//area(P,B,C) = k0 + k1y + k2y
	//from slides k0 = area(oBC) / ABC, k0 + k1 = area(e1BC)/ABC, k0 + k2 area(e2,B,C)/ABC
	//where o = (0,0) e1 = (1,0) e2 = (0,1)
	k0[0] = x[1] * y[2] - x[2] * y[1];
	k0[1] = x[2] * y[0] - x[0] * y[2];
	k0[2] = x[0] * y[1] - x[1] * y[0];

	k1[0] = y[1] - y[2];
	k1[1] = y[2] - y[0];
	k1[2] = y[0] - y[1];

	k2[0] = x[1] - x[2];
	k2[1] = x[2] - x[0];
	k2[2] = x[0] - x[1];
	//determine if each pixel is within triangle via calculating the barycentric weights
	// this is done by iterating through each pixel (i,j)
	for(int j = 0; j < state.image_height; j++){
		for( int i = 0; i < state.image_width; i++){
			alpha = 0.5 * ( k0[0] + (k1[0] * i) + (k2[0] * j) ) / ABC;
			beta  = 0.5 * ( k0[1] + (k1[1] * i) + (k2[1] * j) ) / ABC;
			gamma = 0.5 * ( k0[2] + (k1[2] * i) + (k2[2] * j) ) / ABC;
			
			//if the pixel is within the barycentric coordinates
			if(alpha + beta + gamma == 1){
				state.image_color[i + j * state.image_width] = make_pixel(255,255,255);

			}

		}
	}

	
    //std::cout<<"TODO: implement rasterization"<<std::endl;
}

