#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
static void usage(const char *a) {
	fprintf(stderr,"Usage: %s <pid>\n",a); 
	exit(1);
}

static int isnum(const char*s) {
	for(;*s;s++) 
	if(!isdigit(*s)) return 0; 
	return 1;
}

void print_CmdLine(int pid) {
	char Path[256];
	char CmdLine[1024];
	FILE *file;
	sprintf(Path, "/proc/%d/CmdLine", pid);
	file = fopen(Path, "r");

	if (!file) return;
	int i = 0;
	int ch;
	int Has_Content = 0;

	printf("CmdLine: ");
	while ((ch = fgetc(file)) != EOF && i < 1023) {
	if (ch == '\0') CmdLine[i++] = ' ';
	else CmdLine[i++] = ch;
	Has_Content = 1;
}
	CmdLine[i]= '\0';
	if (Has_Content) printf("%s\n", CmdLine);
	else printf("[No command line]\n");
	fclose(file);
}

void print_Memory(int pid) {
	char Path[256];
	char Line[256];
	FILE *file;
	long vmrss = 0;
	int Found = 0;

	sprintf(Path, "/proc/%d/status", pid);
	file = fopen(Path, "r");

	if (!file) return;
	while(fgets(Line, sizeof(Line), file)) {
		if (strncmp(Line, "VmRSS:", 6) == 0) {
		sscanf(Line, "VmRSS: %ld", &vmrss);
		Found = 1;
		break;
	}
}
	fclose(file);
	if (Found) printf("VmRSS: %ld kB\n", vmrss);
}

void print_Stat_Info(int pid) {
	char Path[256];
	FILE *file;

	int Read_pid;
	char Comm[256];
	char State;
	int ppid;
	unsigned long utime;
	unsigned long stime;

	sprintf(Path, "/proc/%d/stat", pid);
	file = fopen(Path, "r");

	if (!file) { 
	perror("Error opening stat file");
	exit(1);
	}

	long Dummy;
	unsigned int Dummy_u;

	fscanf(file, "%d %s %c %d %ld %ld %ld %ld %u %ld %ld %ld %ld %lu %lu", 
		&Read_pid, Comm, &State, &ppid, &Dummy, &Dummy, &Dummy, &Dummy, &Dummy_u, &Dummy, &Dummy, &Dummy, &Dummy, &utime, &stime);
	fclose(file);

	long ticks_per_sec = sysconf(_SC_CLK_TCK);
	double cpu_seconds = (double)(utime + stime) / ticks_per_sec;

	printf("State: %c\n", State);
	printf("PPID: %d\n", ppid);
	printf("CPU: %.3f\n", cpu_seconds); 
}

int main(int c,char**v){
	if(c != 2 || !isnum(v[1])) usage(v[0]);
	int pid = atoi(v[1]);
	printf("PID: %d\n", pid);

	print_Stat_Info(pid);
	print_Memory(pid);
	print_CmdLine(pid);

	return 0;
}

