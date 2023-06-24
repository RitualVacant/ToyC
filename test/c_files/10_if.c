int main()
{
  int a = 1;
  int b = 3, c = 32, d = 3;
  if (a < b || c > b)
  {
    b = 1;
    c = c * (b + d);
  }
  else
  {
    a += b;
  }

  if (a < b && c > b)
    a = c * (b + d);
  else
    a += b;

  return 0;
}