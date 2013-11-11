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
* Description:  EXPORT_C implementation of drm media WMA agent
*
*/


#include <caf/rightsinfo.h>
#include "mpxmedia.h"
#include "mpxmediadrmdefs.h"
#include "mpxdrmmediawmaagent.h"
#include "mpxlog.h"

// CONSTANTS
_LIT(KMPXWmaDrmDelLicensesType, "licensetype=");
_LIT(KMPXWmaDrmDelUnlimited, "unlimited");
_LIT(KMPXWmaDrmDelTime, "time");
_LIT(KMPXWmaDrmDelCount, "count");
_LIT(KMPXWmaDrmDelDuration, "duration");
_LIT(KMPXWmaDrmDelTimeCount, "time-count");
_LIT(KMPXWmaDrmDelDurationLeft, "duration=");
_LIT(KMPXWmaDrmDelCountLeft, "countleft=");
_LIT(KMPXWmaDrmDelStartTime, "starttime=");
_LIT(KMPXWmaDrmDelEndTime, "endtime=");
_LIT(KMPXWmaDrmDelColon, ":");
_LIT(KMPXWmaDrmDelSemiColon, ";");
_LIT(KMPXWmaDrmTimeFill, "0");

const TInt KMPXWmaDrmMaxLicenseTypeBufSize = 30;
const TInt KMPXWmaDrmDashCount = 2;
const TInt KMPXWmaDrmMaxTimeBufSize = 3;
const TInt KMPXWmaDrmMinutesHourMinSize = 2;

const TText KMPXWmaDrmSemiColonChar = ';';
const TText KMPXWmaDrmDashChar = '-';
const TText KMPXWmaDrmTChar = 'T';
const TText KMPXWmaDrmColonChar = ':';
const TText KMPXWmaDrmPeriodChar = '.';
const TText KMPXWmaDrmZChar = 'Z';
const TText KMPXWmaDrmPlusChar = '+';


// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXDrmMediaWmaAgent* CMPXDrmMediaWmaAgent::NewL()
    {
    MPX_FUNC("CMPXDrmMediaWmaAgent::NewL()");
    CMPXDrmMediaWmaAgent* p = CMPXDrmMediaWmaAgent::NewLC();
    CleanupStack::Pop(p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXDrmMediaWmaAgent* CMPXDrmMediaWmaAgent::NewLC()
    {
    MPX_FUNC("CMPXDrmMediaWmaAgent::NewLC()");
    CMPXDrmMediaWmaAgent* p = new (ELeave) CMPXDrmMediaWmaAgent();
    CleanupStack::PushL(p);
    p->ConstructL();
    return p;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXDrmMediaWmaAgent::~CMPXDrmMediaWmaAgent()
    {
    MPX_FUNC("CMPXDrmMediaWmaAgent::~CMPXDrmMediaWmaAgent()");
    Close();
    delete iRightsManager;
    delete iManager;
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CMPXDrmMediaWmaAgent::CMPXDrmMediaWmaAgent()
    {
    MPX_FUNC("CMPXDrmMediaWmaAgent::CMPXDrmMediaWmaAgent()");
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::ConstructL()
    {
    MPX_FUNC("CMPXDrmMediaWmaAgent::ConstructL()");

    RArray< TAgent > agents;
    CleanupClosePushL( agents );

    iManager = CManager::NewL();
    iManager->ListAgentsL( agents );

    if ( agents.Count() > 0)
        {
        TInt i( 0 );
        for ( i = 0; i < agents.Count(); i++ )
            {
            // break if we find the agent we want
            // CAF UID
            const TDesC& name( agents[i].Name() );
            if ( name.Compare( KMPXWDRMCafAgentName ) == 0 ||
                 name.Compare( KMPXS60WMDRMCafAgent ) == 0 )
                {
                break;
                }
            }
        if ( i < agents.Count() )
            {
            // We found the CAF agent
            MPX_DEBUG2("CMPXDrmMediaWmaAgent::ConstructL(): CAF agent=%d", i);
            iRightsManager = iManager->CreateRightsManagerL( agents[i] );
            }
        }

    CleanupStack::PopAndDestroy( &agents );
    }


// ----------------------------------------------------------------------------
// Initialize utility using a filename
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::InitL( const TDesC& aFileName )
    {
    MPX_DEBUG2("CMPXDrmMediaWmaAgent::InitL(%S) entering", &aFileName );
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::InitL(aFileName)");

    Close();
    CreateMediaL();
    iData = CData::NewL(
        (TVirtualPathPtr) aFileName,
        EPeek,
        EContentShareReadOnly );
    iFileName = aFileName.AllocL();

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::InitL() exiting" );
    }

// ----------------------------------------------------------------------------
// Initialize utility using a file handle
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::InitL( RFile& aFile )
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::InitL(RFile) entering" );
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::InitL(aFile)");

    Close();
    CreateMediaL();
    iData = CData::NewL(
        aFile,
        KDefaultContentObject(),
        EPeek );
    TFileName filename;
    aFile.FullName( filename );
    iFileName = filename.AllocL();
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::InitL(RFile) exiting" );
    }

// ----------------------------------------------------------------------------
// Gets media object with the given attributes
// ----------------------------------------------------------------------------
//
const CMPXMedia& CMPXDrmMediaWmaAgent::GetMediaL( TUint aAttributes )
    {
    MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetMediaL(%d) entering", aAttributes );
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::GetRightsTypeL()");

    if ( iMedia )
        {
        if ( aAttributes & KMPXMediaDrmProtected.iAttributeId )
            {
            GetProtectedL();
            }
        if ( aAttributes & KMPXMediaDrmRightsStatus.iAttributeId )
            {
            GetRightsStatusL();
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
            // Not Supported for WMA DRM
            }
        if ( aAttributes & KMPXMediaDrmAccumulatedTime.iAttributeId )
            {
            // Not Supported for WMA DRM
            }
        if ( aAttributes & KMPXMediaDrmSendingAllowed.iAttributeId )
            {
            GetSendingAllowedL();
            }
        if ( aAttributes & KMPXMediaDrmCanSetAutomated.iAttributeId )
            {
            GetCanSetAutomatedL();
            }
        if ( aAttributes & KMPXMediaDrmHasInfoUrl.iAttributeId )
            {
            // Not Supported for WMA DRM
            }
        if ( aAttributes & KMPXMediaDrmHasPreviewUrl.iAttributeId )
            {
            // Not Supported for WMA DRM
            }
        if ( aAttributes & KMPXMediaDrmAboutToExpire.iAttributeId )
            {
            GetAboutToExpireL();
            }
        }

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetMediaL() exiting" );
    return *iMedia;
    }

// ----------------------------------------------------------------------------
// Consumes the rights for the current media according
// to the specified consume type
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::ConsumeL(TDrmConsumeType /*aType*/)
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::ConsumeL() entering");

    // WMA DRM does not have ability to manually control consumption

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::ConsumeL() exiting" );
    }

// ----------------------------------------------------------------------------
// Cleans up member variables
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::Close()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::Close() entering");

    CMPXDrmMediaAgent::Close();
    delete iFileName;
    iFileName = NULL;
    delete iRightsDescription;
    iRightsDescription = NULL;

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::Close() exiting");
    }

// ----------------------------------------------------------------------------
// Creates a new media object
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::CreateMediaL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::CreateMediaL() entering");

    CMPXDrmMediaAgent::CreateMediaL();

    // Set the type to WMA
    iMedia->SetTObjectValueL( KMPXMediaDrmType,
                              EMPXDrmTypeWMA );

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::CreateMediaL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the rights status
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetRightsStatusL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsStatusL() entering");
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::GetRightsStatusL()");

    if ( !iMedia->IsSupported( KMPXMediaDrmRightsStatus ))
        {
        // Check to make sure it's protected first
        if ( !iMedia->IsSupported( KMPXMediaDrmProtected ))
            {
            GetProtectedL();
            }
        TBool prot( iMedia->ValueTObjectL<TBool>( KMPXMediaDrmProtected ));
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetRightsStatusL(): prot = %d", prot);
        TInt rightsStatus( EMPXDrmRightsFull );
        if ( prot )
            {
            TBool canPlay( ETrue );
#ifndef _DEBUG
            User::LeaveIfError( iData->GetAttribute( ECanPlay, canPlay ));
#else
            MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsStatusL(): Getting ECanPlay");
            TInt playErr( iData->GetAttribute( ECanPlay, canPlay ));
            MPX_DEBUG3("CMPXDrmMediaWmaAgent::GetRightsStatusL(): err = %d, canPlay = %d", playErr, canPlay );
            User::LeaveIfError( playErr );
#endif
            if ( !canPlay )
                {
                rightsStatus = EMPXDrmRightsExpired;
                }
            else
                {
                TBool rightsStateless( ETrue );
                TBool rightsConsumable( EFalse );
#ifndef _DEBUG
                User::LeaveIfError( iData->GetAttribute(
                                        ERightsStateless,
                                        rightsStateless ));
                User::LeaveIfError( iData->GetAttribute(
                                        ERightsConsumable,
                                        rightsConsumable ));
#else
                MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsStatusL(): Getting ERightsStateless");
                TInt statusErr( iData->GetAttribute(
                                    ERightsStateless,
                                    rightsStateless ));
                MPX_DEBUG3("CMPXDrmMediaWmaAgent::GetRightsStatusL(): err = %d, rightsStateless = %d", statusErr, rightsStateless );
                User::LeaveIfError( statusErr );
                MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsStatusL(): Getting ERightsConsumable");
                TInt conumeableErr( iData->GetAttribute(
                                        ERightsConsumable,
                                        rightsConsumable ));
                MPX_DEBUG3("CMPXDrmMediaWmaAgent::GetRightsStatusL(): err = %d, rightsConsumable = %d", conumeableErr, rightsConsumable );
                User::LeaveIfError( conumeableErr );
#endif
                if ( !rightsStateless || rightsConsumable )
                    {
                    rightsStatus = EMPXDrmRightsRestricted;
                    }
                else
                    {
                    if ( !iMedia->IsSupported( KMPXMediaDrmRightsType ))
                        {
                        GetRightsTypeL();
                        }
                    TInt rightsType( iMedia->ValueTObjectL<TInt>(
                        KMPXMediaDrmRightsType ));
                    if ( EMPXDrmRightsTypeNoRestriction == rightsType )
                        {
                        rightsStatus = EMPXDrmRightsFull;
                        }
                    else
                        {
                        rightsStatus = EMPXDrmRightsRestricted;
                        }
                    }
                }
            }
        iMedia->SetTObjectValueL( KMPXMediaDrmRightsStatus,
                                  rightsStatus );
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetRightsStatusL(): setting status = %d", rightsStatus);
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsStatusL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the rights type attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetRightsTypeL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsTypeL() entering");
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::GetRightsTypeL()");

    if ( !iMedia->IsSupported( KMPXMediaDrmRightsType ))
        {
        TInt rightsType( EMPXDrmRightsTypeNoRestriction );

        if ( GetWmdrmRightsDescriptionL() == KErrNone )
            {
            TPtr strPtr( iRightsDescription->Des() );
            TInt end( strPtr.FindF( KMPXWmaDrmDelSemiColon ));
            HBufC* licensetype( HBufC::NewLC( KMPXWmaDrmMaxLicenseTypeBufSize ));
            TPtr licensetypePtr( licensetype->Des());
            licensetypePtr = strPtr.Mid(
                                KMPXWmaDrmDelLicensesType().Length(),
                                end - KMPXWmaDrmDelLicensesType().Length() );

            if ( licensetypePtr == KMPXWmaDrmDelUnlimited )
                {
                rightsType = EMPXDrmRightsTypeNoRestriction;
                }
            else if ( licensetypePtr == KMPXWmaDrmDelTime )
                {
                rightsType = EMPXDrmRightsTypeTime;
                }
            else if ( licensetypePtr == KMPXWmaDrmDelCount )
                {
                rightsType = EMPXDrmRightsTypeCount;
                }
            else if ( licensetypePtr == KMPXWmaDrmDelDuration )
                {
                rightsType = EMPXDrmRightsTypeInterval;
                }
            else if ( licensetypePtr == KMPXWmaDrmDelTimeCount )
                {
                rightsType = EMPXDrmRightsTypeTimeCount;
                }
            CleanupStack::PopAndDestroy( licensetype );
            }

        iMedia->SetTObjectValueL( KMPXMediaDrmRightsType,
                                  rightsType );
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetRightsTypeL() setting type = %d", rightsType);
        }

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetRightsTypeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the count attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetCountL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetCountL() entering");

    // Do not need to check if it already supported, we
    // just overwrite the old value if possible
    TUint32 count( 0 );
    if ( GetWmdrmRightsDescriptionL() == KErrNone )
        {
        TPtr strPtr( iRightsDescription->Des() );
        if ( strPtr.FindF( KMPXWmaDrmDelLicensesType ) == KErrNone )
            {
            TInt begin( strPtr.FindF( KMPXWmaDrmDelCountLeft ));
            if ( begin != KErrNotFound )
                {
                TPtrC countPtr( strPtr.Mid( begin ));
                TInt semiLoc( countPtr.Locate( KMPXWmaDrmSemiColonChar ));
                TInt countLen( KMPXWmaDrmDelCountLeft().Length() );
                TLex lex( strPtr.MidTPtr( begin + countLen, semiLoc - countLen ) );
                User::LeaveIfError( lex.Val( count, EDecimal ) );
                iMedia->SetTObjectValueL( KMPXMediaDrmCount,
                                          count );
                MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetCountL(): setting count = %d", count);
                }
            }
        }

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetCountL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the start time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetStartTimeL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetStartTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmStartTime ))
        {
        TTime startTime;
        if ( GetWmdrmTimeL( KMPXWmaDrmDelStartTime, startTime ) ==
             KErrNone )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmStartTime,
                                      startTime.Int64() );
            MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetStartTimeL(): setting start time");
            }
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetStartTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the end time attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetEndTimeL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetEndTimeL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmEndTime ))
        {
        TTime endTime;
        if ( GetWmdrmTimeL( KMPXWmaDrmDelEndTime, endTime ) ==
             KErrNone )
            {
            iMedia->SetTObjectValueL( KMPXMediaDrmEndTime,
                                      endTime.Int64() );
            MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetEndTimeL(): setting end time");
            }
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetEndTimeL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the interval attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetIntervalL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetIntervalL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmInterval ))
        {
        if ( GetWmdrmRightsDescriptionL() == KErrNone )
            {
            TPtr strPtr( iRightsDescription->Des() );
            if ( strPtr.FindF( KMPXWmaDrmDelLicensesType ) == KErrNone )
                {
                TInt begin = strPtr.FindF( KMPXWmaDrmDelDurationLeft );
                if ( begin != KErrNotFound )
                    {
                    TPtrC countPtr = strPtr.Mid( begin );
                    TInt semiLoc = countPtr.Locate( KMPXWmaDrmSemiColonChar );
                    TInt countLen = KMPXWmaDrmDelDurationLeft().Length();
                    TLex lex( strPtr.MidTPtr( begin + countLen, semiLoc - countLen ) );
                    TInt interval;
                    User::LeaveIfError( lex.Val( interval ) );
                    TTimeIntervalSeconds intervalSeconds( interval );
                    iMedia->SetTObjectValueL( KMPXMediaDrmInterval,
                                              intervalSeconds );
                    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetIntervalL(): setting interval");
                    }
                }
            }
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetIntervalL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the sending allowed attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetSendingAllowedL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetSendingAllowedL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmSendingAllowed ))
        {
        TBool sendingAllowed( ETrue );
#ifndef _DEBUG
        User::LeaveIfError(
            iData->GetAttribute( EIsForwardable, sendingAllowed ));
#else
        TInt sendErr( iData->GetAttribute( EIsForwardable, sendingAllowed ));
        MPX_DEBUG3("CMPXDrmMediaWmaAgent::GetSendingAllowedL(): err = %d, sendingAllowed = %d", sendErr, sendingAllowed);
        User::LeaveIfError( sendErr );
#endif
        iMedia->SetTObjectValueL( KMPXMediaDrmSendingAllowed,
                                  sendingAllowed );
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetSendingAllowedL(): setting sending allowed = %d", sendingAllowed);
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetSendingAllowedL() exiting");
    }

// ----------------------------------------------------------------------------
// Gets the can set automated attribute if not already obtained
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetCanSetAutomatedL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetCanSetAutomatedL() entering");

    if ( !iMedia->IsSupported( KMPXMediaDrmCanSetAutomated ))
        {
        TBool automated( ETrue );
        // For WMA DRM files, they can only be set automated if not protected
        TBool prot( EFalse );
        if ( !iMedia->IsSupported( KMPXMediaDrmProtected ))
            {
            GetProtectedL();
            }
        prot = iMedia->ValueTObjectL<TBool>( KMPXMediaDrmProtected );
        if ( prot )
            {
            automated = EFalse;
            }

        iMedia->SetTObjectValueL( KMPXMediaDrmCanSetAutomated,
                                  automated );
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetCanSetAutomatedL(): setting automated = %d", automated);
        }
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetCanSetAutomatedL() exiting");
    }

// -----------------------------------------------------------------------------
// Get Rights Description String for WMDRM protected files
// -----------------------------------------------------------------------------
//
TInt CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL() entering");
    MPX_DEBUG_THREAD("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL()");
    TInt error( KErrNotFound );

    if ( iRightsDescription )
        {
        error = KErrNone;
        }
    else
        {
        // Currently software is that if no rights available, getting
        // rights string will panic PVWMCaf server.  To be removed
        // if fixed in PV server later
        TBool canPlay( ETrue );
#ifndef _DEBUG
        User::LeaveIfError( iData->GetAttribute( ECanPlay, canPlay ));
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL(): canPlay = %d", canPlay);
#else
        TInt playErr( iData->GetAttribute( ECanPlay, canPlay ));
        MPX_DEBUG3("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL(): err = %d, canPlay = %d", playErr, canPlay );
        User::LeaveIfError( playErr );
#endif
        if ( iRightsManager && canPlay )
            {
            MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL(): iFileName = %S", iFileName);
            RStreamablePtrArray<ContentAccess::CRightsInfo> rightsArray;
            CleanupClosePushL( rightsArray );
            TRAPD( err, iRightsManager->ListRightsL( rightsArray, *iFileName ));
            if ( KErrNone == err )
                {
                if ( rightsArray.Count() > 0 )
                    {
                    ContentAccess::CRightsInfo* rightInfo = rightsArray[0];
                    iRightsDescription = rightInfo->Description().AllocL();
                    error = KErrNone;
                    }
                }
            CleanupStack::PopAndDestroy( &rightsArray );
            }
        }
#ifdef _DEBUG
    if (!error)
        {
        MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL(): %S", iRightsDescription);
        }
#endif
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetWmdrmRightsDescriptionL() exiting");
    return error;
    }

// -----------------------------------------------------------------------------
// CMPXDrmMediaWmaAgent::GetWmdrmTimeL
// -----------------------------------------------------------------------------
//
TInt CMPXDrmMediaWmaAgent::GetWmdrmTimeL(
    const TDesC& aTimeTypeDelimeter,
    TTime& aTime )
    {
    MPX_DEBUG2("CMPXDrmMediaWmaAgent::GetWmdrmTimeL(%S) entering", &aTimeTypeDelimeter);
    TInt err( KErrNone );

    if ( GetWmdrmRightsDescriptionL() == KErrNone )
        {
        TPtr strPtr( iRightsDescription->Des() );
        err = KErrNotFound;
        if ( strPtr.FindF( KMPXWmaDrmDelLicensesType ) == KErrNone )
            {
            TInt begin = strPtr.FindF( aTimeTypeDelimeter );
            if ( begin != KErrNotFound )
                {
                err = KErrNone;
                TPtrC countPtr = strPtr.Mid( begin );
                TInt semiLoc = countPtr.Locate( KMPXWmaDrmSemiColonChar );
                TPtrC timePtr = strPtr.Mid( begin, semiLoc );

                TInt finddash = timePtr.Locate( KMPXWmaDrmDashChar );
                TInt dashcount = 0;
                while ( finddash != KErrNotFound && dashcount < KMPXWmaDrmDashCount )
                    {
                    // after the first dash, subtract 1 from both day and time
                    // as Jan 01 is represented by 00, 00 in TTime.
                    ++dashcount;
                    // convert buffer to TInt and decrement
                    TLex lex( timePtr.Mid( finddash + 1, KMPXWmaDrmDashCount ) );
                    TInt dateInt;
                    User::LeaveIfError( lex.Val( dateInt ) );
                    --dateInt;
                    // convert back to buffer
                    // coverity[size_error]
                    // coverity[buffer_alloc]
                    HBufC* datebuf = HBufC::NewLC( KMPXWmaDrmMaxTimeBufSize );
                    TPtr datebufPtr = datebuf->Des();

                    datebufPtr.Num( dateInt );
                    if ( datebufPtr.Length() < KMPXWmaDrmMinutesHourMinSize )
                        {
                        datebufPtr.Insert( 0, KMPXWmaDrmTimeFill );
                        }
                    strPtr.Replace( ( begin +  finddash + 1 ),
                                    KMPXWmaDrmMinutesHourMinSize, datebufPtr );
                    strPtr.Delete( ( begin  +  finddash ), 1 );
                    finddash = timePtr.Locate( KMPXWmaDrmDashChar );
                    CleanupStack::PopAndDestroy( datebuf );
                    }

                // Format time to correspond TTime time
                TInt findTdelim = timePtr.Locate( KMPXWmaDrmTChar );
                if ( findTdelim != KErrNone )
                    {
                    TInt findcolon = timePtr.Locate( KMPXWmaDrmColonChar );
                    while ( findcolon != KErrNotFound )
                        {
                        strPtr.Delete( ( begin +  findcolon ), 1 );
                        findcolon = timePtr.Locate( KMPXWmaDrmColonChar );
                        }
                    }

                strPtr.Replace( ( begin  +  findTdelim), 1, KMPXWmaDrmDelColon );

                // have removed desired characters from time, now isolate them.
                TInt end = timePtr.Locate( KMPXWmaDrmPeriodChar );
                if ( end == KErrNotFound )
                    {
                    // no milliseconds, look for time zone indicator +, -, or Z
                    end = timePtr.Locate( KMPXWmaDrmZChar );
                    if ( end == KErrNotFound )
                        {
                        end = timePtr.Locate( KMPXWmaDrmDashChar );
                        if ( end == KErrNotFound )
                            {
                            end = timePtr.Locate( KMPXWmaDrmPlusChar );
                            }
                        }
                    }
                  TInt delimLen = aTimeTypeDelimeter.Length();
                  aTime.Set( strPtr.MidTPtr( begin + delimLen ,  end - delimLen ) );
                }
            }
        }
    else
        {
        err = KErrNotFound;
        }

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetWmdrmTimeL() exiting");
    return err;
    }

// ----------------------------------------------------------------------------
// Gets whether the DRM object is about to expire or not
// ----------------------------------------------------------------------------
//
void CMPXDrmMediaWmaAgent::GetAboutToExpireL()
    {
    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetAboutToExpireL() entering");

    TBool aboutToExpire( EFalse );

    // Currently WMDRM does not use about to expire

    iMedia->SetTObjectValueL( KMPXMediaDrmAboutToExpire,
                              aboutToExpire );

    MPX_DEBUG1("CMPXDrmMediaWmaAgent::GetAboutToExpireL() exiting");
    }

// end of file
