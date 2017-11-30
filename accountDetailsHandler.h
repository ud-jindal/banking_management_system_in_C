#ifndef ACCOUNTDETAILSHANDLER_H
#define ACCOUNTDETAILSHANDLER_H

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
#include "transactionHandler.h"
#define ACCOUNTSFILE "accountDetails.bin"

struct account{
    int id;
    int balance;
};

// bool setLock(int fd,int type){
//     static struct flock lock;
//     if(type==0) lock.l_type = F_RDLCK;
//     else lock.l_type = F_WRLCK;
//     lock.l_start = 0;
//     lock.l_whence = SEEK_SET;
//     lock.l_len = 0;
//     lock.l_pid = getpid();
//     int ret = fcntl(fd, F_SETLKW, &lock);
// }

// bool unlock(int fd){
//     static struct flock lock;
//     lock.l_type = F_UNLCK;
//     lock.l_start = 0;
//     lock.l_whence = SEEK_SET;
//     lock.l_len = 0;
//     lock.l_pid = getpid();
//     int ret = fcntl(fd, F_SETLKW, &lock);
// }

int getCountAccounts(){
    int fd=open(ACCOUNTSFILE,O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct account *a=malloc(sizeof(struct account));
    lseek(fd,-sizeof(struct account),SEEK_END);
    read(fd,a,sizeof(struct account));
    unlock(fd);
    close(fd);
    return a->id;
}

int getAccountBalance(int id){
    int fd=open(ACCOUNTSFILE,O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct account *a=malloc(sizeof(struct account));
    lseek(fd,sizeof(struct account),SEEK_SET);
    int tot=getCountAccounts();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct account));
        if(a->id==id){
            unlock(fd);
            close(fd);
            return a->balance;
        }
    }
    unlock(fd);
    close(fd);
    return -1;
}

bool isThereAccount(int id){
    int fd=open(ACCOUNTSFILE,O_RDONLY,0744),i;
    if(fd==-1) return 0;
    setLock(fd,0);
    struct account *a=malloc(sizeof(struct account));
    //lseek(fd,sizeof(struct account),SEEK_SET);
    int tot=getCountAccounts();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct account));
        if(a->id==id){
            unlock(fd);
            close(fd);
            return 1;
        }
    }
    unlock(fd);
    close(fd);
    return 0;
}

int addAccount(){
    int fd=open(ACCOUNTSFILE,O_RDWR,0744),i;
    if(fd==-1) return -1;   
    setLock(fd,1); 
    struct account *a=malloc(sizeof(struct account));
    int tot=getCountAccounts();
    a->id=tot+1;
    a->balance=0;
    lseek(fd,0,SEEK_END);
    write(fd,a,sizeof(struct account));
    unlock(fd);
    close(fd);
    initTransactions(a->id);
    return a->id;
}

int transact(int id,int change){    
    int fd=open(ACCOUNTSFILE,O_RDWR,0744),i;
    if(fd==-1) return -2;
    setLock(fd,1);
    struct account *a=malloc(sizeof(struct account));
    int tot=getCountAccounts();
    if(id>tot){
        unlock(fd);
        close(fd);
        return -3;
    }
    lseek(fd,sizeof(struct account)*id,SEEK_SET);
    read(fd,a,sizeof(struct account));
    if(a->id==id){
        if(a->balance+change<0){
            unlock(fd);
            close(fd);
            return -4;
        }
        int newb=addTransaction(id,change);
        if(newb==-1){
            unlock(fd);
            close(fd);
            return -5;
        }
        a->balance=newb;
        lseek(fd,-sizeof(struct account),SEEK_CUR);
        write(fd,a,sizeof(struct account));
        unlock(fd);
        close(fd);
        return a->balance;
    }
    unlock(fd);
    close(fd);
    return a->balance;
}

bool initAccounts(){
    int fd=open(ACCOUNTSFILE,O_RDWR|O_CREAT|O_EXCL,0744);
    if(fd==-1) return 0;
    struct account *a=malloc(sizeof(struct account));
    a->id=0;
    a->balance=0;
    //a->accountNum=(int)NULL;
    initTransactions(0);
    write(fd,a,sizeof(struct account));
    close(fd);
    return 1;
}

void printdataAccount(){
    int fd=open(ACCOUNTSFILE,O_RDONLY,0744),i;
    struct account *a=malloc(sizeof(struct account));
    lseek(fd,0,SEEK_SET);
    for(i=0;i<=getCountAccounts();i++){
        read(fd,a,sizeof(struct account));
        printf("%d %d\n",a->id,a->balance);
    }
    close(fd);
}
#endif

// int main(int argc, char *argv[]){
//     if(!initAccounts()){
        
//         if(addAccount()==-1) printf("error!\n");
//         if(addAccount()==-1) printf("error!\n");
//         if(addAccount()==-1) printf("error!\n");
//         if(addAccount()==-1) printf("error!\n");
//         if(transact(2,30)==-1) printf("error!\n");
//         if(transact(2,-40)==-1) printf("error!\n");
//         if(transact(1,10)==-1) printf("error!\n");
//         printdataTransactions(2);
//         // if(add("user2")==-1) printf("error!\n");
//         // if(add("user3")==-1) printf("error!\n");
//         // if(add("user2")==-1) printf("error!\n");
//         // deleteByUsr("user1");
//         // deleteByUsr("user1");
//         // deleteById(2);
//          printdataAccount();
//     }
//     else{
//         printf("initialized!\n");
//     }
// }