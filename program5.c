#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<math.h>
#include<float.h>
#define K 4
int num_threads;
long num_points,cluster_count[K];
long** points;
int cluster[K][2]={{75,25},{25,25},{25,75},{75,75}};

void populate_points(){
    points=(long**)malloc(sizeof(long)*num_points);
    long i;
    for(i=0;i<num_points;i++)
    points[i]=(long*)malloc(sizeof(long)*2);
    srand(time(NULL));
    for(i=0;i<num_points;i++){
        points[i][0]=rand()%100;
        points[i][1]=rand()%100;
        printf("%ld\t%ld\n",points[i][0],points[i][1]);
    }
    for(i=0;i<4;i++)
        cluster_count[i]=0;
}

double get_distance(int x1,int y1, int x2, int y2){
    int dx=x2-x1,dy=y2-y1;
    return ((double)sqrt(dx*dx+dy*dy));
}

void classify_points(){
    long i; int j;
    #pragma omp parallel for num_threads(num_threads)
    for(i=0;i<num_points;i++){
        int cluster_index=-1;
        double cur_dist=0,min_dist=DBL_MAX;
        for(j=0;j<K;j++){
            cur_dist=get_distance(points[i][0],points[i][1],cluster[j][0],cluster[j][1]);
            if(cur_dist<min_dist){
                min_dist=cur_dist;
                cluster_index=j;
            }
        }
        cluster_count[cluster_index]++;
    }
}

void main(int argc, char* argv[]){
    printf("Max threads:%d\n",omp_get_max_threads());
    num_points=atol(argv[1]);
    num_threads=atoi(argv[2]);
    populate_points();
    double t1=omp_get_wtime();
    classify_points();
    double t2=omp_get_wtime();
    printf("Time taken %lf\n",t2-t1);
    int i=0;
    printf("Counts\n");
    while(i<4)
        printf("%d->%ld\n",i,cluster_count[i++]);
}
