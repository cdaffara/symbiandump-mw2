/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: File handlder for Media Fetch plug-in
*
*/

#ifndef AUDIOFETCHERFILEHANDLER_H
#define AUDIOFETCHERFILEHANDLER_H


// INCLUDES

#include "audiofetcherdialogutils.h"


#include <mdesession.h>
#include <mdequery.h>
#include <mdelogiccondition.h>
#include <mdeconstants.h>
#include <mdeobjectquery.h>
#include <mdccommon.h>
#include <mdeitem.h>
#include <mdeobject.h>


// FORWARD DECLARATIONS
class CSearchList;

// CONSTANTS

_LIT( KMimeTypeAMR, "audio/amr" );
_LIT( KMimeTypeWAV, "audio/wav" );
_LIT( KMimeMp3, "mp3" );
_LIT( KDriveZ, "z:\\" );
_LIT( KSpace, " " );

/**
* MMediaFileHandlerObserver
*
* For observing CMediaFileHandler events.  
*/
NONSHARABLE_CLASS (MAudioFetcherFileHandlerObserver)
    {
    public:
    enum TFileHandlerEvent
        {
        EInitComplete,
        EQueryComplete,
        EQueryCanceled,
        EUnknownQueryComplete,
        EMediaFileChanged,
        EError
        };
    
    public:
        virtual void HandleFileEventL( TInt aEvent, TInt aError ) = 0;
    };



/**
* CAudioFetcherFileHandler
*
* Used for making media file queries. For example get all songs for
* one artist. Uses metadata engine.
*/
NONSHARABLE_CLASS (CAudioFetcherFileHandler) : public CBase, 
                                        public MMdESessionObserver,
                                        public MMdEQueryObserver,
                                        public MMdEObjectObserver,
                                        public MMdEObjectPresentObserver,
                                        public MActiveCallerObserver
    {
    public:
    enum TMediaType
        {
        EMediaTypeTone = 0,
        EMediaTypeVideo,
        EMediaTypeRecording,
        EMediaTypeMusic,
        EMediaTypeImage,
        EMediaTypeAny,
        };

    enum TStorageType
         {
         EPhoneMemory = 0,
         EMassStorage,
         EMemoryCard
         };

    enum TQueryAttribute
        {
        EAttrMediaType = 20,  // integer
        EAttrFileSize,        // integer
        EAttrStorageType,     // integer
        EAttrMediaFileId,     // integer
        EAttrFileName,     // string
        EAttrFullName,     // string
        EAttrSongName,     // string
        EAttrArtist,       // string
        EAttrAlbum,        // string
        EAttrGenre,        // string
        EAttrComposer      // string
        };

    enum TQueryType
        {
        EQueryNone = 0,
        EQueryAudio,
        EQueryMusic,
        EQueryVideo,
        EQueryRecording,
        EQueryTone,
        EQueryRomFile,
        EQueryImage,
        ESearchAudio,
        ESearchVideo,
        ESearchMusic,
        ESearchTones,
        EQueryEmpty,
        EQueryUnknown,
        
        EQueryArtist,
        EQueryArtistAlbum,
        EQueryArtistAll,
        EQueryArtistUnknown,
        EQueryArtistEmpty,
        };

    public:
        static CAudioFetcherFileHandler* NewL();
        virtual ~CAudioFetcherFileHandler();

    private:
        CAudioFetcherFileHandler();
        void ConstructL();

    public:
        void QueryAudioL();
        void QueryImageL();
        void QueryVideoL();
        TInt QueryType();
        
        
    public:
        TInt ResultCount();
        TBool QueryReady();
        TInt Attribute( TInt aIndex, TInt aAttr, TInt aQueryId );
        void GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue, TInt aQueryId );
        void GetAttribute( TInt aIndex, TInt aAttr, TDes& aValue, TInt aQueryId );
        void CancelQuery();

    public:
        void SetObserver( MAudioFetcherFileHandlerObserver* aObserver );
        void SetQueryId( TInt aId );
        void EnableObserverCall( TBool aEnable );
        
    public:
        TInt RomFileAttribute( TInt aIndex, TInt aAttr );
        void GetRomFileAttribute( TInt aIndex, TInt aAttr, TDes& aValue );
           
    private:
        CMdEPropertyDef& PropertyDefL( TInt aAttr );
        void StrCopy( TDes& aTarget, const TDesC& aSource );
        void CheckAttrL( TInt aAttr );
        void CallObserverWithDelay();
        void CallObserver( TInt aEvent, TInt aError );
        void AddObjectObserverL();
        
    public:
        static CMdEPropertyDef& PropertyDefL( CMdESession* aSession, TInt aAttr );
        
    private:

        void ExcludeRomFilesL( CMdELogicCondition& aCondition );

    private:
        void LeaveIfSessionClosedL();
        void Panic( TInt aReason );
        TInt MediaType( TInt aQueryType );

    private:  // from MMdESessionObserver
        void HandleSessionOpened(CMdESession& aSession, TInt aError);
        void HandleSessionError(CMdESession& aSession, TInt aError);
        
    
    private:  // from MMdEQueryObserver (mdequery.h)
        void HandleQueryNewResults( CMdEQuery& aQuery, TInt aFirstNewItemIndex,
                                    TInt aNewItemCount );
        void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );
        void HandleQueryCompletedL( CMdEQuery& aQuery, TInt aError );
        
    private:  // from MMdEObjectObserver
        void HandleObjectNotification( CMdESession& aSession, 
                                       TObserverNotificationType aType,
                                       const RArray<TItemId>& aObjectIdArray);
                                       
    private:  // from MMdEObjectPresentObserver
        void HandleObjectPresentNotification( CMdESession& aSession, 
                         TBool aPresent, const RArray<TItemId>& aObjectIdArray);
        
    private:   // from MActiveCallerObserver
         void HandleActiveCallL( TInt aCallId );

    private:
    
        // media file event observer
        MAudioFetcherFileHandlerObserver* iObserver;
       
        // session to metadata engine
        CMdESession* iSession;
      
        // metadata query
        CMdEObjectQuery* iQuery;

        // search result list - ui shows items from this list
        CSearchList* iSearchList;

        // metadata search text
        TBuf<128> iSearchText;
         
        // for generating active object calls
        CActiveCaller* iQueryCaller;
        // for generating active object calls
        CActiveCaller* iObjectNotificationCaller;
        
        // for finding out file drive type
        CDriveUtil* iDriveUtil;

        // used to prevent observer call when query is canceled fro ui
        TBool iObserverCallEnabled;
        
        // query id from UI, not used in this class
        TInt iQueryId;
        
        // max audio file file size
        TInt iMaxFileSize;
        
        // is metadata session open
        TBool iSessionOpen;

        // query type 
        TInt iQueryType;

        // query error
        TInt iQueryError;
        
        // query event
        TInt iQueryEvent;

    };




/**
* CSearchListItem
*
* Data for one media file item CSearchList.
*/
NONSHARABLE_CLASS (CSearchListItem) : public CBase 
    {
    public:
        static CSearchListItem* NewLC();
        virtual ~CSearchListItem();

    private:
        CSearchListItem();
        void ConstructL();
        
    public:
        void SetTitleL( const TDesC& aText, TBool aFullName );
        const TDesC& Title() const;
 
    public:
        TInt iId;
        TInt iMediaType;
        TInt iStorageType;
    private:
        HBufC* iTitle;
    };


/**
* CSearchList
*
* List for storing search result for one (most recent) metadata query.
*/
NONSHARABLE_CLASS (CSearchList) : public CBase 
    {
    public:
        static CSearchList* NewL();
        virtual ~CSearchList();

    private:
        CSearchList();
        void ConstructL();
        
    public:
        void SetSession( CMdESession* aSession );
        void TakeResultL( CMdEObjectQuery* aQuery, TInt aMediaType );
        //void TakeArtistResultL( CArtistQuery* aArtistQuery );
        void TakeDistinctResultL( CMdEObjectQuery* aQuery, TInt aMediaType );
  
        void AddAlbumItemL( const TDesC& aAlbum );
        void AddItemL( CMdEObject& aObject, TInt aMediaType );
        void AddArtistItemL( CMdEObject& aObject, TInt aQueryType, const TDesC& aAlbum );

        void Reset();
        void Sort();
        TInt Count();
        void GetAttributeL( TInt aIndex, TInt aAttr, TDes& aValue );
        TInt Attribute( TInt aIndex, TInt aAttr );
        void SetSearchId( TInt aId );
        TInt SearchId();
        
    private:
        static TInt Compare( const CSearchListItem& aFirst, 
                             const CSearchListItem& aSecond );
        
        static void StrCopy( TDes& aTarget, const TDesC& aSource );
        void Panic( TInt aReason );
        const TDesC& TextL( CMdEObject& aObject, TInt aId );
       
    private:
        CMdESession* iSession;  // does not own
        
        // list of search results
        RPointerArray<CSearchListItem> iList;
        
        // for finding out drive type (eg memory card)
        CDriveUtil* iDriveUtil;
        
        // search id - not used in this class
        TInt iSearchId;
    };

#endif  // AUDIOFETCHERFILEHANDLER_H

// End of File
