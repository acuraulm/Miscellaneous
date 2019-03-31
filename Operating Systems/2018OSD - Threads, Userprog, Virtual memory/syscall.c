#include "HAL9000.h"
#include "syscall.h"
#include "gdtmu.h"
#include "syscall_defs.h"
#include "syscall_func.h"
#include "syscall_no.h"
#include "dmp_cpu.h"
#include "process.h"
#include "thread.h"
#include "vmm.h"
#include "thread_internal.h"
#include "process_internal.h"

extern void SyscallEntry();

PUM_FILE_HANDLE
GetFileHandleFromProcess
(
	PPROCESS process,
	UM_HANDLE handle
);

PUM_PROCESS_HANDLE
GetProcessHandleFromProcess
(
	PPROCESS process,
	UM_HANDLE handle
);

void
SyscallHandler(
    INOUT   COMPLETE_PROCESSOR_STATE    *CompleteProcessorState
    )
{
    SYSCALL_ID sysCallId;
    PQWORD pSyscallParameters;
    STATUS status;
    REGISTER_AREA* usermodeProcessorState;

    ASSERT(CompleteProcessorState != NULL);

    // It is NOT ok to setup the FMASK so that interrupts will be enabled when the system call occurs
    // The issue is that we'll have a user-mode stack and we wouldn't want to receive an interrupt on
    // that stack. This is why we only enable interrupts here.
    ASSERT(CpuIntrGetState() == INTR_OFF);
    CpuIntrSetState(INTR_ON);

    LOG_TRACE_USERMODE("The syscall handler has been called!\n");

    status = STATUS_SUCCESS;
    pSyscallParameters = NULL;
    usermodeProcessorState = &CompleteProcessorState->RegisterArea;

    __try
    {
        if (LogIsComponentTraced(LogComponentUserMode))
        {
            DumpProcessorState(CompleteProcessorState);
        }

        sysCallId = usermodeProcessorState->RegisterValues[RegisterR8];

        // The first parameter is the system call ID, we don't care about it => +1
        pSyscallParameters = (PQWORD)usermodeProcessorState->RegisterValues[RegisterRbp] + 1;

		LOG_TRACE_USERMODE("System call ID is %u\n", sysCallId);

		/*switch (sysCallId)
		{
			case SyscallIdIdentifyVersion:
			{
				SYSCALL_IF_VERSION versionId = (SYSCALL_IF_VERSION)pSyscallParameters[0];
				status = SyscallValidateInterface(versionId);
				break;
			}
			case SyscallIdFileWrite:
			{
				UM_HANDLE FileHandle = (UM_HANDLE)pSyscallParameters[0];
				PVOID Buffer = (PVOID)pSyscallParameters[1];
				QWORD BytesToWrite = (QWORD)pSyscallParameters[2];
				PQWORD BytesWritten = (PQWORD)pSyscallParameters[3];

				status = SyscallFileWrite(FileHandle, Buffer, BytesToWrite, BytesWritten);
				break;
			}
			case SyscallIdThreadGetTid:
			{
				UM_HANDLE ThreadHandle = (UM_HANDLE)pSyscallParameters[0];
				TID* Tid = (TID*)pSyscallParameters[1];

				status = SyscallThreadGetTid(ThreadHandle, Tid);

				break;
			}
			case SyscallIdProcessExit:
			{
				STATUS ExitStatus = (STATUS)pSyscallParameters[0];

				status = SyscallProcessExit(ExitStatus);
				break;
			}
			case SyscallIdThreadExit:
			{
				STATUS ExitStatus = (STATUS)pSyscallParameters[0];

				status = SyscallThreadExit(ExitStatus);
				break;
			}
			case SyscallIdProcessCreate:
			{
				char* ProcessPath = (char*)pSyscallParameters[0];
				QWORD PathLength = (QWORD)pSyscallParameters[1];
				char* Arguments = (char*)pSyscallParameters[2];
				QWORD ArgLength = (QWORD)pSyscallParameters[3];
				UM_HANDLE* ProcessHandle = (UM_HANDLE*)pSyscallParameters[4];

				status = SyscallProcessCreate(ProcessPath, PathLength, Arguments, ArgLength, ProcessHandle);
				break;
			}
			case SyscallIdProcessGetPid:
			{
				UM_HANDLE ProcessHandle = (UM_HANDLE)pSyscallParameters[0];
				PID* Pid = (PID*)pSyscallParameters[1];

				status = SyscallProcessGetPid(ProcessHandle, Pid);
				break;
			}
			case SyscallIdThreadGetName:
			{
				char* ThreadName = (char*)pSyscallParameters[0];
				QWORD ThreadNameMaxLength = (QWORD)pSyscallParameters[1];
				
				status = SyscallThreadGetName(ThreadName, ThreadNameMaxLength);
				break;
			}
			case SyscallIdGetTotalThreadNo:
			{
				QWORD* ThreadNo = (QWORD*)pSyscallParameters[0];

				status = SyscallGetTotalThreadNo(ThreadNo);
				break;
			}
			case SyscallIdGetThreadUmStackAddress:
			{
				PVOID* StackAddress = (PVOID*)pSyscallParameters[0];

				status = SyscallGetThreadUmStackAddress(StackAddress);
				break;
			}
			case SyscallIdGetThreadUmStackSize:
			{
				QWORD* StackSize = (QWORD*)pSyscallParameters[0];

				status = SyscallGetThreadUmStackSize(StackSize);
				break;
			}
			case SyscallIdGetThreadUmEntryPoint:
			{
				PVOID* EntryPoint = (PVOID*)pSyscallParameters[0];

				status = SyscallGetThreadUmEntryPoint(EntryPoint);
				break;
			}
			case SyscallIdVirtualAlloc:
			{
				PVOID BaseAddress = (PVOID)pSyscallParameters[0];
				QWORD Size = (QWORD)pSyscallParameters[1];
				VMM_ALLOC_TYPE AllocType = (VMM_ALLOC_TYPE)pSyscallParameters[2];
				PAGE_RIGHTS PageRights = (PAGE_RIGHTS)pSyscallParameters[3];
				UM_HANDLE FileHandle = (UM_HANDLE)pSyscallParameters[4];
				QWORD Key = (QWORD)pSyscallParameters[5];
				PVOID* AllocatedAddress = (PVOID*)pSyscallParameters[6];

				status = SyscallVirtualAlloc(BaseAddress, Size, AllocType, PageRights, FileHandle, Key, AllocatedAddress);
				break;
			}
			case SyscallIdVirtualFree:
			{
				PVOID Address = (PVOID)pSyscallParameters[0];
				QWORD Size = (QWORD)pSyscallParameters[1];
				VMM_FREE_TYPE FreeType = (VMM_FREE_TYPE)pSyscallParameters[2];

				status = SyscallVirtualFree(Address, Size, FreeType);
				break;
			}
			case SyscallIdGetPageFaultNo:
			{
				PVOID AllocatedVirtAddr = (PVOID)pSyscallParameters[0];
				QWORD* PageFaultNo = (QWORD*)pSyscallParameters[1];
				status = SyscallGetPageFaultNo(AllocatedVirtAddr, PageFaultNo);
				break;
			}
			case SyscallIdGetPagePhysAddr:
			{
				PVOID AllocatedVirtAddr = (PVOID)pSyscallParameters[0];
				PVOID* AllocatedPhysAddr = (PVOID*)pSyscallParameters[1];

				status = SyscallGetPagePhysAddr(AllocatedVirtAddr, AllocatedPhysAddr);
				break;
			}
			default:
			{
				status = STATUS_SUCCESS;
				break;
			}
		}
		*/
		switch (sysCallId) {
		case SyscallIdIdentifyVersion: {
			SYSCALL_IF_VERSION version = (SYSCALL_IF_VERSION)pSyscallParameters[0];
			if (&version != NULL)
				status = SyscallValidateInterface(version);
			else
				status = STATUS_INVALID_PARAMETER1;
			break;
		}
		case SyscallIdThreadExit: {
			STATUS ExitStatus = (STATUS)pSyscallParameters[0];
			status = SyscallThreadExit(ExitStatus);
			break;
		}
		case SyscallIdProcessExit: {
			STATUS ExitStatus = (STATUS)pSyscallParameters[0];
			status = SyscallProcessExit(ExitStatus);
			break;
		}
		case SyscallIdFileWrite: {
			UM_HANDLE  FileHandle = (UM_HANDLE)pSyscallParameters[0];
			PVOID      Buffer = (PVOID)pSyscallParameters[1];
			QWORD      BytesToWrite = (QWORD)pSyscallParameters[2];
			QWORD*      BytesWritten = (QWORD*)pSyscallParameters[3];

			status = SyscallFileWrite(FileHandle, Buffer, BytesToWrite, BytesWritten);
			break;
		}
		default: {
			status = STATUS_NOT_IMPLEMENTED;
			break;
		}
		}
    }
    __finally
    {
        LOG_TRACE_USERMODE("Will set UM RAX to 0x%x\n", status);

        usermodeProcessorState->RegisterValues[RegisterRax] = status;

        CpuIntrSetState(INTR_OFF);
    }
}


//B.1
STATUS
SyscallValidateInterface(
	IN  SYSCALL_IF_VERSION          InterfaceVersion
	) {
	if (&InterfaceVersion == NULL)
		return STATUS_INVALID_PARAMETER1;
	if (InterfaceVersion != SYSCALL_IMPLEMENTED_IF_VERSION)
		return STATUS_INCOMPATIBLE_INTERFACE;
	return STATUS_SUCCESS;
}
//B.1
STATUS
SyscallThreadExit(
	IN      STATUS                  ExitStatus
	) {
	if (&ExitStatus == NULL)
		return STATUS_INVALID_PARAMETER1;
	ThreadExit(ExitStatus);
	return STATUS_SUCCESS;
}
//B.1
STATUS
SyscallProcessExit(
	IN      STATUS                  ExitStatus
	) {
	if (&ExitStatus == NULL)
		return STATUS_INVALID_PARAMETER1;
	PPROCESS pProcess = GetCurrentProcess();
	if (pProcess == NULL)
		return STATUS_UNSUCCESSFUL;
	pProcess->TerminationStatus = ExitStatus;
	ProcessTerminate(pProcess);
	return STATUS_SUCCESS;
}
//B.2
STATUS
SyscallFileWrite(
	IN  UM_HANDLE                   FileHandle,
	IN_READS_BYTES(BytesToWrite)
	PVOID                       Buffer,
	IN  QWORD                       BytesToWrite,
	OUT QWORD*                      BytesWritten
	) {
	if (&FileHandle == NULL || (FileHandle != UM_FILE_HANDLE_STDOUT))
		return STATUS_INVALID_PARAMETER1;
	if (Buffer == NULL)
		return STATUS_INVALID_PARAMETER2;
	if (&BytesToWrite == NULL)
		return STATUS_INVALID_PARAMETER3;
	if (BytesWritten == NULL)
		return STATUS_INVALID_PARAMETER4;

	if (MmuIsBufferValid(Buffer, BytesToWrite, PAGE_RIGHTS_WRITE, GetCurrentProcess()) != STATUS_SUCCESS)
		return STATUS_INVALID_BUFFER;

	LOG("[%s]:[%s]\n", ProcessGetName(NULL), Buffer);

	*BytesWritten = BytesToWrite;

	//if (FileHandle != UM_FILE_HANDLE_STDOUT)
	//	return CL_STATUS_INVALID_PARAMETER1;

	return STATUS_SUCCESS;
}

/*
STATUS
SyscallValidateInterface(
	IN  SYSCALL_IF_VERSION          InterfaceVersion
)
{
	STATUS status = STATUS_SUCCESS;
	if (InterfaceVersion == SYSCALL_IMPLEMENTED_IF_VERSION)
	{
		status = STATUS_SUCCESS;
	}
	else
	{
		status = STATUS_INCOMPATIBLE_INTERFACE;
	}
	return status;
}

STATUS
SyscallThreadExit(
	IN      STATUS                  ExitStatus
)
{
	ThreadExit(ExitStatus);
	return STATUS_SUCCESS;
}

STATUS
SyscallThreadGetTid(
	IN_OPT  UM_HANDLE               ThreadHandle,
	OUT     TID*                    ThreadId
)
{
	UNREFERENCED_PARAMETER(ThreadHandle);

	*ThreadId = GetCurrentThread()->Id;

	return STATUS_SUCCESS;
}

STATUS
SyscallProcessCreate(
	IN_READS_Z(PathLength)
				char*               ProcessPath,
	IN          QWORD               PathLength,
	IN_READS_OPT_Z(ArgLength)
				char*               Arguments,
	IN          QWORD               ArgLength,
	OUT         UM_HANDLE*          ProcessHandle
)
{
	PPROCESS process;
	STATUS status = STATUS_SUCCESS;
	ArgLength;
	PathLength;
	char fullPath[MAX_PATH];
	if (ProcessPath == strrchr(ProcessPath, '\\')) {
		sprintf(fullPath, "C:\\Applications\\%s", ProcessPath);
	}
	else {
		strcpy(fullPath,ProcessPath);
	}
	
	status = ProcessCreate(fullPath, Arguments, &process);
	if (!SUCCEEDED(status)) {
		*ProcessHandle = UM_INVALID_HANDLE_VALUE;
		return status;
	}
	UM_PROCESS_HANDLE process_handle;
	process_handle.Process = process;
	process_handle.ProcessHandle = globalHandleCounter++;
	PPROCESS parentProcess = GetCurrentProcess();
	InsertHeadList(&parentProcess->ProcessHandleList, &process_handle.ProcessHandleElem);
	ProcessHandle = &process_handle.ProcessHandle;
	return status;
}

STATUS
SyscallProcessGetPid(
	IN_OPT  UM_HANDLE               ProcessHandle,
	OUT     PID*                    ProcessId
)
{
	/*AcquireSyscallMutex();
	if (ProcessHandle == UM_INVALID_HANDLE_VALUE) {
		*ProcessId = GetCurrentProcess()->Id;
	}
	else {
		PPROCESS process = GetCurrentProcess();
		PUM_PROCESS_HANDLE processHandle = GetProcessHandleFromProcess(process, ProcessHandle);
		*ProcessId = processHandle->Process->Id;
	}
	ReleaseSyscallMutex();*//*

	UNREFERENCED_PARAMETER(ProcessHandle);
	*ProcessId = GetCurrentProcess()->Id;

	return STATUS_SUCCESS;
}

STATUS 
SyscallThreadGetName(
	OUT char* ThreadName,
	IN QWORD ThreadNameMaxLen
)
{
	if (ThreadName != NULL) {
		strncpy(ThreadName, ThreadGetName(NULL), (DWORD) ThreadNameMaxLen);
	}
	else {
		return STATUS_INVALID_PARAMETER1;
	}
	if (strcmp(ThreadName, ThreadGetName(NULL)) == 0) {
		return STATUS_SUCCESS;
	}
	else {
		return STATUS_TRUNCATED_THREAD_NAME;
	}
}

STATUS
SyscallGetTotalThreadNo(
	OUT QWORD* ThreadNo
)
{
	if (ThreadNo == NULL) {
		return STATUS_UNSUCCESSFUL;
	}

	*ThreadNo = GetCurrentProcess()->ActiveThreads;

	return STATUS_SUCCESS;
}

STATUS
SyscallGetThreadUmStackAddress(
	OUT PVOID* StackBaseAddress
)
{
	if (StackBaseAddress == NULL) {
		return STATUS_UNSUCCESSFUL;
	}

	*StackBaseAddress = GetCurrentProcess()->stackAddress;

	return STATUS_SUCCESS;
}

STATUS
SyscallGetThreadUmStackSize(
	OUT QWORD* StackSize
)
{
	if (StackSize == NULL) {
		return STATUS_UNSUCCESSFUL;
	}

	*StackSize = GetCurrentProcess()->stackSize;

	return STATUS_SUCCESS;
}

STATUS
SyscallGetThreadUmEntryPoint(
	OUT PVOID* EntryPoint
)
{
	if (EntryPoint == NULL) {
		return STATUS_UNSUCCESSFUL;
	}

	*EntryPoint = GetCurrentProcess()->entryPoint;

	return STATUS_SUCCESS;
}

STATUS
SyscallProcessExit(
	IN      STATUS                  ExitStatus
)
{
	PPROCESS CurrentProcess = GetCurrentProcess();
	STATUS status = STATUS_SUCCESS;
	if (!CurrentProcess)
	{
		status = STATUS_UNSUCCESSFUL;
		return status;
	}
	CurrentProcess->TerminationStatus = ExitStatus;
	LOGPL("PROCESS %d WILL BE TERMINATED\n", ProcessGetId(CurrentProcess));
	ProcessTerminate(CurrentProcess);
	return ExitStatus;
}

STATUS
SyscallFileWrite(
	IN  UM_HANDLE                   FileHandle,
	IN_READS_BYTES(BytesToWrite)
	PVOID							Buffer,
	IN  QWORD                       BytesToWrite,
	OUT QWORD*                      BytesWritten
)
{

	STATUS status = STATUS_SUCCESS;


	if (FileHandle == UM_FILE_HANDLE_STDOUT)
	{
		LOG("[%s]:[%s]\n", ProcessGetName(NULL), Buffer);
		LOG("Buffer: %s", Buffer);
		*BytesWritten = BytesToWrite;

		status = STATUS_SUCCESS;
		return status;
	}

	return STATUS_UNSUCCESSFUL;
}
*/
void
SyscallPreinitSystem(
    void
    )
{

}

STATUS
SyscallInitSystem(
    void
    )
{
    return STATUS_SUCCESS;
}

STATUS
SyscallUninitSystem(
    void
    )
{
    return STATUS_SUCCESS;
}

void
SyscallCpuInit(
    void
    )
{
    IA32_STAR_MSR_DATA starMsr;
    WORD kmCsSelector;
    WORD umCsSelector;

    memzero(&starMsr, sizeof(IA32_STAR_MSR_DATA));

    kmCsSelector = GdtMuGetCS64Supervisor();
    ASSERT(kmCsSelector + 0x8 == GdtMuGetDS64Supervisor());

    umCsSelector = GdtMuGetCS32Usermode();
    /// DS64 is the same as DS32
    ASSERT(umCsSelector + 0x8 == GdtMuGetDS32Usermode());
    ASSERT(umCsSelector + 0x10 == GdtMuGetCS64Usermode());

    // Syscall RIP <- IA32_LSTAR
    __writemsr(IA32_LSTAR, (QWORD) SyscallEntry);

    LOG_TRACE_USERMODE("Successfully set LSTAR to 0x%X\n", (QWORD) SyscallEntry);

    // Syscall RFLAGS <- RFLAGS & ~(IA32_FMASK)
    __writemsr(IA32_FMASK, RFLAGS_INTERRUPT_FLAG_BIT);

    LOG_TRACE_USERMODE("Successfully set FMASK to 0x%X\n", RFLAGS_INTERRUPT_FLAG_BIT);

    // Syscall CS.Sel <- IA32_STAR[47:32] & 0xFFFC
    // Syscall DS.Sel <- (IA32_STAR[47:32] + 0x8) & 0xFFFC
    starMsr.SyscallCsDs = kmCsSelector;

    // Sysret CS.Sel <- (IA32_STAR[63:48] + 0x10) & 0xFFFC
    // Sysret DS.Sel <- (IA32_STAR[63:48] + 0x8) & 0xFFFC
    starMsr.SysretCsDs = umCsSelector;

    __writemsr(IA32_STAR, starMsr.Raw);

    LOG_TRACE_USERMODE("Successfully set STAR to 0x%X\n", starMsr.Raw);
}

STATUS
SyscallVirtualAlloc(
	IN_OPT      PVOID                   BaseAddress,
	IN          QWORD                   Size,
	IN          VMM_ALLOC_TYPE          AllocType,
	IN          PAGE_RIGHTS             PageRights,
	IN_OPT      UM_HANDLE               FileHandle,
	IN_OPT      QWORD                   Key,
	OUT         PVOID*                  AllocatedAddress
) {
	if (BaseAddress == NULL) {
		if ((PFILE_OBJECT)FileHandle == NULL) {
			if (Key == 0) {
				*AllocatedAddress = VmmAllocRegionEx(BaseAddress, Size, AllocType, PageRights, 
					FALSE, NULL, NULL, NULL, NULL);
				return STATUS_SUCCESS;
			}
			else {
				return STATUS_INVALID_PARAMETER6;
			}
		}
		else {
			return STATUS_INVALID_PARAMETER5;
		}
	}
	else {
		return STATUS_INVALID_PARAMETER1;
	}
}

STATUS
SyscallVirtualFree(
	IN          PVOID                   Address,
	_When_(VMM_FREE_TYPE_RELEASE == FreeType, _Reserved_)
	_When_(VMM_FREE_TYPE_RELEASE != FreeType, IN)
	QWORD                   Size,
	IN          VMM_FREE_TYPE           FreeType
) {
	if (Address == NULL) {
		VmmFreeRegionEx(Address, Size, FreeType,
			FALSE, GetCurrentProcess()->VaSpace, GetCurrentProcess()->PagingData);
		return STATUS_SUCCESS;
	}
	else {
		return STATUS_INVALID_PARAMETER1;
	}
}

STATUS 
SyscallGetPageFaultNo(
	IN PVOID AllocatedVirtAddr, 
	OUT QWORD* PageFaultNo
) {
	UNREFERENCED_PARAMETER(AllocatedVirtAddr);
	UNREFERENCED_PARAMETER(PageFaultNo);
	return STATUS_SUCCESS;
}

STATUS SyscallGetPagePhysAddr(
	IN PVOID AllocatedVirtAddr,
	OUT PVOID* AllocatedPhysAddr
) {

	*AllocatedPhysAddr = MmuGetPhysicalAddress(AllocatedVirtAddr);
	return STATUS_SUCCESS;
}

STATUS 
SyscallGetPageInternalFragmentation(
	IN PVOID AllocatedVirtAddr, 
	OUT QWORD* IntFragSize
) {
	UNREFERENCED_PARAMETER(AllocatedVirtAddr);
	UNREFERENCED_PARAMETER(IntFragSize);
	return STATUS_SUCCESS;
}

PUM_PROCESS_HANDLE
GetProcessHandleFromProcess
(
	PPROCESS process, 
	UM_HANDLE handle
) 
{
	ASSERT(process != NULL);

	LIST_ENTRY processHandlersList = process->ProcessHandleList;
	LIST_ITERATOR iterator;
	ListIteratorInit(&processHandlersList, &iterator);
	LOG("LOOKING FOR HANDLE\n");
	while (&iterator != NULL) {
		UM_PROCESS_HANDLE *process_handle = CONTAINING_RECORD(iterator.CurrentEntry, UM_PROCESS_HANDLE, ProcessHandleElem);
		if (process_handle->ProcessHandle == handle)
			return process_handle;
		ListIteratorNext(&iterator);
	}
	return NULL;
}

PUM_FILE_HANDLE
GetFileHandleFromProcess
(
	PPROCESS process,
	UM_HANDLE handle
)
{
	ASSERT(process != NULL);

	LIST_ENTRY fileHandlersList = process->FileHandleList;
	LIST_ITERATOR iterator;
	ListIteratorInit(&fileHandlersList, &iterator);
	while (&iterator != NULL) {
		UM_FILE_HANDLE *file_handle = CONTAINING_RECORD(iterator.CurrentEntry, UM_FILE_HANDLE, FileHandleElem);
		if (file_handle->FileHandle == handle)
			return file_handle;
		ListIteratorNext(&iterator);
	}
	return NULL;
}