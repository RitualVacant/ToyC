int main() {
  int a = 4;
  switch (a) {
    case 3:
      a = 5;
      break;
    case 2:
      a = 4;
      break;
    case 1:
      a = 3;
      break;
  }
  int b = a;
  return b;
}