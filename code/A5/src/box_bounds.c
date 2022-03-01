#include <stdio.h>
#include "vector.h"
#include "particle.h" 
#include "box_bounds.h"
#include "quad.h"
#include "forces.h"

//Version 1
// int vector_in_box(vector_t v, box_bounds_t box) {
//     if(v.x < box.xupper && v.x > box.xlower && v.y < box.yupper && v.y > box.ylower) {
//         return 1;
//     } else {
//         return 0;
//     }
// }