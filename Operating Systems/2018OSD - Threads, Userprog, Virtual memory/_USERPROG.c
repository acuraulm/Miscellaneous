// Userprog

//##################################################################################################################################################################################################//
// 1. Set up userprog

//thread.c
function
_ThreadSetupMainThreadUserStack() {
	*ResultingStack = (PVOID) PtrDiff(InitialStack, 0x108);
}
//implement SyscallIdIdentifyVersion, SyscallIdProcessExit, SyscallIdThreadExit, SyscallIdFileWrite

//syscall.c

function
SyscallHandler()
{
		switch (sysCallId) {
			case SyscallIdIdentifyVersion:
			{
				SYSCALL_IF_VERSION version = (SYSCALL_IF_VERSION)pSyscallParameters[0];

				if (version != SYSCALL_IMPLEMENTED_IF_VERSION)
					status = STATUS_UNSUCCESSFUL;
				break;
			}
			case SyscallIdProcessExit:
			{

				STATUS exitStatus = (STATUS)pSyscallParameters[0];
				if (&exitStatus == NULL)
					status = STATUS_INVALID_PARAMETER1;
				else {
					PPROCESS pProcess = GetCurrentProcess();
					pProcess->TerminationStatus = exitStatus;
					ProcessTerminate(pProcess);
				}
				break;
			}
			case SyscallIdThreadExit:
			{
				STATUS exitStatus = (STATUS)pSyscallParameters[0];
				if (&exitStatus == NULL)
					status = STATUS_INVALID_PARAMETER1;
				else
					ThreadExit(exitStatus);
				break;
			}
			case SyscallIdFileWrite:
			{
				UM_HANDLE FileHandle = (UM_HANDLE)pSyscallParameters[0];
				PVOID     Buffer = (PVOID)pSyscallParameters[1];
				QWORD     BytesToWrite = (QWORD)pSyscallParameters[2];
				QWORD*    BytesWritten = (QWORD*)pSyscallParameters[3];

				if (FileHandle != UM_FILE_HANDLE_STDOUT) {
					status = STATUS_NOT_IMPLEMENTED;
				}
				if (MmuIsBufferValid(Buffer, BytesToWrite, PAGE_RIGHTS_WRITE, GetCurrentProcess()) != STATUS_SUCCESS) {
					status = STATUS_INVALID_BUFFER;
				}
				if (BytesWritten == NULL) {
					status = STATUS_INVALID_PARAMETER4;
				}
				if (status == STATUS_SUCCESS) {
					*BytesWritten = BytesToWrite;

					LOG("[%s]:[%s]\n", ProcessGetName(NULL), Buffer);
				}
				break;
			}
		}
}


//##################################################################################################################################################################################################//
// 2. Adding a new syscall

//syscall_no.h - Id-ul pentru syscall
typedef enum _SYSCALL_ID
{
	SyscallIdSyscallName,
} SYSCALL_ID;

//syscall_if.c - Syscall entry pentru syscallId
STATUS
SyscallName();//params);
{
    return SyscallEntry(SyscallName );//,params);
}

//syscall_func.h - Syscall function definition
STATUS
SyscallName();//params);


//syscall.c
void
SyscallHandler()
{
	// The first parameter is the system call ID, we don't care about it => +1
		pSyscallParameters = (PQWORD)usermodeProcessorState->RegisterValues[RegisterRbp] + 1;
		//after this
		switch(sysCallId){
			case SyscallIdSyscallName:{
					//params
					status = SyscacallName();//params);
				break;
			}
		}
}

STATUS 	// sycall function implementation
SyscallName()//params)
{
	return STATUS_SUCCESS;
}