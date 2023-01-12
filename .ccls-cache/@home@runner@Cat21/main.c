#include <stdio.h>

int main(void) {
  char buf[4096];
  size_t charsread;
  while ((charsread = fread(buf, 1, sizeof(buf), stdin)) != 0) {
    printf("Read %zu bytes. EOF: %d. Error: %d.\n", charsread, feof(stdin),
           ferror(stdin));
  }
  printf("Read zero bytes. EOF: %d. Error: %d. Exiting.\n", feof(stdin),
         ferror(stdin));
  return 0;
}