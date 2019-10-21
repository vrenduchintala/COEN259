/* qsort.c */

int partition(int a[], int lo, int hi)
{
    int i, j, x, temp;


    x = a[lo];
    i = lo - 1;
    j = hi + 1;

    while (i < j) {
	do
	    j = j - 1;
	while (a[j] > x);

	do
	    i = i + 1;
	while (a[i] < x);

	if (i < j) {
	    temp = a[i];
	    a[i] = a[j];
	    a[j] = temp;
	}
    }

    return j;
}


int quickSort(int a[], int lo, int hi)
{
    int i;


    if (hi > lo) {
	i = partition(a, lo, hi);
	quickSort(a, lo, i - 1);
	quickSort(a, i + 1, hi);
    }
}


int main(void)
{
    int a[20], i;


    for (i = 0; i < 20; i = i + 1)
	a[i] = rand() % 20;

    quickSort(a, 0, 20 - 1);

    for (i = 0; i < 20; i = i + 1)
	printf("%d\n", a[i]);
}
