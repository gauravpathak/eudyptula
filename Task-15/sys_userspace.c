#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
    
    unsigned int high = 0x823d;    //Valid High Byte
    unsigned int low = 0xaa4ac90a; //Valid Low Byte

    long int retval = syscall(294, high, low);
    printf("System call sys_eudyptula returned %ld\n", retval);
    
    high = 0x823d;    //Valid High Byte
    low = 0xaa4ac90b; //Invalid Low Byte

    retval = syscall(294, high, low);
    printf("System call sys_eudyptula returned %ld\n", retval);

    return 0;
}
