// Created by Dor Green on 14/05/2019.
//

#include "user.h"
#include "types.h"
#include "mmu.h"
#include "user.h"

int test_no = 0;

int very_simple(int pid){
    if(pid == 0){
        printf(1, "rest...\n");
        sleep(100);
        printf(1, "exit...\n");
        exit();
    }
    if(pid > 0){
        printf(1, "Parent waiting on test %d\n", test_no);
        wait();
        printf(1, "Done!\n");
        return 1;
    }
    return 0;
}


int simple(int pid){
    if(pid == 0){
        printf(1, "try alloc, access, free...\n");
        char* trash = malloc(3);
        trash[1] = 't' ;
        if(trash[1] != 't'){
            printf(1,"Wrong data in simple! %c\n", trash[1]);
        }
        free(trash);
        printf(1, "Done malloc dealloc, exiting...\n");
        exit();
    }
    if(pid > 0){
        printf(1, "Parent waiting on test %d\n", test_no);
        wait();
        printf(1, "Done! malloc dealloc\n");
        return 1;
    }
    return 0;
}


int test_paging(int pid, int pages){
    if(pid == 0){
        int size = pages;
        char* pp[size];
        for(int i = 0 ; i < size ; i++){
            printf(1, "Call %d for sbrk\n", i);
            pp[i] = sbrk(PGSIZE-1);
            *pp[i] = '0' +(char) i ;
        }
        printf(1, "try accessing this data...\n");

        if(size > 4 && *pp[4] != '4'){
            printf(1,"Wrong data in pp[4]! %c\n", *pp[4]);
        }

        if(*pp[0] != '0'){
            printf(1,"Wrong data in pp[0]! %c\n", *pp[0]);
        }

        if(*pp[size-1] != '0' +(char) size-1 ){
            printf(1,"Wrong data in pp[size-1]! %c\n", *pp[size-1]);
        }

        printf(1, "Done allocing, exiting...\n");
        exit();

    }

    if(pid >  0){
        printf(1, "Parent waiting on test %d\n", test_no);
        wait();
        printf(1, "Done! alloce'd some pages\n");
        return 1;
    }
    return 0;
}



int main(int argc, char *argv[]){

    printf(1, "--------- START TESTING! ---------\n");


    printf(1, "------- test%d -------\n", test_no);
    very_simple(fork());
    test_no++;

    printf(1, "------- test%d -------\n", test_no);
    simple(fork());
    test_no++;

    printf(1, "------- test%d -------\n", test_no);
    test_paging(fork(),2);
    test_no++;

    printf(1, "------- test%d -------\n", test_no);
    test_paging(fork(),6);
    test_no++;

    printf(1, "------- test%d -------\n", test_no);
    test_paging(fork(),13);
    test_no++;

    // TODO: ADD MORE TESTS!!


    printf(1, "--------- DONE  TESTING! ---------\n");
    return 0;
}