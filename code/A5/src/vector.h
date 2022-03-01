typedef struct vector {
    double x;
    double y;
} vector_t;

vector_t r_vector(vector_t v1, vector_t v2);

double norm3(vector_t v);

double norm(vector_t v);

void print_vector_info(vector_t v);
