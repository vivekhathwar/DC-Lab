#include<omp.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int* prime_table(int prime_num){
    int i=2,j,p=0,prime, *primes;
    primes=(int *) malloc(prime_num*sizeof(int));
    while(p<prime_num){
        prime=1;
        for(j=2;j<i;j++){
            if((i%j)==0){
                prime=0;break;
            }
        }
        if(prime) 
            primes[p++]=i;
        i++;
    }
    return primes;
}

double* sine_table(int sine_num){
    double a,*sines,pi=3.141592653589793;
    int i,j;
    sines=(double*)malloc(sizeof(double)*sine_num);
    for(i=0;i<sine_num;i++){
        sines[i]=0.0;
        for(j=0;j<=i;j++){
            a=(double)(j)*pi / (double)(sine_num-1);
            sines[i]+=sin(a);
        }
    }
    return sines;
}

void main(){
    omp_set_nested(1);
    int size=10,i,x,y, *a;
    double t1,t2,*b;
    for(i=0;i<5;i++){
        printf("Size: %d\n",size);
        for(x=0;x<4;x++){
        t1=omp_get_wtime();
            #pragma omp parallel sections
            {
            omp_set_num_threads(2*x);
                #pragma omp section
                {
                a=(int*)malloc(sizeof(int)*size);
                a=prime_table(size);
                }
                #pragma omp section
                {
                b=(double*)malloc(sizeof(double)*size);
                b=sine_table(size);
                }
            }
        t2=omp_get_wtime();
        printf("Time taken is %lf\n",t2-t1);
        free(a);free(b);
        }
        size*=10;
    }
}
