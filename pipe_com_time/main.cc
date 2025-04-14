#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];

  if (pipe(fd) == -1) {
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    return 1;
  }

  const char *message = "WAKE UUUUUP, YOU HAVE TO LIFT";
  const int msg_size = strlen(message) + 1;
  char buffer[100];

  if (pid == 0) {

    read(fd[0], buffer, msg_size);
    std::cout << "Child received: " << buffer << "\n";

    close(fd[0]);
    close(fd[1]);
  } else {

    auto start = std::chrono::high_resolution_clock::now();

    write(fd[1], message, msg_size);
    wait(nullptr);

    auto end = std::chrono::high_resolution_clock::now();

    close(fd[1]);
    close(fd[0]);

    std::chrono::duration<double, std::micro> duration = end - start;
    std::cout << "Communication time: " << duration.count()
              << " microseconds\n";
  }

  return 0;
}
