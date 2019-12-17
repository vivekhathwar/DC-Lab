#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void main(){
    int rows,cols,vs,i,j;
    double **mat,*vec,*res,*checkop;
    
    printf("Enter number of rows,coloums, and vector size\n");
    scanf("%d%d%d",&rows,&cols,&vs);
    
    if(rows<=0 || cols<=0 || vs<=0){
        printf("Size should be positive\n");
        exit(0);
    }
    
    if(cols!=vs){
        printf("Computation not possible\n");
        exit(0);
    }
    
    mat=(double **)malloc(sizeof(double)*rows);
    for(i=0;i<rows;i++)
        mat[i]=(double *)malloc(sizeof(double)*cols);
    
    vec=(double *) malloc(sizeof(double)*vs);
    checkop=(double *)malloc(sizeof(double)*rows);
    res=(double *) malloc(sizeof(double)*rows);
    for(j=0;j<rows;j++)
        {
        vec[i]=0;
        checkop[i]=0;
        }
    
    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++){
            mat[i][j]=i*j+i+j;
            printf("%0.1lf\t",mat[i][j]);
            }
        printf("\n");
    }
    
    printf("Vector\n");
    for(i=0;i<vs;i++){
        vec[i]=rand();
        printf("%0.1lf\t",vec[i]);
    }
    printf("\n");
    
    omp_set_num_threads(32);
    #pragma omp parallel for private(j)
    for(i=0;i<rows;i++)
        for(j=0;j<cols;j++)
            res[i]+=mat[i][j]*vec[j];
    
    printf("Parallel - Result\n");
    for(i=0;i<rows;i++)
        printf("%0.1lf\t",res[i]);
    printf("\n");
    
    for(i=0;i<rows;i++)
        for(j=0;j<cols;j++)
            checkop[i]+=vec[j]*mat[i][j];
    printf("Serial - Result\n");
    for(i=0;i<rows;i++)
        printf("%0.1lf\t",checkop[i]);
    printf("\n");
}
