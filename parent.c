#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    int num_children = 4;
    int second_child_number = 0, third_child_number = 0;

    for (int i = 0; i < num_children; i++) {
        pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {  // Child process
            if (i == 0) {
                execlp("./child1", "./child1", argv[1], argv[2], (char *)NULL);
                perror("execlp failed");
                exit(EXIT_FAILURE);
            } else if (i == 1) {
                close(pipe_fd[0]);
                srand(getpid());
                int random_number = rand() % 11;
                printf("Second child (PID: %d) generated number: %d\n", getpid(), random_number);
                write(pipe_fd[1], &random_number, sizeof(random_number));
                close(pipe_fd[1]);
                exit(EXIT_SUCCESS);
            } else if (i == 2) {
                close(pipe_fd[0]);
                srand(getpid()); 
                int random_number = rand() % 11;
                printf("Third child (PID: %d) generated number: %d\n", getpid(), random_number);
                write(pipe_fd[1], &random_number, sizeof(random_number));
                close(pipe_fd[1]); 
                exit(EXIT_SUCCESS);
            } else if (i == 3) {
                wait(NULL);  
                execlp("date", "date", "+%Y-%m-%d %H:%M:%S", (char *)NULL);
                perror("execlp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            if (i == 1) {
                wait(NULL);  
                close(pipe_fd[1]); 
                read(pipe_fd[0], &second_child_number, sizeof(second_child_number));
                printf("Parent read number from second child: %d\n", second_child_number);
            } else if (i == 2) {
                wait(NULL);  // Wait for the third child
                read(pipe_fd[0], &third_child_number, sizeof(third_child_number));
                printf("Parent read number from third child: %d\n", third_child_number);
                
                if (second_child_number > third_child_number) {
                    printf("Second child is the winner with number: %d\n", second_child_number);
                } else if (third_child_number > second_child_number) {
                    printf("Third child is the winner with number: %d\n", third_child_number);
                } else {
                    printf("It's a tie! Both generated the number: %d\n", second_child_number);
                }
            } else if (i == 3) {
                wait(NULL);
            }
        }
    }

    close(pipe_fd[0]);

    printf("Parent process (PID: %d) completed\n", getpid());
    return 0;
}
