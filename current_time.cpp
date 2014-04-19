#include <stdio.h>
#include <sys/time.h>
int main() {
	struct timeval t;
	struct timezone tz;

	if (gettimeofday(&t, &tz) == 0)
		printf("%ld%d\n", t.tv_sec, t.tv_usec/1000);

	return 0;
}
