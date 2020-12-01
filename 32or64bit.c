tail -n +2 $0 >/tmp/cs.$$.c && gcc -Wall -O2 -o /tmp/cs.$$ /tmp/cs.$$.c && /tmp/cs.$$ $*; rm -f /tmp/cs.$$*; exit
#include <stdio.h>
int main(void) {
  printf("%d\n", (int)sizeof(void *) * 8);
  return 0;
}
