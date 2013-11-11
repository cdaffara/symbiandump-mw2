/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header file describes the class that handles the storage
*                of triggers in RAM Structures.
*
*/


#include "lbttriggeridgenerator.h"

_LIT(KLbtTriggerIdFile,"lbttriggeridgenerator.dat");

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerIdGenerator* CLbtTriggerIdGenerator::NewL()
    {
    CLbtTriggerIdGenerator* self = new( ELeave ) CLbtTriggerIdGenerator;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerIdGenerator::~CLbtTriggerIdGenerator()
    {
    iFileHandle.Close();
    iFileServer.Close();
    }




// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerIdGenerator::CLbtTriggerIdGenerator()
    {
    // Nothing to do now
    }

void CLbtTriggerIdGenerator::SetTriggerId(TInt64 aId)
	{
	HBufC8* trigId = NULL;
	TRAP_IGNORE(trigId=HBufC8::NewL(sizeof(TInt64)));
	TPtr8 ptr=trigId->Des();
	ptr.AppendNum(aId);
	TInt pos = 0;
	iFileHandle.Seek(ESeekStart,pos);
	iFileHandle.Write(ptr);
	delete trigId;
	}

TInt64 CLbtTriggerIdGenerator::GetTriggerId()
	{
	HBufC8* trigId = NULL;
	TRAP_IGNORE(trigId=HBufC8::NewL(sizeof(TInt64)));
	TPtr8 ptr=trigId->Des();	
	TInt pos = 0;
	iFileHandle.Seek(ESeekStart,pos);
	iFileHandle.Read(ptr);
	TLex8 lex(ptr);
	TInt64 iD;
	lex.Val(iD);
	delete trigId;
	return iD;	
	}

// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtTriggerIdGenerator::ConstructL()
    {
	User::LeaveIfError(iFileServer.Connect());
	 // Obtain the file path
    TFileName trigIdFile;
    iFileServer.SessionPath(trigIdFile);

    // Create the  Directory ie the private directory of the process
    iFileServer.MkDirAll(trigIdFile);
    
    // Generate the Db file
    trigIdFile.Append(KLbtTriggerIdFile);
	TInt createError=iFileHandle.Create(iFileServer,trigIdFile,EFileWrite);
	if(createError==KErrAlreadyExists)
	    {
	    createError=iFileHandle.Open(iFileServer,trigIdFile,EFileWrite);
	    }
	else if(createError==KErrNone)
	    {
	    SetTriggerId(0);
	    }
    User::LeaveIfError(createError);	    
    }

// end of file

