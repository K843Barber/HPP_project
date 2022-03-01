vector_t r_vector(vector_t v1, vector_t v2) {
    vector_t r;

    r.x = v2.x - v1.x;
    r.y = v2.y - v1.y;

    return r;
}

#if VERSION < 2
// Version 1
double norm3(vector_t v) {
    return pow((sqrt(pow((v.x), 2) + pow((v.y), 2)) + epsilon), 3);
}
#else
// Version 2
double norm3(vector_t v) {
    double n3;

    n3 = sqrt(v.x*v.x + v.y*v.y) + epsilon; 
    n3 = n3 * n3 * n3; 
    return n3;
}
#endif

#if VERSION < 3
// Version 1 - 2
vector_t force(particle_t pi, particle_t pj) {
    vector_t f;

    f.x = -(G * pi.mass * pj.mass * r_vector(pi.position, pj.position).x) 
            / norm3(r_vector(pi.position, pj.position));
    f.y = -(G * pi.mass * pj.mass * r_vector(pi.position, pj.position).y) 
            / norm3(r_vector(pi.position, pj.position));

    return f;
}

#else
//Version 3 - 
vector_t force(particle_t pi, particle_t pj) {
    vector_t f;
    vector_t r;
    double a;

    r = r_vector(pi.position, pj.position);
    a = -(G * pi.mass * pj.mass) / norm3(r);

    f.x = a * r.x;
    f.y = a * r.y;

    return f;
}
#endif

#if VERSION < 4
// Version 1-3
vector_t total_force(particle_t* p, int i, int N) {
    vector_t f = {0.0,0.0};
    
    for(int j = 0; j < N; j++) {
        if(i != j) {
            vector_t f_temp = force(p[j], p[i]);
            f.x += f_temp.x;
            f.y += f_temp.y;
        }
    }
    return f;
}
#else
// Version 4 -
void compute_forces(vector_t* forces, particle_t* particles, int N) {

    for(int i = 0; i < N; i++) {
        forces[i].x = 0.0;
        forces[i].y = 0.0;
    }
    
    for(int i = 0; i < N; i++) {
        
        for(int j = i + 1; j < N; j++) {
                vector_t f = force(particles[j], particles[i]);
                forces[i].x += f.x;
                forces[i].y += f.y;

                forces[j].x -= f.x;
                forces[j].y -= f.y;
        }
        
    }
}
#endif