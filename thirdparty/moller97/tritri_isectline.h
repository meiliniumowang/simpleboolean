// This file is published by Tomas Akenine-Möller in public domain from here: 
//      http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/ 
#ifndef MOLLER97_TRITRI_ISECTLINE_H
#define MOLLER97_TRITRI_ISECTLINE_H

int tri_tri_intersect(float V0[3],float V1[3],float V2[3],
                      float U0[3],float U1[3],float U2[3]);
                      
int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3],
				     float U0[3],float U1[3],float U2[3],int *coplanar,
				     float isectpt1[3],float isectpt2[3]);

#endif
