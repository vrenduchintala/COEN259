/* string.c */

char a[100], b[100];

int strcmp(char s1[], char s2[])
{
    int i;


    i = 0;

    while (s1[i] != '\0' && s1[i] == s2[i])
	i = i + 1;

    return s1[i] - s2[i];
}


int strcpy(char s1[], char s2[])
{
    int i;


    for (i = 0; s2[i] != '\0'; i = i + 1)
	s1[i] = s2[i];

    s1[i] = '\0';
}


int main(void)
{
    strcpy(a, "ABCD");
    strcpy(b, "ABC");
    printf("%d\n", strcmp(a, b));

    strcpy(a, "ABCD");
    strcpy(b, "ABCd");
    printf("%d\n", strcmp(a, b));
}
