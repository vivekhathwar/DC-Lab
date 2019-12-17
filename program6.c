#include<stdio.h>
#include<gd.h>
#include<string.h>
#include<omp.h>
#include<math.h>
int main(int argc, char* argv[]){
    if(argc<4){
        printf("Using: ./negative input.png output.png num_threads\n");
        return 1;
    }
    char* inf=argv[1];
    char* opf=argv[2];
    int num_threads=atoi(argv[3]);
    int colour,x,y,i,red,green,blue,r,g,b;
    FILE* fp;
    gdImagePtr img;
    if((fp=fopen(inf,"r"))==NULL){
        printf("Error opening file %s\n",inf);
        return 1;
    }
    img=gdImageCreateFromPng(fp);
    int width=gdImageSX(img),height=gdImageSY(img);
    double t1=omp_get_wtime();
    #pragma omp parallel for private(y,colour,red,green,blue) num_threads(num_threads)
    for(x=0;x<width;x++){
        for(y=0;y<height;y++){
            colour= x+0;
            colour=gdImageGetPixel(img,x,y);
            r= gdImageRed(img,colour);
            g= gdImageGreen(img,colour);
            b= gdImageBlue(img,colour);
            red=green=blue=0.2126*r+0.7152*g+0.0722*b;
            colour=gdImageColorAllocate(img,red,green,blue);
            gdImageSetPixel(img,x,y, colour);
        }
    }
    double t2= omp_get_wtime();
    if((fp=fopen(opf,"w"))== NULL){
        printf("Error opening file %s\n",opf);
        return 1;
    }
    gdImagePng(img,fp);
    gdImageDestroy(img);
    fclose(fp);
    printf("File size:%dx%d\n",width,height);
    printf("Time taken:%3lfms\n",(t2-t1)*1000);
}
