/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Active Scheduler Test Module
*
*/

// INCLUDE FILES
#include "OutputPrinter.h"
#include <StifTestModule.h>
#include "TestModuleConstants.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES
// None

// CONSTANTS
// None

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================
// None


/*-------------------------------------------------------------------------------

    DESCRIPTION

    This module contains the implementation of OutputPrinter class
	member functions.

-------------------------------------------------------------------------------*/

// ================= MEMBER FUNCTIONS =========================================

COutputPrinter::COutputPrinter(CTestModuleBase *aTestModule, CStifLogger *aLogger, const TDesC &aConsoleTraceDescription, const TDesC &aTracePrefix, TInt aPrintPriority)
: iPrintPriority(aPrintPriority), iConsoleTrace(aConsoleTraceDescription), iRDebugTracePrefix(aTracePrefix), iOutLogger(aLogger), iTestModule(aTestModule)
{}

/*	Sets the CStifLogger object to output data to
	Returns the previos object used.
	Note that COutputPrinter does not own the CStifLogger object and thus does not delete it
*/
CStifLogger *COutputPrinter::SetLogger(CStifLogger *aNewLogger)
	{
	CStifLogger *oldLogger = iOutLogger;
	iOutLogger = aNewLogger;
	return oldLogger;
	}

CTestModuleBase *COutputPrinter::SetTestModule(CTestModuleBase *aNewTestModule)
	{
	CTestModuleBase *oldTestModule = iTestModule;
	iTestModule = aNewTestModule;
	return oldTestModule;
	}

void COutputPrinter::Echo(const TDesC &aMessage)
	{
	//TestModuleIf().Printf(iPrintPriority, iConsoleTrace, aMessage);
	if (iTestModule)
		{
		iTestModule->TestModuleIf().Printf(iPrintPriority, iConsoleTrace, aMessage);
		}
	if (iOutLogger)
		{
		iOutLogger->Log(aMessage);
		}
	#ifdef _DEBUG
	TBuf<KMaxMessageLength> debugMessage;
	debugMessage = iRDebugTracePrefix;
	//debugMessage+= KDefaultTextSeparator;
	debugMessage+= aMessage;
	RDebug::Print(debugMessage);
	#endif
	}

void COutputPrinter::Echo(const TDesC &aFormat, const TDesC &aMessage)
	{
	TBuf<KMaxMessageLength> message;
	message.Format(aFormat, &aMessage);
	Echo(message);
	}

template<class T> void COutputPrinter::Echo(const TDesC &aFormat, T aMessage)
	{
	TBuf<KMaxMessageLength> message;
	message.Format(aFormat, aMessage);
	Echo(message);
	}

void COutputPrinter::Printf(TRefByValue<const TDesC> aLogInfo, ...)
	{
    VA_LIST list;
    VA_START( list, aLogInfo );
	TBuf<KMaxMessageLength> logInfo;
    //// Create overflow handler. If the log information size is over the
    //// KMaxLogData rest of the information will cut.
    //TDesOverflowHandler overFlowHandler( this, 1 );

    // Parse parameters
    logInfo.AppendFormatList( aLogInfo, list);
	Echo(logInfo);
	}
