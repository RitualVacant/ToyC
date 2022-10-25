int main() {
  int          a = 1;
  long long    b = 2;
  int          c = a + b;
  unsigned int e;
  e = c / a + b + (a / b);
  return e;
}