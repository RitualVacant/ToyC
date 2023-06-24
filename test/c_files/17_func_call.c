char *input(char *str, int len)
{
  *(str + 3) = 'a';
  return str;
}

int main()
{
  int a = 1;
  int b = 3;
  do
  {
    b += a -= 3;
  } while (a < 4);
  return 0;
}
