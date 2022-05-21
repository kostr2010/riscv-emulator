int main()
{
    int i = 0;
    int j = 0;
    while (j < 10) {
        if (i % 2) {
            j += 2;
        } else {
            j += 1;
        }
        i++;
    }

    return i;
}