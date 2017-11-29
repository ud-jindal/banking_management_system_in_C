#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <signal.h>
#include <math.h>

struct transaction{
    int id;
    int change;
    int balance;
};

bool setLock(int fd,int type){
    static struct flock lock;
    if(type==0) lock.l_type = F_RDLCK;
    else lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int ret = fcntl(fd, F_SETLKW, &lock);
}

bool unlock(int fd){
    static struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int ret = fcntl(fd, F_SETLKW, &lock);
}

int getCountTransactions(int account){
    char filein[40];
    sprintf(filein,"%d",account);
    int fd=open(filein,O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct transaction *a=malloc(sizeof(struct transaction));
    lseek(fd,-sizeof(struct transaction),SEEK_END);
    read(fd,a,sizeof(struct transaction));
    unlock(fd);
    close(fd);
    return a->id;
}

int addTransaction(int account,int change){
    char filein[40];
    sprintf(filein,"%d",account);
    int fd=open(filein,O_RDWR,0744),i;
    if(fd==-1) return -1;   
    setLock(fd,1); 
    struct transaction *a=malloc(sizeof(struct transaction));
    int tot=getCountTransactions(account);
    lseek(fd,-sizeof(struct transaction),SEEK_END);
    read(fd,a,sizeof(struct transaction));
    a->change=change;
    a->id++;
    a->balance=a->balance+change;
    lseek(fd,0,SEEK_END);
    write(fd,a,sizeof(struct transaction));
    unlock(fd);
    close(fd);
    ////initTransactionFile(a->id);
    return a->balance;
}

bool initTransactions(int account){
    char filein[40];
    sprintf(filein,"%d",account);
    int fd=open(filein,O_RDWR|O_CREAT|O_EXCL,0744);
    if(fd==-1) return 0;
    struct transaction *a=malloc(sizeof(struct transaction));
    a->id=0;
    a->change=0;
    a->balance=0;
    //a->accountNum=(int)NULL;
    write(fd,a,sizeof(struct transaction));
    close(fd);
    return 1;
}

void printdataTransactions(int account){
    char filein[40];
    sprintf(filein,"%d",account);
    int fd=open(filein,O_RDONLY,0744),i;
    struct transaction *a=malloc(sizeof(struct transaction));
    lseek(fd,0,SEEK_SET);
    for(i=0;i<=getCountTransactions(account);i++){
        read(fd,a,sizeof(struct transaction));
        printf("%d %d %d\n",a->id,a->change,a->balance);
    }
    close(fd);
}

// int main(int argc, char *argv[]){
//     if(!initTransactions(0)){
//         // if(add("user1")==-1) printf("error!\n");
//         // if(add("user2")==-1) printf("error!\n");
//         // if(add("user3")==-1) printf("error!\n");
//         // if(add("user2")==-1) printf("error!\n");
//         // deleteByUsr("user1");
//         // deleteByUsr("user1");
//         // deleteById(2);
//          printdataTransactions(0);
//     }
//     else{
//         printf("initialized!\n");
//     }
// }