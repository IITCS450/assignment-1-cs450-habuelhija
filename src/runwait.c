#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
static void usage(const char *a){
	fprintf(stderr,"Usage: %s <cmd> [args]\n",a); 
	exit(1);
}
static double d(struct timespec a, struct timespec b){
	return (b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;
}
int main(int c,char**v){
/* TODO : ADD CODE HERE
*/

	if (c < 2) usage(v[0]);
struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork failed");
	return 1;
}
	else if (pid == 0) {
	execvp(v[1], &v[1]);
		perror("execvp failed");
	exit(1);
}
	else {
		int status;
		waitpid(pid, &status, 0);
		clock_gettime(CLOCK_MONOTONIC, &end);
		printf("PID: %d\n", pid);

	if (WIFEXITED(status)) {
		printf("Exit Code: %d\n", WEXITSTATUS(status));
	}
	else {
		printf("Exit Code: (Signal)\n");
	}
	printf("Time: %.3f\n", d(start, end));
	}

 return 0;
}
