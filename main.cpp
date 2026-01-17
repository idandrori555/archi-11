#include <stdio.h>
#include <windows.h>

DWORD WINAPI func(LPVOID lparam)
{
  printf("Hello From thread\n");
  return 0;
}

int main(void)
{
  char command[] = "test.exe";
  STARTUPINFOA si = {sizeof(si)};
  PROCESS_INFORMATION pi = {0};

  if (CreateProcessA(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
  {
    printf("Process created\n");
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
  }

  return 0;
}
