/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Forwards client request messages to server operations.
*
*
*/


#include <EPos_Landmarks.h>

#include "EPos_CPosLmServer.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"
#include "EPos_CPosLmServerOperation.h"
#include "EPos_CPosLmInitializer.h"
#include "EPos_MPosLmServerManagerObserver.h"
#include "EPos_CPosLmOperationManager.h"

const TInt KArrayGranularity = 5;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperationManager::CPosLmOperationManager(
    CPosLmServer& aServer,
    TPosOperationManagerType aType,
    MPosLmServerManagerObserver* aObserver ) :
    CBase(),
    iServer( aServer ),
    iType( aType ),
    iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmOperationManager::ConstructL()
    {
    iUriArray = new (ELeave) CDesCArrayFlat(KArrayGranularity);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperationManager* CPosLmOperationManager::NewL(
    CPosLmServer& aServer,
    TPosOperationManagerType aType,
    MPosLmServerManagerObserver* aObserver )
    {
    CPosLmOperationManager* self = new( ELeave ) CPosLmOperationManager(
        aServer, aType, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperationManager::~CPosLmOperationManager()
    {
    iOperations.ResetAndDestroy();
    iOperations.Close();
    delete iUriArray;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmOperationManager::HandleOperationStatus(
    CPosLmServerOperation* aServerOperation,
    TInt aStatus)
    {
    if ( aStatus != KPosLmOperationNotComplete &&
         aServerOperation && !aServerOperation->HasClients() )
        {
        // Remove operation
        TInt index = iOperations.Find(aServerOperation);

        if (index != KErrNotFound)
            {
            delete aServerOperation;
            aServerOperation = NULL;
            iOperations.Remove(index);
            iUriArray->Delete(index);
            }

        if (iOperations.Count() == 0)
            {
            iObserver->HandleManagerFinished();
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmOperationManager::AddRequestL(
    const RMessage2& aMessage,
    const TDesC& aUri,
    TAny* aSession)
    {
    switch (aMessage.Function())
        {
        // Get operation corresponding to the URI and add the message to its
        // wait list.
        case EPosLmServerInitializeSync:
        case EPosLmServerLangSwitchSync:
        case EPosLmServerRecoverSync:
            OperationL( aUri, aSession )->AddToSyncWaitListL( aMessage, aSession );
            break;
        case EPosLmServerInitializeAsync:
        case EPosLmServerLangSwitchAsync:
        case EPosLmServerRecoverAsync:
            OperationL( aUri, aSession )->AddToAsyncWaitListL( aMessage, aSession );
            break;
        case EPosLmServerInitializeCancel:
        case EPosLmServerLangSwitchCancel:
        case EPosLmServerRecoverCancel:
            {
            CPosLmServerOperation* op = FindOperationL( aUri );
            if ( op )
                {
                op->RemoveFromAsyncWaitList( aMessage, aSession );
                }
            else
                {
                LmServerGlobal::Complete( aMessage, KErrNone );
                }
            break;
            }
        default:
            User::Leave(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmOperationManager::Count()
    {
    return iOperations.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmOperationManager::RemoveSessionMessages(
    TAny* aSession)
    {
    for (TInt i = 0; i < Count(); i++)
        {
        iOperations[i]->RemoveSessionMessages(aSession);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation* CPosLmOperationManager::FindOperationL( const TDesC& aUri )
    {
    CPosLmServerOperation* operation = NULL;
    TInt position;
    if ( iUriArray->Find( aUri, position ) == 0 )
        {
        // Operation found at position.
        operation = iOperations[position];
        }
    return operation;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation* CPosLmOperationManager::CreateOperationL( const TDesC& aUri )
    {
    CPosLmServerOperation* operation = NULL;
    switch (iType)
        {
        case EPosInitializerManager:
            operation = CPosLmInitializer::NewL( iServer, this, aUri );
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }

    CleanupStack::PushL(operation);

    // Append URI and operation to arrays.
    iUriArray->AppendL(aUri);
    TInt error = iOperations.Append(operation);

    if (error != KErrNone)
        {
        // Append of operation failed, delete last URI from URI array.
        // Both arrays should match.
        iUriArray->Delete(iUriArray->Count() - 1);
        User::Leave(error);
        }

    CleanupStack::Pop(operation);
    return operation;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation* CPosLmOperationManager::OperationL( 
    const TDesC& aUri,
    TAny* aSession )
    {
    CPosLmServerOperation* operation = FindOperationL( aUri );
    if ( operation )
        {
        if ( operation->Status() != KPosLmOperationNotComplete &&
            !operation->IsRegistered( aSession ) )
            {
            operation = NULL; // new operation for this client 
            }
        else
            {
            // this client has yet to get result of this operation
            }
        }
    
    if ( !operation )
        {
        // need new operation
        operation = CreateOperationL( aUri );
        }

    return operation;
    }
