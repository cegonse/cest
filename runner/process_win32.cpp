#include "process.h"
#include <windows.h>
#include <chrono>
#include <array>

static constexpr int MAX_BUFFER = 4096;

static std::string buildCommandLine(const std::string& path, const std::vector<std::string>& args)
{
  std::string cmd = "\"" + path + "\"";
  for (const auto& arg : args)
    cmd += " " + arg;
  return cmd;
}

int Process::runExecutable(
  const std::string& path,
  std::function<void(std::string)> on_output,
  const std::vector<std::string>& args,
  int64_t& elapsed_time
) {
  const auto start = std::chrono::high_resolution_clock::now();

  SECURITY_ATTRIBUTES sa;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  HANDLE hReadPipe, hWritePipe;
  if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
    return -1;

  SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

  STARTUPINFOA si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  si.hStdOutput = hWritePipe;
  si.hStdError = NULL;
  si.dwFlags |= STARTF_USESTDHANDLES;

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));

  std::string cmd_line = buildCommandLine(path, args);

  BOOL success = CreateProcessA(
    NULL,
    const_cast<char*>(cmd_line.c_str()),
    NULL,
    NULL,
    TRUE,
    0,
    NULL,
    NULL,
    &si,
    &pi
  );

  CloseHandle(hWritePipe);

  if (!success)
  {
    CloseHandle(hReadPipe);
    const auto end = std::chrono::high_resolution_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return -1;
  }

  std::string output;
  std::array<char, MAX_BUFFER> buffer;
  DWORD bytes_read = 0;
  while (ReadFile(hReadPipe, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes_read, NULL) && bytes_read > 0)
  {
    output.append(buffer.data(), bytes_read);
  }
  CloseHandle(hReadPipe);

  on_output(output);

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code = 0;
  GetExitCodeProcess(pi.hProcess, &exit_code);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  const auto end = std::chrono::high_resolution_clock::now();
  elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

  return static_cast<int>(exit_code);
}

bool Process::killedBySignal(int status)
{
  return status != 0 && (static_cast<unsigned int>(status) >= 0xC0000000u);
}
