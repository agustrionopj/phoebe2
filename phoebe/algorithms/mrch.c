/* A copy of marching.py ported to C and wrapped back in python.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Python.h"
#include "numpy/arrayobject.h"

#define PI 3.14159265359

////////////////////////////////////////////////////////////////////////

//Mesh vertex struct
typedef struct
{
    double r[3];
    double n[3];
    double t1[3];
    double t2[3];
    double invM[9]; //Inverse matrix
} MeshVertex;

//Mesh vertex array struct
typedef struct
{
    int size;
    MeshVertex *v;
} VertexArray;

VertexArray *vertex_array_new(void)
{
	VertexArray *va = malloc(sizeof(*va));
	va->size = 0;
	va->v = NULL;
	return va;
}

int vertex_array_alloc(VertexArray *va, int size)
{
    va->size = size;
    va->v = malloc(size * sizeof(*(va->v)));
    
    return 1;
}

int vertex_array_free (VertexArray *va)
{
	if (!va)
        return 1;
	if (va->v) free(va->v);
	free (va);
	return 1;
}

int vertex_array_append (VertexArray *va, MeshVertex v)
{
    //Convenience function for appending vertices to an array 
	va->v = realloc(va->v, sizeof(*(va->v)) * (va->size+1));
    va->v[va->size] = v;
    va->size += 1;
	return 1;
}

int vertex_array_drop_and_stack(VertexArray *P, VertexArray *Pi, int idx)
{
    /* This function replaces idx-th vertex with elements from Pi.
     * 
     * Input:   P = [v0, v1, ..., idx, ... vn]
     * Output:  P = [[Pstart][Pi][Pend]]
     *          Pstart = P[v0 ... idx-1]
     *          Pend   = P[idx+1 ... vn]
     */
    
    int i;
    VertexArray *Pstart = NULL;
    VertexArray *Pend = NULL;
    
    if (idx > 0) {Pstart = vertex_array_new();}
    if (idx < P->size-1) {Pend = vertex_array_new();}
    
    //Make copies of original array  vertices
    if (idx > 0){
        vertex_array_alloc(Pstart, idx);
        for (i = 0; i < idx; i++)
            Pstart->v[i] = P->v[i];
    }
    if (idx < P->size-1){
        vertex_array_alloc(Pend, P->size-idx-1);
        for (i = idx+1; i < P->size; i++)
            Pend->v[i-idx-1] = P->v[i];
    }
    
    //Resize array
    P->v = realloc(P->v, sizeof(*(P->v)) * (P->size+Pi->size-1));
    P->size = P->size+Pi->size-1;
    if (P->size==0){
        if (Pstart) vertex_array_free(Pstart);
        if (Pend) vertex_array_free(Pend);
        return 1;
    }
    
    //Refill resized array
    if (idx > 0){
        for (i=0;i<Pstart->size;i++){
            P->v[i] = Pstart->v[i];
        }
    }
    if (Pi->size>0){
        for (i = 0; i < Pi->size; i++){
            P->v[i+idx] = Pi->v[i];
        }
    }
    if (idx < P->size){
        for (i = idx+Pi->size; i < P->size; i++){
            P->v[i] = Pend->v[i-idx-Pi->size];
        }
    }
    
    if (Pstart) vertex_array_free(Pstart);
    if (Pend) vertex_array_free(Pend);
    
    return 1;
}

//Triangle struct
typedef struct
{
    MeshVertex v0;
    MeshVertex v1;
    MeshVertex v2;
} Triangle;

//Triangle array struct
typedef struct
{
    int size;
    Triangle *t;
} TriangleArray;

TriangleArray *triangle_array_new(void)
{
	TriangleArray *ta = malloc(sizeof(*ta));
	ta->size = 0;
	ta->t = NULL;
	return ta;
}

int triangle_array_alloc(TriangleArray *ta, int size)
{
    ta->size = size;
    ta->t = malloc(size * sizeof(*(ta->t)));
    return 1;
}

int triangle_array_free (TriangleArray *ta)
{
	if (!ta) return 1;
	if (ta->t) free(ta->t);
	free (ta);
	return 1;
}

int triangle_array_append (TriangleArray *ta, Triangle t)
{
	ta->t = realloc (ta->t, sizeof(*(ta->t)) * (ta->size+1));
    ta->t[ta->size] = t;
    ta->size += 1;
	return 1;
}
    
////////////////////////////////////////////////////////////////////////

// Add new potential definitions here. You also need to edit the
// initialize_pars() function and python discretize() function.
// Try to optimize them as much as possible because a lot of time
// is spent here.
// We either need both the potential and derivatives or just derivatives.
// Caluclating both together saves some time, therefore pot function 
// returns all values and der function returns only derivatives. In both 
// cases they are packed into the passed ret array.

//SPHERE++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
void sphere(double r[3], double *p, double ret[4])
{
    ret[0] = 2*r[0];
    ret[1] = 2*r[1];
    ret[2] = 2*r[2];
    ret[3]= r[0]*r[0] + r[1]*r[1] + r[2]*r[2] - p[0]*p[0];
}

void dsphere(double r[3], double *p, double ret[3])
{
    ret[0] = 2*r[0];
    ret[1] = 2*r[1];
    ret[2] = 2*r[2];
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//BINARY ROCHE++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void binary_roche (double r[3], double *p, double ret[4])
{
    double a = r[0]*r[0];
    double b = r[1]*r[1];
    double c = r[2]*r[2];
    double d = r[0]-p[0];
    double e = a+b+c;
    double f = d*d+b+c;
    double g = p[2]*p[2]*(1.0+p[1]);
    double h,i,j;
    double k = p[0]*p[0];
    double l = sqrt(e);
    double m = sqrt(f);
    
    i = 1.0/(e*l);
    j = 1.0/(f*m)*p[1];
    h=i+j;
    
    ret[0] = -r[0]*(i-g) - d*j - p[1]/k;
    ret[1] = -r[1]*(h-g);
    ret[2] = -r[2]*h;
    ret[3] = 1.0/l + p[1]*(1.0/m-r[0]/k) + 0.5*g*(a+b) - p[3];
}

void dbinary_roche (double r[3], double *p, double ret[3])
{
    double a = r[0]*r[0];
    double b = r[1]*r[1];
    double c = r[2]*r[2];
    double d = r[0]-p[0];
    double e = a+b+c;
    double f = d*d+b+c;
    double g = p[2]*p[2]*(1.0+p[1]);
    double h;
    
    e = 1.0/(e*sqrt(e));
    f = 1.0/(f*sqrt(f))*p[1];
    h=e+f;
    
    ret[0] = -r[0]*(e-g) - d*f - p[1]/(p[0]*p[0]);
    ret[1] = -r[1]*(h-g);
    ret[2] = -r[2]*h;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//MISALIGNED BINARY ROCHE+++++++++++++++++++++++++++++++++++++++++++++++
double misaligned_binary_roche(double r[3], double *p, double ret[3])
{
    double delta = (1-pow(cos(p[4]),2)*pow(sin(p[3]),2))*r[0]*r[0] +
            (1-pow(sin(p[4]),2)*pow(sin(p[3]),2))*r[1]*r[1] +
            pow(sin(p[3]),2)*r[2]*r[2] -
            pow(sin(p[3]),2)*sin(2*p[4])*r[0]*r[1] -
            sin(2*p[3])*cos(p[4])*r[0]*r[2] -
            sin(2*p[3])*sin(p[4])*r[1]*r[2];
    
    return 1.0/sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]) + 
           p[1]*(1.0/sqrt((r[0]-p[0])*(r[0]-p[0])+r[1]*r[1]+r[2]*r[2])-r[0]/p[0]/p[0]) + 
           0.5*p[2]*p[2]*(1+p[1])*delta - p[5];
}

void dmisaligned_binary_roche(double r[3], double *p, double ret[3])
{
    double delta;

    delta = 2*(1-pow(cos(p[4]),2)*pow(sin(p[3]),2))*r[0] -
            pow(sin(p[3]),2)*sin(2*p[4])*r[1] -
            sin(2*p[3])*cos(p[4])*r[2];
            
    ret[0] = -r[0]*pow(r[0]*r[0]+r[1]*r[1]+r[2]*r[2],-1.5) -
              p[1]*(r[0]-p[0])*pow((r[0]-p[0])*(r[0]-p[0])+r[1]*r[1]+r[2]*r[2],-1.5) -
              p[1]/p[0]/p[0] + 0.5*p[2]*p[2]*(1+p[1])*delta;


    delta = 2*(1-pow(sin(p[4]),2)*pow(sin(p[3]),2))*r[1] -
            pow(sin(p[3]),2)*sin(2*p[4])*r[0] -
            sin(2*p[3])*sin(p[4])*r[2];
            
    ret[1] = -r[1]*pow(r[0]*r[0]+r[1]*r[1]+r[2]*r[2],-1.5) -
              p[1]*r[1]*pow((r[0]-p[0])*(r[0]-p[0])+r[1]*r[1]+r[2]*r[2],-1.5) + 
              0.5*p[2]*p[2]*(1+p[1])*delta;

    delta = 2*pow(sin(p[3]),2)*r[2] -
            sin(2*p[3])*cos(p[4])*r[0] -
            sin(2*p[3])*sin(p[4])*r[1];
            
    ret[2] = -r[2]*pow(r[0]*r[0]+r[1]*r[1]+r[2]*r[2],-1.5) -
              p[1]*r[2]*pow((r[0]-p[0])*(r[0]-p[0])+r[1]*r[1]+r[2]*r[2],-1.5) + 
              0.5*p[2]*p[2]*(1+p[1])*delta;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//ROTATE ROCHE++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double rotate_roche(double r[3], double *p)
{
    double Omega = p[0]*0.54433105395181736;
    double rp = sqrt(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]);
    return 1.0/p[1] - 1.0/rp -0.5*Omega*Omega*(r[0]*r[0]+r[1]*r[1]);
}

void drotate_roche(double r[3], double *p, double ret[3])
{
    double Omega = p[0]*0.54433105395181736;
    //~ double a = 1.0/exp(1.5*log(r[0]*r[0]+r[1]*r[1]+r[2]*r[2]));
    double a = r[0]*r[0]+r[1]*r[1]+r[2]*r[2];
    a = 1.0/(a*sqrt(a));

    ret[0] = r[0]*a - Omega*Omega*r[0];
    ret[1] = r[1]*a - Omega*Omega*r[1];
    ret[2] = r[2]*a;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//TORUS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double torus(double r[3], double *p)
{
    return p[1]*p[1]-p[0]*p[0]+2*p[0]*sqrt(r[0]*r[0]+r[1]*r[1])-r[0]*r[0]-r[1]*r[1]-r[2]*r[2];
}

void dtorus(double r[3], double *p, double ret[3])
{
    double a = 1.0/sqrt(r[0]*r[0]+r[1]*r[1]);
    
    ret[0] = 2*p[0]*r[0]*a-2*r[0];
    ret[1] = 2*p[0]*r[1]*a-2*r[1];
    ret[2] =-2*r[2];
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//HEART+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double heart(double r[3], double *p)
{
    return (pow(r[0]*r[0] + 9./4.*r[1]*r[1] + r[2]*r[2] - 1,3.0) - 
            r[0]*r[0]*r[2]*r[2]*r[2] - 
            9./80*r[1]*r[1]*r[2]*r[2]*r[2]);
}

void dheart(double r[3], double *p, double ret[3])
{
    double a = r[0]*r[0] + 9./4.*r[1]*r[1] + r[2]*r[2] - 1;
    a = a*a;
    
    ret[0] = (a*2*r[0] - 2*r[0]*r[2]*r[2]*r[2]);
    ret[1] = (a*9./2.*r[1] - 9./40.*r[1]*r[2]*r[2]*r[2]);
    ret[2] = (a*2*r[2] - 3*r[0]*r[0]*r[2]*r[2] - 27./80.*r[1]*r[1]*r[2]*r[2]);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
    /* This struct handles different types of potentials with varying 
     * number of parameters. Its instance is passes to all functions
     * that are dealing with potential equations.
     */
    double *p;
    void (* pot)();
    void (* der)();
} PotentialParameters;

PotentialParameters *initialize_pars(char *potential, double *args)
{
    PotentialParameters *pp=(PotentialParameters*)malloc(sizeof(PotentialParameters));
    pp->p = args;

    if (!strcmp(potential,"Sphere")){
        pp->pot = sphere;
        pp->der = dsphere;
        return pp;
    }
    
    else if (!strcmp(potential,"BinaryRoche")){
        pp->pot = binary_roche;
        pp->der = dbinary_roche;
        return pp;
    }
    
    //~ else if (!strcmp(potential,"MisalignedBinaryRoche")){
        //~ pp->pot = misaligned_binary_roche;
        //~ pp->der = dmisaligned_binary_roche;
        //~ return pp;
    //~ }
    //~ 
    //~ else if (!strcmp(potential,"RotateRoche")){
        //~ pp->pot = rotate_roche;
        //~ pp->der = drotate_roche;
        //~ return pp;
    //~ }
    //~ 
    //~ else if (!strcmp(potential,"Torus")){
        //~ pp->pot = torus;
        //~ pp->der = dtorus;
        //~ return pp;
    //~ }
    //~ 
    //~ else if (!strcmp(potential,"Heart")){
        //~ pp->pot = heart;
        //~ pp->der = dheart;
        //~ return pp;
    //~ }
        
    return pp;
}



////////////////////////////////////////////////////////////////////////

MeshVertex vertex_from_pot(double r[3], PotentialParameters *pp)
{
    double n[3];
    double t1[3];
    double t2[3];
    double nn;
    double detA;
    MeshVertex v;
    
    pp->der(r,pp->p,n);
    
    nn = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    n[0] /= nn;
    n[1] /= nn;
    n[2] /= nn;
    
    if (n[0] > 0.5 || n[1] > 0.5){
        nn = sqrt(n[1]*n[1] + n[0]*n[0]);
        t1[0] = n[1]/nn;
        t1[1] = -n[0]/nn;
        t1[2] = 0.0;
    }       
    else{
        nn = sqrt(n[0]*n[0]+n[2]*n[2]);
        t1[0] = -n[2]/nn;
        t1[1] = 0.0;
        t1[2] = n[0]/nn;
    }
             
    t2[0] = n[1]*t1[2] - n[2]*t1[1];
    t2[1] = n[2]*t1[0] - n[0]*t1[2];
    t2[2] = n[0]*t1[1] - n[1]*t1[0];
    
    v.r[0] = r[0];v.r[1] = r[1];v.r[2] = r[2];
    v.n[0] = n[0];v.n[1] = n[1];v.n[2] = n[2];
    v.t1[0] = t1[0];v.t1[1] = t1[1];v.t1[2] = t1[2];
    v.t2[0] = t2[0];v.t2[1] = t2[1];v.t2[2] = t2[2];
    
    //Calculate inverse matrix for each vertex only once

    detA = v.n[0]*v.t1[1]*v.t2[2] - v.t2[0]*v.t1[1]*v.n[2] + v.t1[0]*v.t2[1]*v.n[2] - v.n[0]*v.t2[1]*v.t1[2] + v.t2[0]*v.n[1]*v.t1[2] - v.t1[0]*v.n[1]*v.t2[2];

    v.invM[0] = (v.t1[1]*v.t2[2] - v.t2[1]*v.t1[2])/detA;
    v.invM[1] = (v.t2[0]*v.t1[2] - v.t1[0]*v.t2[2])/detA;
    v.invM[2] = (v.t1[0]*v.t2[1] - v.t2[0]*v.t1[1])/detA;
    v.invM[3] = (v.t2[1]*v.n[2] - v.n[1]*v.t2[2])/detA;
    v.invM[4] = (v.n[0]*v.t2[2] - v.t2[0]*v.n[2])/detA;
    v.invM[5] = (v.t2[0]*v.n[1] - v.n[0]*v.t2[1])/detA;
    v.invM[6] = (v.n[1]*v.t1[2] - v.n[2]*v.t1[1])/detA;
    v.invM[7] = (v.t1[0]*v.n[2] - v.n[0]*v.t1[2])/detA;
    v.invM[8] = (v.n[0]*v.t1[1] - v.t1[0]*v.n[1])/detA;
    
    return v;
}

MeshVertex vertex_from_pot_without_inverse(double r[3], PotentialParameters *pp)
{
    /* Used for reprojection. No inverse matrix, t1 or t2 are needed there.
     */
    
    double n[3];
    double nn;
    MeshVertex v;
    
    pp->der(r,pp->p,n);
    
    nn = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    n[0] /= nn;
    n[1] /= nn;
    n[2] /= nn;
    
    v.r[0] = r[0];v.r[1] = r[1];v.r[2] = r[2];
    v.n[0] = n[0];v.n[1] = n[1];v.n[2] = n[2];

    return v;
}

void print_vertex(MeshVertex v)
{
    printf(" r = (% .3f, % .3f, % .3f)\t",v.r[0],v.r[1],v.r[2]);
    printf(" n = (% .3f, % .3f, % .3f)\t",v.n[0],v.n[1],v.n[2]);
    printf("t1 = (% .3f, % .3f, % .3f)\t",v.t1[0],v.t1[1],v.t1[2]);
    printf("t2 = (% .3f, % .3f, % .3f)\n",v.t2[0],v.t2[1],v.t2[2]);
}

MeshVertex project_onto_potential(double r[3], PotentialParameters *pp, int inv)
{
    /* Set inv=0 if inverse matrix is not needed (like in reprojection).
     */
    
    double ri[3] = {0.0,0.0,0.0};
    int n_iter = 0;
    double g[4];
    double grsq;
    double s;
    
    while (((r[0]-ri[0])*(r[0]-ri[0]) + (r[1]-ri[1])*(r[1]-ri[1]) + (r[2]-ri[2])*(r[2]-ri[2])) > 1e-12 && n_iter<100){
        ri[0] = r[0];
        ri[1] = r[1];
        ri[2] = r[2];
        
        pp->pot(ri,pp->p,g);
        
        grsq = g[0]*g[0] + g[1]*g[1] + g[2]*g[2];
        
        s = g[3];
        
        r[0] = ri[0]-s*g[0]/grsq;
        r[1] = ri[1]-s*g[1]/grsq;
        r[2] = ri[2]-s*g[2]/grsq;
        
        n_iter++;
    }  
    
    if (n_iter >= 90){
           printf("warning: projection did not converge\n");
    }
    
    if (inv)
        return vertex_from_pot(r,pp);
    else
        return vertex_from_pot_without_inverse(r,pp);
}

int argmin(double *array, int length)
{
    // REPLACE WITH QSORT
    int i,min = 0;
    for (i = 1; i < length; i++){
        if (array[min]-array[i]>1e-6) min = i;
    }
    return min;
}

void cart2local(MeshVertex v, double r[3], double ret[3])
{
    ret[0] = v.invM[0]*r[0] + v.invM[1]*r[1] + v.invM[2]*r[2];
    ret[1] = v.invM[3]*r[0] + v.invM[4]*r[1] + v.invM[5]*r[2];
    ret[2] = v.invM[6]*r[0] + v.invM[7]*r[1] + v.invM[8]*r[2];
}

void local2cart (MeshVertex v, double r[3], double ret[3])
{    
    ret[0] = v.n[0]*r[0] + v.t1[0]*r[1] + v.t2[0]*r[2];
    ret[1] = v.n[1]*r[0] + v.t1[1]*r[1] + v.t2[1]*r[2];
    ret[2] = v.n[2]*r[0] + v.t1[2]*r[1] + v.t2[2]*r[2];
}

PyArrayObject* cdiscretize(double delta, int max_triangles, char *potential, double *args)
{
    double init[3] = {-0.00002,0,0};
    
    PotentialParameters *pp = initialize_pars(potential,args);
    
    MeshVertex p0;
    MeshVertex pk;
    
    VertexArray *V = vertex_array_new();
    VertexArray *P = vertex_array_new();
    VertexArray *Pi = NULL;
    Triangle tri;
    TriangleArray *Ts = triangle_array_new();
    
    int i,j;
    int step = -1;
    
    double qk[3];
    double pi3 = PI/3.0;
    
    int idx1[6] = {1,2,3,4,5,6};
    int idx2[6] = {2,3,4,5,6,1};
    
    double *omega = NULL;
    double rdiff[3] = {0.0,0.0,0.0};
    double adiff;
    double c2l1[3] = {0.0,0.0,0.0};
    double c2l2[3] = {0.0,0.0,0.0};
    double l2c[3] = {0.0,0.0,0.0};
    
    int minidx;
    double minangle;
    int nt;
    double domega;
    
    MeshVertex p0m,v1,v2;
    double norm3;
    
    double side1,side2,side3,s;
    MeshVertex c;
    
    PyArrayObject *table;
    int dims[2];
    
    p0 = project_onto_potential(init,pp,1);
    vertex_array_append(V,p0);

    for (i = 0; i < 6; i++){
        qk[0] = p0.r[0]+delta*cos(i*pi3)*p0.t1[0] + delta*sin(i*pi3)*p0.t2[0];
        qk[1] = p0.r[1]+delta*cos(i*pi3)*p0.t1[1] + delta*sin(i*pi3)*p0.t2[1];
        qk[2] = p0.r[2]+delta*cos(i*pi3)*p0.t1[2] + delta*sin(i*pi3)*p0.t2[2];
        pk = project_onto_potential(qk,pp,1);
        vertex_array_append(P,pk);
        vertex_array_append(V,pk);
    }
    
    for (i = 0; i < 6; i++)
    {
        tri.v0 = V->v[0];
        tri.v1 = V->v[idx1[i]];
        tri.v2 = V->v[idx2[i]];
        triangle_array_append(Ts,tri);
    }

    while (P->size > 0){
        step += 1;
        
        if (max_triangles > 0 && step > max_triangles)
            break;

        omega = malloc(P->size * sizeof(double));
        
        for (i = 0; i < P->size; i++){
            
            if (i == 0) j = P->size-1;
            else j = i-1;
            
            rdiff[0] = P->v[j].r[0]-P->v[i].r[0];
            rdiff[1] = P->v[j].r[1]-P->v[i].r[1];
            rdiff[2] = P->v[j].r[2]-P->v[i].r[2];
            cart2local(P->v[i], rdiff, c2l1);
            
            if (i < P->size-1) j=i+1;
            else j=0;
            
            rdiff[0] = P->v[j].r[0]-P->v[i].r[0];
            rdiff[1] = P->v[j].r[1]-P->v[i].r[1];
            rdiff[2] = P->v[j].r[2]-P->v[i].r[2];
            cart2local(P->v[i], rdiff, c2l2);
            
            adiff = atan2(c2l2[2], c2l2[1]) - atan2(c2l1[2], c2l1[1]);
            if (adiff < 0) adiff += 2*PI;
            omega[i] = fmod(adiff, 2*PI);
        }
        
        minidx = argmin(omega, P->size);
        minangle = omega[minidx];
        free(omega); //we don't need it anymore
        
        nt = trunc(minangle*3.0/PI) + 1;   
        domega = minangle/nt;
        if (domega < 0.8 && nt > 1){
            nt -= 1;
            domega = minangle/nt;
        }
        
        if (minidx == 0) i = P->size - 1;
        else i = minidx - 1;
        if (minidx < P->size-1) j = minidx + 1;
        else j = 0;
        
        p0m = P->v[minidx];
        v1 = P->v[i];
        v2 = P->v[j];

        for (i = 1; i < nt; i++){
            
            rdiff[0] = v1.r[0] - p0m.r[0];
            rdiff[1] = v1.r[1] - p0m.r[1];
            rdiff[2] = v1.r[2] - p0m.r[2];
            cart2local(P->v[minidx], rdiff, c2l1);
            
            c2l2[0] = 0.0; 
            c2l2[1] = c2l1[1]*cos(i*domega) - c2l1[2]*sin(i*domega);
            c2l2[2] = c2l1[1]*sin(i*domega) + c2l1[2]*cos(i*domega);
            
            norm3 = sqrt(c2l2[1]*c2l2[1] + c2l2[2]*c2l2[2]);
            c2l2[1] /= norm3/delta;
            c2l2[2] /= norm3/delta;

            local2cart (p0m, c2l2, l2c);
            
            qk[0] = p0m.r[0] + l2c[0];
            qk[1] = p0m.r[1] + l2c[1];
            qk[2] = p0m.r[2] + l2c[2];
            
            pk = project_onto_potential(qk,pp,1);
            vertex_array_append(V,pk);

            if (i == 1) tri.v0 = v1;
            else tri.v0 = V->v[V->size-2];
            tri.v1 = pk;
            tri.v2 = p0m;
            triangle_array_append(Ts,tri);
        }
        
        if (nt == 1){
            tri.v0 = v1;
            tri.v1 = v2;
            tri.v2 = p0m;
            triangle_array_append(Ts,tri);
        }
        else{
            tri.v0 = V->v[V->size-1];
            tri.v1 = v2;
            tri.v2 = p0m;
            triangle_array_append(Ts,tri);
        }
        
        Pi = vertex_array_new();
        vertex_array_alloc(Pi,nt-1);
        for (i = 1; i < nt; i++){
            Pi->v[i-1] = V->v[V->size-nt+i];
        }

        vertex_array_drop_and_stack(P, Pi, minidx);
        vertex_array_free(Pi);
    }


    dims[0] = Ts->size;
    dims[1] = 16;
    table = (PyArrayObject *)PyArray_FromDims(2, dims, PyArray_DOUBLE);

    for (i = 0; i < Ts->size; i++){

        qk[0] = (Ts->t[i].v0.r[0] + Ts->t[i].v1.r[0] + Ts->t[i].v2.r[0])/3.0;
        qk[1] = (Ts->t[i].v0.r[1] + Ts->t[i].v1.r[1] + Ts->t[i].v2.r[1])/3.0;
        qk[2] = (Ts->t[i].v0.r[2] + Ts->t[i].v1.r[2] + Ts->t[i].v2.r[2])/3.0;
        c=project_onto_potential(qk,pp,1);
        
        side1 = sqrt((Ts->t[i].v0.r[0] - Ts->t[i].v1.r[0])*(Ts->t[i].v0.r[0] - Ts->t[i].v1.r[0])+
                     (Ts->t[i].v0.r[1] - Ts->t[i].v1.r[1])*(Ts->t[i].v0.r[1] - Ts->t[i].v1.r[1])+
                     (Ts->t[i].v0.r[2] - Ts->t[i].v1.r[2])*(Ts->t[i].v0.r[2] - Ts->t[i].v1.r[2]));
                     
        side2 = sqrt((Ts->t[i].v0.r[0] - Ts->t[i].v2.r[0])*(Ts->t[i].v0.r[0] - Ts->t[i].v2.r[0])+
                     (Ts->t[i].v0.r[1] - Ts->t[i].v2.r[1])*(Ts->t[i].v0.r[1] - Ts->t[i].v2.r[1])+
                     (Ts->t[i].v0.r[2] - Ts->t[i].v2.r[2])*(Ts->t[i].v0.r[2] - Ts->t[i].v2.r[2]));
                     
        side3 = sqrt((Ts->t[i].v2.r[0] - Ts->t[i].v1.r[0])*(Ts->t[i].v2.r[0] - Ts->t[i].v1.r[0])+
                     (Ts->t[i].v2.r[1] - Ts->t[i].v1.r[1])*(Ts->t[i].v2.r[1] - Ts->t[i].v1.r[1])+
                     (Ts->t[i].v2.r[2] - Ts->t[i].v1.r[2])*(Ts->t[i].v2.r[2] - Ts->t[i].v1.r[2]));
        s = 0.5*(side1 + side2 + side3);

        *(double *)(table->data + i*table->strides[0] +  0*table->strides[1]) = c.r[0];
        *(double *)(table->data + i*table->strides[0] +  1*table->strides[1]) = c.r[1];
        *(double *)(table->data + i*table->strides[0] +  2*table->strides[1]) = c.r[2];
        *(double *)(table->data + i*table->strides[0] +  3*table->strides[1]) = sqrt(s*(s-side1)*(s-side2)*(s-side3));
        *(double *)(table->data + i*table->strides[0] +  4*table->strides[1]) = Ts->t[i].v0.r[0];
        *(double *)(table->data + i*table->strides[0] +  5*table->strides[1]) = Ts->t[i].v0.r[1];
        *(double *)(table->data + i*table->strides[0] +  6*table->strides[1]) = Ts->t[i].v0.r[2];
        *(double *)(table->data + i*table->strides[0] +  7*table->strides[1]) = Ts->t[i].v1.r[0];
        *(double *)(table->data + i*table->strides[0] +  8*table->strides[1]) = Ts->t[i].v1.r[1];
        *(double *)(table->data + i*table->strides[0] +  9*table->strides[1]) = Ts->t[i].v1.r[2];
        *(double *)(table->data + i*table->strides[0] + 10*table->strides[1]) = Ts->t[i].v2.r[0];
        *(double *)(table->data + i*table->strides[0] + 11*table->strides[1]) = Ts->t[i].v2.r[1];
        *(double *)(table->data + i*table->strides[0] + 12*table->strides[1]) = Ts->t[i].v2.r[2];
        *(double *)(table->data + i*table->strides[0] + 13*table->strides[1]) = c.n[0];
        *(double *)(table->data + i*table->strides[0] + 14*table->strides[1]) = c.n[1];
        *(double *)(table->data + i*table->strides[0] + 15*table->strides[1]) = c.n[2];
    }

    vertex_array_free(V);
    vertex_array_free(P);
    triangle_array_free(Ts);
    free(pp);
    
    return table;
}


PyArrayObject* creproject(PyArrayObject *table, int rows, char *potential, double *args)
{
    MeshVertex p;
    double q[3];
    PotentialParameters *pp = initialize_pars(potential,args);
    int i,j;
    
    PyArrayObject *new_table;
    int dims[2];
    
    dims[0] = rows;
    dims[1] = 16;
    new_table = (PyArrayObject *)PyArray_FromDims(2, dims, PyArray_DOUBLE);

    for (i = 0; i < rows; i++){
        for (j = 0; j < 3; j++){
            q[0] = *(double *)(table->data + i*table->strides[0] + (4+3*j)*table->strides[1]);
            q[1] = *(double *)(table->data + i*table->strides[0] + (5+3*j)*table->strides[1]);
            q[2] = *(double *)(table->data + i*table->strides[0] + (6+3*j)*table->strides[1]);

            p = project_onto_potential(q,pp,0);
            
            *(double *)(new_table->data + i*table->strides[0] + (4+3*j)*table->strides[1]) = p.r[0];
            *(double *)(new_table->data + i*table->strides[0] + (5+3*j)*table->strides[1]) = p.r[1];
            *(double *)(new_table->data + i*table->strides[0] + (6+3*j)*table->strides[1]) = p.r[2];
        }

        q[0] = *(double *)(table->data + i*table->strides[0]);
        q[1] = *(double *)(table->data + i*table->strides[0] + table->strides[1]);
        q[2] = *(double *)(table->data + i*table->strides[0] + 2*table->strides[1]);
        
        p = project_onto_potential(q,pp,0);
        
        *(double *)(new_table->data + i*table->strides[0]) = p.r[0];
        *(double *)(new_table->data + i*table->strides[0] + table->strides[1]) = p.r[1];
        *(double *)(new_table->data + i*table->strides[0] + 2*table->strides[1]) = p.r[2];
        *(double *)(new_table->data + i*table->strides[0] + 13*table->strides[1]) = p.n[0];
        *(double *)(new_table->data + i*table->strides[0] + 14*table->strides[1]) = p.n[1];
        *(double *)(new_table->data + i*table->strides[0] + 15*table->strides[1]) = p.n[2];
    }
        
    free(pp);
    return new_table;
}


static PyObject *discretize(PyObject *self, PyObject *args)
{
    double delta;
    int max_triangles;
    char *potential;
    double ipars[6];
    double *pars=NULL;
    int npars = PyTuple_Size(args);
    int i;
    
    PyArrayObject *table;
    
    if (npars<4) {
        PyErr_SetString(PyExc_ValueError, "Not enough parameters.");
        return NULL;
    }

    //Supports up to 6 extra parameters. Change if more are needed.
    if (!PyArg_ParseTuple(args, "dis|dddddd", &delta, &max_triangles, &potential, &ipars[0], &ipars[1], &ipars[2], &ipars[3], &ipars[4], &ipars[5]))
        return NULL;

    // !!!More error handling to check parameter values!!!


    //Edit this part to add error handling for new potential types.
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (!strcmp(potential,"Sphere")) {
        if (npars!=4){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
    }
    else if (!strcmp(potential,"BinaryRoche")) {
        if (npars<6 || npars>7){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
        if (npars==6){// to handle optional Omega
            ipars[3]=0.0;
            npars+=1;
        }
    }
    else if (!strcmp(potential,"MisalignedBinaryRoche")) {
        if (npars<8 || npars>9){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
        if (npars==8){// to handle optional Omega
            ipars[5]=0.0;
            npars+=1;
        }
    }
    else if (!strcmp(potential,"RotateRoche")) {
        if (npars!=5){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
    }
    else if (!strcmp(potential,"Torus")) {
        if (npars!=5){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
    }
    else if (!strcmp(potential,"Heart")) {
        if (npars!=4){
            PyErr_SetString(PyExc_ValueError, "Wrong number of parameters for this type of potential.");
            return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_ValueError, "Unavailable potential.");
        return NULL;    
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    pars = malloc((npars-3) * sizeof(double));
    for (i=0;i<npars-3;i++){
        pars[i]=ipars[i];
    }       
    
    table = cdiscretize(delta, max_triangles, potential, pars);
    
    if (pars) free(pars);
    return PyArray_Return(table);
}

static PyObject *reproject(PyObject *self, PyObject *args)
{
    PyArrayObject *table, *new_table;
    char *potential;
    double ipars[6];
    double *pars=NULL;
    int npars = PyTuple_Size(args);
    int rows;
    int i;
    
    if (npars<3) {
        PyErr_SetString(PyExc_ValueError, "Not enough parameters.");
        return NULL;
    }
    
    if (!PyArg_ParseTuple(args, "O!s|dddddd", &PyArray_Type, &table, &potential, &ipars[0], &ipars[1], &ipars[2], &ipars[3], &ipars[4], &ipars[5]))
        return NULL;
        
    if (table->nd != 2) {
		PyErr_SetString(PyExc_ValueError, "Table not two dimensional.");
		return NULL;
	}
    
    rows = table->dimensions[0];
    
    pars = malloc((npars-2) * sizeof(double));
    for (i=0;i<npars-2;i++){
        pars[i]=ipars[i];
    }       
    
    new_table = creproject(table, rows, potential, pars);
    
    if (pars) free(pars); 
    return PyArray_Return(new_table);
}

static PyMethodDef marchingMethods[] = {
  {"discretize",  discretize,  METH_VARARGS, "Create a mesh of an implicit function surface."},
  {"reproject",  reproject, METH_VARARGS, "Reproject the surface."},
  {NULL, NULL, 0, NULL},
};

PyMODINIT_FUNC
initcmarching(void)
{
  (void) Py_InitModule("cmarching", marchingMethods);
  import_array();
}