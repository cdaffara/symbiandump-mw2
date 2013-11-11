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
* Description:  Composer image active object
*
*/


#include <e32base.h>
#include <e32debug.h>

#include <mderelation.h>
#include <mderelationquery.h>
#include <mdelogiccondition.h>

#include "mdeharvestersession.h"
#include "mdeproperty.h"
#include "mdenamespacedef.h"
#include "imagecomposerao.h"
#include "harvesterlog.h"
#include "mdeconstants.h"
#include "mdsutils.h"
#include "mderelationcondition.h"
#include "mdeobjectdef.h"
#include "harvestercommon.h"

using namespace MdeConstants;

_LIT( KJpegMimeType, "image/jpeg" );

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CImageComposerAO* CImageComposerAO::NewL()
    {
    WRITELOG( "CImageComposerAO::NewL()" );

    CImageComposerAO* self = new (ELeave) CImageComposerAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ~CImageComposerAO
// ---------------------------------------------------------------------------
//
CImageComposerAO::~CImageComposerAO() // destruct
    {   
    WRITELOG( "CImageComposerAO::~CImageComposerAO()" );
    
    Cancel();
    
    iItemQueue.Close();
    
    iForceObjectIds.Close();
    iNextItemsSkip.Close();
    
    if ( iMdeObject )
        {
        delete iMdeObject;
        iMdeObject = NULL;
        }
    if ( iExifUtil )
    	{
    	delete iExifUtil;
    	iExifUtil = NULL;
    	}

    delete iRelationQuery;
    iRelationQuery = NULL;
    iFs.Close();
    
    delete iMdEHarvesterSession;
    iMdEHarvesterSession = NULL;
    }

// ---------------------------------------------------------------------------
// CComposerImagePlugin
// ---------------------------------------------------------------------------
//
CImageComposerAO::CImageComposerAO() : // first-phase C++ constructor
		CActive( KHarvesterPriorityComposerPlugin )
    {
    WRITELOG( "CImageComposerAO::CImageComposerAO()" );
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::ConstructL() // second-phase constructor
    {
    WRITELOG( "CImageComposerAO::ConstructL()" );
    iMdeObject = NULL;
    iNextRequest = ERequestReady;
    iDefaultNamespace = NULL;
    iImageObjectDef = NULL;
    iObjectDef = NULL;
    iLocationObjectDef = NULL;

    CActiveScheduler::Add( this );
    
    iExifUtil = CHarvesterExifUtil::NewL();
    iFastModeEnabled = EFalse;
    User::LeaveIfError( iFs.Connect() );
    }
    
// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CImageComposerAO::DoCancel()
    {
    WRITELOG( "CImageComposerAO::DoCancel()" );
    }
    
// ---------------------------------------------------------------------------
// AddToQueue
// ---------------------------------------------------------------------------
//
void CImageComposerAO::AddToQueue( const RArray<TItemId>& aItems, TBool aForce )
    {
    WRITELOG( "CImageComposerAO::AddToQueue()" );
    
    // check if we should skip some items
    const TInt itemsCount = aItems.Count();
    for ( TInt i = 0; i < itemsCount; ++i )
        {
        const TInt res = iNextItemsSkip.FindInOrder( aItems[i],
        		TLinearOrder<TItemId>( CImageComposerAO::CompareTItemIds ) );
        if ( res != KErrNotFound && res >= 0 )
            {
            RArray<TItemId> objectId;
            objectId.Append( aItems[i] );
            TRAP_IGNORE( iMdEHarvesterSession->ResetPendingL( objectId ) );
            iNextItemsSkip.Remove( res );
            
            if( iNextItemsSkip.Count() == 0 )
            	{
            	iNextItemsSkip.Compress();
            	}
            objectId.Close();
            }
        else
            {
            iItemQueue.Append( aItems[i] );
            if ( aForce )
            	{
            	iForceObjectIds.Append( aItems[i] );
            	}
            }
        }
    if ( iNextRequest == ERequestReady )
    	{
    	SetNextRequest( ERequestGetObject );
    	}
    }
    
// ---------------------------------------------------------------------------
// IsComposingComplete()
// ---------------------------------------------------------------------------
//
TBool CImageComposerAO::IsComposingComplete()
    {
    WRITELOG( "CImageComposerAO::IsComposingComplete()" );
    return iNextRequest == ERequestReady;
    }

// ---------------------------------------------------------------------------
// SetSession
// ---------------------------------------------------------------------------
//
void CImageComposerAO::SetSession( CMdESession* aSession )
    {
    WRITELOG( "CImageComposerAO::SetSession()" );
    iSession = aSession;
    iExifUtil->SetSession(iSession);
    if( iSession )
    	{
    	iDefaultNamespace = NULL;
    	TRAP_IGNORE( iDefaultNamespace = &iSession->GetDefaultNamespaceDefL() );
    	
    	TRAP_IGNORE( iImageObjectDef = &iDefaultNamespace->GetObjectDefL( 
    			Image::KImageObject ) );
    	TRAP_IGNORE( iObjectDef = &iDefaultNamespace->GetObjectDefL( 
    			Object::KBaseObject ) );
    	TRAP_IGNORE( iLocationObjectDef = &iDefaultNamespace->GetObjectDefL( 
    			Location::KLocationObject ) );
    	
    	
		iMdEHarvesterSession = NULL;
    	TRAP_IGNORE( iMdEHarvesterSession 
    	        = CMdEHarvesterSession::NewL ( *iSession ));
    	}
    }

// ---------------------------------------------------------------------------
// RemoveSession
// ---------------------------------------------------------------------------
//
void CImageComposerAO::RemoveSession()
    {
    iSession = NULL;
    iExifUtil->SetSession( NULL );

    delete iMdEHarvesterSession;
    iMdEHarvesterSession = NULL;
    
    iDefaultNamespace = NULL;
    iImageObjectDef = NULL;
    iObjectDef = NULL;
    iLocationObjectDef = NULL;
    }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::RunL()
    {
    WRITELOG( "CImageComposerAO::RunL()" );
    User::LeaveIfError( iStatus.Int() );
    
    if ( !iDefaultNamespace && iSession )
        {
        iDefaultNamespace = &iSession->GetDefaultNamespaceDefL();
        }
    
    switch ( iNextRequest )
        {
        case ERequestGetObject:
            {
            if( iItemQueue.Count() <= 0 )
            	{
                iItemQueue.Compress();
                if( iFastModeEnabled )
                    {
                    SetPriority( KHarvesterPriorityComposerPlugin );
                    }
            	SetNextRequest( ERequestReady );
            	}
            else
            	{
            	TItemId mdeObjectId = KNoId;
	            TRAPD( err, GetObjectFromMdeL( mdeObjectId ) );
	            
	            if ( err == KErrNone )
	                {    
	                CMdEProperty* prop = NULL;
	                CMdEPropertyDef& originPropDef = iMdeObject->Def().GetPropertyDefL( Object::KOriginProperty );
	                iMdeObject->Property( originPropDef, prop );
	                if( prop && prop->Uint8ValueL() == MdeConstants::Object::ECamera && !iFastModeEnabled )
	                    {
	                    iFastModeEnabled = ETrue;
	                    SetPriority( KHarvesterPriorityMonitorPlugin );
	                    }
	                else if( iFastModeEnabled )
	                    {
	                    iFastModeEnabled = EFalse;
	                    SetPriority( KHarvesterPriorityComposerPlugin );
	                    }
	                
	                SetNextRequest( ERequestCompose );
	                }
	            // if object does not exists, or data is not modified, find next
	            else if ( err == KErrNotFound || err == KErrAbort )
	                 {
	                 if ( err == KErrAbort && mdeObjectId != KNoId )
	                	 {
		                 RArray<TItemId> objectId;
		                 objectId.Append( mdeObjectId );
		                 CleanupClosePushL( objectId );
		                 iMdEHarvesterSession->ResetPendingL( objectId );
		                 CleanupStack::PopAndDestroy( &objectId );
	                	 }
	                 SetNextRequest( ERequestGetObject );
	                 }
	                 
	            // something goes really wrong
	            else
	                 {
	                 User::Leave( err );
	                 }
            	}

            }
            break;
            
        case ERequestCompose:
            {
            ComposeL();

            if ( iMdeObject )
                {
                RArray<TItemId> objectId;
                objectId.Append( iMdeObject->Id() );
                TRAP_IGNORE( iMdEHarvesterSession->ResetPendingL( objectId ) );
                objectId.Close();

                delete iMdeObject;
                iMdeObject = NULL;
                }
            }
            break;
            
        case ERequestReady:
            {
            iForceObjectIds.Compress();
            }
            break;
            
        default:
            {
            User::Leave( KErrUnknown );
            }
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CImageComposerAO::RunError( TInt aError )
#else
TInt CImageComposerAO::RunError( TInt )
#endif
    {
    WRITELOG1( "CImageComposerAO::RunError() - error code: %d", aError );
    if ( iMdeObject && iSession )
    	{
    	TRAP_IGNORE( iSession->CancelObjectL( *iMdeObject ) );
    	}
    SetNextRequest( ERequestGetObject );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// GetObjectFromMde
// ---------------------------------------------------------------------------
//
void CImageComposerAO::GetObjectFromMdeL(TItemId& aMdEObjectId)
    {
#ifdef _DEBUG
    _LIT( KPanicCategoryNsd, "NSD=NULL" );
    _LIT( KPanicCategoryOd,  "OD=NULL" );
    _LIT( KPanicCategoryId,  "ID=NULL" );
    _LIT( KPanicCategoryLd,  "LD=NULL" );
    __ASSERT_DEBUG( iDefaultNamespace, User::Panic( KPanicCategoryNsd, KErrBadHandle ) );
    __ASSERT_DEBUG( iObjectDef, User::Panic( KPanicCategoryOd,  KErrBadHandle ) );
    __ASSERT_DEBUG( iImageObjectDef, User::Panic( KPanicCategoryId,  KErrBadHandle ) );
    __ASSERT_DEBUG( iLocationObjectDef, User::Panic( KPanicCategoryLd,  KErrBadHandle ) );
#endif    
    
    WRITELOG( "CImageComposerAO::GetObjectFromMdeL() - start" );
    
    if ( !iSession )
    	{
    	WRITELOG( "CImageComposerAO::GetObjectFromMdeL() - iSession is NULL" );
    	User::Leave( KErrSessionClosed );
    	}

    TItemId objectId = KNoId;
    
    // get the object id from queue
    if( iItemQueue.Count() > 0 )
    	{
    	objectId = iItemQueue[0];
    	aMdEObjectId = objectId;
    	iItemQueue.Remove( 0 );
    	}
    
    // get object from db (NULL if not found)
    CMdEObject* mdeObject = iSession->GetObjectL( objectId, *iImageObjectDef );

    CleanupStack::PushL( mdeObject );
    
    if ( !mdeObject )
        {
        WRITELOG1( "CImageComposerAO::GetObjectFromMdeL() - could not find object id %d", objectId );
        User::Leave( KErrNotFound );
        }

    TInt force;
    force = iForceObjectIds.Find( objectId );
    if ( force != KErrNotFound )
    	{
    	iForceObjectIds.Remove( force );
    	
    	if( iForceObjectIds.Count() == 0 )
    		{
    		iForceObjectIds.Compress();
    		}
    	}
    else
    	{
	    // check if file's and object's last modified dates are equal
	    CMdEPropertyDef& lastModifiedDatePropDef = mdeObject->Def().GetPropertyDefL( 
	    		Object::KLastModifiedDateProperty );
	    CMdEProperty* lastModifiedDateProp = NULL;
	    mdeObject->Property( lastModifiedDatePropDef, lastModifiedDateProp );
	    if( lastModifiedDateProp )
	    	{
	    	TTime time = ((CMdETimeProperty*)lastModifiedDateProp)->Value();
	
	    	TEntry entry;
	    	TInt error = iFs.Entry( mdeObject->Uri(), entry );
	
	    	if( error != KErrNone || entry.iModified == time )
	    		{
	    	    WRITELOG( "CImageComposerAO::GetObjectFromMdeL() - image data has not been modified - abort" );
	    		User::Leave( KErrAbort );
	    		}
	    	}
	    else
	    	{
	    	User::Leave( KErrNotFound );
	    	}   
    	}
    
    CleanupStack::Pop( mdeObject );
    iMdeObject = mdeObject;
    
    WRITELOG( "CImageComposerAO::GetObjectFromMdeL() - end" );
    }

// ---------------------------------------------------------------------------
// ComposeL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::ComposeL()
    {
#ifdef _DEBUG
    _LIT( KPanicCategoryNsd, "NSD=NULL" );
    _LIT( KPanicCategoryOd,  "OD=NULL" );
    _LIT( KPanicCategoryId,  "ID=NULL" );
    _LIT( KPanicCategoryLd,  "LD=NULL" );
    __ASSERT_DEBUG( iDefaultNamespace, User::Panic( KPanicCategoryNsd, KErrBadHandle ) );
    __ASSERT_DEBUG( iObjectDef, User::Panic( KPanicCategoryOd,  KErrBadHandle ) );
    __ASSERT_DEBUG( iImageObjectDef, User::Panic( KPanicCategoryId,  KErrBadHandle ) );
    __ASSERT_DEBUG( iLocationObjectDef, User::Panic( KPanicCategoryLd,  KErrBadHandle ) );
#endif    
    
#ifdef _DEBUG    
    WRITELOG( "CImageComposerAO::ComposeL()" );
    WRITELOG1( "CImageComposerAO::ComposeL() - Compose Start Object ID: %d", iMdeObject->Id() );
#endif

    if ( !iSession )
    	{
    	WRITELOG( "CImageComposerAO::ComposeL() - iSession is NULL!" );
    	User::Leave( KErrSessionClosed );
    	}

    // 1. Read Exif image from the file to a buffer...
    RFile64 file;
    CleanupClosePushL( file );
    WRITELOG( "CImageComposerAO::ComposeL() - open file for reading" );
    User::LeaveIfError( file.Open( iFs, iMdeObject->Uri(), EFileRead ) );

    TInt64 dataSize = 0;
    file.Size( dataSize );
    HBufC8* exif = HBufC8::NewL( dataSize );
    CleanupStack::PushL( exif );
    TPtr8 exifPtr = exif->Des();
    User::LeaveIfError( file.Read( exifPtr ) );
    CleanupStack::Pop( exif );             // exif needs to be popped and pushed again
    CleanupStack::PopAndDestroy( &file );  // to get file out of CleanupStack
    CleanupStack::PushL( exif );
    
    HBufC8* modifiedExif = NULL;
    
    iExifUtil->ComposeExifDataL(*iMdeObject, exifPtr, modifiedExif);
   
    // modifiedExif is NULL if no changes were made
    if ( modifiedExif )
        {
        CleanupStack::PushL( modifiedExif );
        
        if ( !iMdeObject->OpenForModifications() )
            {
            const TItemId objectId = iMdeObject->Id();
            delete iMdeObject;
            iMdeObject = NULL;
            iMdeObject = iSession->OpenObjectL( objectId, *iImageObjectDef );
            if ( !iMdeObject )
                {
                User::Leave( KErrAccessDenied );
                }
            }

    	// set position to begin of file
        WRITELOG( "CImageComposerAO::ComposeL() - open file for writing" );
        User::LeaveIfError( file.Open( iFs, iMdeObject->Uri(), EFileWrite ) );
        CleanupClosePushL( file );

        TInt64 pos = 0;
        WRITELOG( "CImageComposerAO::ComposeL() - seek to position 0" );
        User::LeaveIfError( file.Seek( ESeekStart, pos ) );

        WRITELOG( "CImageComposerAO::ComposeL() - write buffer (exif) to file" );
        User::LeaveIfError( file.Write( modifiedExif->Des(), modifiedExif->Des().Length() ) );

        CleanupStack::PopAndDestroy( 2, modifiedExif ); // file, modifiedExif
        TEntry fileEntry;
        iFs.Entry( iMdeObject->Uri(), fileEntry );

        WRITELOG( "CImageComposerAO::ComposeL() - store Size and LastModifiedDate properties to MDE" );
        CMdEPropertyDef& sizePropDef = iImageObjectDef->GetPropertyDefL( Object::KSizeProperty );
            {
            CMdEProperty* sizeProp = NULL;
            iMdeObject->Property( sizePropDef, sizeProp, 0 );

            if ( sizeProp )
                {
                sizeProp->SetUint32ValueL( fileEntry.iSize );
                }
            else
                {
                iMdeObject->AddUint32PropertyL( sizePropDef, fileEntry.iSize );
                }
            }

        CMdEPropertyDef& lastModDatePropDef = iImageObjectDef->GetPropertyDefL(
        		Object::KLastModifiedDateProperty );
        
        CMdEProperty* lastModDateProp = NULL;
        iMdeObject->Property( lastModDatePropDef, lastModDateProp, 0 );

        if ( lastModDateProp )
            {
            lastModDateProp->SetTimeValueL( fileEntry.iModified );
            }
        else
            {
            iMdeObject->AddTimePropertyL( lastModDatePropDef, fileEntry.iModified );
            }
        iSession->CommitObjectL( *iMdeObject );
        iNextItemsSkip.InsertInOrder( iMdeObject->Id(),
        		TLinearOrder<TItemId>( CImageComposerAO::CompareTItemIds ) );
        }

    CleanupStack::PopAndDestroy( exif );

    WRITELOG( "CImageComposerAO::ComposeL() - Start writing GPS tags" );
    
    WriteGPSTagsL( iMdeObject->Id() );

#ifdef _DEBUG
    WRITELOG1( "CImageComposerAO::ComposeL() - Compose End Object ID: %d", iMdeObject->Id() );
#endif
    }

// ---------------------------------------------------------------------------
// WriteGPSTagsL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::WriteGPSTagsL( TItemId aObjectId )
    {
#ifdef _DEBUG
    _LIT( KPanicCategoryNsd, "NSD=NULL" );
    _LIT( KPanicCategoryOd,  "OD=NULL" );
    _LIT( KPanicCategoryId,  "ID=NULL" );
    _LIT( KPanicCategoryLd,  "LD=NULL" );
    __ASSERT_DEBUG( iDefaultNamespace, User::Panic( KPanicCategoryNsd, KErrBadHandle ) );
    __ASSERT_DEBUG( iObjectDef, User::Panic( KPanicCategoryOd,  KErrBadHandle ) );
    __ASSERT_DEBUG( iImageObjectDef, User::Panic( KPanicCategoryId,  KErrBadHandle ) );
    __ASSERT_DEBUG( iLocationObjectDef, User::Panic( KPanicCategoryLd,  KErrBadHandle ) );
#endif    
    
    delete iRelationQuery;
    iRelationQuery = NULL;
    
    if ( !iSession )
    	{
    	User::Leave( KErrSessionClosed );
    	}
    
    iRelationQuery = iSession->NewRelationQueryL( *iDefaultNamespace, this );
    
    iRelationQuery->SetResultMode( EQueryResultModeItem );
    iRelationQuery->Conditions().SetOperator( ELogicConditionOperatorAnd );
    
    CMdERelationCondition& filterCond = iRelationQuery->Conditions().
    		AddRelationConditionL( ERelationConditionSideRight );
    
    // Left object in relation must have this ID.
    filterCond.LeftL().AddObjectConditionL( aObjectId );
    
    // Right object in relation must be a location object.
    filterCond.RightL().AddObjectConditionL( *iLocationObjectDef );
    
    iRelationQuery->FindL( 1, 1 ); // results to a call to HandleQueryCompleted()
    }
    
// ---------------------------------------------------------------------------
// SetNextRequest
// ---------------------------------------------------------------------------
//
void CImageComposerAO::SetNextRequest( TRequestType aNextRequest )
    {
    iNextRequest = aNextRequest;
    
    if ( !IsActive() )
        {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* ptrStatus = &iStatus;
        User::RequestComplete( ptrStatus, KErrNone );
        }
    }
    
// ---------------------------------------------------------------------------
// HandleQueryNewResults
// ---------------------------------------------------------------------------
//
void CImageComposerAO::HandleQueryNewResults( CMdEQuery& /*aQuery*/,
	TInt /*aFirstNewItemIndex*/, TInt /*aNewItemCount*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// HandleQueryNewResults
// ---------------------------------------------------------------------------
//
void CImageComposerAO::HandleQueryNewResults( CMdEQuery& /*aQuery*/,
	TInt /*aNewObjectItemCount*/, TInt /*aNewRelationItemCount*/,
	TInt /*aNewEventItemCount*/ )
    {
    }

// ---------------------------------------------------------------------------
// HandleQueryCompleted
// ---------------------------------------------------------------------------
//
void CImageComposerAO::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
    {
    if ( aError != KErrNone )
    	{
    	WRITELOG1( "CImageComposerAO::HandleQueryCompleted() - query error: %d", aError );
    	SetNextRequest( ERequestGetObject );
    	return;
    	}
    if ( aQuery.Count() == 0 )
        {
        WRITELOG( "CImageComposerAO::HandleQueryCompleted() - no gps items found" );
        SetNextRequest( ERequestGetObject );
        return;
        }

#ifdef _DEBUG        
    TRAPD( error, StartWritingGPSTagsL( aQuery ) );
    WRITELOG1( "CImageComposerAO::HandleQueryCompleted() - error code from StartWritingGPSTagsL: %d", error );
#else
    TRAP_IGNORE( StartWritingGPSTagsL( aQuery ) );
#endif
    
    SetNextRequest( ERequestGetObject );
    }

// ---------------------------------------------------------------------------
// StartWritingGPSTagsL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::StartWritingGPSTagsL( CMdEQuery& aQuery )
    {
#ifdef _DEBUG
    _LIT( KPanicCategoryNsd, "NSD=NULL" );
    _LIT( KPanicCategoryOd,  "OD=NULL" );
    _LIT( KPanicCategoryId,  "ID=NULL" );
    _LIT( KPanicCategoryLd,  "LD=NULL" );
    __ASSERT_DEBUG( iDefaultNamespace, User::Panic( KPanicCategoryNsd, KErrBadHandle ) );
    __ASSERT_DEBUG( iObjectDef, User::Panic( KPanicCategoryOd,  KErrBadHandle ) );
    __ASSERT_DEBUG( iImageObjectDef, User::Panic( KPanicCategoryId,  KErrBadHandle ) );
    __ASSERT_DEBUG( iLocationObjectDef, User::Panic( KPanicCategoryLd,  KErrBadHandle ) );
#endif    
    
    if ( !iSession )
    	{
    	WRITELOG( "CImageComposerAO::StartWritingGPSTagsL - iSession is NULL!" );
    	User::Leave( KErrSessionClosed );
    	}
    
    CMdERelation& result = static_cast<CMdERelation&>( aQuery.ResultItem( 0 ) );
    TItemId rightId = result.RightObjectId();
    CMdEObject* location = iSession->GetObjectL( rightId, *iLocationObjectDef );
    CleanupStack::PushL( location );
    
    TItemId leftId = result.LeftObjectId();
    CMdEObject* mdeObject = iSession->GetObjectL( leftId, *iObjectDef );
    CleanupStack::PushL( mdeObject );
    
    DoWriteExifL( mdeObject, location );
    
    CleanupStack::PopAndDestroy( mdeObject );
    CleanupStack::PopAndDestroy( location );
    }


// ---------------------------------------------------------------------------
// DoWriteExifL
// ---------------------------------------------------------------------------
//
void CImageComposerAO::DoWriteExifL( CMdEObject* aMdEObject, CMdEObject* aLocationObject )
    {
#ifdef _DEBUG
    _LIT( KPanicCategoryNsd, "NSD=NULL" );
    _LIT( KPanicCategoryOd,  "OD=NULL" );
    _LIT( KPanicCategoryId,  "ID=NULL" );
    _LIT( KPanicCategoryLd,  "LD=NULL" );
    __ASSERT_DEBUG( iDefaultNamespace, User::Panic( KPanicCategoryNsd, KErrBadHandle ) );
    __ASSERT_DEBUG( iObjectDef, User::Panic( KPanicCategoryOd,  KErrBadHandle ) );
    __ASSERT_DEBUG( iImageObjectDef, User::Panic( KPanicCategoryId,  KErrBadHandle ) );
    __ASSERT_DEBUG( iLocationObjectDef, User::Panic( KPanicCategoryLd,  KErrBadHandle ) );
#endif
    
    WRITELOG( "CImageComposerAO::DoWriteExifL()" );
    if ( !aMdEObject || !aLocationObject )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - null parameter(s)!" );
        User::Leave( KErrArgument );
        }

    CMdEProperty* itemTypeProperty = NULL;
    CMdEProperty* latitudeProperty = NULL;
    CMdEProperty* longitudeProperty = NULL;
    CMdEProperty* altitudeProperty = NULL;
    CMdEProperty* qualityProperty = NULL;

    aMdEObject->Property( iObjectDef->GetPropertyDefL(
    		Object::KItemTypeProperty ), itemTypeProperty, 0 );
    aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
    		Location::KLatitudeProperty ), latitudeProperty, 0 );
    aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
    		Location::KLongitudeProperty ), longitudeProperty, 0 );
    aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
    		Location::KAltitudeProperty ), altitudeProperty, 0 );
    aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
    		Location::KQualityProperty ), qualityProperty, 0 );

    if ( !itemTypeProperty )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - NULL item type property!" );
        User::Leave( KErrBadHandle );
        }
    const TDesC& uri = aMdEObject->Uri();

    const TDesC& mimeType = itemTypeProperty->TextValueL();

    if ( !IsJpeg( const_cast<TDesC&>(mimeType) ) )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - object mimetype is not image/jpeg!" );
        User::Leave( KErrNotSupported );
        }

    // Check whether the file is open
    TBool isOpen( EFalse );
    const TInt openError = iFs.IsFileOpen( uri, isOpen );
    if( openError != KErrNone )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - check for open file failed!" );
        User::Leave( openError );
        }
    else if ( isOpen )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - file handle is open!" );
        User::Leave( KErrInUse );
        }

    TInt64 imageFileSize = 0;
    RFile64 file;
    User::LeaveIfError( file.Open( iFs, uri, EFileRead ) );
    CleanupClosePushL( file );

    User::LeaveIfError( file.Size( imageFileSize ) );
    HBufC8* imageData = HBufC8::NewL( imageFileSize );
    CleanupStack::PushL( imageData );
    TPtr8 myImagePtr = imageData->Des();

    WRITELOG( "CImageComposerAO::DoWriteExifL() - reading IMAGE file" );
    const TInt readError = file.Read( myImagePtr ) ;
    if ( readError != KErrNone )
        {
        WRITELOG( "CImageComposerAO::DoWriteExifL() - error while reading image file!" );
        User::Leave( KErrGeneral );
        }

    WRITELOG( "CImageComposerAO::DoWriteExifL() - reading IMAGE file - DONE! - closing file" );
    CleanupStack::Pop( imageData );
    CleanupStack::PopAndDestroy( &file );
    CleanupStack::PushL( imageData );

    HBufC8* modifiedExif = NULL;
    iExifUtil->ComposeLocationL( aLocationObject, myImagePtr, modifiedExif );

    // write the EXIF data to the image

    if ( modifiedExif )
        {
        CleanupStack::PushL( modifiedExif );

        CMdEObject* mdeObject = iSession->OpenObjectL( aMdEObject->Id(), 
        		*iImageObjectDef );
        if ( !mdeObject )
            {
            User::Leave( KErrAccessDenied );
            }
        CleanupStack::PushL( mdeObject );

        User::LeaveIfError( file.Open( iFs, mdeObject->Uri(), EFileWrite ) );
        CleanupClosePushL( file );

        // set position to begin of file
        TInt64 pos = 0;
        User::LeaveIfError( file.Seek( ESeekStart, pos ) );

        User::LeaveIfError( file.Write( modifiedExif->Des(), modifiedExif->Des().Length() ) );

        CleanupStack::PopAndDestroy( &file );
        TEntry fileEntry;
        iFs.Entry( mdeObject->Uri(), fileEntry );

        CMdEPropertyDef& sizePropDef = iImageObjectDef->GetPropertyDefL( Object::KSizeProperty );
        CMdEProperty* sizeProp = NULL;
        mdeObject->Property( sizePropDef, sizeProp, 0 );

        if ( sizeProp )
            {
            sizeProp->SetUint32ValueL( fileEntry.iSize );
            }
        else
            {
            mdeObject->AddUint32PropertyL( sizePropDef, fileEntry.iSize );
            }

        CMdEPropertyDef& lastModDatePropDef = iImageObjectDef->GetPropertyDefL(
        		Object::KLastModifiedDateProperty );
        CMdEProperty* lastModDateProp = NULL;
        mdeObject->Property( lastModDatePropDef, lastModDateProp, 0 );

        if ( lastModDateProp )
            {
            lastModDateProp->SetTimeValueL( fileEntry.iModified );
            }
        else
            {
            mdeObject->AddTimePropertyL( lastModDatePropDef, fileEntry.iModified );
            }

        iSession->CommitObjectL( *mdeObject );
        iNextItemsSkip.InsertInOrder( mdeObject->Id(),
        		TLinearOrder<TItemId>( CImageComposerAO::CompareTItemIds ) );
        CleanupStack::PopAndDestroy( mdeObject );
        CleanupStack::PopAndDestroy( modifiedExif );
        }

    // remove empty (=unneeded) location objects
    if ( !latitudeProperty && !longitudeProperty && !altitudeProperty ) // && !satellitesProperty ) // check these first...
        {
        // get the rest of the properties
        CMdEProperty* cellIdProperty = NULL;
        CMdEProperty* countryCodeProperty = NULL;
        CMdEProperty* networkCodeProperty = NULL;
        CMdEProperty* locationAreaCodeProperty = NULL;
        CMdEProperty* speedProperty = NULL;
        CMdEProperty* directionProperty = NULL;
        CMdEProperty* qualityProperty = NULL;

        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KCellIdProperty ), cellIdProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KCountryCodeProperty ), countryCodeProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KNetworkCodeProperty ), networkCodeProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KLocationAreaCodeProperty ), locationAreaCodeProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KSpeedProperty ), speedProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KDirectionProperty ), directionProperty, 0 );
        aLocationObject->Property( iLocationObjectDef->GetPropertyDefL(
        		Location::KQualityProperty ), qualityProperty, 0 );

        // if object doesn't contain any properties, remove it
        if ( !cellIdProperty && !countryCodeProperty && !networkCodeProperty
            && !locationAreaCodeProperty && !speedProperty && !directionProperty && !qualityProperty )
            {
            // Ignore return value - if operation does not succeed, there is no much that can be done here
            iSession->RemoveObjectL( aLocationObject->Id(), iDefaultNamespace );
            }
        }
    
    CleanupStack::PopAndDestroy( imageData );

    }

// ---------------------------------------------------------------------------
// IsJpeg
// ---------------------------------------------------------------------------
//
TBool CImageComposerAO::IsJpeg( const TDesC& aMimeType )
    {
    WRITELOG( "CImageComposerAO::IsJpeg()" );

    if ( MdsUtils::Compare( KJpegMimeType, aMimeType ) == 0 )
        {
        WRITELOG( "CImageComposerAO::IsJpeg() - image is Jpeg" );
        return ETrue;        
        }

    return EFalse;
    }


