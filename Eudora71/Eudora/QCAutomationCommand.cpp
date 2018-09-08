// QCAutomationCommand.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eudora.h"
#include "QCAutomationCommand.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"


IMPLEMENT_DYNAMIC(QCAutomationCommand, QCCommandObject)

QCAutomationCommand::QCAutomationCommand(
	QCAutomationDirector* pDirector) : QCCommandObject((QCCommandDirector*)pDirector)
{

}

QCAutomationCommand::~QCAutomationCommand()
{

}

void QCAutomationCommand::Execute(COMMAND_ACTION_TYPE theAction, void* pData)
{

}



