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
* Description:  Implementation of playback engine media helper
*
*/


#include <mpxtaskqueue.h>
#include <mpxcollectionutility.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxlog.h>
#include "mpxplaybackengine.h"
#include "mpxplaybackmediahelper.h"

// CONSTANTS
const TInt KTaskMediaWithAttributeSpec = 0;

// ----------------------------------------------------------------------------
// Two-phased constructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackMediaHelper* CMPXPlaybackMediaHelper::NewL(
    CMPXPlaybackEngine& aEngine )
    {
    CMPXPlaybackMediaHelper* p= new (ELeave) CMPXPlaybackMediaHelper( aEngine );
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackMediaHelper::CMPXPlaybackMediaHelper(
    CMPXPlaybackEngine& aEngine )
    : iEngine( aEngine )
    {
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackMediaHelper::ConstructL()
    {
    iCollectionUtility = MMPXCollectionUtility::NewL( this );

#ifdef __USE_MESSAGE_SUBSCRIPTION
    // This class does not care about collection messages
    iCollectionUtility->Collection().ClearSubscriptionsL();
#endif
    iTaskQueue = CMPXActiveTaskQueue::NewL();
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackMediaHelper::~CMPXPlaybackMediaHelper()
    {
    if ( iCollectionUtility )
        {
        iCollectionUtility->Close();
        }
    if ( iTaskQueue )
        {
        iTaskQueue->CancelRequests();
        delete iTaskQueue;
        }
    }

// ----------------------------------------------------------------------------
// Retrieves the media given the current path, and upon return,
// either calls back the observer, or broadcasts the message to
// the given client list
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::MediaL(
    const CMPXCollectionPath& aPath,
    CMPXCommand* aCmd,
    MMPXPlaybackEngineObserver* aObserver,
    TBool aBroadcast /*= EFalse */,
    CMPXClientList* aClientList /*= NULL */ )
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::MediaL(): Entering");

    CMPXCollectionPath* path( CMPXCollectionPath::NewL( aPath ) );
    CleanupStack::PushL( path );
    CleanupStack::PushL( aCmd );

    if ( aBroadcast )
        {
        iTaskQueue->AddTaskL( KTaskMediaWithAttributeSpec,
                              aClientList,
                              this,
                              ETrue,
                              NULL,
                              NULL,
                              path,
                              aCmd );
        }
    else
        {
        iTaskQueue->AddTaskL( KTaskMediaWithAttributeSpec,
                              aObserver,
                              this,
                              EFalse,
                              NULL,
                              NULL,
                              path,
                              aCmd );
        }
    // Ownership of aAttrs and path passed to the task queue
    CleanupStack::Pop( aCmd );
    CleanupStack::Pop( path );

    MPX_DEBUG1("CMPXPlaybackMediaHelper::MediaL(): Exiting");
    }

// ----------------------------------------------------------------------------
// Cancels all outstanding requests
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::CancelRequests()
    {
    MPX_FUNC_EX("CMPXPlaybackMediaHelper::CancelRequests()");
    if ( iCollectionUtility )
        {
        iCollectionUtility->Collection().CancelRequest();
        }
    if ( iTaskQueue )
        {
        iTaskQueue->CancelRequests();
        }
    }

// ----------------------------------------------------------------------------
// From MMPXTaskQueueObserver
// Executes task
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::ExecuteTask(
    TInt aTask,
    TInt aParamData,
    TAny* aPtrData,
    const CBufBase& aBuf,
    TAny* aCallback,
    CBase* aCObject1,
    CBase* aCObject2)
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::ExecuteTask(): Entering");

    TRAPD( err, DoExecuteTaskL( aTask, aParamData, aPtrData,
                                aBuf, aCallback, aCObject1, aCObject2 ));
    if ( KErrNone != err )
        {
        TRAP_IGNORE(
            CMPXMedia* dummy( CMPXMedia::NewL() );
            CleanupStack::PushL( dummy );
            HandleCollectionMediaL( *dummy, err );
            CleanupStack::PopAndDestroy( dummy );
            )
        }

    MPX_DEBUG1("CMPXPlaybackMediaHelper::ExecuteTask(): Exiting");
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackMediaHelper::HandleTaskError
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::HandleTaskError(
    TInt /* aTask */,
    TAny* /*aPtrData*/,
    TAny* /*aCallback*/,
    TInt /* aError */)
    {
    // do nothing, queued tasks are not canceled
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionObserver
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::HandleCollectionMessage(
    CMPXMessage* /*aMsg*/,
    TInt /*aErr*/)
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleCollectionMessageL(CMPXMessage): Entering");
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleCollectionMessageL(CMPXMessage): Exiting");
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionObserver
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::HandleOpenL(
    const CMPXMedia& /*aEntries*/,
    TInt /*aIndex*/,
    TBool /*aComplete*/,
    TInt /*aError*/ )
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleOpenL(): Entering");
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleOpenL(): Exiting");
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionObserver
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::HandleOpenL(
    const CMPXCollectionPlaylist& /*aPlaylist*/,
    TInt /*aError*/ )
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleOpenL(): Entering");
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleOpenL(): Exiting");
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionObserver
// ----------------------------------------------------------------------------
void CMPXPlaybackMediaHelper::HandleCommandComplete(
    CMPXCommand* /*aCommandResult*/,
    TInt /*aError*/ )
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleCommandComplete(): Entering");
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleCommandComplete(): Exiting");
    }

// ----------------------------------------------------------------------------
// From MMPXCollectionmediaObserver
// ----------------------------------------------------------------------------
//
void CMPXPlaybackMediaHelper::HandleCollectionMediaL(
    const CMPXMedia& aMedia,
    TInt aError )
    {
    MPX_FUNC_EX("CMPXPlaybackMediaHelper::HandleCollectionMediaL");
    MPX_DEBUG2("CMPXPlaybackMediaHelper::HandleCollectionMediaL(): error %d", aError);
    MPX_DEBUG2("CMPXPlaybackMediaHelper::HandleCollectionMediaL task %d",
               iTaskQueue->Task());
               
    if ( iTaskQueue->Param() )
        {
        // Broadcast
        CMPXMessage* msg( CMPXMessage::NewL() );
        CleanupStack::PushL( msg );
        msg->SetTObjectValueL<TMPXMessageId>( KMPXMessageGeneralId,
                                              KMPXMessagePbMediaChanged );
        msg->SetCObjectValueL<CMPXMedia>( KMPXMessagePbMedia,
                                          const_cast<CMPXMedia*>( &aMedia ));
        CMPXClientList* clientList(
            static_cast<CMPXClientList*>( iTaskQueue->Callback() ));
        clientList->SendMsg(msg, KErrNone);
        CleanupStack::PopAndDestroy( msg );
        }
    else
        {
        // Callback
        MPX_DEBUG2("CMPXPlaybackMediaHelper::HandleCollectionMediaL task cb 0x%08x",
                   iTaskQueue->Callback());
        MMPXPlaybackEngineObserver* callback(
            static_cast<MMPXPlaybackEngineObserver*>( iTaskQueue->Callback() ));
        callback->HandleMedia( aMedia, aError );
        }
        
    MPX_DEBUG1("CMPXPlaybackMediaHelper::HandleCollectionMediaL complete task");
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// Executes task, leaving method
// ----------------------------------------------------------------------------
//
void CMPXPlaybackMediaHelper::DoExecuteTaskL(
    TInt aTask,
    TInt /*aParamData*/,
    TAny* /*aPtrData*/,
    const CBufBase& /*aBuf*/,
    TAny* /*aCallback*/,
    CBase* aCObject1,
    CBase* aCObject2)
    {
    MPX_DEBUG1("CMPXPlaybackMediaHelper::DoExcuteTaskL(): Entering");

    if ( KTaskMediaWithAttributeSpec == aTask )
        {
        CMPXCollectionPath* path( static_cast<CMPXCollectionPath*>(aCObject1));
        CMPXMedia* media( static_cast<CMPXMedia*>(aCObject2));

        CMPXAttributeSpecs* specs =
                media->Value<CMPXAttributeSpecs>( KMPXCommandMediaAttributeSpecs );

        const TDesC& attr = media->ValueText( KMPXCommandMediaAttribute );
        TPtrC8 ptr = MPXUser::Ptr( attr );
        RDesReadStream readStream( ptr );
        CleanupClosePushL( readStream );
        // Internalize attributes
        RArray<TMPXAttribute> attrs;
        CleanupClosePushL( attrs );
        ::InternalizeL( attrs, readStream );
        iCollectionUtility->Collection().MediaL(*path,
                                                attrs.Array(),
                                                specs );

        CleanupStack::PopAndDestroy( &attrs );
        CleanupStack::PopAndDestroy( &readStream );
        }
    else
        {
        iTaskQueue->CompleteTask();
        }

    MPX_DEBUG1("CMPXPlaybackMediaHelper::DoExcuteTaskL(): Exiting");
    }

//End of file
