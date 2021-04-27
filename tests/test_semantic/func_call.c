int f(int a[][2], int b[], int c)
{

}

void g(int c)
{

}

int main()
{
	int a[2][2];
	const int b[2][2] = {};
	g(1);
	g(a[1][1]);
	g(b[1][1]);
	g(a[1]);
	(1 + g(1)) + (g(1) + 1);
	1 + f(a, a[1], a[1][1]) + 1;
	f(a[1], a, a);
	f(b, b[1], b[1][1]);
}