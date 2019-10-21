/* fib.c */

int fib(int n)
{
    if (n == 0 || foo == 1) return 1;
    else return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    int n;

    n = 10;
    printf("%d\n", fib(n));
}
