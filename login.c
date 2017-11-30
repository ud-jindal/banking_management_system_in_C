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

//verify login

int verifyLogin(char username[40],char pass[40],int type){
    int id=getId(username);
    if(id==-1) return -1;
    if(verify(id,pass,type)) return id;
    else return -1;
}

// int main(int argc, char *argv[]){
    
// }