//Stanley Muzhuthettu 861305506
#include "driver_state.h"
#include <cstring>
#include <limits>


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
	//you need to initialize the depth per each pixel
	state.image_depth = new float[width * height]; 
	//set each pixel in our render to be black
	for(int i = 0; i < width * height; i++){
		state.image_color[i] = make_pixel(0,0,0);
		//initialize the depth as the highest float, youll update later
		state.image_depth[i] = std::numeric_limits<float>::max();
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

/**/
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

			for(int j = 0; j < NumberOfTriangles; j++){
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
	float z[3], k0[3], k1[3], k2[3], ABC, alpha, beta, gamma, Aprime, Bprime, Gprime, k, wA, wB, wC;	
	float  w = state.image_width / 2.0f;
	float  h = state.image_height / 2.0f;
	data_fragment FragShader;
	data_output pixel_color;
	FragShader.data = new float[MAX_FLOATS_PER_VERTEX];
	
    //i = xprime = w/2 + (w/2 -1/2)
	//j = yprime = h/2 + (h/2 - 1/2)
	//conversion to homogenous coordinates
	for(int i = 0; i < 3; i++){
			x[i] = static_cast<int>(w * (*in)[i].gl_Position[0]/(*in)[i].gl_Position[3] + (w + 0.5f));
			y[i] = static_cast<int>(h * (*in)[i].gl_Position[1]/(*in)[i].gl_Position[3] + (h + 0.5f));	

	}
	
	//with i and j we can plug in to solve for alpha
	//using area of the triangle formula that is given to us
	//area(ABC) = 0.5 * ((bxcy - bycx) - (axcy - cxay) - (bxcy - cxby))
	ABC = 0.5f * ((x[1] * y[2] - x[2]*y[1]) - (x[0] * y[2] - x[2] * y[0]) + (x[0] * y[1] - x[1] * y[0]) );

   	k0[0] = x[1] * y[2] - x[2] * y[1];
    	k1[0] = y[1] - y[2];
   	k2[0] = x[2] - x[1];

    	k0[1] = x[2] * y[0] - x[0] * y[2];
    	k1[1] = y[2] - y[0];
    	k2[1] = x[0] - x[2];

    	k0[2] = x[0] * y[1] - x[1] * y[0];
    	k1[2] = y[0] - y[1];
    	k2[2] = x[1] - x[0];
	
	//determine if each pixel is within triangle via calculating the barycentric weights
	// this is done by iterating through each pixel (i,j)
	//for(int j = 0; j < state.image_height; j++){
	//	for( int i = 0; i < state.image_width; i++){
	for(int j = 0; j < state.image_height; j++){
		for(int i = 0; i < state.image_width; i++){
			alpha = 0.5f * ( k0[0] + (k1[0] * i) + (k2[0] * j) ) / ABC;
			beta  = 0.5f * ( k0[1] + (k1[1] * i) + (k2[1] * j) ) / ABC;
			gamma = 0.5f * ( k0[2] + (k1[2] * i) + (k2[2] * j) ) / ABC;
			
			//calculate the z-buffer of each vertice then determine the distance among the z axis
			//currently not working
			/**/
			float dist = 0;
			for(int k = 0; k < 3; k++){
			z[k] = (*in)[k].gl_Position[2] / (*in)[k].gl_Position[3];
			}
			dist = (alpha * z[0]) + (beta * z[1]) + (gamma * z[2]);
			/**/
			//check if the pixel is within the barycentric coordinates
			//if pixel is within, we know build color.				
		//	if((alpha > 0 && alpha < 1)&& (beta > 0 && beta < 1) && (gamma > 0 && gamma < 1)){
	
			if((alpha > 0 && alpha < 1)&& (beta > 0 && beta < 1) && (gamma > 0 && gamma < 1) && dist < state.image_depth[i + j * state.image_width]){
				//state.image_color[i + j * state.image_width] = make_pixel(255,255,255);
				//initialize your weights primes
				Aprime = alpha, Bprime = beta, Gprime = gamma;

				for(int i = 0; i < state.floats_per_vertex; i++){
					switch(state.interp_rules[i]){
						
						case interp_type::flat:
							FragShader.data[i] = (*in)[0].data[i];
							break;
						case interp_type::smooth:
							//we need perspective correct interpolation. First we convert our weights from screen space to world space
							//using the given equations on the perspective correct interpolation
							wA = (*in)[0].gl_Position[3];
							wB = (*in)[1].gl_Position[3];
							wC = (*in)[2].gl_Position[3];
							k = (Aprime / wA) + (Bprime / wB) + (Gprime / wC);
							alpha = Aprime / (k * wA);
							beta = Bprime / (k * wB);
							gamma = Gprime / (k * wC);
							//once we are in world space we implement the interpolation rule.
							FragShader.data[i] = (alpha * (*in)[0].data[i]) + (beta * (*in)[1].data[i]) + (gamma * (*in)[2].data[i]);
							break;
						case interp_type::noperspective:
							FragShader.data[i] = (alpha * (*in)[0].data[i]) + (beta * (*in)[1].data[i]) + (gamma * (*in)[2].data[i]);
							break;
						
						default:
							break; 

						
						}
				}
				state.fragment_shader(FragShader, pixel_color, state.uniform_data);
				//the rgb values of pixel color are > 1, thus we must multiply the value by 255 to get the correct rgb value.
				state.image_color[i + j * state.image_width] = make_pixel((pixel_color.output_color[0] * 255), (pixel_color.output_color[1] * 255),(pixel_color.output_color[2] * 255));
				state.image_depth[i + j * state.image_width] = dist;
		
			}
		}
	}
	delete[] FragShader.data;
    //std::cout<<"TODO: implement rasterization"<<std::endl;
}

