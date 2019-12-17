#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void timestamp(){
    #define TIME_SIZE 40
    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now;
    now = time(NULL);
    tm=localtime(&now);
    strftime(time_buffer,TIME_SIZE, "%d %B %Y %I:%M:%S %p",tm);
    fprintf(stdout, "%s\n",time_buffer);
    return;
    #undef TIME_SIZE
}

int main(int argc, char* argv[]){
    MPI_Comm even_comm_id, odd_comm_id;
    MPI_Group even_group_id, odd_group_id, world_group_id;
    int even_id, even_id_sum, even_p, *even_rank, i, j, id, p, ierr, odd_id, odd_id_sum, *odd_rank, odd_p;
    
    ierr=MPI_Init(&argc, &argv);
    
    if(ierr!=0){
        printf("Fatal communicator error:\n\tReturned non zero error flag.\n");
        exit(1); 
    }
    
    ierr=MPI_Comm_size(MPI_COMM_WORLD, &p);
    ierr=MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    if(id==0){
        timestamp();
        printf("\nCommunicatorMPI: Master process\n");
        printf("The number of processes is %d.\n",p);
    }
    printf("Process %d says 'Hello World!'\n",id);
    MPI_Comm_group(MPI_COMM_WORLD, &world_group_id);
    
    even_p=(p+1)/2;
    even_rank=(int*)malloc(even_p*sizeof(int));
    j=0;
    for(i=0;i<p;i+=2)
        even_rank[j++]=i;
    MPI_Group_incl(world_group_id,even_p,even_rank,&even_group_id);
    MPI_Comm_create(MPI_COMM_WORLD,even_group_id,&even_comm_id);
    
    odd_p=p/2;
    odd_rank=(int*)malloc(odd_p*sizeof(int));
    j=0;
    for(i=1;i<p;i+=2)
        odd_rank[j++]=i;
    MPI_Group_incl(world_group_id,odd_p,odd_rank,&odd_group_id);
    MPI_Comm_create(MPI_COMM_WORLD,odd_group_id,&odd_comm_id);
    
    if(id%2!=0){
        ierr=MPI_Comm_rank(odd_comm_id, &odd_id);
        even_id=-1;
    }
    if(id%2==0){
        ierr=MPI_Comm_rank(even_comm_id, &even_id);
        odd_id=-1;
    }
    
    if(even_id!=-1){
        MPI_Reduce(&id,&even_id_sum,1,MPI_INT,MPI_SUM,0,even_comm_id);
    }
    if(even_id==0){
        printf("Number of processes in even communicator=%d\nSum of global ID's in even communicator is :%d\n",even_p,even_id_sum);
    }
    
    if(odd_id!=-1){
        MPI_Reduce(&id, &odd_id_sum,1,MPI_INT,MPI_SUM,0,odd_comm_id);
    }
    if(odd_id==0){
        printf("Number of processes in odd communicator=%d\nSum of global ID's in odd communicator is : %d\n",odd_p,odd_id_sum);
    }
    
    ierr=MPI_Finalize();
    
    if(id==0){
        printf("Communicator: Normal end of execution\n");
        timestamp();
    }
    return 0;
}
