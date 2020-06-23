#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

#include <iostream>
using namespace std; 

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color, R, I_d, I_s, I_a, r, light_pos;
    double max_t = std::numeric_limits<int>::max();

    for (int i =0; i < world.lights.size(); ++i) {

    	// initialize ray, light position/direction, and get what objects are hit
    	light_pos = world.lights[i]->position;
    	R = world.lights[i]->Emitted_Light(light_pos-intersection_point);
    	Ray l_to_ip(intersection_point, light_pos-intersection_point);
    	vec3 ldir = light_pos-intersection_point;
	    Hit h = world.Closest_Intersection(l_to_ip);
	    float ldist= sqrt(pow(ldir[0], 2.0f) + pow(ldir[1], 2.0f) + pow(ldir[2], 2.0f)); 

	    // total ambient color 
		I_a = color_ambient * world.ambient_color * world.ambient_intensity; 

		// enable shadows
		if ((h.dist >= small_t && h.dist <= max_t && ldist > h.dist) && world.enable_shadows ) {
			// shadow 
			// color = {1,1,1};
		}
		else {
			//diffuse
		    double m = dot(normal, (light_pos-intersection_point).normalized());
		    m = m>0 ? m : 0;
		    I_d = color_diffuse * R * m;

		    //specular
		    r = (2.0 * dot((light_pos-intersection_point).normalized(), normal) * normal) - (light_pos-intersection_point).normalized(); 
		    m = dot(r, (world.camera.position-intersection_point).normalized());
			m = m>=0 ? m : 0;
			m = pow(m,specular_power);
		    I_s = color_specular * R * m; 
		    if(debug_pixel) cout << "**debugger specular " << R << "  ,  " << m << "  ,  " << color_specular << endl; 
		    if (debug_pixel) cout << "**debugger phong color " << I_d << "  ,  " << I_s << endl; 
		    color += I_d + I_s;  
		}

		
	}
	
	// total color = ambient + diffuse + specular
	color +=  I_a; 
    return color;
}

