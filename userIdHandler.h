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

struct data{
    int id;
    char username[40];
    bool isThere;
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

int getCount(){
    int fd=open("userId.bin",O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct data *a=malloc(sizeof(struct data));
    lseek(fd,-sizeof(struct data),SEEK_END);
    read(fd,a,sizeof(struct data));
    unlock(fd);
    close(fd);
    return a->id;
}

int getId(char usr[40]){
    int fd=open("userId.bin",O_RDONLY,0744),i;
    if(fd==-1) return -1;
    setLock(fd,0);
    struct data *a=malloc(sizeof(struct data));
    lseek(fd,sizeof(struct data),SEEK_SET);
    int tot=getCount();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct data));
        if(a->isThere==1 && !strcmp(a->username,usr)){
            unlock(fd);
            close(fd);
            return a->id;
        }
    }
    unlock(fd);
    close(fd);
    return -1;
}

int add(char usr[40]){
    int fd=open("userId.bin",O_RDWR,0744),i;
    if(fd==-1) return -1;
    if(getId(usr)!=-1){
        printf("close %s %d\n",usr,getId(usr));
        close(fd);
        return -1;
    }
    setLock(fd,1); 
    struct data *a=malloc(sizeof(struct data));
    int tot=getCount();
    a->id=tot+1;
    strcpy(a->username,usr);
    a->isThere=true;
    lseek(fd,0,SEEK_END);
    write(fd,a,sizeof(struct data));
    unlock(fd);
    close(fd);
    return a->id;
}

bool deleteById(int id){
    int fd=open("userId.bin",O_RDWR,0744),i;
    if(fd==-1) return 0;
    setLock(fd,1);
    struct data *a=malloc(sizeof(struct data));
    lseek(fd,sizeof(struct data),SEEK_SET);
    int tot=getCount();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct data));
        if(a->isThere==1 && a->id==id){
            a->isThere=false;
            lseek(fd,-sizeof(struct data),SEEK_CUR);
            write(fd,a,sizeof(struct data));
            unlock(fd);
            close(fd);
            return true;
        }
    }
    unlock(fd);
    close(fd);
    return 0;
}

bool deleteByUsr(char usr[40]){
    int fd=open("userId.bin",O_RDWR,0744),i;
    if(fd==-1) return 0;
    setLock(fd,1);
    struct data *a=malloc(sizeof(struct data));
    lseek(fd,sizeof(struct data),SEEK_SET);
    int tot=getCount();
    for(i=0;i<tot;i++){
        read(fd,a,sizeof(struct data));
        if(a->isThere==1 && !strcmp(a->username,usr)){
            a->isThere=false;
            lseek(fd,-sizeof(struct data),SEEK_CUR);
            write(fd,a,sizeof(struct data));
            unlock(fd);
            close(fd);
            return true;
        }
    }
    unlock(fd);
    close(fd);
    return 0;
}

bool init(){
    int fd=open("userId.bin",O_RDWR|O_CREAT|O_EXCL,0744);
    if(fd==-1) return 0;
    struct data *a=malloc(sizeof(struct data));
    a->id=0;
    strcpy(a->username,"admin");
    a->isThere=true;
    write(fd,a,sizeof(struct data));
    close(fd);
    return 1;
}

void printdata(){
    int fd=open("userId.bin",O_RDONLY,0744),i;
    struct data *a=malloc(sizeof(struct data));
    lseek(fd,0,SEEK_SET);
    for(i=0;i<=getCount();i++){
        read(fd,a,sizeof(struct data));
        if(a->isThere==1)
        printf("%d %s %d\n",a->id,a->username,a->isThere);
    }
    close(fd);
}

// int main(int argc, char *argv[]){
//     if(!init()){
//         if(add("user1")==-1) printf("error!\n");
//         if(add("user2")==-1) printf("error!\n");
//         if(add("user3")==-1) printf("error!\n");
//         if(add("user2")==-1) printf("error!\n");
//         deleteByUsr("user1");
//         deleteByUsr("user1");
//         deleteById(2);
//         printdata();
//     }
//     else{
//         printf("initialized!\n");
//     }
// }