/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Server session class implementation.
*
*/

// INCLUDE FILES

#include <s32mem.h>
#include <lbs/EPos_RPosRequestorStack.h>

#include "locprivacyinternal.h" 
#include "locprivacyserver.h"
#include "locprivacyserversession.h"
#include "locprivacyserverdebugpanic.h"
#include "locrequestorutilsresolver.h"
#include "locutilsdebug.h"




// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor
CLocPrivacyServerSession* CLocPrivacyServerSession::NewL(
        CLocPrivacyServer& aServer)
    {
    CLocPrivacyServerSession* self = new (ELeave) CLocPrivacyServerSession();
    CleanupStack::PushL(self);
    self->ConstructL(aServer);
    CleanupStack::Pop();
    return self;
    }

// Destructor
CLocPrivacyServerSession::~CLocPrivacyServerSession()
    {
    CloseSession();
    delete iBufFlat;
    iBufFlat = NULL;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CLocPrivacyServerSession::CLocPrivacyServerSession() :
    CSession2(), iBufFlat(NULL)
    {
    // Doesn't really do anything according to the header file (e32def.h,
    // lines 139 and 184) and the documentation doesn't mention it.
    __DECLARE_NAME(_S("CLocPrivacyServerSession"));
    }

// EPOC default constructor
void CLocPrivacyServerSession::ConstructL(CLocPrivacyServer& aServer)
    {
    LOCUTILSDEBUG( "+CLocPrivacyServerSession::ConstructL" )
    aServer.IncrementSessions();
    iBufFlat = CBufFlat::NewL(KPosBufFlatExpandSize);
    LOCUTILSDEBUG( "-CLocPrivacyServerSession::ConstructL" )
    }

// ---------------------------------------------------------
// CLocPrivacyServerSession::ServiceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLocPrivacyServerSession::ServiceL(const RMessage2& aMessage)
    {
    LOCUTILSDEBUG( "+CLocPrivacyServerSession::ServiceL" ) 
    TRAPD(err, DispatchL(aMessage));

    aMessage.Complete(err);
    LOCUTILSDEBUG( "-CLocPrivacyServerSession::ServiceL" )

    }

// ---------------------------------------------------------
// CLocPrivacyServerSession::CloseSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLocPrivacyServerSession::CloseSession()
    {
    LOCUTILSDEBUG( "+CLocPrivacyServerSession::CloseSession" ) 
    CLocPrivacyServer
            * server =
                    static_cast<CLocPrivacyServer*> (const_cast<CServer2*> (Server()));
    server->DecrementSessions(this);
    LOCUTILSDEBUG( "-CLocPrivacyServerSession::CloseSession" ) 
    }

// ---------------------------------------------------------
// CLocPrivacyServerSession::DispatchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLocPrivacyServerSession::DispatchL(const RMessage2& aMessage)
    {
    LOCUTILSDEBUG( "+CLocPrivacyServerSession::DispatchL" ) 
    switch ( aMessage.Function() )
        {
        case ELocPrivacyGetSize:
            
           {
           TInt desLength=0;
           desLength = aMessage.GetDesLength( 1 );          

           CBufFlat* bufFlat1 = CBufFlat::NewL(desLength);
           CleanupStack::PushL(bufFlat1);
           bufFlat1->ResizeL(desLength);
           
           TPtr8 ptr(bufFlat1->Ptr(0));
           // Copy content to local buffer
           aMessage.ReadL(1, ptr);

           RBufReadStream readStream(*bufFlat1);
           CleanupClosePushL(readStream);
           
           RPosRequestorStack* requestors1 = new (ELeave) RPosRequestorStack;
           CleanupStack::PushL(requestors1);
            requestors1->InternalizeL(readStream);

            CLocRequestorUtilsResolver* res =
                    CLocRequestorUtilsResolver::NewL();
            CleanupStack::PushL(res);

            res->ResolveRequestorsL(*requestors1);

               
           // use data member buffer and externalize 

            RBufWriteStream writeStream;
            // Reset the buffer
            iBufFlat->Reset();
            writeStream.Open(*iBufFlat);
            CleanupClosePushL(writeStream);

            requestors1->ExternalizeL(writeStream);
            writeStream.CommitL();

            // Find the new size

            TInt newsize  = iBufFlat->Size();

            CleanupStack::PopAndDestroy(&writeStream);
            CleanupStack::PopAndDestroy(res);
            CleanupStack::PopAndDestroy(requestors1);
            CleanupStack::PopAndDestroy(&readStream);
            CleanupStack::PopAndDestroy(bufFlat1);

            // write back new size
            TPtr8 ptr1(reinterpret_cast<TUint8*> (&newsize),
                    sizeof(newsize), sizeof(newsize));
                  
            aMessage.WriteL(0, ptr1);
         	}     
            break;

        case ELocPrivacyResolve:
            {
            TPtr8 ptr2 = iBufFlat->Ptr(0);
            aMessage.WriteL(0, ptr2);
        	}
            break;
            
        default:
            aMessage.Panic(KLocPrivSrvDebugPanicCategory,
                    ELocPrivSrvPanicUnknownActivity);
            break;
        }
    LOCUTILSDEBUG( "-CLocPrivacyServerSession::DispatchL" )
    }

//  End of File
