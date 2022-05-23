#include <stdlib.h>

int main()
{
    char word[20] = "hi. i'm dave\n";
    char* w = calloc(20, sizeof(char));
    return (int)word[10]; // 118
}