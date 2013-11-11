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
* Description:  Implementation of drm media OMA agent
*
*/


#include <caf/rightsinfo.h>
#ifdef CAMESE_IN_DRM_UTILITY
#include <drmuihandling.h>
#include <drmutility.h>
#include <drmrightsinfo.h>
#include <drmautomatedusage.h>
#include <caf/data.h>
#include <DRMRights.h>
#include <DRMCommon.h>
#else
#include <DRMHelper.h>
#endif
#include "mpxmedia.h"
#include "mpxmediadrmdefs.h"
#include "mpxdrmmediaomaagent.h"
#include "mpxlog.h"

// CONSTANTS
const TUint32 KMPXAboutToExpireCount = 3;
const TInt KMPXAboutToExpireTime = 7;
const TInt KMPXAboutToExpireAccTime = 15*60;    // 15 minutes
const TInt KMPXSecondsInDay = 86400;
#ifdef CAMESE_IN_DRM_UTILITY
const TInt KUrlMaxLen( 1024 ); // Same than in DRMHelper
#endif


// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXDrmMediaOmaAgent* CMPXDrmMediaOmaAgent::NewL()
    {
    MPX_FUNC("CMPXDrmMediaOmaAgent::NewL()");
    CMPXDrmMediaOmaAgent* p = CMPXDrmMediaOmaAgent::NewLC();
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXDrmMediaOmaAgent* CMPXDrmMediaOmaAgent::NewLC()
    {
    MPX_FUNC("CMPXDrmMediaOmaAgent::NewLC()");
    CMPXDrmMediaOmaAgent* p = new (ELeave) CMPXDrmMediaOmaAgent();
    CleanupStack::PushL(p);
    p->ConstructL();
    return p;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXDrmMediaOmaAgent::~CMPXDrmMediaOmaAgent()
    {
    MPX_FUNC("CMPXDrmMediaOmaAgent::~CMPXDrmMediaOmaAgent()");
    Close();
#ifdef CAMESE_IN_DRM_UTILITY
    delete iDrmHandler;
    
    if ( iDRMCommon )
        {
        iDRMCommon->Disconnect();
        }
    delete iDRMCommon;
#else
    delete iDrmHelper;
#endif
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CMPXDrmMediaOmaAgent::CMPXDrmMediaOmaAgent()
    {
    MPX_FUNC("CMPXDrmMediaOmaAgent::CMPXDrmMediaOmaAgent()");
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::ConstructL()
    {
    MPX_FUNC("CMPXDrmMediaOmaAgent::ConstructL()");

    User::LeaveIfError( iFs.Connect() );
#ifdef CAMESE_IN_DRM_UTILITY
    iDrmHandler = DRM::CDrmUiHandling::NewL();
    
    iDRMCommon = DRMCommon::NewL();
    User::LeaveIfError( iDRMCommon->Connect() );
#else
    iDrmHelper = CDRMHelper::NewL();
#endif
    }


// ----------------------------------------------------------------------------
// Initialize utility using a filename
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::InitL( const TDesC& aFileName )
    {
    MPX_DEBUG2("CMPXDrmMediaOmaAgent::InitL(%S) entering", &aFileName );

    Close();
    CreateMediaL();
    iData = CData::NewL(
        (TVirtualPathPtr) aFileName,
        EPeek,
        EContentShareReadOnly );
    User::LeaveIfError( iFile.Open( iFs, aFileName, EFileShareReadersOrWriters ));

    MPX_DEBUG1("CMPXDrmMediaOmaAgent::InitL() exiting" );
    }

// ----------------------------------------------------------------------------
// Initialize utility using a file handle
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::InitL( RFile& aFile )
    {
    MPX_DEBUG1("CMPXDrmMediaOmaAgent::InitL(RFile) entering" );

    Close();
    CreateMediaL();
    iData = CData::NewL(
        aFile,
        KDefaultContentObject(),
        EPeek );
    User::LeaveIfError( iFile.Duplicate( aFile ) );

    MPX_DEBUG1("CMPXDrmMediaOmaAgent::InitL(RFile) exiting" );
    }

// ----------------------------------------------------------------------------
// Gets media object with the given attributes
// ----------------------------------------------------------------------------
//
const CMPXMedia& CMPXDrmMediaOmaAgent::GetMediaL( TUint aAttributes )
    {
    MPX_DEBUG2("CMPXDrmMediaOmaAgent::GetMediaL(%d) entering", aAttributes );

    if ( iMedia )
        {
        if ( aAttributes & KMPXMediaDrmProtected.iAttributeId )
            {
            GetProtectedL();
            }
        if (( aAttributes & KMPXMediaDrmRightsStatus.iAttributeId ) ||
            ( aAttributes & KMPXMediaDrmSendingAllowed.iAttributeId ))
            {
            GetRightsDetailsL();
            }
        if ( aAttributes & KMPXMediaDrmRightsType.iAttributeId )
            {
            GetRightsTypeL();
            }
        if ( aAttributes & KMPXMediaDrmCount.iAttributeId )
            {
            GetCountL();
            }
        if ( aAttributes & KMPXMediaDrmStartTime.iAttributeId )
            {
            GetStartTimeL();
            }
        if ( aAttributes & KMPXMediaDrmEndTime.iAttributeId )
            {
            GetEndTimeL();
            }
        if ( aAttributes & KMPXMediaDrmInterval.iAttributeId )
            {
            GetIntervalL();
            }
        if ( aAttributes & KMPXMediaDrmIntervalStartTime.iAttributeId )
            {
            GetIntervalStartTimeL();
            }
        if ( aAttributes & KMPXMediaDrmAccumulatedTime.iAttributeId )
            {
            GetAccumulatedTimeL();
            }
        if ( aAttributes & KMPXMediaDrmCanSetAutomated.iAttributeId )
            {
            GetCanSetAutomatedL();
            }
        if ( aAttributes & KMPXMediaDrmHasInfoUrl.iAttributeId )
            {
            GetHasInfoUrlL();
            }
        if ( aAttributes & KMPXMediaDrmHasPreviewUrl.iAttributeId )
            {
            GetHasPreviewUrlL();
            }
        if ( aAttributes & KMPXMediaDrmAboutToExpire.iAttributeId )
            {
            GetAboutToExpireL();
            }
        }

    MPX_DEBUG1("CMPXDrmMediaOmaAgent::GetMediaL() exiting" );
    return *iMedia;
    }

// ----------------------------------------------------------------------------
// Consumes the rights for the current media according
// to the specified consume type
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::ConsumeL( TDrmConsumeType aType )
    {
    MPX_DEBUG1("CMPXDrmMediaOmaAgent::ConsumeL() entering");
#ifdef CAMESE_IN_DRM_UTILITY
    // ConsumeL is not used at the moment
    ASSERT(0);
    User::Leave( KErrNotSupported );
#else
    if ( iMedia )
        {
        CDRMHelper::TDRMHelperConsumeAction action( CDRMHelper::EStart );
        switch ( aType )
            {
            case ( EMPXDrmConsumeStart ):
            {
            action = CDRMHelper::EStart;
            break;
            }
            case ( EMPXDrmConsumePause ):
            {
            action = CDRMHelper::EPause;
            break;
            }
            case ( EMPXDrmConsumeContinue ):
            {
            action = CDRMHelper::EContinue;
            break;
            }
            case ( EMPXDrmConsumeFinish ):
            {
            action = CDRMHelper::EFinish;
            break;
            }
            default:
                {
                break;
                }
            }

        User::LeaveIfError( iDrmHelper->ConsumeFile2(
                iFile,
                ContentAccess::EPlay,
                action ));
        }
#endif 
     MPX_DEBUG1("CMPXDrmMediaOmaAgent::ConsumeL() exiting" );
    }

// ----------------------------------------------------------------------------
// Closes and uninitializes the agent and cleans up member variables
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::Close()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::Close() entering");

    CMPXDrmMediaAgent::Close();
    delete iRightsConstraints;
    iRightsConstraints = NULL;
    iFile.Close();

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::Close() exiting");
    }

// ----------------------------------------------------------------------------
// Creates a new media object
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::CreateMediaL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::CreateMediaL() entering");

    CMPXDrmMediaAgent::CreateMediaL();

    // Set the type to OMA
    iMedia->SetTObjectValueL( KMPXMediaDrmType, EMPXDrmTypeOMA );

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::CreateMediaL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the rights details
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetRightsDetailsL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetRightsDetailsL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmRightsStatus ) ||
         !iMedia->IsSupported( KMPXMediaDrmSendingAllowed ))
        {
#ifdef CAMESE_IN_DRM_UTILITY
        TBool expired( EFalse );
        TBool sendingAllowed( ETrue );
        HBufC* contentURI = NULL;
        HBufC8* contentURI8 = NULL;
        CDRMRights* activeRights = NULL;
        
        // Get forwardable status from CAF
        User::LeaveIfError( iData->GetAttribute( EIsForwardable, sendingAllowed ) ) ;
        
        // Get constraints. Copied from CDRMHelper::GetRightsDetailsL 
        contentURI = HBufC::NewLC( KUrlMaxLen );
        TPtr ptr = contentURI->Des();
        User::LeaveIfError( iData->GetStringAttribute( EContentID, ptr ) );

        contentURI8 = HBufC8::NewLC( contentURI->Length() );
        contentURI8->Des().Copy( contentURI->Des() );
        
        // GetActiveRights returns negative value if rights are not valid
        if ( iDRMCommon->GetActiveRights( *contentURI8, ContentAccess::EPlay, activeRights ) > 0 )
            {
            // rights are valid
            activeRights->GetPlayRight( iRightsConstraints );
            }
        else 
        	{
        	expired = ETrue;        
        	}
        CleanupStack::PopAndDestroy( 2 ); // contentURI8, contentURI
#else
        TInt error( KErrNone );
        TBool expired( EFalse );
        TBool sendingAllowed( ETrue );
        CDRMHelperRightsConstraints* ignore1( NULL );
        CDRMHelperRightsConstraints* ignore2( NULL );
        CDRMHelperRightsConstraints* ignore3( NULL );
        delete iRightsConstraints;
        iRightsConstraints = NULL;

        TRAP( error, iDrmHelper->GetRightsDetailsL(
                iFile,
                ContentAccess::EPlay,
                expired,
                sendingAllowed,
                iRightsConstraints,
                ignore1,
                ignore2,
                ignore3 ));
        delete ignore1;
        delete ignore2;
        delete ignore3;
#endif  

        if ( expired )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                      EMPXDrmRightsExpired );
            }
        else if ( iRightsConstraints )
            {
            if ( iRightsConstraints->IsPreview() )
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                          EMPXDrmRightsPreview );
                }
            else if ( iRightsConstraints->FullRights() )
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                          EMPXDrmRightsFull );
                }
            else
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                          EMPXDrmRightsRestricted );
                }
            }
        else
            {
            // Not expired, no constraints object obtainable,
            // so check if it's protected
            if ( !iMedia->IsSupported( KMPXMediaDrmProtected ))
                {
                GetProtectedL();
                }
            TBool prot( iMedia->ValueTObjectL<TBool>( KMPXMediaDrmProtected ));
            if ( prot )
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                          EMPXDrmRightsMissing );
                }
            else
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                          EMPXDrmRightsFull );
                }
            }

        iMedia->SetTObjectValueL( KMPXMediaDrmSendingAllowed,
                                  sendingAllowed );
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetRightsDetailsL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the rights type attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetRightsTypeL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetRightsTypeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmRightsType ))
        {
        // Check if it's protected or not first
        if ( !iMedia->IsSupported( KMPXMediaDrmProtected ))
            {
            GetProtectedL();
            }
        TBool prot( iMedia->ValueTObjectL<TBool>( KMPXMediaDrmProtected ));
        if ( !prot )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                      EMPXDrmRightsTypeNoRestriction );
            }
        else
            {
            // Else it's a protected file, so get the rights constraints
            if ( !iRightsConstraints )
                {
                GetRightsDetailsL();
                }

            // Check if it's a count based rights
            GetCountL();
            if ( !iMedia->IsSupported( KMPXMediaDrmCount ))
                {
                // Check if it's time based
                GetStartTimeL();
                if ( !iMedia->IsSupported( KMPXMediaDrmStartTime ))
                    {
                    // Check if it's interval based
                    GetIntervalL();
                    if ( !iMedia->IsSupported( KMPXMediaDrmInterval ))
                        {
                        // Check if it's accumulated time based
                        GetAccumulatedTimeL();
                        if ( !iMedia->IsSupported( KMPXMediaDrmAccumulatedTime ))
                            {
                            // not have any of the restriction types
                            iMedia->SetTObjectValueL(
                                KMPXMediaDrmRightsType,
                                EMPXDrmRightsTypeNoRestriction );
                            }   // else not accumulated time
                        }   // else not interval
                    }   // else not time based
                }   // else not count based
            }   // else protected file
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetRightsTypeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the count attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetCountL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL() entering");

    // Do not need to check if it already supported, we
    // just overwrite the old value if possible
    if ( !iRightsConstraints )
        {
        GetRightsDetailsL();
        }
    if ( iRightsConstraints )
        {
        TUint32 countLeft( 0 );
        TUint32 originalCount( 0 );
        TUint32 timedCount( 0 );
#ifdef CAMESE_IN_DRM_UTILITY
        // Logic copied from CDRMHelperRightsConstraints
        
        TInt error1 = iRightsConstraints->GetCounters( countLeft, originalCount );
                
        TBool timedCounter = EFalse;
        if ( ( iRightsConstraints->GetConstraint()).iActiveConstraints &
        	EConstraintTimedCounter )
            {
            timedCount = ( iRightsConstraints->GetConstraint()).iTimedCounter;
            timedCounter = ETrue;
            }
        
        if ( timedCounter )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                      EMPXDrmRightsTypeTimeCount );
            if ( error1 != DRMCommon::ENoRights )
                {
                countLeft = Min( countLeft, timedCount );
                }
            else
                {
                countLeft = timedCount;
                }
            iMedia->SetTObjectValueL( KMPXMediaDrmCount,
                                      countLeft );
            MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetCountL(): count = %d", countLeft);
            }
        else if ( error1 != DRMCommon::ENoRights )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                      EMPXDrmRightsTypeCount );
            iMedia->SetTObjectValueL( KMPXMediaDrmCount,
                                      countLeft );
            MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetCountL(): count = %d", countLeft);
            }
#else
        TTimeIntervalSeconds ignoreTime;
        TRAPD( error1, iRightsConstraints->GetCountersL(
                countLeft,
                originalCount ));
        TRAPD( error2, iRightsConstraints->GetTimedCountL(
                timedCount,
                originalCount,
                ignoreTime ));
        if ( KErrNone == error2 )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                    EMPXDrmRightsTypeTimeCount );
            if ( KErrNone == error1 )
                {
                countLeft = Min( countLeft, timedCount );
                }
            else
                {
                countLeft = timedCount;
                }
            iMedia->SetTObjectValueL( KMPXMediaDrmCount,
                    countLeft );
            MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetCountL(): count = %d", countLeft);
            }
        else if ( KErrNone == error1 )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                    EMPXDrmRightsTypeCount );
            iMedia->SetTObjectValueL( KMPXMediaDrmCount,
                    countLeft );
            MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetCountL(): count = %d", countLeft);
            }
#endif  		
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the start time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetStartTimeL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetStartTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmStartTime ))
        {
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }
        if ( iRightsConstraints )
            {
            TTime startTime;
#ifdef CAMESE_IN_DRM_UTILITY
            if( iRightsConstraints->GetStartTime( startTime ) != DRMCommon::ENoRights )
#else
                TRAPD( error, iRightsConstraints->GetStartTimeL( startTime ));
            if ( KErrNone == error )
#endif
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                          EMPXDrmRightsTypeTime );
                iMedia->SetTObjectValueL( KMPXMediaDrmStartTime,
                                          startTime.Int64() );
                MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL(): setting start time");
                }
            }
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetStartTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the end time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetEndTimeL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetEndTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmEndTime ))
        {
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }
        if ( iRightsConstraints )
            {
            TTime endTime;
#ifdef CAMESE_IN_DRM_UTILITY
            if( iRightsConstraints->GetEndTime( endTime ) != DRMCommon::ENoRights )
#else
                TRAPD( error, iRightsConstraints->GetEndTimeL( endTime ));
            if ( KErrNone == error )
#endif
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                          EMPXDrmRightsTypeTime );
                iMedia->SetTObjectValueL( KMPXMediaDrmEndTime,
                                          endTime.Int64() );
                MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL(): setting end time");
                }
            }
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetEndTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the interval attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetIntervalL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetIntervalL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmInterval ))
        {
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }
        if ( iRightsConstraints )
            {
            TTimeIntervalSeconds interval;
#ifdef CAMESE_IN_DRM_UTILITY
            if( iRightsConstraints->GetInterval( interval ) != DRMCommon::ENoRights )
#else
                TRAPD( error, iRightsConstraints->GetIntervalL( interval ));
            if ( KErrNone == error )
#endif
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                          EMPXDrmRightsTypeInterval );
                iMedia->SetTObjectValueL( KMPXMediaDrmInterval,
                                          interval );
                MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL(): setting interval");
                }
            }
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetIntervalL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the interval start time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetIntervalStartTimeL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetIntervalStartTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmIntervalStartTime ))
        {
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }
        if ( iRightsConstraints )
            {
            TTime intervalStart;
#ifdef CAMESE_IN_DRM_UTILITY
            if( iRightsConstraints->GetIntervalStart( intervalStart ) != DRMCommon::ENoRights )
#else
                TRAPD( error, iRightsConstraints->GetIntervalStartL(
                        intervalStart ));
            if ( KErrNone == error )
#endif
                {
                iMedia->SetTObjectValueL( KMPXMediaDrmIntervalStartTime,
                                          intervalStart.Int64() );
                MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL(): setting interval start time");
                }
            }
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetIntervalStartTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the accumlated time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetAccumulatedTimeL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetAccumulatedTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmAccumulatedTime ))
        {
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }
        if ( iRightsConstraints )
            {
#ifdef CAMESE_IN_DRM_UTILITY
            // Copied from DRMHelper::GetAccumulatedTimeL
            if ((iRightsConstraints->GetConstraint()).iActiveConstraints &
           		EConstraintAccumulated )
                {
                TTimeIntervalSeconds time = 
                (iRightsConstraints->GetConstraint()).iAccumulatedTime;
#else
                TTimeIntervalSeconds time;
                TRAPD( error, iRightsConstraints->GetAccumulatedTimeL( time ));
                if ( KErrNone == error )
                    {
#endif	
                    iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                            EMPXDrmRightsTypeAccTime );
                    iMedia->SetTObjectValueL( KMPXMediaDrmAccumulatedTime,
                            time );
                    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCountL(): setting accumulated time");
                    }
                }
            }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetAccumulatedTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the can set automated attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetCanSetAutomatedL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCanSetAutomatedL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmCanSetAutomated ))
        {
        TBool canSetAutomated( ETrue );
        
#ifdef CAMESE_IN_DRM_UTILITY
        DRM::CDrmAutomatedUsage* drmAutomatedUsage = DRM::CDrmAutomatedUsage::NewLC();
        
		TRAP_IGNORE(
			canSetAutomated = drmAutomatedUsage->CanSetAutomatedL( iFile, 
        		ContentAccess::EPlay, DRM::EAUAutomatedTypeRingingTone );
			);
        CleanupStack::PopAndDestroy( drmAutomatedUsage );
        
        iMedia->SetTObjectValueL( KMPXMediaDrmCanSetAutomated, canSetAutomated );
#else
        iDrmHelper->CanSetAutomated( iFile, canSetAutomated );
        iMedia->SetTObjectValueL( KMPXMediaDrmCanSetAutomated,
                canSetAutomated );
#endif
        MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetCanSetAutomatedL(): set to %d", canSetAutomated );
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetCanSetAutomatedL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the info url attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetHasInfoUrlL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetHasInfoUrlL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmHasInfoUrl ))
        {
#ifdef CAMESE_IN_DRM_UTILITY
        DRM::TDrmUiUrlType types = DRM::TDrmUiUrlType( 0 );
        TRAP_IGNORE( iDrmHandler->AvailableUrlsL( iFile, types ) );
        TBool hasInfoUrl = ( types & DRM::EUHInfoUrl ) == DRM::EUHInfoUrl;
        
        iMedia->SetTObjectValueL( KMPXMediaDrmHasInfoUrl, hasInfoUrl );
#else
        TFileName filename;
        iFile.FullName( filename );
        HBufC8* infoUrl( NULL );
        TBool hasInfoUrl( EFalse );
        TRAP_IGNORE( iDrmHelper->HasInfoUrlL( filename, infoUrl ));
        delete infoUrl;
        iMedia->SetTObjectValueL( KMPXMediaDrmHasInfoUrl,
                hasInfoUrl );
#endif
        MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetHasInfoUrlL(): set to %d", hasInfoUrl );
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetHasInfoUrlL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the preview url attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetHasPreviewUrlL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetHasPreviewUrlL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmHasPreviewUrl ))
        {
#ifdef CAMESE_IN_DRM_UTILITY
        DRM::TDrmUiUrlType types = DRM::TDrmUiUrlType( 0 );
        TRAP_IGNORE( iDrmHandler->AvailableUrlsL( iFile, types ) );
        TBool hasPreviewUrl = ( ( types & DRM::EUHPreviewRightsUrl ) 
			== DRM::EUHPreviewRightsUrl );
        iMedia->SetTObjectValueL( KMPXMediaDrmHasPreviewUrl, hasPreviewUrl );
#else
        TFileName filename;
        iFile.FullName( filename );
        HBufC8* previewUrl( NULL );
        TRAP_IGNORE( iDrmHelper->HasPreviewL( filename, previewUrl ));
        TBool hasPreviewUrl( previewUrl != NULL );
        delete previewUrl;
        iMedia->SetTObjectValueL( KMPXMediaDrmHasPreviewUrl,
                hasPreviewUrl );
#endif
        MPX_DEBUG2("CXMPXDrmMediaOmaAgent::GetHasPreviewUrlL(): set to %d", hasPreviewUrl );
        }

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetHasPreviewUrlL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets whether the DRM object is about to expire or not
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaOmaAgent::GetAboutToExpireL()
    {
    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetAboutToExpireL() entering");

    TBool aboutToExpire( EFalse );
    if ( !iMedia->IsSupported( KMPXMediaDrmRightsStatus ))
        {
        GetRightsDetailsL();
        }

    // Only restricted types can expire
    if ( EMPXDrmRightsRestricted ==
         iMedia->ValueTObjectL<TBool>( KMPXMediaDrmRightsStatus ))
        {
        // Get the type
        if ( !iRightsConstraints )
            {
            GetRightsDetailsL();
            }

        // Check if it's a count based rights
        GetCountL();
        if ( iMedia->IsSupported( KMPXMediaDrmCount ) )
            {
            // count based, or time/count based
            MPX_DEBUG1( "CXMPXDrmMediaOmaAgent::GetAboutToExpireL EMPXDrmRightsTypeCount/EMPXDrmRightsTypeTimeCount" );
            // Try to get count value
            TInt count(iMedia->ValueTObjectL<TInt>( KMPXMediaDrmCount ));
            if ( count <= KMPXAboutToExpireCount )
                {
                aboutToExpire = ETrue;
                }
            }   // end if count type
        else
            {
            GetStartTimeL();
            if ( iMedia->IsSupported( KMPXMediaDrmStartTime ) )
                {
                // time based
                MPX_DEBUG1( "CXMPXDrmMediaOmaAgent::GetAboutToExpireL EMPXDrmRightsTypeTime" );
                if ( !iMedia->IsSupported( KMPXMediaDrmEndTime ))
                    {
                    GetEndTimeL();
                    }

                // Check if end time attribute was actually fetched
                // in GetEndTimeL()
                if ( iMedia->IsSupported( KMPXMediaDrmEndTime ))
                    {
                    TInt64 end(
                        iMedia->ValueTObjectL<TInt64>( KMPXMediaDrmEndTime ));
                    TTime endTime( end );
                    TTime currentTime;
                    currentTime.HomeTime();
                    TInt diff( endTime.DaysFrom( currentTime ).Int() );
                    if ( diff <= KMPXAboutToExpireTime )
                        {
                        aboutToExpire = ETrue;
                        }
                    }
                }   // end if time type
            else
                {
                GetIntervalL();
                if ( iMedia->IsSupported( KMPXMediaDrmInterval ) )
                    {
                    // interval based
                    MPX_DEBUG1( "CXMPXDrmMediaOmaAgent::GetAboutToExpireL EMPXDrmRightsTypeInterval" );
                    if ( !iMedia->IsSupported( KMPXMediaDrmInterval ))
                        {
                        GetIntervalL();
                        }
                    if ( !iMedia->IsSupported( KMPXMediaDrmIntervalStartTime ))
                        {
                        GetIntervalStartTimeL();
                        }

                    TInt intervalDiff( 0 );
                    const TTimeIntervalSeconds& interval(
                        iMedia->ValueTObjectL<TTimeIntervalSeconds>(
                            KMPXMediaDrmInterval ));
                    if ( iMedia->IsSupported( KMPXMediaDrmIntervalStartTime ))
                        {
                        // Interval has already started
                        TTime currentTime;
                        currentTime.HomeTime();
                        TInt64 start(
                            iMedia->ValueTObjectL<TInt64>( KMPXMediaDrmIntervalStartTime ));
                        TTime intervalStart( start );
                        TTimeIntervalSeconds secondsFromStart;
                        currentTime.SecondsFrom( intervalStart, secondsFromStart );
                        intervalDiff = ( interval.Int() - secondsFromStart.Int() ) /
                                            KMPXSecondsInDay;
                        }
                    else
                        {
                        // Interval has not started yet
                        intervalDiff = interval.Int() / KMPXSecondsInDay;
                        }
                    if ( intervalDiff <= KMPXAboutToExpireTime )
                        {
                        aboutToExpire = ETrue;
                        }
                    }   // end if interval type
                else
                    {
                    GetAccumulatedTimeL();
                    if ( iMedia->IsSupported( KMPXMediaDrmAccumulatedTime ) )
                        {
                        MPX_DEBUG1( "CXMPXDrmMediaOmaAgent::GetAboutToExpireL EMPXDrmRightsTypeAccTime" );
                        if ( !iMedia->IsSupported( KMPXMediaDrmAccumulatedTime ))
                            {
                            GetAccumulatedTimeL();
                            }
                        const TTimeIntervalSeconds& accTime(
                            iMedia->ValueTObjectL<TTimeIntervalSeconds>(
                                KMPXMediaDrmAccumulatedTime ));
                        if ( accTime.Int() < KMPXAboutToExpireAccTime )
                            {
                            aboutToExpire = ETrue;
                            }
                        }   // end if accumulated time type
                    }
                }
            }
        }

    iMedia->SetTObjectValueL( KMPXMediaDrmAboutToExpire,
                              aboutToExpire );

    MPX_DEBUG1("CXMPXDrmMediaOmaAgent::GetAboutToExpireL() exiting");
    }

// end of file
