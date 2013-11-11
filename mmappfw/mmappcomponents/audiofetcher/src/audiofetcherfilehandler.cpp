/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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



#include "audiofetcherfilehandler.h"
#include "audiofetcherlog.h"

#include <pathinfo.h>
#include <bautils.h>


/******************************************************************************
 * class CAudioFetcherFileHandler
 ******************************************************************************/



// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::NewL
// 
// -----------------------------------------------------------------------------
//
CAudioFetcherFileHandler* CAudioFetcherFileHandler::NewL()
    {
    CAudioFetcherFileHandler* self = new (ELeave) CAudioFetcherFileHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CAudioFetcherFileHandler::~CAudioFetcherFileHandler()
    {
    WLOG("CAudioFetcherFileHandler::~CAudioFetcherFileHandler >");
    
    if ( iSearchList )
        {
        iSearchList->Reset();
        }
    delete iSearchList;
    delete iQueryCaller;
    delete iObjectNotificationCaller;
    
    delete iDriveUtil;
    WLOG("CAudioFetcherFileHandler::~CAudioFetcherFileHandler before iQuery");
    if ( iQuery )
        {
        iQuery->Cancel();
        }
    delete iQuery;
    WLOG("CAudioFetcherFileHandler::~CAudioFetcherFileHandler after iQuery");
    delete iSession;
    WLOG("CAudioFetcherFileHandler::~CAudioFetcherFileHandler <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::CAudioFetcherFileHandler
// 
// -----------------------------------------------------------------------------
//
CAudioFetcherFileHandler::CAudioFetcherFileHandler()
    {
    iSessionOpen = EFalse;
    iObserverCallEnabled = ETrue;
    
    iMaxFileSize = KErrNotFound;
    iQueryId = KErrNotFound;
    iQueryType = KErrNotFound;
    iQueryError = KErrNone;
    iQueryEvent = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::ConstructL()
    {
    WLOG("CAudioFetcherFileHandler::ConstructL >");
    
    iSession = CMdESession::NewL( *this );
    iSearchList = CSearchList::NewL();
    
    iQueryCaller = CActiveCaller::NewL( this );
    iObjectNotificationCaller = CActiveCaller::NewL( this );
    
    iDriveUtil = CDriveUtil::NewL();
    
    WLOG("CAudioFetcherFileHandler::ConstructL <");
    }





// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleSessionOpened (from MMdESessionObserver)
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleSessionOpened( CMdESession& /*aSession*/, TInt aError )
    {
    WLOG("CAudioFetcherFileHandler::HandleSessionOpened >");
    if ( aError != KErrNone )
        {
        delete iSession;
        iSession = NULL;
        iSessionOpen = EFalse;
        }
    else
        {
        iSessionOpen = ETrue;

        TRAP_IGNORE( AddObjectObserverL() );
        }

    CallObserver( MAudioFetcherFileHandlerObserver::EInitComplete, aError );
    WLOG("CAudioFetcherFileHandler::HandleSessionOpened <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::AddObjectObserverL
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::AddObjectObserverL()
    {
    WLOG("CAudioFetcherFileHandler::AddObjectObserverL >");
    if ( iSessionOpen )
        {
        TUint32 notificationType = ENotifyAdd | ENotifyModify | ENotifyRemove;
        CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
        iSession->AddObjectObserverL( *this, NULL, notificationType, &defNS );
        
        iSession->AddObjectPresentObserverL( *this );
        }
    WLOG("CAudioFetcherFileHandler::AddObjectObserverL <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleSessionError (from MMdESessionObserver)
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleSessionError( CMdESession& /*aSession*/, TInt aError )
    {
    WLOG("CAudioFetcherFileHandler::HandleSessionError >");
    if ( aError == KErrNone )
        {
        return;
        }
        
    delete iSession;
    iSession = NULL;
    iSessionOpen = EFalse;

    CallObserver( MAudioFetcherFileHandlerObserver::EError, aError );
    WLOG("CAudioFetcherFileHandler::HandleSessionError <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleQueryNewResults (from MMdEQueryObserver)
// 
// This observer function is called during query. Calling frequency is defined
// in second parameter of CMdEQuery::FindL.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleQueryNewResults( CMdEQuery& /*aQuery*/, 
                                               TInt /*aFirstNewItemIndex*/,
                                               TInt /*aNewItemCount*/ )
    {
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleObjectNotification (MMdEObjectObserver)
// 
// Called when metadata database changes.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleObjectNotification( CMdESession& /*aSession*/, 
                                        TObserverNotificationType aType,
                                        const RArray<TItemId>& aObjectIdArray )
    {
    WLOG("CAudioFetcherFileHandler::HandleObjectNotification >");
    if ( aType == ENotifyAdd || aType == ENotifyModify || aType == ENotifyRemove )
        {
        if ( iObserver )
            {
            TInt count = aObjectIdArray.Count();
            WLOG1("CAudioFetcherFileHandler::HandleObjectNotification: count is %d", count );
            
            // delayed call to avoid many consecutive observer calls
            CallObserverWithDelay();
            }
        }
    WLOG("CAudioFetcherFileHandler::HandleObjectNotification <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleObjectPresentNotification (MMdEObjectPresentObserver)
// 
// Called when previously used memory card is inserted and hidden
// metadata files are made present again.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleObjectPresentNotification( CMdESession& /*aSession*/, 
                         TBool /*aPresent*/, const RArray<TItemId>& aObjectIdArray )
    {
    WLOG("CAudioFetcherFileHandler::HandleObjectPresentNotification >");
    if ( iObserver )
        {
        TInt count = aObjectIdArray.Count();
        WLOG1("CAudioFetcherFileHandler::HandleObjectPresentNotification: count is %d", count );

        // delayed call to avoid many consecutive observer calls
        CallObserverWithDelay();
        }
    WLOG("CAudioFetcherFileHandler::HandleObjectPresentNotification <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleQueryCompleted (MMdEQueryObserver)
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
    {
    WLOG("CAudioFetcherFileHandler::HandleQueryCompleted >");
    TInt err = KErrNone;
    TRAP( err, HandleQueryCompletedL( aQuery, aError ) );
    
    if ( err == KErrCancel )
        {
        CallObserver( MAudioFetcherFileHandlerObserver::EQueryCanceled, KErrNone );
        }
    else if ( err != KErrNone )
        {
        iSearchList->Reset();
        CallObserver( MAudioFetcherFileHandlerObserver::EError, err );
        }
    WLOG("CAudioFetcherFileHandler::HandleQueryCompleted <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleQueryCompletedL
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleQueryCompletedL( CMdEQuery& /*aQuery*/, TInt aError )
    {
    WLOG("CAudioFetcherFileHandler::HandleQueryCompletedL >");
    TInt err = aError;
    
    if ( !iQuery )
        {
        err = KErrGeneral;
        }
    if ( err == KErrCancel )
        {
        iSearchList->Reset();
        return;
//        User::Leave( err );
        }
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
    
    iSearchList->SetSession( iSession );
    TQueryResultMode resultMode = iQuery->ResultMode();
    
    if ( resultMode == EQueryResultModeCount )
        {
        // EQueryResultModeCount currently not used
        CallObserver( MAudioFetcherFileHandlerObserver::EUnknownQueryComplete, KErrNone );
        }
    else if ( resultMode == EQueryResultModeItem )
        {
        if ( iQueryType == ESearchAudio )
            {
            // ESearchAudio currently not used
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );           
            }
        else if ( iQueryType == ESearchMusic )
            {
            // search is done in 3 steps (music, tones and videos)
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            }
        else if ( iQueryType == ESearchTones )
            {
            iSearchList->TakeResultL( iQuery, EMediaTypeTone );
            }
        else if ( iQueryType == EQueryUnknown )
            {
            iSearchList->Reset();
            iSearchList->TakeResultL( iQuery, EMediaTypeMusic );
            iSearchList->SetSearchId( iQueryId );     
            
            CallObserver( MAudioFetcherFileHandlerObserver::EUnknownQueryComplete, KErrNone );
            }
        else
            {
            TInt mediaType = MediaType( iQueryType );
            iSearchList->TakeResultL( iQuery, mediaType );
            iSearchList->SetSearchId( iQueryId );
            iSearchList->Sort();
        
            CallObserver( MAudioFetcherFileHandlerObserver::EQueryComplete, KErrNone );
            }
        }
    else
        {
        CallObserver( MAudioFetcherFileHandlerObserver::EError, KErrNotSupported );
        }
    WLOG("CAudioFetcherFileHandler::HandleQueryCompletedL <");
    }

// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::EQueryImage
// 
// Finds image files.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::QueryImageL()
    {
    WLOG("CAudioFetcherFileHandler::EQueryImageL >");
    
    LeaveIfSessionClosedL();
       
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& imageObjectDef =
    defNS.GetObjectDefL( MdeConstants::Image::KImageObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, imageObjectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );
   
    CMdELogicCondition& conditions = iQuery->Conditions();
    ExcludeRomFilesL( conditions );
   
    iQuery->SetResultMode( EQueryResultModeItem );
    iQuery->FindL();
    
    iQueryType = EMediaTypeImage;
    
    WLOG("CAudioFetcherFileHandler::QueryImageL <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::QueryAudioL
// 
// Finds music files.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::QueryAudioL()
    {
    WLOG("CAudioFetcherFileHandler::QueryAudioL >");
    
    LeaveIfSessionClosedL();
       
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& musicObjectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, musicObjectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );
   
    CMdELogicCondition& conditions = iQuery->Conditions();
    ExcludeRomFilesL( conditions );
    
    // define sort order
    // iQuery->AppendOrderRuleL( TMdEOrderRule( namePropertyDef, ESortAscending ) );
   
    iQuery->SetResultMode( EQueryResultModeItem );
    iQuery->FindL();
    
    iQueryType = EMediaTypeMusic;
    
    WLOG("CAudioFetcherFileHandler::QueryAudioL <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::QueryAudioL
// 
// Finds music files.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::QueryVideoL()
    {
    WLOG("CAudioFetcherFileHandler::QueryVideoL >");
    
    LeaveIfSessionClosedL();
       
    CMdENamespaceDef& defNS = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& videoObjectDef =
    defNS.GetObjectDefL( MdeConstants::Video::KVideoObject );
  
    delete iQuery;
    iQuery = NULL;
    iQuery = iSession->NewObjectQueryL( defNS, videoObjectDef, this );
    
    // set attributes that are included in query result  
    CMdEPropertyDef& namePropertyDef = PropertyDefL( EAttrSongName );
    iQuery->AddPropertyFilterL( &namePropertyDef );
    CMdEPropertyDef& fileTypePropertyDef = PropertyDefL( EAttrMediaType );
    iQuery->AddPropertyFilterL( &fileTypePropertyDef );
   
    CMdELogicCondition& conditions = iQuery->Conditions();
    ExcludeRomFilesL( conditions );
   
    iQuery->SetResultMode( EQueryResultModeItem );
    iQuery->FindL();
    
    iQueryType = EMediaTypeVideo;
    
    WLOG("CAudioFetcherFileHandler::QueryVideoL <");
    }

// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::ResultCount
// 
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherFileHandler::ResultCount()
    {
    WLOG("CAudioFetcherFileHandler::ResultCount");
    if ( !iSessionOpen )
        {
        return 0;
        }
    
    return iSearchList->Count();
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::CancelQuery
// 
// Cancels ongoing query.
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::CancelQuery()
    {
    WLOG("CAudioFetcherFileHandler::CancelQuery >");
    if ( !iSessionOpen || !iQuery )
        {
        return;
        }
    
    if ( !iQuery->IsComplete() )
        {
        iQuery->Cancel(); // this calls HandleQueryCompleted synchronously
        }
    WLOG("CAudioFetcherFileHandler::CancelQuery <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::LeaveIfSessionClosedL
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::LeaveIfSessionClosedL()
    {
    WLOG("CAudioFetcherFileHandler::LeaveIfSessionClosedL >");
    if ( !iSession || !iSessionOpen )
        {
        User::Leave( KErrDisconnected );
        }
    WLOG("CAudioFetcherFileHandler::LeaveIfSessionClosedL <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::SetObserver
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::SetObserver( MAudioFetcherFileHandlerObserver* aObserver )
    {
    iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::GetAttributeL
// 
// NOTE: GetAttributeL only gets full name or file name
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::GetAttributeL( TInt aIndex, TInt aAttr, 
                                       TDes& aValue, TInt aQueryId )
    {
    WLOG("CAudioFetcherFileHandler::GetAttributeL");
    aValue = KNullDesC;
    
    if ( !iSessionOpen )
         {
         return;
         }
    
    TInt id = iSearchList->SearchId();
    if ( id != aQueryId )
        {
        return; // search list and UI do not match
        }
    
    iSearchList->SetSession( iSession );
    iSearchList->GetAttributeL( aIndex, aAttr, aValue );
    }


    
// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::GetAttribute
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::GetAttribute( TInt aIndex, TInt aAttr,
                                      TDes& aValue, TInt aQueryId  )
    {
    WLOG("CAudioFetcherFileHandler::GetAttribute");
    TRAP_IGNORE( GetAttributeL( aIndex, aAttr, aValue, aQueryId ) );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::Attribute
// 
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherFileHandler::Attribute( TInt aIndex, TInt aAttr, TInt aQueryId )
    {
    WLOG("CAudioFetcherFileHandler::Attribute");
    if ( !iSessionOpen )
         {
         return KErrNotFound;
         }
     
    TInt id = iSearchList->SearchId();
    if ( id != aQueryId )
        {
        return KErrNotFound;
        }
    
    return iSearchList->Attribute( aIndex, aAttr );
    }




// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::PropertyDefL
// 
// -----------------------------------------------------------------------------
//
CMdEPropertyDef& CAudioFetcherFileHandler::PropertyDefL( TInt aAttr )
    {
    WLOG("CAudioFetcherFileHandler::PropertyDefL");
    return PropertyDefL( iSession, aAttr );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::PropertyDefL
// 
// -----------------------------------------------------------------------------
//
CMdEPropertyDef& CAudioFetcherFileHandler::PropertyDefL( CMdESession* aSession, TInt aAttr )
    {
    WLOG("CAudioFetcherFileHandler::PropertyDefL");
    CMdENamespaceDef& defNS = aSession->GetDefaultNamespaceDefL();
    
    CMdEObjectDef& objectDef =
    defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
   
    if ( aAttr == EAttrFileSize )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
        }
    else if ( aAttr == EAttrMediaType )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
        }
    else if ( aAttr == EAttrSongName || aAttr == EAttrFileName )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
        }
    else if ( aAttr == EAttrArtist )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KArtistProperty );
        }
    else if ( aAttr == EAttrAlbum )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KAlbumProperty );
        }
    else if ( aAttr == EAttrGenre )
        {
        return objectDef.GetPropertyDefL( MdeConstants::MediaObject::KGenreProperty );
        }
    else if ( aAttr == EAttrComposer )
        {
        return objectDef.GetPropertyDefL( MdeConstants::Audio::KComposerProperty );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    
    return objectDef.GetPropertyDefL( MdeConstants::Object::KTitleProperty );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::QueryReady
//
// -----------------------------------------------------------------------------
//
TBool CAudioFetcherFileHandler::QueryReady() 
    {
    WLOG("CAudioFetcherFileHandler::QueryReady");
    
    TBool isWaitingObserverCall = iQueryCaller->IsActive();
    if ( isWaitingObserverCall )
        {
        return EFalse; // query is ready but observer not called yet
        }
    
    if ( iQuery )
        {
        return iQuery->IsComplete();    
        }
     
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::ExcludeRomFilesL
// 
// Exclude audio files that are in rom (default tones).
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::ExcludeRomFilesL( CMdELogicCondition& aCondition )
    {
    WLOG("CAudioFetcherFileHandler::ExcludeRomFilesL");
    _LIT( KDriveZ, "z:\\" );
   
    CMdEObjectCondition& cond = aCondition.AddObjectConditionL(
                                EObjectConditionCompareUriBeginsWith, KDriveZ );
    cond.SetNegate( ETrue );
    }


// -------------------------------------------------------------------------------
// CAudioFetcherFileHandler::StrCopy
//
// String copy with lenght check.
// -------------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::StrCopy( TDes& aTarget, const TDesC& aSource )
    {
    WLOG("CAudioFetcherFileHandler::StrCopy");
    TInt len = aTarget.MaxLength();
    if( len < aSource.Length() ) 
        {
        aTarget.Copy( aSource.Left( len ) );
        return;
        }
    aTarget.Copy( aSource );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::EnableObserverCall
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::EnableObserverCall( TBool aEnable )
    {
    iObserverCallEnabled = aEnable;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::CallObserver
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::CallObserver( TInt aEvent, TInt aError )
    {
    WLOG("CAudioFetcherFileHandler::CallObserver");
    const TInt KDelay = 100;  // milliseconds
    
    if ( !iObserverCallEnabled )
        {
        return;    
        }

    iQueryError = aError;
    iQueryEvent = aEvent;
    
    iQueryCaller->Start( 0, KDelay );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::CallObserverWithDelay
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::CallObserverWithDelay()
    {
    WLOG("CAudioFetcherFileHandler::CallObserverWithDelay");
    const TInt KOneSecond = 1000;
    
    if ( !iObserverCallEnabled )
        {
        return;    
        }

    iObjectNotificationCaller->Start( 1, KOneSecond );
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::HandleActiveCallL (from MActiveCallerObserver)
//
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::HandleActiveCallL( TInt aCallId )
    {
    WLOG("CAudioFetcherFileHandler::HandleActiveCallL");
    if ( !iObserver )
        {
        return;
        }
    
    // inform observer that query is ready
    if ( aCallId ==  0 )
        {
        TRAP_IGNORE( iObserver->HandleFileEventL( iQueryEvent, iQueryError ) );
        }

    // inform observer that database has changed
    if ( aCallId == 1 )
        {
        if ( QueryReady() )
            {
            TRAP_IGNORE( iObserver->HandleFileEventL( MAudioFetcherFileHandlerObserver::EMediaFileChanged, KErrNone ) );
            }
        }
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::SetQueryId
//
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::SetQueryId( TInt aId ) 
    {
    // this id is passed to CSearchList after successful database search
    iQueryId = aId;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::Panic
//
// -----------------------------------------------------------------------------
//
void CAudioFetcherFileHandler::Panic( TInt aReason ) 
    {
    _LIT( KPanicCategory, "CAudioFetcherFileHandler" );
    
    User::Panic( KPanicCategory, aReason ); 
    }


// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::MediaType
// 
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherFileHandler::MediaType( TInt aQueryType )
    {
    TInt mediaType = EMediaTypeAny;
    
 	if ( aQueryType == EMediaTypeMusic )
		{
		mediaType = EMediaTypeMusic;
		}
 	else if ( aQueryType == EMediaTypeImage )
 	    {
 	   mediaType = EMediaTypeImage;
 	    }
 	else if ( aQueryType == EMediaTypeVideo )
 	    {
 	   mediaType = EMediaTypeVideo;
 	    }
    return mediaType;
    }

// -----------------------------------------------------------------------------
// CAudioFetcherFileHandler::QueryType
// 
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherFileHandler::QueryType()
    {
    return iQueryType;
    }


/******************************************************************************
 * class CSearchListItem
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CSearchListItem::NewL
// 
// -----------------------------------------------------------------------------
//
CSearchListItem* CSearchListItem::NewLC()
    {
    CSearchListItem* self = new (ELeave) CSearchListItem();
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CSearchListItem::~CSearchListItem()
    {
    delete iTitle;
    }


// -----------------------------------------------------------------------------
// CSearchListItem::CSearchListItem
// 
// -----------------------------------------------------------------------------
//
CSearchListItem::CSearchListItem()
    {
    iId = KErrNotFound;
    iMediaType = KErrNotFound;
    iStorageType = KErrNotFound;
    iTitle = NULL;
    }


// -----------------------------------------------------------------------------
// CSearchListItem::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CSearchListItem::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSearchListItem::SetTitleL
// 
// -----------------------------------------------------------------------------
//

void CSearchListItem::SetTitleL( const TDesC& aText, TBool aFullName )
    {
    delete iTitle;
    iTitle = NULL;
    
    if ( aFullName )
        {
        TParsePtrC parse( aText );
        iTitle = parse.Name().AllocL();
        }
    else
        {
        iTitle = aText.AllocL();    
        }
    }


// -----------------------------------------------------------------------------
// CSearchListItem::Title
// 
// -----------------------------------------------------------------------------
//

const TDesC& CSearchListItem::Title() const
    {
    return *iTitle;
    }




/******************************************************************************
 * class CSearchList
 ******************************************************************************/


// -----------------------------------------------------------------------------
// CSearchList::NewL
// 
// -----------------------------------------------------------------------------
//
CSearchList* CSearchList::NewL()
    {
    CSearchList* self = new (ELeave) CSearchList();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CSearchList::~CSearchList()
    {
    WLOG("CSearchList::~CSearchList >");
    iList.ResetAndDestroy();
    delete iDriveUtil;
    WLOG("CSearchList::~CSearchList <");
    }


// -----------------------------------------------------------------------------
// CSearchList::CSearchList
// 
// -----------------------------------------------------------------------------
//
CSearchList::CSearchList()
    {
    iSearchId = KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CSearchList::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::ConstructL()
    {
    WLOG("CSearchList::ConstructL >");
    iDriveUtil = CDriveUtil::NewL();
    WLOG("CSearchList::ConstructL");
    }


// -----------------------------------------------------------------------------
// CSearchList::SetSession
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SetSession( CMdESession* aSession )
    {
    iSession = aSession;
    }


// -----------------------------------------------------------------------------
// CSearchList::SetSearchId
// 
// -----------------------------------------------------------------------------
//
void CSearchList::SetSearchId( TInt aSearchId )
    {
    iSearchId = aSearchId;
    }


// -----------------------------------------------------------------------------
// CSearchList::SearchId
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::SearchId()
    {
    return iSearchId;
    }


// -----------------------------------------------------------------------------
// CSearchList::TakeResultL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::TakeResultL( CMdEObjectQuery* aQuery, TInt aMediaType )
    {
    WLOG("CSearchList::TakeResultL >");
    if ( !aQuery )
        {
        return;
        }
    
    TInt count = aQuery->Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        CMdEObject* object =  (CMdEObject*) aQuery->TakeOwnershipOfResult( i );
        CleanupStack::PushL( object );
        AddItemL( *object, aMediaType );
        CleanupStack::PopAndDestroy( object );
        }
    WLOG("CSearchList::TakeResultL <");
    }

// -----------------------------------------------------------------------------
// CSearchList::TakeDistinctResultL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::TakeDistinctResultL( CMdEObjectQuery* aQuery, 
                                       TInt /*aMediaType*/ )
    {
    if ( !aQuery )
        {
        return;
        }

    TInt count = aQuery->Count();
    
    for ( TInt i=0; i<count; i++ )
        {
        TPtrC ptr = aQuery->ResultDistinctValue( i );
        
        CSearchListItem* item = CSearchListItem::NewLC();
         
        item->SetTitleL( ptr, EFalse );
        item->iMediaType = KErrNotFound;
        item->iStorageType = KErrNotFound;

        iList.AppendL( item );
        
        CleanupStack::Pop( item );
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::AddItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddItemL( CMdEObject& aObject, TInt aMediaType )
    {
    WLOG("CSearchList::AddItemL >");
    CSearchListItem* item = CSearchListItem::NewLC();
    
    TPtrC songName = TextL( aObject, CAudioFetcherFileHandler::EAttrSongName );
    
    if ( songName.Length() > 0 )
        {
        item->SetTitleL( songName, EFalse );
        }
    else
        {
        item->SetTitleL( aObject.Uri(), ETrue );    
        }
    

    item->iId = aObject.Id();
    item->iMediaType = aMediaType;
   
    TInt storageType = CAudioFetcherFileHandler::EPhoneMemory;
    if ( iDriveUtil->IsMemoryCard( aObject.Uri() ) )
        {
        storageType = CAudioFetcherFileHandler::EMemoryCard;
        }
    else if ( iDriveUtil->IsMassStorage( aObject.Uri() ) )
        {
        storageType = CAudioFetcherFileHandler::EMassStorage;
        }
    item->iStorageType = storageType;
    
    iList.AppendL( item );
    
    CleanupStack::Pop( item );
    WLOG("CSearchList::AddItemL <");
    }


// -----------------------------------------------------------------------------
// CSearchList::AddAlbumItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddAlbumItemL( const TDesC& aAlbum )
    {
    TLinearOrder<CSearchListItem> sortFunction( CSearchList::Compare );
       
    CSearchListItem* item = CSearchListItem::NewLC();
   
    item->SetTitleL( aAlbum, EFalse );
    
    CleanupStack::Pop( item );
    
    item->iMediaType = KErrNotFound;
    item->iStorageType = KErrNotFound;
    item->iId = KErrNotFound;

    TInt err = iList.InsertInOrder( item, sortFunction );
    if ( err != KErrNone )
        {
        delete item;
        }
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::AddArtistItemL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::AddArtistItemL( CMdEObject& aObject, 
                                  TInt aQueryType, const TDesC& aAlbum )
    {
    // all songs for one artist
    if ( aQueryType == CAudioFetcherFileHandler::EQueryArtistAll )
        {
        AddItemL( aObject, CAudioFetcherFileHandler::EMediaTypeMusic );
        return;
        }

    TPtrC album = TextL( aObject, CAudioFetcherFileHandler::EAttrAlbum );

    // unknown songs (no album metadata) for one artist 
    if ( aQueryType == CAudioFetcherFileHandler::EQueryArtistUnknown )
        {
        if ( album.Length() == 0 )
            {
            AddItemL( aObject, CAudioFetcherFileHandler::EMediaTypeMusic );
            }
        }

    // all albums for one artist
    if ( aQueryType == CAudioFetcherFileHandler::EQueryArtist )
        {
        if ( album.Length() > 0 )
            {
            AddAlbumItemL( album );
            }
        }

    // songs in one album
    if ( aQueryType == CAudioFetcherFileHandler::EQueryArtistAlbum )
        {
        if ( album.Length() > 0 )
            {
            if ( album.Compare( aAlbum ) == 0 )
                {
                AddItemL( aObject, CAudioFetcherFileHandler::EMediaTypeMusic );
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::Sort
// 
// -----------------------------------------------------------------------------
//
void CSearchList::Sort()
    {
    iList.Sort( TLinearOrder<CSearchListItem>(CSearchList::Compare) );
    }


// -----------------------------------------------------------------------------
// CSearchList::Reset
// 
// -----------------------------------------------------------------------------
//
void CSearchList::Reset()
    {
    iSearchId = KErrNotFound;
    iList.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CSearchList::Count
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Count()
    {
    WLOG("CSearchList::Count");
    return iList.Count();
    }


// -----------------------------------------------------------------------------
// CSearchList::GetAttributeL
// 
// -----------------------------------------------------------------------------
//
void CSearchList::GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue )
    {
    WLOG("CSearchList::GetAttributeL");
    aValue = KNullDesC;
    
    if ( !iSession )
        {
        return;
        }

    if ( aIndex == KErrNotFound )
        {
        return;
        }

    TInt count = iList.Count();
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
   
    if ( aAttr == CAudioFetcherFileHandler::EAttrFullName )
        {
        CSearchListItem* item = iList[aIndex];
        CMdEObject* object = iSession->GetFullObjectL( item->iId );
        CleanupStack::PushL( object );
        StrCopy( aValue, object->Uri() );
        CleanupStack::PopAndDestroy( object );
        }
    
    else if ( aAttr == CAudioFetcherFileHandler::EAttrSongName ||
              aAttr == CAudioFetcherFileHandler::EAttrFileName )
        {
        CSearchListItem* item = iList[aIndex];
        StrCopy( aValue, item->Title() );
        if ( aValue.Length() == 0 )
            {
            CMdEObject* object = iSession->GetFullObjectL( item->iId );
            CleanupStack::PushL( object );
            TParsePtrC parse( object->Uri() );
            StrCopy( aValue, parse.Name() );
            CleanupStack::PopAndDestroy( object );
            }
        }
    }


// -----------------------------------------------------------------------------
// CSearchList::Attribute
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Attribute( TInt aIndex, TInt aAttr )
    {
    WLOG("CSearchList::Attribute");
    if ( aIndex == KErrNotFound )
        {
        return KErrNotFound;
        }

    TInt count = iList.Count();   
    __ASSERT_ALWAYS( aIndex >= 0 && aIndex < count, Panic( KErrGeneral ) );
    
    
    TInt ret = KErrNotFound;
    if ( aAttr == CAudioFetcherFileHandler::EAttrMediaType )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iMediaType;
        }
    
    else if ( aAttr == CAudioFetcherFileHandler::EAttrStorageType )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iStorageType;
        }
    else if ( aAttr == CAudioFetcherFileHandler::EAttrMediaFileId )
        {
        CSearchListItem* item = iList[aIndex];
        ret = item->iId;
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CSearchList::Compare
// 
// -----------------------------------------------------------------------------
//
TInt CSearchList::Compare( const CSearchListItem& aFirst, 
                           const CSearchListItem& aSecond )
    {
    TPtrC first = aFirst.Title();
    TPtrC second = aSecond.Title();
    
    return first.Compare( second );    
    }

// -----------------------------------------------------------------------------
// CSearchList::StrCopy
//
// -----------------------------------------------------------------------------
//
void CSearchList::StrCopy( TDes& aTarget, const TDesC& aSource )
    {
    WLOG("CSearchList::StrCopy >");
    TInt len = aTarget.MaxLength();
    if( len < aSource.Length() ) 
        {
        aTarget.Copy( aSource.Left(len) );
        return;
        }
    aTarget.Copy( aSource );
    WLOG("CSearchList::StrCopy <");
    }

// -----------------------------------------------------------------------------
// CSearchList::Panic
//
// -----------------------------------------------------------------------------
//
void CSearchList::Panic( TInt aReason ) 
    {
    _LIT( KPanicCategory, "CSearchList" );
    
    User::Panic( KPanicCategory, aReason ); 
    }


// -----------------------------------------------------------------------------
// CSearchList::TextL
// 
// -----------------------------------------------------------------------------
//
const TDesC& CSearchList::TextL( CMdEObject& aObject, TInt aId )
    {
    WLOG("CSearchList::TextL");
    CMdEPropertyDef& propDef = 
    CAudioFetcherFileHandler::PropertyDefL( iSession, aId );
    
    CMdEProperty* property = NULL;
    TInt err = aObject.Property( propDef, property, 0 );
    
    if ( err != KErrNotFound && property )
        {
        return property->TextValueL();
        }
    
    return KNullDesC;
    }


// End of File
