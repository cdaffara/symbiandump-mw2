// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "cscriptfileprocessor.h"

CScriptFileProcessor* CScriptFileProcessor::NewLC(const TDesC& aScriptFile)
	{
	CScriptFileProcessor* self=new (ELeave)CScriptFileProcessor;
	CleanupStack::PushL(self);
	self->ConstructL(aScriptFile);
	return self;
	}


CScriptFileProcessor* CScriptFileProcessor::NewL(const TDesC& aScriptFile)
	{
	CScriptFileProcessor* self=CScriptFileProcessor::NewLC(aScriptFile);
	CleanupStack::Pop(self);
	return self;
	}
	
void CScriptFileProcessor::ConstructL(const TDesC& aScriptFile)
	{
	User::LeaveIfError(iFSession.Connect());
    User::LeaveIfError(iFile.Open(iFSession, aScriptFile, EFileRead));
    iFileReadStream.Attach(iFile);   
	}

CScriptFileProcessor::CScriptFileProcessor()
	{//nothing to do here
	}
		
CScriptFileProcessor::~CScriptFileProcessor()
	{
	iFile.Close();
	iFileReadStream.Close();
	iFSession.Close();
	}
	
CScriptFileProcessor::TDataDirection CScriptFileProcessor::DataDirection() const
	{
	return iDataDirection; 	
	}

 TInt CScriptFileProcessor::ProcessLine(TDes8& aLine)
	{
	//get the direction of the data
	const TInt KSpecifierLength=3;	
	_LIT8(KFromServer,"<< ");
	_LIT8(KFromClient,">> ");
	
	//if the data is from the server then the line will contain "<<" else ">>"
	TInt pos;
	pos=aLine.Find(KFromServer);
	
	if(pos==KErrNotFound)
		{
		pos=aLine.Find(KFromClient);
		if(pos==KErrNotFound)
			{
			//there has been an error, the specifier was not found
			return KErrNotFound;
			}
		else
			{
			iDataDirection=EFromClient;
			}
		}
	else
		{
		iDataDirection=EFromServer;			
		}
			
	//extract from the specifier to the end of the line	
	aLine=aLine.Mid(KSpecifierLength+pos);
	//trim off trailing \r\n
	aLine.TrimRight();
	return KErrNone;
	}
	
	
TInt CScriptFileProcessor::ReadLine(TDes8& aLine)
 	{
	aLine.Zero();
	//new line
	const TChar KDelimiter=0x0A;
	//read the line
 	TRAPD(err, iFileReadStream.ReadL(aLine, KDelimiter));
 	if (err == KErrNone)
    	{
     	err=ProcessLine(aLine);
     	} 
 	return err; 
	}	

