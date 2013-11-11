/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester image plugin
*
*/


#include <e32base.h>
#include <smsclnt.h>
#include <smuthdr.h>
#include <smsclnt.h>
#include <smut.h>
#include <gsmuelem.h>
#include <msventry.h>
#include <msvuids.h>
#include <mmsconst.h>
#include <mtclreg.h>
#include <mmsclient.h>
#include <mdetextproperty.h>

#include "harvestermessageplugin.h"
#include "harvesterlog.h"
#include "mdsutils.h"
#include "mdeobjectdef.h"

#ifdef _DEBUG
_LIT( KMsgHarvesterMsvPanic, "Message harvester: Message server terminated" );
#endif
_LIT( KSemicolonSpace, "; " );
_LIT( KCharSpace, " " );
_LIT( KCharLeftAddressIterator, "<" );
_LIT( KCharRightAddressIterator, ">" );

/**
* Default constructor
*/
CHarvesterMessagePlugin::CHarvesterMessagePlugin() : CHarvesterPlugin()
	, iItemType(NULL, 0)
	, iSubject(NULL, 0)
	{
	}

/**
* Construction
* @return Harvester image plugin
*/
CHarvesterMessagePlugin* CHarvesterMessagePlugin::NewL()
	{
	WRITELOG("CHarvesterMessagePlugin::NewL()");
	CHarvesterMessagePlugin* self = new(ELeave) CHarvesterMessagePlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

/**
* Destruction
*/
CHarvesterMessagePlugin::~CHarvesterMessagePlugin()
	{
	WRITELOG("CHarvesterMessagePlugin::~CHarvesterMessagePlugin()");
	delete iMmsMtm;
	iMmsMtm = NULL;
	delete iSmsMtm;
	iSmsMtm = NULL;
	delete iMtmReg;
	iMtmReg = NULL;
	delete iMsvSession;
	iMsvSession = NULL;
	}

/**
* 2nd phase constructor
*/
void CHarvesterMessagePlugin::ConstructL()
	{
	WRITELOG("CHarvesterMessagePlugin::ConstructL()");
	if ( iMsvSession )
		{
		delete iMsvSession;
		iMsvSession = NULL;
		}
	iMsvSession = CMsvSession::OpenSyncL( *this ); 
	
	iMtmReg = CClientMtmRegistry::NewL( *iMsvSession );
    iMmsMtm = static_cast<CMmsClientMtm*>
                ( iMtmReg->NewMtmL( KUidMsgTypeMultimedia ) );
	iSmsMtm = static_cast<CSmsClientMtm*>
                ( iMtmReg->NewMtmL( KUidMsgTypeSMS ) );

	}

void CHarvesterMessagePlugin::HarvestL( CHarvesterData* aHD )
	{
	CMdEObject& mdeObject = aHD->MdeObject();
    mdeObject.SetMediaId( 0 );

    TRAPD( error, GatherDataL( mdeObject ) );
    if ( error != KErrNone )
        {
        WRITELOG1( "CHarvesterMessagePlugin::HarvestSingleFileL() - TRAP error: %d", error );
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        return;
        }
    
#ifdef _DEBUG    
    TRAP(error, SetPropertiesL( *aHD ));

    if ( error != KErrNone )
        {
        WRITELOG1( "CHarvesterMessagePlugin::HarvestSingleFileL() - Handling object failed: %d", error );
        }
#else
    TRAP_IGNORE( SetPropertiesL( *aHD ) );
#endif        

	}

// ---------------------------------------------------------------------------
// CHarvesterMessagePlugin::GatherDataL
// ---------------------------------------------------------------------------
//	
TInt CHarvesterMessagePlugin::GatherDataL( CMdEObject& aMetadataObject )
	{
			
    WRITELOG( "CHarvesterMessagePlugin::GatherDataL enter" );
    const TChar KPoint( '.' );
            
	const TDesC& uri = aMetadataObject.Uri();
	const TInt i = uri.Locate( KPoint );
	TMsvId msgId( 0 );	

    TLex lex( uri.Left( i ) );
    lex.Val( msgId );
	WRITELOG1( "msgId: %d", msgId );     	
    
	TMsvId service = 0;
    TMsvEntry tEntry;
	TInt err = iMsvSession->GetEntry( msgId, service, tEntry );    
	User::LeaveIfError( err );
        
	CBaseMtm* actMtm( iSmsMtm ); 
	
	WRITELOG1( "tEntry.iMtm.iUid: %d", tEntry.iMtm.iUid );     	
	
	
	if ( tEntry.iMtm.iUid == KUidMsgTypeMultimedia.iUid )
		{
		WRITELOG( "MMS MESSAGE" );
		actMtm = iMmsMtm;
		}
	actMtm->SwitchCurrentEntryL( msgId );
	actMtm->LoadMessageL();
	
	tEntry = actMtm->Entry().Entry();
	
	WRITELOG1( "tEntry.iMtm.iUid: %d", tEntry.iMtm.iUid );     		
	
	TMsvId parentId = tEntry.Parent();
	
	WRITELOG1( "parentId: %d", parentId );     		
	
	iSize = (TUint)tEntry.iSize;
	
	WRITELOG1( "size: %d", iSize );     			
	
	iDate = tEntry.iDate;
	
#ifdef _DEBUG
	WRITELOG1( "date.DaysInMonth: %d", iDate.DaysInMonth() );
#endif
	
	delete iFromOrTo;
	iFromOrTo = NULL;
	
	if ( KMsvSentEntryId == parentId )	// sent
		{		
		iIncoming = EFalse;
        const CDesCArray& array = static_cast<const CDesCArray&>(
            actMtm->AddresseeList().RecipientList() );
        TInt count( array.Count() );
        TInt maxLength( 0 );
            
        for ( TInt i( 0 ); i < count; ++i )
        	{
        	maxLength += array[i].Length();
        	}
		WRITELOG1( "maxLength: %d", maxLength );     		
        	
		iFromOrTo = HBufC::NewL( maxLength + 3 * count );
		TPtr desReceipients( iFromOrTo->Des() ) ;
		
        for ( TInt i( 0 ); i < count; ++i )
        	{
        	desReceipients.Append( array[i] );
       		desReceipients.Append( KSemicolonSpace );        		
        	}	
        count = desReceipients.Length();
		WRITELOG1("count: %d", count );     		
        
        if ( count )
        	{        	
        	desReceipients.Delete( count - 2, 2 );
        	}
		}
	else	// KMsvGlobalInBoxIndexEntryId received
		{
		iIncoming = ETrue;
		WRITELOG( "Got incoming" );
		
		HBufC* senderNumber = NULL;
		if ( tEntry.iMtm.iUid == KUidMsgTypeMultimedia.iUid )
			{
			senderNumber = iMmsMtm->Sender().AllocLC();
			}
		else
			{
			senderNumber= iSmsMtm->SmsHeader().FromAddress().AllocLC();
			}

        TInt senderLength = senderNumber->Length();
        if ( senderLength > 0 )
            {
            // Check if we have an alias
            if ( senderNumber->Compare( tEntry.iDetails ) )
            	{
                // Additional space for chars: '<' '>' ','
            	senderLength += KCharSpace.iTypeLength +
					KCharLeftAddressIterator.iTypeLength +
					KCharRightAddressIterator.iTypeLength;
            	
                iFromOrTo = HBufC::NewL( tEntry.iDetails.Length() + senderLength );
                TPtr fromTextPtr = iFromOrTo->Des();
                fromTextPtr.Append( tEntry.iDetails );
                fromTextPtr.Append( KCharSpace );
                fromTextPtr.Append( KCharLeftAddressIterator );
                fromTextPtr.Append( *senderNumber );
                fromTextPtr.Append( KCharRightAddressIterator );
            	}
            else
                {
                iFromOrTo = senderNumber->AllocL();
                }
            }
        else
            {
            iFromOrTo = senderNumber->AllocL();
            }
        
        CleanupStack::PopAndDestroy( senderNumber );
		}

	// Set subject - MMS has subject but SMS doesn't have one
	if ( tEntry.iMtm.iUid == KUidMsgTypeMultimedia.iUid )
		{
		iItemType.Set( MdeConstants::Message::KMMSItemType );
		iSubject.Set( iMmsMtm->SubjectL() );
		}
	else
		{
		iItemType.Set( MdeConstants::Message::KSMSItemType );
		iSubject.Set( KNullDesC );
		}
		
	WRITELOG1( "fromOrTo: %S", iFromOrTo );
	WRITELOG1( "incoming: %b", iIncoming );     		
	
	WRITELOG( "CHarvesterMessagePlugin::GatherDataL return" );
	return KErrNone;	
	}

void CHarvesterMessagePlugin::SetPropertiesL( CHarvesterData& aHD )
	{	    
	WRITELOG( "CHarvesterMessagePlugin::SetPropertiesL enter" );
	CMdEObject& metadataObject = aHD.MdeObject();
	
	CMdEObjectDef& messageObjectDef = metadataObject.Def();
	
	CMdEPropertyDef& creationTimeDef = messageObjectDef.GetPropertyDefL( 									  
		MdeConstants::Object::KCreationDateProperty ) ;
	
	CMdEPropertyDef& lastModTimeDef = messageObjectDef.GetPropertyDefL(
        MdeConstants::Object::KLastModifiedDateProperty ) ;									  	

	CMdEPropertyDef& sizeDef =  messageObjectDef.GetPropertyDefL(
        MdeConstants::Object::KSizeProperty ) ;

    CMdEPropertyDef& receivedDef = messageObjectDef.GetPropertyDefL(
        MdeConstants::Message::KReceivedProperty ) ;
	
	CMdEPropertyDef& textDef = iIncoming
        ? messageObjectDef.GetPropertyDefL( MdeConstants::Message::KSenderProperty )
		: messageObjectDef.GetPropertyDefL( MdeConstants::Message::KToWhomProperty );

	CMdEPropertyDef& ItemtypeDef = messageObjectDef.GetPropertyDefL(
			MdeConstants::Object::KItemTypeProperty );
	
	CMdEPropertyDef& offSetDef = messageObjectDef.GetPropertyDefL( 
			MdeConstants::Object::KTimeOffsetProperty );
	
	CMdEPropertyDef& titleDef = messageObjectDef.GetPropertyDefL(
			MdeConstants::Object::KTitleProperty );

	TTimeIntervalSeconds timeOffset = User::UTCOffset();
	TTime now;
	now.HomeTime();
	
	if ( EHarvesterAdd == aHD.EventType() )
		{
	    metadataObject.AddTimePropertyL( creationTimeDef, now );			
	    metadataObject.AddTimePropertyL( lastModTimeDef, iDate );										
	    metadataObject.AddBoolPropertyL( receivedDef, iIncoming );
		if (iFromOrTo && iFromOrTo->Length())
			{
		    metadataObject.AddTextPropertyL( textDef, *iFromOrTo );
			}
		metadataObject.AddTextPropertyL( ItemtypeDef, iItemType );									
		metadataObject.AddUint32PropertyL( sizeDef, iSize );
		metadataObject.AddInt16PropertyL( offSetDef, timeOffset.Int() / 60 );
		
		CMdEProperty* prop = NULL;	
		TInt index = metadataObject.Property( titleDef, prop );
		
		if (iSubject.Length())
			{
			if( prop )
				{
				prop->SetTextValueL( iSubject );
				}
			else
				{
			    metadataObject.AddTextPropertyL( titleDef, iSubject );
				}
			}
		else
			{
			if (iFromOrTo && iFromOrTo->Length())
				{
				if( prop )
					{
					prop->SetTextValueL( *iFromOrTo );
					}
				else
					{
				    metadataObject.AddTextPropertyL( titleDef, *iFromOrTo );
					}
				}
			}
		}
	else
		{	
		CMdEProperty* prop = NULL;	
		TInt index = metadataObject.Property( textDef, prop );
		if (iFromOrTo && iFromOrTo->Length())
			{
			if (prop)
				{
				prop->SetTextValueL( *iFromOrTo );
				}
			else
				{
			    metadataObject.AddTextPropertyL( textDef, *iFromOrTo );
				}
			}
		else if (index >= 0)
			{
		    metadataObject.RemoveProperty(index);
			}
		
		metadataObject.Property( sizeDef, prop );
		static_cast<CMdEUint32Property *>(prop)->SetValueL( iSize );
		
		metadataObject.Property( lastModTimeDef, prop );
		static_cast <CMdETimeProperty *>(prop)->SetValueL( iDate );										
		}

    iStatus = KErrNone;
	WRITELOG( "CHarvesterMessagePlugin::SetPropertiesL return" );
	}

// -----------------------------------------------------------------------------
// CHarvesterMessagePlugin::HandleSessionEventL
// -----------------------------------------------------------------------------
//
void CHarvesterMessagePlugin::HandleSessionEventL( TMsvSessionEvent aEvent, 
	TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/ )
	{	
    WRITELOG( "CHarvesterMessagePlugin::HandleSessionEventL enter" );	
		
	switch ( aEvent )
		{
		case EMsvGeneralError: // not used after v5		
		case EMsvServerFailedToStart:			
		case EMsvServerTerminated:
			{
			__ASSERT_DEBUG( EFalse, User::Panic( KMsgHarvesterMsvPanic, KErrServerTerminated ));
			break;				
			}					
		case EMsvEntriesCreated:
		case EMsvEntriesChanged:
		case EMsvEntriesDeleted:
		case EMsvEntriesMoved:
		case EMsvMtmGroupInstalled:
		case EMsvMtmGroupDeInstalled:
		case EMsvServerReady:
		case EMsvCorruptedIndexRebuilt:
		case EMsvMediaChanged:
		case EMsvMediaUnavailable:
		case EMsvMediaAvailable:
		case EMsvMediaIncorrect:
		case EMsvCorruptedIndexRebuilding:
		case EMsvCloseSession:		
		default:
			{
			break;
			}							
		}	
    WRITELOG( "CHarvesterMessagePlugin::HandleSessionEventL return" );			
	}
