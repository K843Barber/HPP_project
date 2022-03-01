typedef struct box_bounds {
    double xlower;
    double xupper;
    double ylower;
    double yupper;
} box_bounds_t;

int vector_in_box(vector_t v, box_bounds_t box);