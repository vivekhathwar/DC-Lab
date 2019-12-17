#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>

int f(int i){
    int i4_huge=21475646,j,k,value=i;
    
    for(j=1;j<=5;j++){
        k=value/127773;
        value=16807*(value-k*1277773)-k*2836;
        if(value<=0)
            value+=i4_huge;
    }
    return value;
}

int search(int a, int b, int c, int id, int p){
    int fi,i,j=-1;
    
    for(i=a+id;0<i&&i<=b;i+=p){
        fi=f(i);
        if(fi==c){
            j=i;
            break;
        }
    }
    return j;
}

void timestamp(){
    #define TIME_SIZE 40
    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now=time(NULL);
    tm=localtime(&now);
    
    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p",tm);
    printf("%s\n",time_buffer);
    return;
    #undef TIME_SIZE
}

double cpu_time(void){
    double value=(double)clock()/(double)CLOCKS_PER_SEC;
    return value;
}

void main(int argc, char* argv[]){
    int a,b,c,i4_huge=214756461,id,j,p;
    double wtime;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    a=1; b=i4_huge; c=45;
    
    if(id==0){
        timestamp();
        printf("A=%d\t,B=%d\t,C=%d\n",a,b,c);
    }
    
    wtime=MPI_Wtime();
    j=search(a,b,c,id,p);
    wtime=MPI_Wtime()-wtime;
    
    if(j!=-1)
        printf("Process %d found. J=%d\n. Verify F(j)=%d\n",id,j,f(j));
    
    MPI_Finalize();
    
    if(id==0){
        printf("Normal end of execution.\n");
        timestamp();
    }
}
