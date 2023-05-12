#include <stdlib.h>
#include <stdio.h>

int main() {
    char ip_address[] = "192.168.1.1"; // Replace with the IP address you want to ping
    char command[50];

    sprintf(command, "ping %s", ip_address);
    system(command);

    return 0;
}