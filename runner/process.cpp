#include "process.h"
#include <unistd.h>
#include <sys/wait.h>

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

  for (int i=0; i<args.size(); ++i)
  {
    c_args[i] = (char *)args[i].c_str();
  }

  execv(path.c_str(), c_args.data());
}

static void waitForChildren()
{
  pid_t pid;
  int status = 0;

  while ((pid = wait(&status)) > 0);
}

static void handleParentProcess(int pipe_fd[2], std::function<void(std::string)> on_output)
{
  std::array<char, MAX_BUFFER> buffer;

  buffer.fill('\0');
  close(pipe_fd[1]);
  read(pipe_fd[0], buffer.data(), buffer.size());

  on_output(std::string(buffer.data()));

  waitForChildren();
}

void Process::runExecutable(const std::string& path, std::function<void(std::string)> on_output)
{
  int pipe_fd[2];
  pipe(pipe_fd);

  const auto pid = fork();

  if (isChildProcess(pid))
  {
    std::vector<std::string> args = { "-o" };
    handleChildProcess(pipe_fd, path, args);
  }
  else
  {
    handleParentProcess(pipe_fd, on_output);
  }
}
