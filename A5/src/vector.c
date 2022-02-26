#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

// Version 1
// const double epsilon = 1e-3;
// // //========================================================
// // Version 1
// vector_t r_vector(vector_t v1, vector_t v2) {
//     vector_t r;

//     r.x = v2.x - v1.x;
//     r.y = v2.y - v1.y;

//     return r;
// }
// //========================================================
// // Version 1
// double norm(vector_t v) {
//     return sqrt(v.x*v.x + v.y*v.y);   
// }
// // Version 1
// double norm3(vector_t v) {
//     double n3;

//     n3 = sqrt(v.x*v.x + v.y*v.y) + epsilon; 
//     n3 = n3 * n3 * n3;
//     return n3;
// }


//=======================================================
void print_vector_info(vector_t v) {
    printf("(%.20lf, %.20lf)\n", v.x, v.y);
}