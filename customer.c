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
    if(accId==-1) return -1;
    return printdataTransactions(buf,accId);
}

int main(int argc, char *argv[]){
    
}