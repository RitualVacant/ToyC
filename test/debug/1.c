int f(int a, int b)
{
  return a + b ? a : b;
}
int main()
{
  int a = 1;
  int b[3][5];
  for (int i = 1; i < 10; i++)
  {
    a += i;
    f(i, a);
  }
}