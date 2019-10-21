/* isort.c */

int insertionSort(int a[], int n)
{
    int i, j, temp;


    for (i = 1; i < n; i = i + 1) {
	temp = a[i];

	for (j = i - 1; j >= 0 && temp < a[j]; j = j - 1)
	    a[j + 1] = a[j];

	a[j + 1] = temp;
    }
}


int main(void)
{
    int a[20], i;


    for (i = 0; i < 20; i = i + 1)
	a[i] = rand() % 20;

    insertionSort(a, 20);

    for (i = 0; i < 20; i = i + 1)
	printf("%d\n", a[i]);
}
