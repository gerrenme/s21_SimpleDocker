#include <stdio.h>

#include "fcgi_stdio.h"

int main(void){
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\nStatus: 200 OK\r\n\r\nHello World!");
    }
    return 0;
}
