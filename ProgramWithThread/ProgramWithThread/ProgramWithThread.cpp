#include <stdio.h>
#include <Windows.h>

int count = 0;
CRITICAL_SECTION cs;

DWORD WINAPI MyThread(LPVOID param) {
    DWORD threadID = GetCurrentThreadId();
    Sleep(30000);
    EnterCriticalSection(&cs);
    count++;
    LeaveCriticalSection(&cs);
    printf("This is my thread%d\n", threadID);
    return 0;
}

int main()
{
    InitializeCriticalSection(&cs);
    DWORD threadID = GetCurrentThreadId();
    DWORD ID = 0;
    printf("Hello my thread!%d\n", threadID);
    HANDLE myThreadHandle[64];
    for (int i = 0; i < 64; i++) {
        myThreadHandle[i] = CreateThread(NULL, 0, MyThread, NULL, 0, &ID);
    }
        
    WaitForMultipleObjects(64, myThreadHandle, TRUE, INFINITE);
    
    printf("Done! %d", count);
    DeleteCriticalSection(&cs);
    return 0;
}
