/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of cell supervisor class
*
*/


#include <flogger.h>
#include "lbtloggerimpl.h"

// STATIC
CLbtLoggerImpl* CLbtLoggerImpl::iSelf = NULL;


// ======== MEMBER FUNCTIONS ========

EXPORT_C CLbtLoggerImpl* CLbtLoggerImpl::CreateLogger()
	{
	if(!iSelf)
	    {
	    CLbtLoggerImpl* impl = NULL;
    	TRAPD( error, impl = new (ELeave) CLbtLoggerImpl() );
    	if( error != KErrNone )
    		{
    		iSelf = NULL;
    		return NULL;
    		}
    	iSelf = impl;
	    }
	return iSelf;
	}

EXPORT_C void CLbtLoggerImpl::Destroy()
	{
	delete iSelf;
    iSelf = NULL;
	}

EXPORT_C RFileLogger& CLbtLoggerImpl::GetFileLogger()
	{
	return iLogger;
	}

CLbtLoggerImpl::CLbtLoggerImpl()
	{
	TInt error;
	error = iLogger.Connect();
	if(error == KErrNone)
	    {
	    _LIT16(filename,"lbt");	
	    TBuf16<40> buffer( filename );
	    TTime currentTime;
	    currentTime.HomeTime();
	    TDateTime currentDateTime = currentTime.DateTime();
	    buffer.AppendNum( currentDateTime.Day()+ 1 );
	    buffer.Append('_');
	    buffer.AppendNum( currentDateTime.Month()+1);
	    buffer.Append('_');
	    buffer.AppendNum(currentDateTime.Hour());
	    buffer.Append('_');
		buffer.AppendNum(currentDateTime.Minute());
		buffer.Append('_');
		buffer.AppendNum(currentDateTime.Second());
		buffer.Append(_L(".log"));

	    iLogger.CreateLog(_L("epos"), buffer, EFileLoggingModeAppend);
	    iLogger.SetDateAndTime(ETrue, ETrue);
	    }	
	}

CLbtLoggerImpl::~CLbtLoggerImpl()
	{
	iLogger.CloseLog();
	iLogger.Close();
	}


// end of file


