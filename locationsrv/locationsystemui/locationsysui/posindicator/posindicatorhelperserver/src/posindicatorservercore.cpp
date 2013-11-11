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
* Description: Implementation of server core class.
*
*/

#include "posindicatorservercore.h"
#include "posindicatorserversession.h"
#include "posindicatorserverconst.h"
#include "posindicatorlogger.h"

// Priorty of server
// This number has its origin in Symbian development.
// The priority of a server active object should be 950.
enum
    {
    EPriority=950
    };

//---------------------------------------------------------------
// CPosIndicatorServerCore::NewLC
//---------------------------------------------------------------
CPosIndicatorServerCore* CPosIndicatorServerCore::NewLC()
    {
    FUNC("CPosIndicatorServerCore::NewLC");
    CPosIndicatorServerCore* self = new( ELeave )
                                    CPosIndicatorServerCore;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


//---------------------------------------------------------------
// CPosIndicatorServerCore::CPosIndicatorServerCore
//---------------------------------------------------------------
CPosIndicatorServerCore::CPosIndicatorServerCore( 
                        TServerType aServerType ):
                        CServer2( EPriority,aServerType ),
                        iSessionCount( 0 )
    {
    FUNC("CPosIndicatorServerCore::CPosIndicatorServerCore");
    }

//---------------------------------------------------------------
// CPosIndicatorServerCore::~CPosIndicatorServerCore
//---------------------------------------------------------------
CPosIndicatorServerCore::~CPosIndicatorServerCore()
    {
    FUNC("CPosIndicatorServerCore::~CPosIndicatorServerCore");
    delete iLocationRequestor;
    iLocationRequestor = NULL;
    }

//---------------------------------------------------------------
// CPosIndicatorServerCore::StartL
//---------------------------------------------------------------     
void CPosIndicatorServerCore::StartL(const TDesC &aName)
    {
    FUNC("CPosIndicatorServerCore::StartL");
    CServer2::StartL(aName);
    }

//---------------------------------------------------------------
// CPosIndicatorServerCore::ConstructL
//---------------------------------------------------------------
void CPosIndicatorServerCore::ConstructL()
    {
    FUNC("CPosIndicatorServerCore::ConstructL");
    iLocationRequestor = CPosIndicatorLocationRequestor::NewL();
    }
        
//---------------------------------------------------------------
// CPosIndicatorServerCore::NewSessionL
//---------------------------------------------------------------
CSession2* CPosIndicatorServerCore::NewSessionL( const TVersion& aVersion, 
                                                 const RMessage2& /*aMessage*/ ) const
   {
   FUNC("CPosIndicatorServerCore::NewSessionL"); 
   // Check if we're the right version
   TVersion version( KMajorVersionNumber,
                     KMinorVersionNumber,
                     KBuildVersionNumber );
   if (!User::QueryVersionSupported(version, aVersion))
       {
       User::Leave(KErrNotSupported);
       }
   CPosIndicatorServerSession* session = CPosIndicatorServerSession::NewL( *this );
   iSessionCount++;
   return session; 
   }

//---------------------------------------------------------------
// CPosIndicatorServerCore::GetLocationRequestorHandle
//---------------------------------------------------------------
CPosIndicatorLocationRequestor* CPosIndicatorServerCore::
                                        GetLocationRequestorHandle() const
    {
    FUNC("CPosIndicatorServerCore::GetLocationRequestorHandle");
    return iLocationRequestor;
    }

//---------------------------------------------------------------
// CPosIndicatorServerCore::HandleSessionClosure
//---------------------------------------------------------------
void CPosIndicatorServerCore::HandleSessionClosure() const
    {
    FUNC("CPosIndicatorServerCore::HandleSessionClosure");
    // If all the sessions are closed, server should exit. 
    // This can be achieved by stop the active scheduler.
    iSessionCount--;
    
    // Check the count of session iterator
    if( !iSessionCount )
        {
        CActiveScheduler::Stop();
        }
    }


// End of file.
