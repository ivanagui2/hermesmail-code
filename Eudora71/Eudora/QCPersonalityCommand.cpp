// QCPersonalityCommand.cpp: implementation of the QCPersonalityCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "STDAFX.H"
#include "QCCommandActions.h"
#include "QCPersonalityCommand.h"
#include "QCPersonalityDirector.h"

#include "resource.h"	// fake command id prompt
#include "summary.h"


//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(QCPersonalityCommand, QCCommandObject)

QCPersonalityCommand::QCPersonalityCommand(
QCPersonalityDirector*	pDirector,
LPCSTR					szPersonality ) : QCCommandObject( pDirector )
{
	m_szPersonality = szPersonality;
}

QCPersonalityCommand::~QCPersonalityCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//
void	QCPersonalityCommand::Execute(
COMMAND_ACTION_TYPE	theAction,
void*				pData )
{

	NotifyDirector( theAction, pData );	

	switch( theAction ) 
	{
		case CA_CHANGE_PERSONA:
		{
			
			ASSERT( pData );

			CSummary* pSummary = ( CSummary* ) pData;
			ASSERT_KINDOF(CSummary, pSummary);
			if ( ! pSummary->IsComp() )
				pSummary->SetPersona( m_szPersonality );
		}
		break;
	}
}


UINT QCPersonalityCommand::GetFlyByID(
COMMAND_ACTION_TYPE theAction )
{
	switch( theAction )
	{
		case CA_CHANGE_PERSONA:
			return ID_CHANGE_PERSONA;
		break;
	}

	return 0;
}


CString	QCPersonalityCommand::GetToolTip(
COMMAND_ACTION_TYPE theAction)
{
	CString szToolTip( "" );

	switch( theAction )
	{
		case CA_CHANGE_PERSONA:
			szToolTip.LoadString( IDS_USERDEF_TOOLTIP_CHANGE_PERSONA );
			szToolTip += " ";
			szToolTip += m_szPersonality;
		break;
	}

	return szToolTip;
}
