#include "stdafx.h"

#include <cassert>
#include <process.h>
#include "qcthread.h"

#include "DebugNewHelpers.h"


#include <process.h>

QCMutex::QCMutex(
	bool bInitiallyOwn,					// [in] ? should caller obtain initial ownership of the Mutex
	LPCTSTR pstrName,					// [in] name for the Mutex (or NULL)
	LPSECURITY_ATTRIBUTES lpsaAttribute)// [in] security attributes (or NULL)
{
	m_hObject = ::CreateMutex(lpsaAttribute, (BOOL) bInitiallyOwn, pstrName);
//	if (m_hObject == NULL)
//		;//throw exception
}


QCMutex::~QCMutex()
{
	::CloseHandle(m_hObject);
}

bool QCMutex::Lock(DWORD dwTimeout)
{
	assert(m_hObject != NULL);

	DWORD waitResult = ::WaitForSingleObject( m_hObject, dwTimeout );
	switch ( waitResult ) 
	{
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
			return true;

		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		default:
			return false;
	}
}

bool QCMutex::Unlock( )
{
	assert(m_hObject != NULL);

	return (::ReleaseMutex(m_hObject) != FALSE);
}

