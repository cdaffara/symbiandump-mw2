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


#ifndef OUTPUT_PRINTER_H
#define OUTPUT_PRINTER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#include <StifTestInterface.h>
#include <StifLogger.h>
#include "TestModuleConstants.h"

class COutputPrinter : CBase
	{
	public:
		COutputPrinter(CTestModuleBase *aTestModule, CStifLogger *aLogger, const TDesC &aConsoleTrace, const TDesC &aTracePrefix, TInt aPrintPriority=0);

	private:
		void Echo(const TDesC &aMessage);
		void Echo(const TDesC &aFormat, const TDesC &aMessage);
		template<class T> void Echo (const TDesC &aFormat, T aMessage);
	public:
		void Printf(TRefByValue<const TDesC> aLogInfo, ...);

		CStifLogger *SetLogger(CStifLogger *aNewLogger);
		CTestModuleBase *SetTestModule(CTestModuleBase *aNewTestModule);

	private:
		TInt iPrintPriority;
		TBufC<KMaxMessageLength> iConsoleTrace;
		TBufC<KMaxMessageLength> iRDebugTracePrefix;
		CStifLogger *iOutLogger;	//COutputPrinter does not own iLogger
		CTestModuleBase *iTestModule;	//COutputPrinter does not own iTestModule
	};
#endif      // OUTPUT_PRINTER_H
