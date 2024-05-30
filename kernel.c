#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>

#include "system.h"
#include "fork_call.h"
#include "ipc_call.h"


void print_minios(char* str);


int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }
        
        if (strcmp(input,"minisystem") == 0){
            minisystem();
        }
        
        else if (strcmp(input, "fork_call") == 0){
            fork_call();
        }
       
        else if (strcmp(input, "ipc_call") == 0){
            ipc_call();
        }
	else if(strcmp(input, "round_robin")==0)
	{
	   round_robin_ex();
	}
        else if(strcmp(input, "pi_mutex")==0)
        {
           pi_mutex();
        }
        else system(input);
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}

void print_minios(char* str) {
        printf("%s\n",str);
}
