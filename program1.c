#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void merge(int *a, int l, int mid, int r){
    int i,j,n1=mid-l+1,n2= r-mid, b[n1],c[n2],k=l;
    for(i=0;i<n1;i++) b[i]=a[k++];
    for(i=0;i<n2;i++) c[i]=a[k++];
    i=0;j=0;k=l;
    while(i<n1 & j<n2){
        if(b[i]<c[j])
            a[k++]=b[i++];
        else a[k++]=c[j++];
    }
    while(i<n1)
        a[k++]=b[i++];
    while(j<n2)
        a[k++]=c[j++];
}

void mergesort(int *a,int l,int r){
    if(l<r){
        int mid;
        #pragma omp parallel sections
        {
            mid=(l+r)/2;
            #pragma omp section
            {
                mergesort(a,l,mid);
            }
            #pragma omp section
            {
                mergesort(a,mid+1,r);
            }
        }
        merge(a,l,mid,r);
    }
}

void main(){
    omp_set_nested(1);
    int *a=(int*)malloc(100*sizeof(int));
    int start=1,i;
    for(i=0;i<100;i++)
        a[i]=rand()%1001;
    for(i=0;i<100;i++)
        printf("%d\t",a[i]);
    printf("\n");
    mergesort(a,0,99);
    for(i=0;i<100;i++)
        printf("%d\t",a[i]);
    printf("\n");
}
