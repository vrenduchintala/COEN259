/* bsearch.c */

int bsearch(int a[], int lo, int hi, int x)
{
    int mid;


    if (lo > hi)
	return 0;

    mid = (lo + hi) / 2;

    if (x == a[mid])
	return 1;

    if (x < a[mid])
	return bsearch(a, lo, mid - 1, x);

    return bsearch(a, mid + 1, hi, x);
}


int main(void)
{
    int a[20], i, n, max, x;


    n = 20;

    for (i = 0; i < n; i = i + 1)
	a[i] = i * 5;

    max = a[n - 1];

    for (i = 0; i < n; i = i + 1) {
	x = rand() % max;
	printf("%d %d\n", x, bsearch(a, 0, n - 1, x));
    }
}
