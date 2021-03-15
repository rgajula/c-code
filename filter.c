#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

void exec1();
void exec2();

// ls -l | wc -l
int pipe1[2];  

int main(int argc, char  **argv){

	pid_t wpid, pid;
	int status=0, nbytes;
	char readbuffer[500];

	if (pipe(pipe1) == -1){
		fprintf(stderr, "failed to create pipe1 %d", errno);
		exit(1);
	}

	if ((pid = fork()) == -1){
		fprintf(stderr,"failed to create fork %d", errno);
		close(pipe1[0]);
		close(pipe1[1]);
		exit(2);
	} else if (pid == 0) {
		exec1();
		exit(0);
	}

	if ((pid = fork()) == -1){
		fprintf(stderr,"failed to create fork %d", errno);
		close(pipe1[0]);
		close(pipe1[1]);
		exit(2);
	} else if (pid == 0) {
		exec2();
		exit(0);
	}

	// close unncesssory fds
	close(pipe1[0]);
	close(pipe1[1]);

	// wait until all child process are completed
	while((wpid = wait(&status)) > 0);
}

// create stdout 
void exec1(){
	char *args[3] = {"ls", "-l", NULL};

	//redirect std output
	dup2(pipe1[1], 1);
	
	//close unncessory fds
	close(pipe1[0]);
	close(pipe1[1]);

	execvp("ls", args);

  perror("failed to execute ls -l");
	exit(1);
}

void exec2(){

	char *argv[3] = {"wc", "-l", NULL};
	
	//redirect to std input
	dup2(pipe1[0], 0);
	
	close(pipe1[0]);
	close(pipe1[1]);

	execvp("wc", argv);
	
	perror("failed to execute wc -l");
	exit(14);
}
