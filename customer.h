#ifndef CUSTOMER_H
#define CUSTOMER_H

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

//deposit/withraw
int moneyTransaction(int userId,int money){
    int accId=getAccountNumber(userId);
    if(accId==-1) return -1;
    int newBalance=transact(accId,money);
    return newBalance;
}

//balance
int checkBalance(int userId){
    int accId=getAccountNumber(userId);
    if(accId==-1) return -1;
    int balance=getAccountBalance(accId);
    return balance;
}

//changepass
bool changeUserPassword(int id,char old[40],char new[40]){
    return changePassword(id,old,new);
}

//viewtransactiondetails(pass the buffer in the arguments)
bool viewTransactionDetails(int userId,char *buf){
    int accId=getAccountNumber(userId);
    if(accId==-1) return false;
    return printdataTransactions(buf,accId);
}

#endif

// int main(int argc, char *argv[]){
//     char a[1000]="";
//     printdataUserId();
//     printdataUser();
//     printdataAccount();
//     //printf("%d\n",moneyTransaction(2,50));
//     // moneyTransaction(2,20);
//     // moneyTransaction(2,-60);
//     changeUserPassword(8,"user2","user3");
//     changeUserPassword(2,"user2","user4");
//     int bal=checkBalance(2);
//     printf("%d\n",bal);
//     if(viewTransactionDetails(2,a)) printf("%s",a);
// }