/*
 * PROJECT:         ReactOS Win32K
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            subsystems/win32/win32k/eng/engevent.c
 * PURPOSE:         Event Support Routines
 * PROGRAMMERS:     Stefan Ginsberg (stefan__100__@hotmail.com)
 */

/* INCLUDES ******************************************************************/

#include <win32k.h>
#define NDEBUG
#include <debug.h>

#define TAG_ENG TAG('E', 'n', 'g', ' ')

/* PUBLIC FUNCTIONS **********************************************************/

BOOL
APIENTRY
EngCreateEvent(OUT PEVENT* Event)
{
    PKEVENT LocalEvent;

    /* Allocate memory for the event */
    LocalEvent = EngAllocMem(FL_NONPAGED_MEMORY,
	                         sizeof(KEVENT),
	                         TAG_ENG);

    /* Check if we are out of memory  */
	if (!LocalEvent)
	{
	    /* We are, fail */
        return FALSE;
    }

    /* Initialize the event and return it */
    KeInitializeEvent(LocalEvent,
                      SynchronizationEvent,
                      FALSE);
    *Event = (PEVENT)LocalEvent;

	/* Return success */
    return TRUE;	
}

BOOL
APIENTRY
EngDeleteEvent(IN PEVENT Event)
{
    /* Just free the event */
    EngFreeMem(Event);
    return TRUE;
}

VOID
APIENTRY
EngClearEvent(IN PEVENT Event)
{
    /* Clear the event */
    KeClearEvent((PKEVENT)Event);
}

LONG
APIENTRY
EngSetEvent(IN PEVENT Event)
{
    /* Set the event */
    return KeSetEvent((PKEVENT)Event,
                      IO_NO_INCREMENT,
                      FALSE);
}

LONG
APIENTRY
EngReadStateEvent(IN PEVENT Event)
{
    /* Read the event state */
    return KeReadStateEvent((PKEVENT)Event);
}

PEVENT
APIENTRY
EngMapEvent(IN HDEV hDev,
            IN HANDLE hUserObject,
            IN PVOID Reserved1,
            IN PVOID Reserved2,
            IN PVOID Reserved3)
{
    PKEVENT Event;
    NTSTATUS Status;

    /* Assume failure */
    Event = NULL;

    /* Reference the object  */
    Status = ObReferenceObjectByHandle(hUserObject,
				                       EVENT_MODIFY_STATE,
				                       ExEventObjectType,
				                       KernelMode,
				                       (PVOID*)&Event,
				                       NULL);

    /* Only do this if we succeed */
    if (NT_SUCCESS(Status))
    {
        /* Pulse the event */
        KePulseEvent(Event,
                     EVENT_INCREMENT,
                     FALSE);
    }
    
    /* Return the event body */
    return (PEVENT)Event;
}

BOOL
APIENTRY
EngUnmapEvent(IN PEVENT Event)
{
    /* Just dereference the event */
    ObDereferenceObject(Event);
    return TRUE;
}

BOOL
APIENTRY
EngWaitForSingleObject(IN PEVENT Event,
                       IN PLARGE_INTEGER TimeOut)
{
    NTSTATUS Status;

    /* Validate parameters */
    if (!Event) return FALSE;

   /* Wait for the event */
   Status = KeWaitForSingleObject(Event,
                                  Executive,
                                  KernelMode,
                                  FALSE,
                                  TimeOut);

    /* Check if we timed out */
    if (Status != STATUS_SUCCESS)
    {
        /* We did, fail */
        return FALSE;
    }

    /* Return success */
    return TRUE;
}
