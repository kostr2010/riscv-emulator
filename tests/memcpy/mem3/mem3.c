#include <string.h>

int main()
{
    char word[20] = "hi. i'm dave\n";
    char w[20] = "";
    memcpy(w, word, 20);
    return (int)w[10]; // 118
}