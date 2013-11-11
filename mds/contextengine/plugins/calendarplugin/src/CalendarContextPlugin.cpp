/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
 *
*/


// INCLUDE FILES
#include <mdeobject.h>

#include <calinstance.h>
#include <calinstanceview.h>
#include <calsession.h>
#include <caldataexchange.h>
#include <caldataformat.h>

#include <s32mem.h>

#include "CalendarContextPlugin.h"
#include "harvesterlog.h"
#include "mdeconstants.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "harvesterdata.h"
#include "mdsutils.h"

using namespace MdeConstants;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::NewL
// -----------------------------------------------------------------------------
//
CCalendarContextPlugin* CCalendarContextPlugin::NewL()
	{
	WRITELOG( "CCalendarContextPlugin::NewL -- ENTER" );

	CCalendarContextPlugin* self = new(ELeave) CCalendarContextPlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	WRITELOG( "CCalendarContextPlugin::NewL -- RETURN" );

	return self;
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::CCalendarContextPlugin
// -----------------------------------------------------------------------------
//
CCalendarContextPlugin::CCalendarContextPlugin() :
    iCalInstanceView( NULL ), iMdeSession( NULL ),
    iCalSession( NULL ), iCalInstanceViewCompleted( EFalse ), 
    iRetryCount( 0 )
	{
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::ConstructL() // second-phase constructor
	{
	WRITELOG( "CCalendarContextPlugin::ConstructL -- ENTER" );

	OpenViewL();
	
	WRITELOG( "CCalendarContextPlugin::ConstructL  -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::~CCalendarContextPlugin
// -----------------------------------------------------------------------------
//
CCalendarContextPlugin::~CCalendarContextPlugin() // destruct
	{
	WRITELOG( "CCalendarContextPlugin::~CCalendarContextPlugin -- ENTER" );
	delete iCalInstanceView;
	iCalInstanceView = NULL;
	delete iCalSession;
	iCalSession = NULL;
	
	if( iInitTimer )
	    {
	    iInitTimer->Cancel();
	    delete iInitTimer;
	    iInitTimer = NULL;
	    }
		
	WRITELOG( "CCalendarContextPlugin::~CCalendarContextPlugin -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::InitL
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::Init( MContextPluginObserver& aObserver )
	{
	WRITELOG( "CCalendarContextPlugin::InitL  -- ENTER" );

	// Notification to context framework
	aObserver.PluginInitializationStatus( KErrNone );

	WRITELOG( "CCalendarContextPlugin::InitL -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::ContextSnapshot
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::ContextSnapshot( MContextPluginObserver& aObserver,
		CHarvesterData& aHD )
	{
	WRITELOG( "CCalendarContextPlugin::ContextSnapshotL, 1 obj. --ENTER" );

	CMdEObject& mdeObject = aHD.MdeObject();
	TRAPD( error, AddMetaDataL( mdeObject ) )	;
	// Notification to context framework
	aHD.SetErrorCode( error );
	aObserver.PluginSnapshotStatus( &aHD );

	WRITELOG( "CCalendarContextPlugin::ContextSnapshotL, 1 obj. --RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::ContextSnapshot
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::ContextSnapshot(MContextPluginObserver& aObserver,
		RPointerArray< CHarvesterData >& aHDArray)
	{
#ifdef _DEBUG
	WRITELOG( "CCalendarContextPlugin::ContextSnapshotL, array -- ENTER" );
	WRITELOG1("Size of array: %d", aHDArray.Count() );
#endif
	
	TInt err = KErrNone;
	const TInt count( aHDArray.Count() );
	for( TInt i = 0; i < count; ++i )
		{
		CHarvesterData* hd = aHDArray[i];
		CMdEObject& mdeObject = hd->MdeObject();
		TRAP( err,AddMetaDataL( mdeObject ) );
		hd->SetErrorCode( err );
		// Notification to context framework
		aObserver.PluginSnapshotStatus( hd );
		}

	WRITELOG( "CCalendarContextPlugin::ContextSnapshotL, array -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::GetTimeMdEPropL
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
TTime CCalendarContextPlugin::GetTimeMdEPropL( const TDesC& aNameSpace,
    const TDesC& aProp, CMdEObject& aMdEObject) const
#else
TTime CCalendarContextPlugin::GetTimeMdEPropL( const TDesC&,
    const TDesC& aProp, CMdEObject& aMdEObject) const
#endif
	{
	WRITELOG( "CCalendarContextPlugin::GetTimeMdEPropL -- ENTER" );
	WRITELOG1("aNameSpace: %S", &aNameSpace );
	WRITELOG1("aProp: %S", &aProp );

	CMdEPropertyDef& timeDef = aMdEObject.Def().GetPropertyDefL( aProp );

	CMdEProperty* timeProp = NULL;
	aMdEObject.Property( timeDef, timeProp );

	if ( !timeProp )
		{
		User::Leave( KErrNotFound );
		}

	WRITELOG( "CCalendarContextPlugin::GetTimeMdEPropL  -- RETURN" );

	return timeProp->TimeValueL();
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::AddMetaDataL
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::AddMetaDataL( CMdEObject& aMdEObject ) const
	{
	if ( aMdEObject.Id() == KNoId )
		return;

	WRITELOG( "CCalendarContextPlugin::AddMetaDataL -- ENTER" );

	if ( !iCalInstanceViewCompleted )
		{
		// The CalendarInstanceView has not yet reported completion by calling
		// the callback routine, so no calendar information can be looked up.
		WRITELOG( "iCalInstanceViewCompleted==EFalse, returning." );
		WRITELOG( "CCalendarContextPlugin::AddMetaDataL -- RETURN" );
		return;
		}

	RPointerArray<CCalInstance> instances;
	TCleanupItem cleanupItem( MdsUtils::CleanupPtrArray<CCalInstance>, &instances );
	CleanupStack::PushL( cleanupItem );

	// From the mdsAddTime find the start of the day and the end of the day
	// in order to set the time range for the search in the calendar database
	TTime mdsAddTime = GetTimeMdEPropL(
		KMdEDefaultNameSpace, Object::KCreationDateProperty, aMdEObject );

	TDateTime dateTime = mdsAddTime.DateTime();
	dateTime.SetHour( 0 );
	dateTime.SetMinute( 0 );
	dateTime.SetSecond( 0 );
	dateTime.SetMicroSecond( 0 );
	TTime dayBeginTime( dateTime );
	TCalTime startCalTime;
	TCalTime stopCalTime;
	startCalTime.SetTimeLocalL( dayBeginTime );
	stopCalTime.SetTimeLocalL( dayBeginTime + 
			TTimeIntervalDays ( 1 ) - TTimeIntervalMicroSeconds( 1 ) );

	// Get a list of the all instances in the calendar database associated
	// with this day
	iCalInstanceView->FindInstanceL( instances, CalCommon::EIncludeAll,
									 CalCommon::TCalTimeRange( startCalTime, stopCalTime) );

	const TInt instanceCount = instances.Count();

	WRITELOG1( "CCalendarContextPlugin::AddMetaDataL -- instanceCount: %d", instanceCount );

	if( instanceCount == 0 )
	    {
        CleanupStack::PopAndDestroy( &instances );

        WRITELOG( "CCalendarContextPlugin::AddMetaDataL - no calendar items found -- RETURN" );	
        return;
	    }
	
	// required object properties
	WRITELOG( "CCalendarContextPlugin::AddMetaDataL" );
	User::LeaveIfNull( iMdeSession );
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	WRITELOG( "CCalendarContextPlugin::AddMetaDataL" );

	CMdERelationDef& containsRelDef = namespaceDef.GetRelationDefL(
		Relations::KContains );
	CMdEObjectDef& calendarObjectDef = namespaceDef.GetObjectDefL(
		Calendar::KCalendarObject );

	CMdEPropertyDef& creationDef = calendarObjectDef.GetPropertyDefL(
		Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = calendarObjectDef.GetPropertyDefL(
		Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = calendarObjectDef.GetPropertyDefL(
		Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = calendarObjectDef.GetPropertyDefL(
		Object::KItemTypeProperty );
	CMdEPropertyDef& titleDef = calendarObjectDef.GetPropertyDefL(
			Object::KTitleProperty );
	CMdEPropertyDef& offSetDef = calendarObjectDef.GetPropertyDefL( 
			Object::KTimeOffsetProperty );

	// calendar properties
	CMdEPropertyDef& startDef    = calendarObjectDef.GetPropertyDefL(
		Calendar::KStartTimeProperty );
	CMdEPropertyDef& endDef      = calendarObjectDef.GetPropertyDefL(
		Calendar::KEndTimeProperty );
	CMdEPropertyDef& caltypeDef  = calendarObjectDef.GetPropertyDefL(
		Calendar::KCalendarTypeProperty );
	CMdEPropertyDef& locationDef = calendarObjectDef.GetPropertyDefL(
		Calendar::KLocationProperty );
	CMdEPropertyDef& localUidDef = calendarObjectDef.GetPropertyDefL(
			Calendar::KLocalUidProperty );

	TTime curtime( 0 );
	curtime.UniversalTime();

	const TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();
	TTime localTime = curtime + timeOffsetSeconds;

	TInt timeOffset = timeOffsetSeconds.Int() / 60;

	for ( TInt i = 0; i < instanceCount; ++i )
		{
		WRITELOG1( "CCalendarContextPlugin::AddMetaDataL - loop index: %d", i );

		CCalInstance* instance = instances[i];
		CCalEntry& entry = instance->Entry();

		const TDesC& location = entry.LocationL();
		const TDesC& title = entry.SummaryL();
		TTime beginTime = entry.StartTimeL().TimeLocalL();
		TTime endTime = entry.EndTimeL().TimeLocalL();
		TUint32 localUid = entry.LocalUidL();

		CCalEntry::TType type = entry.EntryTypeL();

		// Anniversaries and such are whole day events
		if ( type == CCalEntry::EAnniv || type == CCalEntry::ETodo )
			{
			// Set end time to the end of the day
			endTime += TTimeIntervalDays ( 1 );
			endTime -= TTimeIntervalMicroSeconds( 1 );
			}
		// Other type entry must be currently active
		else if ( ( mdsAddTime < beginTime ) || ( endTime < mdsAddTime ) )
			{
			WRITELOG( "CCalendarContextPlugin::AddMetaDataL - Calendar event not active - SKIP" );
			continue;
			}

		WRITELOG( "CCalendarContextPlugin::AddMetaDataL - Create calendar item" );
		CMdEObject* calendarObject=  NULL;
		calendarObject = iMdeSession->NewObjectLC(
			calendarObjectDef, Object::KAutomaticUri );
		calendarObject->AddTimePropertyL( creationDef, localTime );
		calendarObject->AddTimePropertyL( modifiedDef, curtime );
		calendarObject->AddUint32PropertyL( sizeDef, 0 ); // always zero size for calendar objects
		calendarObject->AddTextPropertyL( itemTypeDef, Calendar::KCalendarItemType );
		calendarObject->AddInt16PropertyL( offSetDef, timeOffset );

		//Calendar properties
		calendarObject->AddTimePropertyL( startDef, beginTime );
		calendarObject->AddTimePropertyL( endDef, endTime );
		calendarObject->AddUint8PropertyL( caltypeDef, type );
		calendarObject->AddUint32PropertyL( localUidDef, localUid );
		if ( location.Length() > 0 )
			{
			calendarObject->AddTextPropertyL( locationDef, location );
			}
		if ( title.Length() > 0 )
			{
			calendarObject->AddTextPropertyL( titleDef, title );
			}

		WRITELOG( "CCalendarContextPlugin::AddMetaDataL - Add calendar item to MDE" );
		TRAPD( addError, iMdeSession->AddObjectL( *calendarObject ) );
		if ( addError != KErrNone )
			{
			WRITELOG1( "CCalendarContextPlugin::AddMetaDataL - Add calendar item to MDE error: %d", addError );
			CleanupStack::PopAndDestroy( calendarObject );
			continue;
			}

		const TItemId calid = calendarObject->Id();
		const TItemId objid = aMdEObject.Id();

		CMdERelation* relationObject = iMdeSession->NewRelationLC(
			containsRelDef, objid, calid, 0 );
		if ( !relationObject )
			{
			WRITELOG( "CCalendarContextPlugin::AddMetaDataL - Couldn't create relation object!" );
			User::Leave( KErrBadHandle );
			}

		WRITELOG( "CCalendarContextPlugin::AddMetaDataL - Add calendar relation to MDE" );
#ifdef _DEBUG
		TRAP( addError, iMdeSession->AddRelationL( *relationObject ) );
		if ( addError != KErrNone)
			{
			WRITELOG1( "CCalendarContextPlugin::AddMetaDataL - Add calendar relation to MDE error: %d", addError );
			}
#else
		TRAP_IGNORE( iMdeSession->AddRelationL( *relationObject ) );	
#endif

		CleanupStack::PopAndDestroy( relationObject );
		CleanupStack::PopAndDestroy( calendarObject );
		}

	CleanupStack::PopAndDestroy( &instances );

	WRITELOG( "CCalendarContextPlugin::AddMetaDataL -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::HandleSessionOpened( CMdESession& /*aSession*/,
		TInt /*aError*/ )
	{
	WRITELOG( "CCalendarContextPlugin::HandleSessionOpened -- ENTER" );
	WRITELOG( "CCalendarContextPlugin::HandleSessionOpened -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::HandleSessionError
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::HandleSessionError( CMdESession& /*aSession*/,
	TInt /*aError*/ )
	{
	WRITELOG( "CCalendarContextPlugin::HandleSessionError -- ENTER" );
	WRITELOG( "CCalendarContextPlugin::HandleSessionError -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::Progress
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::Progress( TInt /*aPercentageCompleted*/ )
	{
	WRITELOG( "CCalendarContextPlugin::Progress -- ENTER" );
	WRITELOG( "CCalendarContextPlugin::Progress -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::Completed
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::Completed( TInt aError )
	{
	WRITELOG( "CCalendarContextPlugin::Completed -- ENTER" );

	if ( aError != KErrNone )
		{
		iCalInstanceViewCompleted = EFalse;
		WRITELOG1( "aError: %d", aError );
		TRAP_IGNORE( StartRetryL() );
		}
	else
		{
		iCalInstanceViewCompleted = ETrue;
		}

	WRITELOG( "CCalendarContextPlugin::Completed -- RETURN" );
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::NotifyProgress
// -----------------------------------------------------------------------------
//
TBool CCalendarContextPlugin::NotifyProgress()
	{
	WRITELOG( "CCalendarContextPlugin::NotifyProgress -- ENTER" );
	WRITELOG( "CCalendarContextPlugin::NotifyProgress -- RETURN" );

	return EFalse;
	}


// -----------------------------------------------------------------------------
// CCalendarContextPlugin::SetMdeSession
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::SetMdeSession( CMdESession& aSession )
	{
	iMdeSession = &aSession;
	}

// -----------------------------------------------------------------------------
// CCalendarContextPlugin::OpenView
// -----------------------------------------------------------------------------
//
void CCalendarContextPlugin::OpenViewL(TBool aRetry)
    {
    WRITELOG( "CCalendarContextPlugin::OpenViewL() ENTER" );
    TInt err(KErrNone);
    
    if(aRetry)
        {
        WRITELOG( "CCalendarContextPlugin::OpenViewL() retry" );
        iInitTimer->Cancel();
        delete iInitTimer;
        iInitTimer = NULL;
        }
    
    if(!iCalSession)
        {
        WRITELOG( "CCalendarContextPlugin::OpenViewL() new calsession");
        TRAP(err, iCalSession = CCalSession::NewL());
        
        if( err != KErrNone)
            {
            WRITELOG1( "CCalendarContextPlugin::OpenViewL() CCalSession::NewL() err = %d", err );
            iCalSession = NULL;
            StartRetryL();
            return;
            }
        }

    WRITELOG( "CCalendarContextPlugin::OpenViewL() open calsession");
    TRAP(err, iCalSession->OpenL( KNullDesC ));

    if(err != KErrNone )
        {
        WRITELOG1( "CCalendarContextPlugin::OpenViewL() iCalSession->OpenL() err = %d", err );
        StartRetryL();
        return;
        }

    WRITELOG( "CCalendarContextPlugin::OpenViewL() open view");
    TRAP( err, iCalInstanceView = CCalInstanceView::NewL( *iCalSession, *this ) );
        
    if(err != KErrNone )
        {
        WRITELOG1( "CCalendarContextPlugin::OpenViewL() CCalInstanceView::NewL() err = %d", err );
        iCalInstanceView = NULL;
        StartRetryL();
        return;
        }

    WRITELOG( "CCalendarContextPlugin::OpenViewL() RETURN" );
    }


void CCalendarContextPlugin::StartRetryL()
    {
    WRITELOG( "CCalendarContextPlugin::StartRetryL() ENTER" );
    
    if(iRetryCount++ > KMaxRetryCount)
        {
        delete iCalInstanceView;
        iCalInstanceView = NULL;
        delete iCalSession;
        iCalSession = NULL;
        
        if( iInitTimer )
            {
            iInitTimer->Cancel();
            delete iInitTimer;
            iInitTimer = NULL;
            }
        
		return; 
        }
    
#ifdef _DEBUG
    TInt err( KErrNone );
    if(!iInitTimer)
        {
        TRAP(err, iInitTimer = CPeriodic::NewL( CActive::EPriorityStandard ) );
        }
                
    if( err != KErrNone )
        {
        WRITELOG1( "CCalendarContextPlugin::OpenViewL() iInitTimer->Start failed %d", err );
        User::Leave( err );
        }
#else
    if(!iInitTimer)
        {
        iInitTimer = CPeriodic::NewL( CActive::EPriorityStandard );
        }    
#endif
    
    WRITELOG( "CCalendarContextPlugin::OpenViewL() iInitTimer->Start" );
    iInitTimer->Start( KInitRetryTimeout, KInitRetryTimeout, TCallBack(InitRetryCallback, this));
    WRITELOG( "CCalendarContextPlugin::StartRetryL() RETURN" );
    }
	
// ---------------------------------------------------------------------------
// CCalendarContextPlugin::InitRetryCallback()
// ---------------------------------------------------------------------------
//
TInt CCalendarContextPlugin::InitRetryCallback(TAny* aAny)
    {
    WRITELOG( "CCalendarContextPlugin::InitRetryCallback() ENTER");
    CCalendarContextPlugin* self = static_cast<CCalendarContextPlugin*>( aAny );

    TRAP_IGNORE( self->OpenViewL(ETrue) );
    
    WRITELOG( "CCalendarContextPlugin::InitRetryCallback() RETURN");
    return KErrNone;
    }
	
