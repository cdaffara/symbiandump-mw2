/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Base class for SUPL subsessions
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <s32mem.h>
#include <centralrepository.h>
#include <epos_suplterminal.h>

#include "epos_suplterminalipc.h"
#include "epos_suplterminalerrors.h"
#include "epos_csuplprotocolmanagerbase.h"

#include "epos_csuplsessionmanager.h"
#include "epos_csuplglobal.h"
#include "epos_csuplserverpanic.h"
#include "epos_csuplipcsubsession.h"
#include "epos_csuplnetinitiatedsession.h"
#include "epos_mnetinitiatedobserver.h"


// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplnetinitiatedsession.cpp");
//#endif

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplNetInitiatedSession::CSuplNetInitiatedSession()
    {
    }

// EPOC default constructor can leave.
void CSuplNetInitiatedSession::ConstructL(CSuplSessionManager& aSessnMgr, CSUPLProtocolManagerBase::TSuplReqType aReqType,CSuplServer* aServer,HBufC8* aClientBuf)
    {

    DEBUG_TRACE("CSuplNetInitiatedSession::ConstructL", __LINE__)
    RSuplTerminalSubSession::TSuplServiceType suplService;
    TInt majorVersion = KErrNotFound;
    TInt err = aSessnMgr.GetSUPLMessageVersionL(majorVersion,aClientBuf->Des());
    
    if(majorVersion == 2)
	{
		if(!CheckSuplTriggerServiceStatus())
    	{
    			User::Leave(KErrNotSupported);
    	}
		suplService = RSuplTerminalSubSession::ESUPL_2_0;
	}
    else 
    	suplService = RSuplTerminalSubSession::ESUPL_1_0;
    
    iSuplSession = aSessnMgr.CreateNewSessionL(aReqType, 0, suplService );
    if (iSuplSession)
    {	
    iSuplSession->SetSUPLVersion(majorVersion);
    iSuplSessnReq = CSuplSessionRequest::NewL(aSessnMgr, iSuplSession, aServer);
    }  
    else
    	if (!iSuplSession && suplService == RSuplTerminalSubSession::ESUPL_2_0)
    		{
    			 suplService = RSuplTerminalSubSession::ESUPL_1_0;
    			 iSuplSession = aSessnMgr.CreateNewSessionL(aReqType, 0, suplService );
    					if (iSuplSession)
   						 {	
   						 iSuplSession->SetSUPLVersion(majorVersion);
    					 iSuplSessnReq = CSuplSessionRequest::NewL(aSessnMgr, iSuplSession, aServer);
    					 }
    		}

    }

// Two-phased constructor.
CSuplNetInitiatedSession* CSuplNetInitiatedSession::NewLC(CSuplSessionManager& aSessnMgr, CSUPLProtocolManagerBase::TSuplReqType aReqType,CSuplServer* aServer,HBufC8* aClientBuf)
    {
    DEBUG_TRACE("CSuplNetInitiatedSession::NewLC", __LINE__)
    CSuplNetInitiatedSession* self = new (ELeave) CSuplNetInitiatedSession();
    CleanupClosePushL(*self);
    self->ConstructL(aSessnMgr, aReqType,aServer,aClientBuf);
    return self;
    }

// Destructor
CSuplNetInitiatedSession::~CSuplNetInitiatedSession()
    {    
    DEBUG_TRACE("CSuplNetInitiatedSession::~CSuplIPCSubSession", __LINE__)
    delete iSuplSessnReq;
    delete iClientBuf;
    iClientBuf = NULL;
    }






// ---------------------------------------------------------
// CSuplNetInitiatedSession::RequestComplete
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplNetInitiatedSession::RequestComplete(
    const RMessage2& aMessage, 
    TInt aCompleteCode)
    {
     DEBUG_TRACE("CSuplNetInitiatedSession::RequestComplete", __LINE__)
    if (!aMessage.IsNull())
        {
        aMessage.Complete(aCompleteCode);
        }
    }
 // ---------------------------------------------------------
// CSuplNetInitiatedSession::GetHandle
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*TInt CSuplNetInitiatedSession::GetHandle()
{
	DEBUG_TRACE("CSuplNetInitiatedSession::GetHandle", __LINE__)
	TInt aId=iHandle;
	return aId;
}*/
		
// ---------------------------------------------------------
// CSuplNetInitiatedSession::ForwardMessageL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
		
void CSuplNetInitiatedSession::ForwardMessageL(const RMessage2& aMessage)
{
	DEBUG_TRACE("CSuplNetInitiatedSession::ForwardMessageL", __LINE__)
/*	TInt length=clientBuf->Length();
	iClientBuf=HBufC8::NewL(length);
    *iClientBuf=*clientBuf;*/
    if (iSuplSession != NULL)
    {
    	delete iClientBuf;
    	iClientBuf = NULL;
    
		iClientBuf = SuplGlobal::CopyClientBuffer8LC(aMessage, 0);
    	CleanupStack::Pop(iClientBuf);
		RequestComplete(aMessage,KErrNone);
    	iSuplSessnReq->ForwardMessageRequestL(iSuplSession,iHandle,iClientBuf);
    }
    else
    	RequestComplete(aMessage,KErrNotReady);
	   	
}
// ---------------------------------------------------------
// CSuplNetInitiatedSession::SetHandle
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*void CSuplNetInitiatedSession::SetHandle(TInt aHandle)
{
	DEBUG_TRACE("CSuplNetInitiatedSession::SetHandle", __LINE__)
	iHandle=aHandle;
}*/

void CSuplNetInitiatedSession::DestroySession(CSuplSessionManager* aSessionMgr)
{
	if(aSessionMgr)
	aSessionMgr->DestroySession(iSuplSession);
}

// ---------------------------------------------------------
// CSuplNetInitiatedSession::CheckSuplTriggerServiceStatus
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSuplNetInitiatedSession::CheckSuplTriggerServiceStatus()
{
	
	CSuplSettings* 	suplStorageSettings = NULL;
	
	CSuplSettings::TSuplTriggerStatus suplTriggerStatus;

	// create local object iSuplStorageSettings
	TRAPD(err,suplStorageSettings = CSuplSettings::NewL());
	if(err == KErrNone)
	{
			suplStorageSettings->GetSuplTriggeredServiceStatus(suplTriggerStatus);
			delete suplStorageSettings;
			suplStorageSettings = NULL;

			if(suplTriggerStatus == CSuplSettings::ESuplTriggerOn)
			{
					return ETrue;						
			}
			else
			{
					return EFalse;	
			}
	}		
	else
	{
			return ETrue;
	}
}

// End of File

