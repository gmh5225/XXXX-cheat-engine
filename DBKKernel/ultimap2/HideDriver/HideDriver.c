#include <fltKernel.h>
#include "HideDriver.h"


extern PDRIVER_OBJECT g_pDriverObject;

/////////////////////////////////////////////////////////////////////////////////
//// struct

typedef struct _KLDR_DATA_TABLE_ENTRY_COMMON
{
	LIST_ENTRY InLoadOrderLinks;
	PVOID ExceptionTable;
	ULONG ExceptionTableSize;
	// ULONG padding on IA64
	PVOID GpValue;
	/*PNON_PAGED_DEBUG_INFO*/ PVOID NonPagedDebugInfo;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT __Unused5;
	PVOID SectionPointer;
	ULONG CheckSum;
	// ULONG padding on IA64
	PVOID LoadedImports;
	PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY_COMMON, * PKLDR_DATA_TABLE_ENTRY_COMMON;

/////////////////////////////////////////////////////////////////////////////////
//// global
unsigned char g_bHideDriverInit = FALSE;
unsigned char g_bHideDriverHidden = FALSE;

wchar_t g_wcLastName = 0;

/////////////////////////////////////////////////////////////////////////////////
//// function
long
HideDriverInit()
{
	long ns = -1;
	KLDR_DATA_TABLE_ENTRY_COMMON* pEntry = (KLDR_DATA_TABLE_ENTRY_COMMON*)(g_pDriverObject->DriverSection);
	if (pEntry)
	{
		g_wcLastName = pEntry->FullDllName.Buffer[pEntry->FullDllName.Length / 2 - 5];

		// -> 'z'
		pEntry->FullDllName.Buffer[pEntry->FullDllName.Length / 2 - 5] = L'z';
		pEntry->BaseDllName.Buffer[pEntry->BaseDllName.Length / 2 - 5] = L'z';
		ns = 0;
		g_bHideDriverInit = TRUE;
	}

	return ns;
}

void
HideDriverUnInit()
{
	if (g_bHideDriverInit)
	{
		g_bHideDriverInit = FALSE;
		KLDR_DATA_TABLE_ENTRY_COMMON* pEntry = (KLDR_DATA_TABLE_ENTRY_COMMON*)(g_pDriverObject->DriverSection);
		if (pEntry)
		{
			// restore
			pEntry->FullDllName.Buffer[pEntry->FullDllName.Length / 2 - 5] = g_wcLastName;
			pEntry->BaseDllName.Buffer[pEntry->BaseDllName.Length / 2 - 5] = g_wcLastName;
		}
	}
}

