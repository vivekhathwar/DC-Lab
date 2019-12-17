#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

long long factorial(long n){
    if(n==1||n==0) return (long long)1;
    else return n*factorial(n-1);
}

void main(int argc, char **argv){
    int i,j,threads;
    long long *x,n=12;
    if(argc>1){
        puts(argv[1]);
        threads=atoi(argv[1]);
        if(omp_get_dynamic()){
            omp_set_dynamic(0);
            printf("Called omp_set_dynamic(0)\n");
        }
        omp_set_num_threads(threads);
    }
    printf("%d threads\n",omp_get_max_threads());
    x=(long long*) malloc(n*sizeof(long));
    for(i=0;i<n;i++) 
        x[i]=factorial(i);
    j=0;
    double t1,t2;
    t1=omp_get_wtime();
    #pragma omp parallel for firstprivate(x,j)
    for(i=1;i<n;i++){
        j+=i;
        x[i]=j*x[i-1];
    }
    t2=omp_get_wtime();
    for(i=0;i<n;i++)
        printf("Factorial(%2d)=%14lld \t x[%2d]=%14lld\n",i,factorial(i),i,x[i]);
    printf("Time taken %lf\n",t2-t1);
}
