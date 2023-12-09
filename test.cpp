#include<iostream>

int main()
{
    char s[10] = "A   b";

    char *res = std :: strtok((char *)s," ");
    printf("%s\n",res);
}