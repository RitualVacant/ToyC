int main(int argc, char **argv)
{
  int a = 0, b = 2 + 3, *c;
  int d = a + b;
  for (int i = 1; i < b; i = i + 4)
  {
    if (b < 3 && (a == 1 || b >= a) && d != 0)
    {
      a = 1 + d;
    }
  }
  return 0;
}