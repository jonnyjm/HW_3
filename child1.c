#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char* sourceText = argv[1];
    char* destinationText = argv[2];
    int source_fd = open("source.txt", O_RDONLY);
    int destination_fd = open(destinationText, O_WRONLY | O_CREAT | O_TRUNC);

    if (source_fd == -1) {
        printf("Error opening source file\n");
        return -1;
    }

    if (destination_fd == -1) {
        printf("Error opening destination file\n");
        return -1;
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(source_fd, buffer, sizeof(buffer))) > 0) {
        if (write(destination_fd, buffer, bytesRead) == -1) {
            printf("Error writing to destination file\n");
            return -1;
        }
    }

    close(source_fd);
    close(destination_fd);  
}