#include <iostream>


int main()
{
    const char *string = "hello9";
    std :: string s;

    s = string;
    size_t pos = s.find("9");
    std :: cout << pos << std :: endl;
    std :: cout << sizeof(string) << std :: endl;
}