adsfasdfsdf

// for visual-studio header
#ifdef _MSC_VER
#include "stdafx.h"

// for regular C header
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "malloc.h"
#include "time.h"
#include "tchar.h"

#else
#include "math.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#endif


// for private header
#include "var_def.h"			// define parameters and structe variables
#include "mem_func.h"			// allocate/deallocate memory 
#include "plt_figure.h"			// call gnuplot for ploting figure
#include "den_to_pos.h"			// module for transferring dot_density to dot_position 
#include "debug_fun.h"			// just for debugging individual module.

// function for memory arrangement
void allocmem_opm(opt_mat *opm);
void allocmem_ops(opt_source *ops);
void allocmem_rays(ray_traces *rays, ray_trace1 *ray1);
void allocmem_record(opt_record *opr);
void allocmem_local_str(local_str *lstr);
void allocmem_dot_density(dot_density *dden);
void allocmem_dot_position(dot_position *dpos);
void allocmem_gnuplot_2d(long int n, plot_2d *plt2d);
void allocmem_gnuplot_3d(long int n, plot_3d *plt3d);
void allocmem_gnuplot_3d_matrix(long int n1, long int n2, plot_3d_matrix *plt3dm);
void deallocmem_opm(opt_mat *opm);
void deallocmem_ops(opt_source *ops);
void deallocmem_rays(ray_traces *rays, ray_trace1 *ray1);
void deallocmem_record(opt_record *opr);
void deallocmem_local_str(local_str *lstr);
void deallocmem_dot_density(dot_density *dden);
void deallocmem_dot_position(dot_position *dpos);
void deallocmem_gnuplot_2d(plot_2d *plt2d);
void deallocmem_gnuplot_3d(plot_3d *plt3d);
void deallocmem_gnuplot_3d_matrix(plot_3d_matrix *plt3dm);

// function for moudles
void den2pos(dot_density *dden, dot_position *dpos);			// transfer dot_density to dot_position
void den2pos_tetgen(dot_density *dden, dot_position *dpos);		// transfer dot_density to dot_position by tetgen software

// functino for figure
void gnuplot_2d(plot_2d *plt2d);
void gnuplot_3d(plot_3d *plt3d);
void gnuplot_3d_matrix(plot_3d_matrix *plt3dm);

// function for debug
void debug_den_to_pos(dot_density *dden, dot_position *dpos);

// main program
#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main(void)
#endif
{
	// define variables
	opt_mat opm;					// for optic materials
	opt_source ops;					// for optic source
	opt_record opr;					// for recording rays emitted from light guides
	ray_traces rays;				// for samplings of ray tracing
	ray_trace1 ray1;
	local_str lstr;					// for local microstructure
	dot_density dden;				// for global dot density
	dot_position dpos;				// for global dot potision

	srand((unsigned)time(NULL));	// initiate rand seed 

	// allocate memory
	allocmem_opm(&opm);				
	allocmem_ops(&ops);				
	allocmem_rays(&rays, &ray1);	
	allocmem_record(&opr);			
	allocmem_local_str(&lstr);		
	allocmem_dot_density(&dden);	
	allocmem_dot_position(&dpos);	

	// program body
	debug_den_to_pos(&dden, &dpos);		// debug for dot_density to dot_position
	// moduel 1...
	// moduel 2...
	// moduel 3...
	// moduel 4...
	// moduel 5...

	//deallocate memory
	deallocmem_opm(&opm);
	deallocmem_ops(&ops);
	deallocmem_rays(&rays, &ray1);
	deallocmem_record(&opr);
	deallocmem_local_str(&lstr);
	deallocmem_dot_density(&dden);
	deallocmem_dot_position(&dpos);
	return 0;
}

