#include <stdio.h>
#include <string.h>

char calcSum(const char *str) {
  char r = 0x00;
  int i = 0;

  while (str[i] != '\0' && str[i] != '*') {
    r ^= str[i];
    i++;
  }

  return r;
}

int main() {
  char out[100];
  char in[81];

  scanf("%s", in);

  int r = calcSum(in);

  sprintf(out, "checksum: %X\n\n", r);
  printf(out);

  return 0;
}
