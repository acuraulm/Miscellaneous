// Threads

//##################################################################################################################################################################################################//
// 1. Thread time maintenance

//thread_internal.h - definesti field in structura de thread
typedef struct _THREAD
{
	QWORD					Timestamp;
	LIST_ENTRY				TimedList;
} THREAD, *PTHREAD;

// thread.c

#include "iomu.h"
static FUNC_CompareFunction     _ThreadTimeCompareFunction;

typedef struct _THREAD_SYSTEM_DATA
{
	LOCK					TimedThreadLock;
	_Guarded_by_(AllTimedLock)
	LIST_ENTRY				TimedThreadList; //head-ul listei ordonate
} THREAD_SYSTEM_DATA, *PTHREAD_SYSTEM_DATA;

// initializezi head-ul listei si lock-ul
function
ThreadSystemPreinit()
{
	InitializeListHead(&m_threadSystemData.TimedThreadList);
	LockInit(&m_threadSystemData.TimedThreadLock);
}

// initializezi field-ul din thread, timestamp-ul cu Iomugetsystemticks
// inserezi in head-ul listei (msystemdata) ordonat, cu functia de compare de mai jos
function
_ThreadInit()
{


	pThread->Timestamp = IomuGetSystemTicks(NULL);
	LOGPL("\n\n[ACU-Timing] - Thread [tid=0x%7x] is being created at [time=%X]\n\n", pThread->Id, pThread->Timestamp);

	INTR_STATE xoldIntrState;
	LockAcquire(&m_threadSystemData.TimedThreadsLock, &xoldIntrState);
	InsertOrderedList(&m_threadSystemData.TimedThreadsList, &pThread->TimedList, _ThreadTimeCompareFunction, NULL);
	LockRelease(&m_threadSystemData.TimedThreadsLock, xoldIntrState);
}

// stergi din lista (head, msystemdata) entry-ul(thread-ul)
function
ThreadExit()
{

	INTR_STATE xoldState;

	LockAcquire(&m_threadSystemData.TimedThreadsLock, &xoldState);
	RemoveEntryList(&pThread->TimedList);
	LockRelease(&m_threadSystemData.TimedThreadsLock, xoldState);
}


// lista de comparare
static
INT64
(__cdecl _ThreadTimeCompareFunction) (
	IN      PLIST_ENTRY     FirstElem,
	IN      PLIST_ENTRY     SecondElem,
	IN_OPT  PVOID           Context
	)
{
	ASSERT(NULL != FirstElem);
	ASSERT(NULL != SecondElem);
	ASSERT(Context == NULL);

	PTHREAD pFirstThread = CONTAINING_RECORD(FirstElem, THREAD, TimedList);
	PTHREAD pSecondThread = CONTAINING_RECORD(SecondElem, THREAD, TimedList);

	return (pFirstThread->Timestamp - pSecondThread->Timestamp);
}
	
//##################################################################################################################################################################################################//
// 2. Thread children maintenance

//thread_internal.h
typedef struct _THREAD
{
	QWORD					NrOfChildren;
	struct _THREAD*			Parent;

	LOCK					ChildrenLock;		// lock-ul listei de copii
	LIST_ENTRY				ChildrenListHead;	// head-ul listei de copii
	
	LIST_ENTRY				ParentEntry;
} THREAD, *PTHREAD;

//thread.c
static FUNC_ListFunction		printChildren;

function
_ThreadInit()
{
	InitializeListHead(&pThread->ChildrenListHead);		// init head lista (pthread)
	LockInit(&pThread->ChildrenLock);					// init lock lista (pthread)
	pThread->NrOfChildren = 0;							// init nr copii

	INTR_STATE dummystate;

	PTHREAD parent = GetCurrentThread();
		if (parent != NULL && parent->Id > 0x20) {		// verifici sa nu fie null(in caz main/ sau idle)
			
			LockAcquire(&parent->ChildrenLock, &dummystate);		// lock pe lista
					LOGPL("\n\n[Thread [tid=0x%7x] has %d / %d siblings\n", pThread->Id, ListSize(&parent->ChildrenListHead), parent->NrOfChildren);
					ForEachElementExecute(&parent->ChildrenListHead, printChildren, NULL, FALSE);		// functie care printeaza id-ul fiecarui element din lista de copii
					LOGPL("\n\n");
				InsertTailList(&parent->ChildrenListHead, &pThread->ParentEntry); 	// insert thread-ul creat, in lista de copii ai parintelui
				parent->NrOfChildren++;												// increment nr copii ai parinteului
				pThread->Parent = parent;											// setezi parintele ca si currentthread(de mai sus)

			LockRelease(&parent->ChildrenLock, dummystate);			// release lock
		}
}

function
ThreadExit()
{
	if (pThread->Parent != NULL) {			//daca are parinte
		pThread->Parent->NrOfChildren--;	//decrement nr copii ai parintelui
		INTR_STATE dummystate;

		LockAcquire(&pThread->Parent->ChildrenLock, &dummystate);	// lock lista
		RemoveEntryList(&pThread->ParentEntry);					//stergi referinta spre parinte a copilului(se sterge din lista de copii ai parintelui)
		LockRelease(&pThread->Parent->ChildrenLock, dummystate);	// release lock
	}
}

// printeaza id-ul thread-ului
static
STATUS
(__cdecl printChildren) (
	IN      PLIST_ENTRY     ListEntry,
	IN_OPT  PVOID           FunctionContext
	)
{
	PTHREAD pThread;

	ASSERT(NULL != ListEntry);
	ASSERT(NULL == FunctionContext);

	pThread = CONTAINING_RECORD(ListEntry, THREAD, ParentEntry);
	if (pThread!= NULL)
		LOGPL("\n[Child] ~Tid:%x,", pThread->Id);

	return STATUS_SUCCESS;
}


//##################################################################################################################################################################################################//
// 3. Add new command for testing children

//cmd_interpreter.c

static const COMMAND_DEFINITION COMMANDS[] =
{
	{ "testchildren", "Tests children command", CmdTestChildren, 0, 0 }
}

//cmd_thread_helper.h
FUNC_GenericCommand CmdTestChildren;

//cmd_thread_helper.c
void
(__cdecl CmdTestChildren)(
	IN          QWORD       NumberOfParameters
	)
{

	ASSERT(NumberOfParameters == 0);
	printf("Testing children command\n");
	
	TestChildrenCommand(); // apel catre functia de testare din thread.c (poate sa fie si aici)
}

//thread_internal.h
void TestChildrenCommand(); // sa aiba acces in cmd_thread_helper.c

//thread.c

static FUNC_ThreadStart     ThreadOneFunction; //thread1 start func
static FUNC_ThreadStart     ThreadTwoFunction; //thread2 start func
static FUNC_ThreadStart     DoNothingFunction; //thread does nothing
static FUNC_ThreadStart     CreateOneThreadFunction; //creates a thread

void TestChildrenCommand() {

	STATUS threadExitStatus;
	PTHREAD Thread1, Thread2;

	ThreadCreate("th1", ThreadPriorityDefault, ThreadOneFunction, NULL, &Thread1);
	ThreadCreate("th2", ThreadPriorityDefault, ThreadTwoFunction, NULL, &Thread2);

	ThreadWaitForTermination(Thread1, &threadExitStatus);
	ThreadWaitForTermination(Thread2, &threadExitStatus);

}

STATUS
(__cdecl ThreadOneFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);

	STATUS threadExitStatus;
	PTHREAD pThread[3];
	ThreadCreate("Th1 one", ThreadPriorityDefault, CreateOneThreadFunction, NULL, &pThread[0]);
	ThreadCreate("Th1 two", ThreadPriorityDefault, CreateOneThreadFunction, NULL, &pThread[1]);
	ThreadCreate("Th1 three", ThreadPriorityDefault, CreateOneThreadFunction, NULL, &pThread[2]);

	ThreadWaitForTermination(pThread[0], &threadExitStatus);
	ThreadWaitForTermination(pThread[1], &threadExitStatus);
	ThreadWaitForTermination(pThread[2], &threadExitStatus);

	return STATUS_SUCCESS;
}

STATUS
(__cdecl CreateOneThreadFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);
	STATUS threadExitStatus;

	PTHREAD pThread;
	ThreadCreate("createone", ThreadPriorityDefault, DoNothingFunction, NULL, &pThread);

	ThreadWaitForTermination(pThread, &threadExitStatus);
	return STATUS_SUCCESS;
}

STATUS
(__cdecl DoNothingFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);
	LOGPL("\n\n[ACU - TestChildren] - This thread does nothing\n");
	return STATUS_SUCCESS;

}

STATUS
(__cdecl ThreadTwoFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);
	PTHREAD pThread = GetCurrentThread();
		LOGPL("\n\n[ACU - TestChildren] - %x", pThread->Id);
	return STATUS_SUCCESS;

}


//##################################################################################################################################################################################################//
// 4. Thread Id allocation - TID = Prev_TID + 1 if even
//						     TID = Prev_TID * 2 if odd

//thread.c
function
_ThreadSystemGetNextTid()
{
	static volatile TID __currentTid = 0;
	if (__currentTid % 2 == 0)
		return _InterlockedExchangeAdd64(&__currentTid, 1);
	else
		return _InterlockedExchangeAdd64(&__currentTid, __currentTid);
}

//##################################################################################################################################################################################################//
// 5. Thread Id allocation - TID = Prev_TID - 1 - X(random)

//thread.c
function
_ThreadSystemGetNextTid()
{
	static volatile TID __currentTid = 0xFFFFFFFFFFFFFFFF;
	QWORD expression = -1 - (IomuGetSystemTicks(NULL) % 10);

	return _InterlockedExchangeAdd64(&__currentTid, expression);
}

//##################################################################################################################################################################################################//
// 6. Maintain parent of thread:  main thread - null parent
//								  Display ancestors at creation
//								  thread terminates, set children`s parent to parent.


//thread_internal.h
typedef struct _THREAD
{
	struct _THREAD*			ParentAncestor;
} THREAD, *PTHREAD;

//thread.c
static FUNC_ListFunction        setChildrenParentToThreadParent;

function
ThreadSystemInitMainForCurrentCPU()
{
	pThread->ParentAncestor = NULL; // inainte de SetCurrentThread(pThread)
}

function
ThreadExit()
{
	LockAcquire(&m_threadSystemData.AllThreadsLock, &oldState);
	if (pThread->Parent != NULL) {
		ForEachElementExecute(&pThread->AllList, setChildrenParentToThreadParent, NULL, FALSE); // when thread terminates, sets all it`s children parents to its parent
	}
	RemoveEntryList(&pThread->AllList);
	LockRelease(&m_threadSystemData.AllThreadsLock, oldState);
}

function
_ThreadInit()
{
	PTHREAD parentAncestor = GetCurrentThread();
		if (parentAncestor != NULL) {
			pThread->ParentAncestor = parentAncestor;
			LOGPL("\n\n[ACU-Ancestors] Thread [tid = 0x%X] is being created. Its ancestors are:", pThread->Id);
			PTHREAD aux = pThread;
			int i = 1;
			while (1) {			//while thread has parent, print it until first ancestor
				PTHREAD ancestor = aux->ParentAncestor;
				if (ancestor != NULL) {
					LOGPL("\n\n[Ancestor%d]0x%X, ",i, ancestor->Id);
					if (ancestor->Id == 0)
						break;
					aux = ancestor;
					i++;
				}
				else
					break;
			}
		}
}

static
STATUS
(__cdecl setChildrenParentToThreadParent) (
	IN      PLIST_ENTRY     ListEntry,
	IN_OPT  PVOID           FunctionContext
	)
{
	PTHREAD pThread;

	ASSERT(NULL != ListEntry);
	ASSERT(NULL == FunctionContext);
	pThread = CONTAINING_RECORD(ListEntry, THREAD, AllList);
	PTHREAD currentThread = GetCurrentThread();
	if(pThread->ParentAncestor == currentThread)
		pThread->ParentAncestor = currentThread->ParentAncestor;
		LOGPL("\n\n[ACU-Ancestors] Current thread:[0x%x] Changed childs [0x%x]`s parent to [0x%x]\n",currentThread->Id, pThread->Id, currentThread->ParentAncestor->Id);
	return STATUS_SUCCESS;
}

//##################################################################################################################################################################################################//
// 7. Add a new command /getancestors that prints each thread and its list of ancestors


//cmd_interpreter.c

static const COMMAND_DEFINITION COMMANDS[] =
{
	{ "getancestors", "Prints a list of ancestors for each thread", CmdGetAncestors, 0, 0 }
}

//cmd_thread_helper.h
FUNC_GenericCommand CmdGetAncestors;

//cmd_thread_helper.c
void
(__cdecl CmdGetAncestors)(
	IN          QWORD       NumberOfParameters
	)
{

	ASSERT(NumberOfParameters == 0);
	printf("Testing children command\n");
	GetAncestors();
}
//thread_internal.h
void GetAncestors(); // sa aiba acces in cmd_thread_helper.c

//thread.c
static FUNC_ListFunction		printAncestors;

void GetAncestors() {
	INTR_STATE dummyState;
	LockAcquire(&m_threadSystemData.AllThreadsLock, &dummyState);
	ForEachElementExecute(&m_threadSystemData.AllThreadsList, printAncestors, NULL, FALSE);
	LockRelease(&m_threadSystemData.AllThreadsLock, dummyState);
}

static
STATUS
(__cdecl printAncestors) (
	IN      PLIST_ENTRY     ListEntry,
	IN_OPT  PVOID           FunctionContext
	)
{
	PTHREAD pThread;

	ASSERT(NULL != ListEntry);
	ASSERT(NULL == FunctionContext);

	pThread = CONTAINING_RECORD(ListEntry, THREAD, AllList);

	if (pThread != NULL) {
		LOGPL("\n\n[ACU-Ancestors] Thread [tid = 0x%X] printing its ancestors:", pThread->Id);
		PTHREAD ancestor = pThread->ParentAncestor;
		int i = 1;
		while (1) { // cat timp are parinte(/nu e main), si parintele are parinte(nu e null), printeaza id-ul
			if (ancestor != NULL && ancestor->Id < 0xAFAFAF) {
				LOGPL("\n\n[Ancestor%d]0x%X, ", i, ancestor->Id);
				if (ancestor->ParentAncestor == NULL)
					break;
				if (ancestor->ParentAncestor->ParentAncestor == NULL)
					break;
				ancestor = ancestor->ParentAncestor;
				i++;
			}
			else
				break;
		}
	}
	return STATUS_SUCCESS;
}

//##################################################################################################################################################################################################//
// 8. Mentain the number of predecessors for each thread. main - 0
//														  when thread dies, display number of predecessors, do not update.

//thread_internal.h
typedef struct _THREAD
{
	QWORD					NrOfPredecessors;
	struct _THREAD*			Predecessor;
} THREAD, *PTHREAD;


//thread.c

function
ThreadSystemInitMainForCurrentCPU(){
	pThread->NrOfPredecessors = 0;
	pThread->Predecessor = NULL;
}

function
ThreadExit()
{
	LOGPL("\n\n[ACU-Predecessors] - Thread [tid=0x%x] is being terminated and it had [%x] predecessors", pThread->Id, pThread->NrOfPredecessors);
}

function
_ThreadInit()
{
	PTHREAD predecessor = GetCurrentThread();
		if (predecessor != NULL) {
			pThread->Predecessor = predecessor;
			pThread->NrOfPredecessors = predecessor->NrOfPredecessors + 1;
		}
		else{ //for main e.g.
			pThread->NrOfPredecessors = 0;
			pThread->Predecessor = NULL; 
		}
}



//##################################################################################################################################################################################################//
// 9. Add a new command /testpredecessors 	- create 2 threads:  one - print stack size and thread priority
//																 two - create two aditional threads that create another two threads.

//cmd_interpreter.c
static const COMMAND_DEFINITION COMMANDS[] =
{
	{ "testpredecessors", "Tests predecessor command", CmdTestPredecessors, 0, 0}
};

//cmd_thread_helper.h
FUNC_GenericCommand CmdTestPredecessors;

//cmd_thread_helper.c
void
(__cdecl CmdTestPredecessors)(
	IN          QWORD       NumberOfParameters
	)
{
	ASSERT(NumberOfParameters == 0);
	printf("Testing predecessors command\n");
	TestPredecessors();
}

//thread_internal.h
void TestPredecessors();


//thread.c
static FUNC_ThreadStart			PredecessorOneFunction;
static FUNC_ThreadStart			PredecessorTwoFunction;
static FUNC_ThreadStart			CreateTwoThreadsFunction;
static FUNC_ThreadStart		    DoNothingFunction;


void TestPredecessors() {

	STATUS threadExitStatus;
	PTHREAD Thread1, Thread2;

	ThreadCreate("th1", ThreadPriorityDefault, PredecessorOneFunction, NULL, &Thread1);
	ThreadCreate("th2", ThreadPriorityDefault, PredecessorTwoFunction, NULL, &Thread2);

	ThreadWaitForTermination(Thread1, &threadExitStatus);
	ThreadWaitForTermination(Thread2, &threadExitStatus);

}

STATUS
(__cdecl PredecessorOneFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);
	PTHREAD pThread = GetCurrentThread();
	if (pThread != NULL) {
		LOGPL("\n\n[ACU-Predecessors] First Thread: stack size[%x], thread priority[%x]\n", pThread->StackSize, pThread->Priority);
	}

	return STATUS_SUCCESS;
}

STATUS
(__cdecl PredecessorTwoFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);

	STATUS threadExitStatus;
	PTHREAD Thread1, Thread2;
	ThreadCreate("Th1 one", ThreadPriorityDefault, CreateTwoThreadsFunction, NULL, &Thread1);
	ThreadCreate("Th1 two", ThreadPriorityDefault, CreateTwoThreadsFunction, NULL, &Thread2);

	ThreadWaitForTermination(Thread1, &threadExitStatus);
	ThreadWaitForTermination(Thread2, &threadExitStatus);

	return STATUS_SUCCESS;
}

STATUS
(__cdecl CreateTwoThreadsFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);

	STATUS threadExitStatus;
	PTHREAD Thread1, Thread2;
	ThreadCreate("TwoThreads one", ThreadPriorityDefault, DoNothingFunction, NULL, &Thread1);
	ThreadCreate("TwoThreads two", ThreadPriorityDefault, DoNothingFunction, NULL, &Thread2);

	ThreadWaitForTermination(Thread1, &threadExitStatus);
	ThreadWaitForTermination(Thread2, &threadExitStatus);

	return STATUS_SUCCESS;
}

STATUS
(__cdecl DoNothingFunction)(
	IN_OPT PVOID Context
	) {

	UNREFERENCED_PARAMETER(Context);
	LOGPL("\n\n[ACU - TestPredecessors] - This thread does nothing\n");
	return STATUS_SUCCESS;

}