
// for regular C header
//#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include "malloc.h"
#endif
#include "time.h"
// #include "tchar.h"

// for private header
#include "conf_util.h"
#include "var_def.h"			// define parameters and structe variables
#include "plt_figure.h"			// call gnuplot for ploting figure
#include "ini_var.h"			// initialize variables
#include "mem_func.h"			// allocate/deallocate memory 
#include "microstr.h"			// local microstructure profile
#include "gen_source_ray.h"		// generate light-source rays
#include "globalstr.h"
#include "den_to_pos.h"			// module for transferring dot_density to dot_position 
#include "debug_fun.h"			// just for debugging individual module.
#include "Module_IV.h"
#include "dbg_log.h"
#include "time_util.h"
#include "data_file_util.h"
#include "ray_tracer.h"

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;

void hanldOneRay(ray_trace1 *ray, dot_position *dpos, opt_record *opr, local_str *lstr)
{
	int i, j;
	int type;
	struct ray_trace1 output_ray[2][MAX_OUTPUT_RAY];
	struct ray_trace1 src_ray[2];

	ray->ngaus 	= 1; 
	// ray->inty 	= 1.0; 
	ray->n1		= 1.0; 
	ray->n2 	= 1.0;
	ray->xr 	= 0.0; 
	ray->yr 	= 0.0; 
	ray->zr 	= 0; 
	ray->thar 	= 100; 
	ray->phir 	= 0.0;

	iteration_count++;
	find_str_hit_global(ray, dpos, opr, &type);
	if (!find_str_hit_local(ray, lstr))
	{
		hit_local_error_count++;
		return;
	}
	CalcMainReflectiveRay(ray, &src_ray[0]);
	CalcMainTransmittanceRay(ray, &src_ray[1]);

	for(i=0; i<2; i++)
	{
		output_ray[i][0].phir = 1;
		output_ray[i][1].phir = 2;
		output_ray[i][2].phir = 3;
		output_ray[i][3].phir = 4;
		output_ray[i][4].phir = 5;
		CalcGaussScatteredRay(&src_ray[i], (ray_trace1 *)&output_ray[i]);

		for(j=0; j<MAX_OUTPUT_RAY; j++)
		{
			if(output_ray[i][j].inty > IntensityThreshold)
			{
				hanldOneRay(&output_ray[i][j], dpos, opr, lstr);
			}
		}
	}
	// call_CalcGaussScatteredRay(&source_ray[0]);
}

int dbg_start()
{

	int i;
	// define variables
	opt_mat opm;					// for optic materials
	opt_source ops;					// for optic source
	opt_record opr;					// for recording rays emitted from light guides
	ray_traces rays;				// for samplings of ray tracing
	ray_trace1 ray1;
	local_str lstr;					// for local microstructure
	dot_density dden;				// for global dot density
	dot_position dpos;				// for global dot potision
	char fpname[256];					// for reading parameters
	struct ray_trace1 source_ray[2];

	set_start_time("Total");
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname);



	// allocate memory
	allocmem_opm(n_wl, n_mat, &opm);			
	allocmem_ops(n_x, n_y, n_z, n_tha, n_phi, xl_or, yl_or, zl_or, xl_rng, yl_rng, zl_rng, &ops);				
	allocmem_rays(n_ray, n_gaus, &rays, &ray1);	
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr);			
	allocmem_local_str(nx_str, ny_str, center_x, center_y, xstr_rng, ystr_rng, &lstr);		
	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);	
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);	

	// program body
		

	set_start_time("gen_source_ray");
	// generate light source rays & initialize microstructure
	gen_source_ray(&ops, &rays);
	set_end_time("gen_source_ray");
	
	set_start_time("read_microstr");
	read_microstr(str_file, &lstr);
	set_end_time("read_microstr");

	// moduel 2...
	//debug test
	set_start_time("debug_den_to_pos");
	debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
	set_end_time("debug_den_to_pos");

	set_start_time("part_dots");
	part_dots(&dpos);
	set_end_time("part_dots");


	

	set_start_time("ray tracing");

	for(i=0; i<n_ray; i++)
	// for(i=0; i<1; i++)
	{

		ray1.ngaus 	= 1; 
		ray1.inty 	= 1.0; 
		ray1.n1 	= 1.0; 
		ray1.n2 	= 1.0;
		ray1.xr 	= rays.xr[i]; 
		
		ray1.yr 	= 0.0; 
		// ray1.yr 	= rays.yr[i]; 
		
		ray1.zr 	= rays.zr[i]; 
		
		ray1.thar 	= 100; 
		// ray1.thar 	= rays.thar[i]; 
		
		ray1.phir 	= 0;
		
		traceOneRay(&ray1, &dpos, &opr, &lstr);

		// find_str_hit_global(&ray1, &dpos, &opr);
		// if (!find_str_hit_local(&ray1, &lstr))
		// {
		// 	dumpRay1(&ray1);
		// 	continue;	
		// }
		// CalcMainReflectiveRay(&ray1, &source_ray[0]);
		// CalcMainTransmittanceRay(&ray1, &source_ray[1]);
		// call_CalcGaussScatteredRay(&source_ray[0]);
	}
	set_end_time("ray tracing");

	// for(i=0; i<1; i++)
	// {
	// 	// CalcMainReflectiveRay(&incident_ray[i], &source_ray[i]);
	// 	// CalcMainTransmittanceRay(&incident_ray[i], &source_ray[i+100]);

	// 	CalcMainReflectiveRay(&ray1, &source_ray[i]);
	// 	CalcMainTransmittanceRay(&ray1, &source_ray[i+100]);
	// }

	// for(i=0; i<200; i++)
	// {
	// 	call_CalcGaussScatteredRay(&source_ray[i]);
	// }

	pLU(hit_local_error_count);
	pLU(iteration_count);

	// module 5...	




	//deallocate memory
	deallocmem_opm(&opm);
	deallocmem_ops(&ops);
	deallocmem_rays(&rays, &ray1);
	deallocmem_record(&opr);
	deallocmem_local_str(&lstr);
	deallocmem_dot_density(&dden);
	deallocmem_dot_position(&dpos);
	set_end_time("Total");
	print_all_execution_time();
	return 0;
}


// int main(int argc, char** argv)
// {
// 	dbg_start();
// 	return 0;
// }

