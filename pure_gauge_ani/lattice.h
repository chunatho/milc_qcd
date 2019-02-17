#ifndef _LATTICE_H
#define _LATTICE_H
/****************************** lattice.h ********************************/

/* include file for MIMD version 7
   This file defines global scalars and the fields in the lattice. */

#include "defines.h"
#include "../include/macros.h"  /* For MAXFILENAME, EVENFIRST */
#include "../include/random.h"  /* For double_prn */
#include "../include/io_lat.h"	/* For gauge_file */

/* Begin definition of site structure */

#include "../include/su3.h"
#include "../include/random.h"   /* For double_prn */

/* The lattice is an array of sites.  */
#define MOM_SITE   /* If there is a mom member of the site struct */
typedef struct {
    /* The first part is standard to all programs */
	/* coordinates of this site */
	short x,y,z,t;
	/* is it even or odd? */
	char parity;
	/* my index in the array */
	int index;
#ifdef SITERAND
	/* The state information for a random number generator */
	double_prn site_prn;
	/* align to double word boundary (kludge for Intel compiler) */
	int space1;
#endif

    /* Now come the physical fields, program dependent */
	/* gauge field */
	su3_matrix link[4];
#ifdef ANISOTROPY
        su3_matrix staple_a[2];
        /* NOTE: a) staple_a[0] - spatial, staple_a[1] - temporal
                 b) the "staple" variable below is different from isotropic
                    case: here staple=beta[0]*staple_a[0]+beta[1]*staple_a[1],
                    while in the isotropic case it would be simply
                    staple=staple_a[0]+staple_a[1] */
#endif
	su3_matrix tempmat1,staple;
#ifdef HMC_ALGORITHM
 	su3_matrix old_link[4];
	/* For accept/reject */
#endif
	/* antihermitian momentum matrices in each direction */
 	anti_hermitmat mom[4];
#ifdef RMD_ALGORITHM

	/* temporary matrices */
	su3_matrix tempmat2;
#else
	/* temporary matrices */
	su3_vector tempvec;  /* for gaugefix */
#ifdef FUZZ
	su3_matrix tempmat2;
#endif
#endif

} site;


/* End definition of site structure */

/* Definition of globals */

#ifdef CONTROL
#define EXTERN 
#else
#define EXTERN extern
#endif

/* The following are global scalars */
EXTERN	int nx,ny,nz,nt;	/* lattice dimensions */
EXTERN  int volume;			/* volume of lattice = nx*ny*nz*nt */
EXTERN	int iseed;		/* random number seed */
EXTERN	int warms,trajecs,steps,stepsQ,propinterval;
#ifndef ANISOTROPY
EXTERN	Real beta;
#else
EXTERN	Real beta[2]; // beta[0] - space, beta[1] - time
#endif
EXTERN	Real epsilon;
EXTERN	char startfile[MAXFILENAME],savefile[MAXFILENAME];
EXTERN  double g_ssplaq, g_stplaq;
EXTERN  double_complex linktrsum;
EXTERN  u_int32type nersc_checksum;
EXTERN  char stringLFN[MAXFILENAME];  /** ILDG LFN if applicable **/
EXTERN	int startflag;	/* beginning lattice: CONTINUE, RELOAD, FRESH */
EXTERN  int fixflag;  /* gauge fix: COULOMB_GAUGE_FIX, NO_GAUGE_FIX */
EXTERN	int saveflag;	/* do with lattice: 1=save; */
EXTERN	int total_iters;

/* Some of these global variables are node dependent */
/* They are set in "make_lattice()" */
EXTERN	int sites_on_node;		/* number of sites on this node */
EXTERN	int even_sites_on_node;	/* number of even sites on this node */
EXTERN	int odd_sites_on_node;	/* number of odd sites on this node */
EXTERN	int number_of_nodes;	/* number of nodes in use */
EXTERN  int this_node;		/* node number of this node */

EXTERN	gauge_file *startlat_p;


/* Each node maintains a structure with the pseudorandom number
   generator state */
EXTERN  double_prn node_prn ;

/* The lattice is a single global variable - (actually this is the
   part of the lattice on this node) */
EXTERN Real boundary_phase[4];
EXTERN site *lattice;

/* Vectors for addressing */
/* Generic pointers, for gather routines */
#define N_POINTERS 8
EXTERN char ** gen_pt[N_POINTERS];


#endif /* _LATTICE_H */