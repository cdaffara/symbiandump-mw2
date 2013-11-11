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
* Description:  EXPORT_C implementation of drm media utility
*
*/


#include <bautils.h>
#include <mpxmedia.h>
#include "mpxdrmmediautility.h"
#include "mpxdrmmediaomaagent.h"
#include "mpxdrmmediawmaagent.h"
#include "mpxmediadrmdefs.h"
#include "mpxlog.h"

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXDrmMediaUtility* CMPXDrmMediaUtility::NewL()
    {
    MPX_FUNC("CMPXDrmMediaUtility::NewL()");
    CMPXDrmMediaUtility* p = new (ELeave) CMPXDrmMediaUtility();
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXDrmMediaUtility* CMPXDrmMediaUtility::NewLC()
    {
    MPX_FUNC("CMPXDrmMediaUtility::NewLC()");
    CMPXDrmMediaUtility* p = new (ELeave) CMPXDrmMediaUtility();
    CleanupStack::PushL(p);
    p->ConstructL();
    return p;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXDrmMediaUtility::~CMPXDrmMediaUtility()
    {
    MPX_FUNC("CMPXDrmMediaUtility::~CMPXDrmMediaUtility()");
    iAgents.ResetAndDestroy();
    iAgents.Close();
    delete iDefaultMedia;
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CMPXDrmMediaUtility::CMPXDrmMediaUtility()
    {
    MPX_FUNC("CMPXDrmMediaUtility::CMPXDrmMediaUtility()");
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaUtility::ConstructL()
    {
    MPX_FUNC("CMPXDrmMediaUtility::ConstructL()");

    // Create the OMA Agent
    CMPXDrmMediaOmaAgent* omaAgent = CMPXDrmMediaOmaAgent::NewLC();
    iAgents.Insert( omaAgent, EOmaDrmAgent );
    CleanupStack::Pop( omaAgent );

    // Create the WMA Agent
    CMPXDrmMediaWmaAgent* wmaAgent = CMPXDrmMediaWmaAgent::NewLC();
    iAgents.Insert( wmaAgent, EWmaDrmAgent );
    CleanupStack::Pop( wmaAgent );

    // Create a default media
    RArray<TInt> supportedIds;
    CleanupClosePushL( supportedIds );
    supportedIds.AppendL(KMPXMediaIdDrm);
    iDefaultMedia = CMPXMedia::NewL( supportedIds.Array() );
    CleanupStack::PopAndDestroy( &supportedIds );
    iDefaultMedia->SetTObjectValueL( KMPXMediaDrmProtected,
                                     EFalse );

    // Create file system session
    User::LeaveIfError( iFs.Connect() );
    }


// ----------------------------------------------------------------------------
// Initialize utility using a filename
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXDrmMediaUtility::InitL( const TDesC& aFileName )
    {
    MPX_DEBUG2("CMPXDrmMediaUtility::InitL(%S) entering", &aFileName );

    Close();

    if ( BaflUtils::FileExists( iFs, aFileName ))
        {
        CContent* content( CContent::NewLC( aFileName, EContentShareReadWrite ));
        FindAgent( *content );
        if ( iCurrentAgent )
            {
            iCurrentAgent->InitL( aFileName );
            }
        CleanupStack::PopAndDestroy( content );
        }
    iConsumeStarted = EFalse;

    MPX_DEBUG1("CMPXDrmMediaUtility::InitL() exiting" );
    }

// ----------------------------------------------------------------------------
// Initialize utility using a file handle
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXDrmMediaUtility::InitL( RFile& aFile )
    {
    MPX_DEBUG1("CMPXDrmMediaUtility::InitL(RFile) entering" );

    Close();
    CContent* content = CContent::NewLC( aFile );
    FindAgent( *content );
    if ( iCurrentAgent )
        {
        iCurrentAgent->InitL( aFile );
        }
    CleanupStack::PopAndDestroy( content );
    iConsumeStarted = EFalse;

    MPX_DEBUG1("CMPXDrmMediaUtility::InitL(RFile) exiting" );
    }

// ----------------------------------------------------------------------------
// Gets media object with the given attributes
// ----------------------------------------------------------------------------
//
EXPORT_C const CMPXMedia* CMPXDrmMediaUtility::GetMediaL( TUint aAttributes )
    {
    MPX_DEBUG2("CMPXDrmMediaUtility::GetMediaL(%d) entering", aAttributes );

    const CMPXMedia* media( NULL );
    if ( iCurrentAgent )
        {
        media = &( iCurrentAgent->GetMediaL( aAttributes ));
        }
    else
        {
        MPX_DEBUG1("CMPXDrmMediaUtility::GetMediaL(): Returning default media");
        media = iDefaultMedia;
        }

    MPX_DEBUG1("CMPXDrmMediaUtility::GetMediaL() exiting" );
    return media;
    }


// ----------------------------------------------------------------------------
// Consumes the rights for the current media according
// to the specified consume type
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXDrmMediaUtility::ConsumeL( TDrmConsumeType aType )
    {
    MPX_DEBUG1("CMPXDrmMediaUtility::ConsumeL() entering");

    if ( iCurrentAgent )
        {
        TDrmConsumeType type( EMPXDrmConsumeNone );
        const CMPXMedia& media( iCurrentAgent->GetMediaL( EMPXMediaDrmProtected ));
        if ( media.ValueTObjectL<TBool>( KMPXMediaDrmProtected ))
            {
            switch ( aType )
                {
                case EMPXDrmConsumeStart:
                    {
                    type = aType;
                    iConsumeStarted = ETrue;
                    break;
                    }
                case EMPXDrmConsumePause:
                case EMPXDrmConsumeContinue:
                case EMPXDrmConsumeFinish:
                    {
                    if ( iConsumeStarted )
                        {
                        type = aType;
                        if ( EMPXDrmConsumeFinish == aType )
                            {
                            iConsumeStarted = EFalse;
                            }
                        }
                    break;
                    }
                case EMPXDrmConsumeStartOrContinue:
                    {
                    if ( iConsumeStarted )
                        {
                        type = EMPXDrmConsumeContinue;
                        }
                    else
                        {
                        type = EMPXDrmConsumeStart;
                        iConsumeStarted = ETrue;
                        }
                    break;
                    }
                default:
                    {
                    type = EMPXDrmConsumeNone;
                    iConsumeStarted = EFalse;
                    break;
                    }
                }

            if ( EMPXDrmConsumeNone != type )
                {
                iCurrentAgent->ConsumeL( type );
                }
            }
        else
            {
            iConsumeStarted = EFalse;
            }
        }

    MPX_DEBUG1("CMPXDrmMediaUtility::ConsumeL() exiting" );
    }

// ----------------------------------------------------------------------------
// Closes and unitializes the utility
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXDrmMediaUtility::Close()
    {
    MPX_DEBUG1("CMPXDrmMediaUtility::Close() entering" );

    if ( iCurrentAgent )
        {
        iCurrentAgent->Close();
        iCurrentAgent = NULL;
        }

    MPX_DEBUG1("CMPXDrmMediaUtility::Close() exiting" );
    }

// ----------------------------------------------------------------------------
// Finds an agent to handle the content
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaUtility::FindAgent( CContent& aContent )
    {
    MPX_DEBUG1("CMPXDrmMediaUtility::FindAgent() entering" );

    TAgent agent( aContent.Agent() );
    if ( agent.Name() == KMPXWDRMCafAgentName ||
         agent.Name() == KMPXS60WMDRMCafAgent )
        {
        iCurrentAgent = iAgents[EWmaDrmAgent];
        }
    else if ( agent.Name() == KMPXOMACafAgentName )
        {
        iCurrentAgent = iAgents[EOmaDrmAgent];
        }
    else
        {
        iCurrentAgent = NULL;
        }
    MPX_DEBUG1("CMPXDrmMediaUtility::FindAgent() exiting" );
    }
