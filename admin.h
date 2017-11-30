#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "userIdHandler.h"

//add
int addNormalCustomer(char username[40], char password[40], char fname[40], char lname[40]){
    int id=addUserId(username,password,fname,lname,0,0);
    if(id==-1) return -1;
}

int addJointCustomer(char username[40], char password[40], char fname[40], char lname[40], int acnum){
    int id=addUserId(username,password,fname,lname,1,acnum);
    if(id==-1) return -1;
}

//delete
bool delete(int id){
    return deleteById(id);
}

//modify
bool changeFname(int id,char new[40]){
    return changefname(id,new);
}

bool changeLname(int id,char new[40]){
    return changelname(id,new);
}

int resetPassword(int id){
    return resetPass(id);
}

bool deleteTransaction(int account, int TransId){
    return deleteTrans(account,TransId);
}


//retrieve

// int main(int argc, char *argv[]){
//     if(!init()){
//         addNormalCustomer("user1","user1","user","1");
//         addNormalCustomer("user2","user2","user","2");
//         addNormalCustomer("user3","user3","user","3");
//         addJointCustomer("user5","user4","user","4",3);
//         delete(3);
//         printdataUserId();
//         printdataUser();
//         printdataAccount();

//         //printdataTransactions();
//     }
//     else{
//         printf("initialized!\n");
//     }
// }