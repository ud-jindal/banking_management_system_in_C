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
#include "accountDetailsHandler.h"
#define USERFILE "userDetails.bin"

struct user{
    int id;
    char fname[40];
    char lname[40];
    int type; //0 for admin, 1 for normal, 2 for joint
    int accountNum;
    char password[40];
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

int getCountUser(){
    int fd=open(USERFILE,O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct user *a=malloc(sizeof(struct user));
    lseek(fd,-sizeof(struct user),SEEK_END);
    read(fd,a,sizeof(struct user));
    unlock(fd);
    close(fd);
    return a->id;
}

int getAccountNumber(int id){
    int fd=open(USERFILE,O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct user *a=malloc(sizeof(struct user));
    lseek(fd,sizeof(struct user),SEEK_SET);
    int tot=getCountUser();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct user));
        if(a->id==id){
            unlock(fd);
            close(fd);
            return a->accountNum;
        }
    }
    unlock(fd);
    close(fd);
    return -1;
}

int addNormal(int id,char fname[40],char lname[40],char pass[40]){
    int fd=open(USERFILE,O_RDWR,0744),i;
    if(fd==-1) return -1;
    setLock(fd,1); 
    struct user *a=malloc(sizeof(struct user));
    strcpy(a->fname,fname);
    strcpy(a->lname,lname);
    strcpy(a->password,pass);
    a->type=1;
    a->id=id;
    //get new account number!
    a->accountNum=addAccount();
    if(a->accountNum==-1){
        unlock(fd);
        close(fd);
        return -1;
    }
    lseek(fd,0,SEEK_END);
    write(fd,a,sizeof(struct user));
    unlock(fd);
    close(fd);
    return a->accountNum;
    //return a->id;
}

int addJoint(int id,char fname[40],char lname[40],char pass[40],int accountNum){
    int fd=open(USERFILE,O_RDWR,0744),i;
    if(fd==-1) return -1;
    setLock(fd,1); 
    struct user *a=malloc(sizeof(struct user));
    strcpy(a->fname,fname);
    strcpy(a->lname,lname);
    strcpy(a->password,pass);
    a->type=2;
    a->id=id;
    a->accountNum=accountNum;
    lseek(fd,0,SEEK_END);
    write(fd,a,sizeof(struct user));
    unlock(fd);
    close(fd);
    return a->accountNum;
    //return a->id;
}

bool changePassword(int id, char old[40], char new[40]){
    int fd=open(USERFILE,O_RDWR,0744),i;
    if(fd==-1) return 0;
    setLock(fd,1);
    struct user *a=malloc(sizeof(struct user));
    lseek(fd,sizeof(struct user),SEEK_SET);
    int tot=getCountUser();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct user));
        if(a->id==id && !strcmp(old,a->password)){
            strcpy(a->password,new);
            lseek(fd,-sizeof(struct user),SEEK_CUR);
            write(fd,a,sizeof(struct user));
            unlock(fd);
            close(fd);
            return true;
        }
    }
    unlock(fd);
    close(fd);
    return 0;
}

bool initUser(){
    int fd=open(USERFILE,O_RDWR|O_CREAT|O_EXCL,0744);
    if(fd==-1) return 0;
    struct user *a=malloc(sizeof(struct user));
    a->id=0;
    strcpy(a->fname,"admin");
    strcpy(a->lname,"admin");
    strcpy(a->password,"admin");
    a->type=0;
    a->accountNum=0;
    initAccounts();
    write(fd,a,sizeof(struct user));
    close(fd);
    return 1;
}

void printdataUser(){
    int fd=open(USERFILE,O_RDONLY,0744),i;
    struct user *a=malloc(sizeof(struct user));
    lseek(fd,0,SEEK_SET);
    for(i=0;i<=getCountUser();i++){
        read(fd,a,sizeof(struct user));
        printf("%d %s %s %s %d\n",a->id,a->fname,a->lname,a->password,a->type);
    }
    close(fd);
}

//  int main(int argc, char *argv[]){
//     if(!initUser()){
//         // if(add("user1")==-1) printf("error!\n");
//         // if(add("user2")==-1) printf("error!\n");
//         // if(add("user3")==-1) printf("error!\n");
//         // if(add("user2")==-1) printf("error!\n");
//         // deleteByUsr("user1");
//         // deleteByUsr("user1");
//         // deleteById(2);
//          printdataUser()();
//     }
//     else{
//         printf("initialized!\n");
//     }
//}