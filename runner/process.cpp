#include "process.h"
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

static constexpr int MAX_ARGS = 32;
static constexpr int MAX_BUFFER = 4096;

static constexpr bool isChildProcess(pid_t pid)
{
  return pid == 0;
}

static void handleChildProcess(int pipe_fd[2], const std::string& path, std::vector<std::string> args)
{
  std::array<char *, MAX_ARGS> c_args;

  dup2(pipe_fd[1], STDOUT_FILENO);
  close(STDERR_FILENO);
  close(pipe_fd[0]);
  close(pipe_fd[1]);

  c_args.fill(NULL);
  c_args[0] = (char *)path.c_str();
  for (size_t i=0; i<args.size(); ++i)
  {
    c_args[i+1] = (char *)args[i].c_str();
  }

  execv(path.c_str(), c_args.data());
}

static int waitForChildren()
{
  pid_t pid;
  int status = 0;

  while ((pid = wait(&status)) > 0);

  return status;
}
#include <iostream>
static int handleParentProcess(int pipe_fd[2], std::function<void(std::string)> on_output)
{
  std::string output = "";
    close(pipe_fd[1]);

  ssize_t read_bytes = 0;
  do
  {
    std::array<char, MAX_BUFFER> buffer;
    buffer.fill('\0');
    read_bytes = read(pipe_fd[0], buffer.data(), buffer.size());
    buffer[read_bytes + 1] = '\0';
    output += buffer.data();
  } while (read_bytes != 0);

  on_output(output);

  return waitForChildren();
}

int Process::runExecutable(
  const std::string& path,
  std::function<void(std::string)> on_output,
  const std::vector<std::string>& args,
  int64_t& elapsed_time
) {
  const auto start = std::chrono::high_resolution_clock::now();
  int status = 0;
  int pipe_fd[2];
  if (pipe(pipe_fd) < 0)
    return -1;

  const auto pid = fork();

  if (isChildProcess(pid))
  {
    handleChildProcess(pipe_fd, path, args);
  }
  else
  {
    status = handleParentProcess(pipe_fd, on_output);
  }

  const auto end = std::chrono::high_resolution_clock::now();
  elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

  return status;
}

bool Process::killedBySignal(int status)
{
  return status != 0 && WIFSIGNALED(status);
}
