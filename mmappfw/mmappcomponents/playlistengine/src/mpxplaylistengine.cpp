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
* Description:  playlist engine
*
*  CMPXPlaylistEngine contains a CMPXPlaylistPluginHandler and a task queue.
*  CMPXPlaylistEngine uses CMPXPlaylistPluginHandler to select an appropriate
*  playlist plugin to handle the client's requests. It's also used for querying
*  about the currently loaded playlist plugin.
*
*  The task queue is used to manage async requests submitted by the client.
*  When an InternalizePlaylistL or ExternalizePlaylistL request is received,
*  it's added to the task queue. When a task in the queue is ready for
*  execution, ExecuteTask is called. To process the task, CMPXPlaylistEngine
*  will select an appropriate plugin; once one is found, the request is
*  handed over to the plugin and CMPXPlaylistEngine becomes active. Before
*  the plugin completes the request, CMPXPlaylistEngine is notified of the
*  results through MMPXPlaylistPluginObserver interface; CMPXPlaylistEngine
*  passes the results to its client through MMPXPlaylistEngineObserver
*  interface. Once CMPXPlaylistEngine's client completes the handling of the
*  results, plugin completes the request and CMPXPlaylistEngine::RunL
*  is called to complete one task processing cycle. CMPXPlaylistEngine is
*  ready to process the next task if any at the completion of RunL.
*
*  DESIGN DECISION:
*  Only one task queue is used instead of one per plugin because this is
*  running on the client thread. No processing time gained for having
*  separate task queues.
*
*
*/

#include <s32mem.h>
#include <bamdesca.h>
#include <badesca.h>
#include <bautils.h>
#include <uri16.h>
#include <apmrec.h>
#include <syslangutil.h>
#include <languages.hrh>
#include <data_caging_path_literals.hrh>
#include <mpxplaylisttopcharacterset.rsg>
#include <mpxlog.h>
#include <mpxtaskqueue.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include "mpxplaylistenginedefs.hrh"
#include "mpxplaylistplugin.h"
#include "mpxplaylistengine.h"

// ============================ CONSTANTS =====================================
const TInt KMPXBufExpandSize = 0x40;
const TInt KMPXArrayGranularity = 12;
_LIT( KMPXPlaylistEnginePanic, "CMPXPlaylistEngine");
_LIT(KMPXPlaylistCharacterSetRscFile, "mpxplaylisttopcharacterset.rsc");
_LIT( KMPXPlaylistExtension, ".m3u" );

// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaylistEngine::CMPXPlaylistEngine(
    MMPXPlaylistEngineObserver& aObserver)
:   CActive(EPriorityStandard),
    iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::ConstructL()
    {
    User::LeaveIfError(iFs.Connect());
     iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
     iTopCharacterSet =
         new (ELeave)CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>(KMPXArrayGranularity);
     GenerateTopCharacterSetsL();

    iTaskQueue = CMPXActiveTaskQueue::NewL();
    iPluginHandler = CMPXPlaylistPluginHandler::NewL(
                        *this, *this, iFs, *iTopCharacterSet, *iCharacterSet);
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaylistEngine* CMPXPlaylistEngine::NewL(
    MMPXPlaylistEngineObserver& aObserver)
    {
    CMPXPlaylistEngine* self=new(ELeave)CMPXPlaylistEngine(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaylistEngine::~CMPXPlaylistEngine()
    {
    Cancel();
    Cleanup();

    iFs.Close();
    delete iTaskQueue;
    delete iPluginHandler;
    delete iCharacterSet;
     delete iTopCharacterSet;
     iRscFile.Close();
    }

// =========================== EXTERNAL FUNCTIONS =============================
// ----------------------------------------------------------------------------
// Return a handle to playlist plugin handler
// ----------------------------------------------------------------------------
EXPORT_C CMPXPlaylistPluginHandler& CMPXPlaylistEngine::PlaylistPluginHandler()
    {
    ASSERT( iPluginHandler );
    return *iPluginHandler;
    }

// ----------------------------------------------------------------------------
// Determines whether the given media is a playlist
// Currently, Music Player only supports m3u playlist.
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXPlaylistEngine::IsPlaylistL( const TDesC& aUri )
    {
    // Check if the file extension is ".m3u".
    TBool isPlaylist = EFalse;
    TParsePtrC parse( aUri );
    if ( !parse.Ext().CompareF( KMPXPlaylistExtension ) )
        {
        isPlaylist = ETrue;
        }

    return isPlaylist;
    }

// ----------------------------------------------------------------------------
// Internalize a playlist (async)
//
// Add the request to the task queue
// ----------------------------------------------------------------------------
EXPORT_C void CMPXPlaylistEngine::InternalizePlaylistL(const TDesC& aPlaylistUri)
    {
    MPX_DEBUG1("CMPXPlaylistEngine::InternalizePlaylistL");

    //
    // leave if the given file does not exist
    //
    if (!BaflUtils::FileExists(iFs, aPlaylistUri))
        {
        User::Leave( KErrNotFound );
        }

    //
    // externalize parameters
    //
    CBufBase* taskParam = CBufFlat::NewL( KMPXBufExpandSize );
    CleanupStack::PushL( taskParam );
    taskParam->ResizeL( KMPXBufExpandSize );

    RBufWriteStream writeStream( *taskParam );
    CleanupClosePushL( writeStream );

    // externalize playlist URI
    writeStream.WriteInt32L( aPlaylistUri.Length() );
    writeStream << aPlaylistUri;

    writeStream.CommitL();
    taskParam->Compress();

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( EInternalizePlaylist,
                          NULL,        // callback when task completed
                          this,        // task queue observer
                          0,           // Integer parameter, not used
                          taskParam ); // task queue assumes ownership of taskParam

    CleanupStack::PopAndDestroy( &writeStream );
    CleanupStack::Pop( taskParam ); // taskParam
    }

// ----------------------------------------------------------------------------
// Internalize a playlist (async)
// ----------------------------------------------------------------------------
EXPORT_C void CMPXPlaylistEngine::InternalizePlaylistL(const RFile& aPlaylistFileHandle)
    {
    MPX_DEBUG1("CMPXPlaylistEngine::InternalizePlaylistL");

    if ( !aPlaylistFileHandle.SubSessionHandle() )
        {
        User::Leave(KErrArgument);
        }

    TFileName fullName;
    aPlaylistFileHandle.FullName( fullName );

    InternalizePlaylistL( fullName );
    }

// ----------------------------------------------------------------------------
// Externalize a playlist (async)
//
// Add the request to the task queue
// ----------------------------------------------------------------------------
EXPORT_C void CMPXPlaylistEngine::ExternalizePlaylistL(
    const CMPXMedia& aPlaylist,
    const TDesC& aFilePath)
    {
    MPX_DEBUG1("CMPXPlaylistEngine::ExternalizePlaylistL");

    //
    // leave if the given playlist doesn't contain the following attributes
    //
    if (!aPlaylist.IsSupported(KMPXMediaGeneralTitle) ||
        !aPlaylist.IsSupported(KMPXMediaGeneralType) ||
        !aPlaylist.IsSupported(KMPXMediaGeneralCategory) ||
        !aPlaylist.IsSupported(KMPXMediaArrayContents) ||
        !aPlaylist.IsSupported(KMPXMediaArrayCount))
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media isn't a playlist (i.e. type must be EMPXItem and category
    // must be EMPXPlaylist
    //
    TMPXGeneralType mediaType =
         aPlaylist.ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType);

    TMPXGeneralCategory mediaCategory =
         aPlaylist.ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);

    if ( mediaType != EMPXItem ||
         mediaCategory != EMPXPlaylist )
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given file path does not exist
    //
    if (!BaflUtils::PathExists(iFs, aFilePath))
        {
        User::Leave( KErrPathNotFound );
        }

    // check if a plugin has been selected. the client
    // is required to select a plugin before issuing
    // this request
    if ( !iPluginHandler->PluginFound() )
        {
        User::Leave( KErrNotFound );
        }

    //
    // externalize parameters
    //
    CBufBase* taskParam = CBufFlat::NewL( KMPXBufExpandSize );
    CleanupStack::PushL( taskParam );
    taskParam->ResizeL( KMPXBufExpandSize );

    RBufWriteStream writeStream( *taskParam );
    CleanupClosePushL( writeStream );

    // externalize playlist
    writeStream << aPlaylist;

    // externalize file path
    writeStream.WriteInt32L( aFilePath.Length() );
    writeStream << aFilePath;

    // externalize plugin Uid
    writeStream.WriteInt32L( iPluginHandler->PluginUid().iUid );

    writeStream.CommitL();
    taskParam->Compress();

    //
    // add request to the task queue
    //
    CMPXMedia* playlist = CMPXMedia::NewL(aPlaylist);
    CleanupStack::PushL(playlist);

    iTaskQueue->AddTaskL( EExternalizePlaylist,
                          NULL,        // callback when task completed
                          this,        // task queue observer
                          0,           // Integer parameter, not used
                          taskParam,   // task queue assumes ownership
                          NULL,
                          playlist );  // keep media alive. ownership transferred

    CleanupStack::Pop( playlist );
    CleanupStack::PopAndDestroy( &writeStream );
    CleanupStack::Pop( taskParam ); // taskParam
    }

// ----------------------------------------------------------------------------
// Cancel Requests
// ----------------------------------------------------------------------------
EXPORT_C void CMPXPlaylistEngine::CancelRequests()
    {
    MPX_DEBUG1("CMPXPlaylistEngine::CancelRequests");
    Cancel();
    iTaskQueue->CancelRequests();
    }

// =========================== CALLBACK FUNCTIONS =============================
// ----------------------------------------------------------------------------
// Handles plugin callback for InternalizePlaylistL request
// ----------------------------------------------------------------------------
void CMPXPlaylistEngine::HandlePlaylistL(
    CMPXMedia* aPlaylist,
    const TInt aError,
    const TBool aCompleted)
    {
    MPX_DEBUG1("CMPXPlaylistEngine::HandlePlaylistL");

    // notify playlist engine observer
    iObserver.HandlePlaylistL( aPlaylist, aError, aCompleted );
    }

// ----------------------------------------------------------------------------
// Handle plugin callback for ExternalizePlaylistL request
// ----------------------------------------------------------------------------
void CMPXPlaylistEngine::HandlePlaylistL(
    const TDesC& aPlaylistUri,
    const TInt aError)
    {
    MPX_DEBUG1("CMPXPlaylistEngine::HandlePlaylistL");

    // notify playlist engine observer
    iObserver.HandlePlaylistL( aPlaylistUri, aError );
    }

// =========================== INTERNAL FUNCTIONS =============================
// ----------------------------------------------------------------------------
// Handles request completion event
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::RunL()
    {
    MPX_DEBUG2("CMPXPlaylistEngine::RunL - status %d", iStatus.Int());

    // clean up data set during processing of the current task
    Cleanup();

    // we are done with the current request and ready for the next one if there is
    // any
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// Cancellation of an outstanding request.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::DoCancel()
    {
    if ( iPluginHandler->PluginFound() )
        {
        iPluginHandler->Plugin()->Cancel();
        }
    Cleanup();
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// Execute an async task
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::ExecuteTask(
    TInt      aTask,
    TInt      /*aParamData*/,
    TAny*     /*aPtrData*/,
    const CBufBase& aBuf,
    TAny*     /*aCallback*/,
    CBase* /*aCObject1*/,
    CBase* /*aCObject2*/)
    {
    TRAPD(err, ExecuteTaskL(aTask, aBuf));
    if (err != KErrNone)
        {
        HandleExecuteTaskError(aTask, err);
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaylistEngine::HandleTaskError
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::HandleTaskError(
    TInt /* aTask */,
    TAny* /*aPtrData*/,
    TAny* /*aCallback*/,
    TInt /* aError */)
    {
    // do nothing
    }

// ----------------------------------------------------------------------------
// CMPXPlaylistEngine::HandlePluginHandlerEvent
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::HandlePluginHandlerEvent(
    TPluginHandlerEvents /* aEvent */,
    const TUid& /* aPluginUid */,
    TBool /* aLoaded */,
    TInt /* aData */)
    {
    // Playlist plugins are stateless and they are resolved for every call.
    // There is no need to cancel the existing queued requests in case the
    // plugin is updated as the new version of the plugin is expected to handle
    // these calls the same as the previous version did.
    //
    // In case the plugin is removed it is expected that all queued ResolvePluginL
    // calls will leave with KErrNotSupported, which will be sent to the caller.
    }

// ----------------------------------------------------------------------------
// Execute an async task. Leaves when an error is encountered.
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::ExecuteTaskL(TInt aTask, const CBufBase& aBuf)
    {
    __ASSERT_ALWAYS( !IsActive(), User::Panic(KMPXPlaylistEnginePanic, KErrInUse) );

    RBufReadStream readStream( aBuf );
    CleanupClosePushL( readStream );

    switch( aTask )
        {
        case EInternalizePlaylist:
            {
            // internalize playlist parameter
            iPlaylistUri = HBufC::NewL( readStream, readStream.ReadInt32L() );

            // automatically select a plugin that is capable of handling
            // the specified media file. SelectPlaylistPluginL leaves
            // with KErrNotSupported when an appropriate plugin cannot
            // be found
            iPluginHandler->SelectPlaylistPluginL( *iPlaylistUri, KNullDesC8 );

            // an appropriate plugin is found, issue the request to the
            // selected plugin
            iPluginHandler->Plugin()->InternalizePlaylistL( iStatus, *iPlaylistUri );
            }
            break;

        case EExternalizePlaylist:
            {
            // internalize playlist
            iPlaylist = CMPXMedia::NewL();
            readStream >> *iPlaylist;

            // internalize file path
            iFilePath = HBufC::NewL( readStream, readStream.ReadInt32L() );

            // internalize plugin Uid
            TUid pluginUid = TUid::Uid( readStream.ReadInt32L() );

            //
            // select the specified plugin. When the specified plugin
            // cannot be found, SelectPlaylistPluginL will leave with
            // KErrNotSupported error code.
            //
            // Possible scenario:
            // The specified plugin is available when the client made
            // the selection before issuing the request, but the plugin
            // has since been uninstalled when we are ready to process
            // this request.
            //
            iPluginHandler->SelectPlaylistPluginL( pluginUid );

            // found the specified plugin, issue the request
            iPluginHandler->Plugin()->ExternalizePlaylistL(
                    iStatus, *iPlaylist, *iFilePath );
            }
            break;

        //
        // command not supported
        //
        default:
            {
            ASSERT(0);
            break;
            }
        }

    CleanupStack::PopAndDestroy( &readStream ); // readStream

    SetActive();
    }

// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler ExecuteTaskL
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::HandleExecuteTaskError(TInt aTask, TInt aError)
    {
    TRAP_IGNORE(HandleExecuteTaskErrorL(aTask, aError));

    // clean up data set during processing of the current task
    Cleanup();

    // although an error has occured while processing the current task, we are
    // done with the current request and ready for the next one if there is
    // any
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler ExecuteTaskL
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::HandleExecuteTaskErrorL(TInt aTask, TInt aError)
    {
    MPX_DEBUG3("CMPXPlaylistEngine::HandleExecuteTaskErrorL(task %d, error %d)", aTask, aError);

    //
    // notify client
    //
    switch (aTask)
        {
        case EInternalizePlaylist:
            {
            iObserver.HandlePlaylistL( NULL, aError, ETrue );
            }
            break;

        case EExternalizePlaylist:
            {
            const TDesC& playlist =
                iPlaylist? iPlaylist->ValueText(KMPXMediaGeneralTitle) : KNullDesC;

            iObserver.HandlePlaylistL( playlist, aError );
            }
            break;

        default:
            {
            ASSERT(0);
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// cleanup
// ----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::Cleanup()
    {
    delete iPlaylistUri;
    iPlaylistUri = NULL;

    delete iFilePath;
    iFilePath = NULL;

    delete iPlaylist;
    iPlaylist = NULL;
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistEngine::GenerateTopCharacterSets()
// -----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::GenerateTopCharacterSetsL()
     {
     CArrayFixFlat<TInt>* installedLanguages = NULL;
     SysLangUtil::GetInstalledLanguages(installedLanguages);
     CleanupStack::PushL(installedLanguages);


     TParse* fp = new(ELeave) TParse();
     fp->Set(KMPXPlaylistCharacterSetRscFile, &KDC_RESOURCE_FILES_DIR, NULL);
     CleanupStack::PushL(fp);

     TFileName resourceFile = fp->FullName();
     User::LeaveIfError( MPXUser::CompleteWithDllPath( resourceFile ) );

     // Open the resource file
     TRAPD(err, iRscFile.OpenL(iFs, resourceFile));
     // if there is no resource file, then there is no top character set list
     if (err)
         {
          CleanupStack::PopAndDestroy(2, installedLanguages);
          return;
         }

     for (TInt i=0; i < installedLanguages->Count(); i++)
          {
          SelectCharacterSetsForLanguageL(installedLanguages->At(i));
          }
     iTopCharacterSet->Compress();
     installedLanguages->Reset();
     CleanupStack::PopAndDestroy(2, installedLanguages);
     iRscFile.Close();
     }

// -----------------------------------------------------------------------------
// CMetaDataParser::SelectCharacterSetsForLanguage()
// -----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::SelectCharacterSetsForLanguageL(TInt aLanguage)
     {
     switch ( aLanguage )
          {
          case ELangEnglish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ENGLISH_CHAR_SET);
               break;
          case ELangFrench:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_FRENCH_CHAR_SET);
               break;
          case ELangGerman:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_GERMAN_CHAR_SET);
               break;
          case ELangTurkish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_TURKISH_CHAR_SET);
               break;
          case ELangFinnish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_FINNISH_CHAR_SET);
               break;
          case ELangSwedish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SWEDISH_CHAR_SET);
               break;
          case ELangRussian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_RUSSIAN_CHAR_SET);
               break;
          case ELangArabic:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ARABIC_CHAR_SET);
               break;
          case ELangHebrew:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_HEBREW_CHAR_SET);
               break;
          case ELangFarsi:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_FARSI_CHAR_SET);
               break;
          case ELangItalian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ITALIAN_CHAR_SET);
               break;
          case ELangPolish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_POLISH_CHAR_SET);
               break;
          case ELangHungarian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_HUNGARIAN_CHAR_SET);
               break;
          case ELangSpanish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SPANISH_CHAR_SET);
               break;
          case ELangDutch:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_DUTCH_CHAR_SET);
               break;
          case ELangPortuguese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_PORTUGUESE_CHAR_SET);
               break;
          case ELangAmerican:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ENGLISH_CHAR_SET);
               break;
          case ELangCanadianFrench:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_FRENCH_CHAR_SET);
               break;
          case ELangBrazilianPortuguese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_PORTUGUESE_CHAR_SET);
               break;
          case ELangLatinAmericanSpanish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SPANISH_CHAR_SET);
               break;
          case ELangLatvian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_LATVIAN_CHAR_SET);
               break;
          case ELangGreek:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_GREEK_CHAR_SET);
               break;
          case ELangEstonian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ESTONIAN_CHAR_SET);
               break;
          case ELangLithuanian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_LITHUANIAN_CHAR_SET);
               break;
          case ELangRomanian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ROMANIAN_CHAR_SET);
               break;
          case ELangUkrainian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_UKRAINIAN_CHAR_SET);
               break;
          case ELangBulgarian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_BULGARIAN_CHAR_SET);
               break;
          case ELangCroatian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_CROATIAN_CHAR_SET);
               break;
          case ELangSerbian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SERBIAN_CHAR_SET);
               break;
          case ELangIndonesian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_INDONESIAN_CHAR_SET);
               break;
          case ELangMalay:
          case ELangTagalog:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_MALAY_CHAR_SET);
               break;
          case ELangIcelandic:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ICELANDIC_CHAR_SET);
               break;
          case ELangDanish:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_DANISH_CHAR_SET);
               break;
          case ELangNorwegian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_NORWEGIAN_CHAR_SET);
               break;
          case ELangHindi:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_INDIAN_CHAR_SET);
               break;
          case ELangUrdu:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_URDU_CHAR_SET);
               break;
          case ELangCzech:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_CZECH_CHAR_SET);
               break;
          case ELangSlovak:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SLOVAK_CHAR_SET);
               break;
          case ELangSlovenian:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_SLOVENIAN_CHAR_SET);
               break;
          case ELangTaiwanChinese:
          case ELangHongKongChinese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_TAIWAN_HK_CHINESE_CHAR_SET);
               break;
          case ELangPrcChinese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_CHINESE_CHAR_SET);
               break;
          case ELangEnglish_Taiwan:
          case ELangEnglish_Prc:
          case ELangEnglish_Japan:
          case ELangEnglish_Thailand:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_ENGLISH_CHAR_SET);
               break;
          case ELangJapanese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_JAPANESE_CHAR_SET);
               break;
          case ELangThai:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_THAI_CHAR_SET);
               break;
          case ELangVietnamese:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_VIETNAMESE_CHAR_SET);
               break;
          case ELangMalay_Apac:
               ReadCharacterSetResourceL(R_MPX_PLAYLIST_MALAY_CHAR_SET);
               break;
          default:
               break;
          }
     }

// -----------------------------------------------------------------------------
// CMPXPlaylistEngine::ReadCharacterSetResourceL()
// -----------------------------------------------------------------------------
//
void CMPXPlaylistEngine::ReadCharacterSetResourceL(TInt aResourceId)
     {
     TResourceReader rscReader;                       // Resource reader
     HBufC8* rscBuf;                                      // Buffer where resource is read

     rscBuf = iRscFile.AllocReadL(aResourceId);
     rscReader.SetBuffer(rscBuf);
     CleanupStack::PushL(rscBuf);

     TUint characterSetElementId;
     TInt numCharacterSetElements = rscReader.ReadInt16();
     TUint elemId;
     CCnvCharacterSetConverter::SCharacterSet elem;

     for (TInt i = 0; i < numCharacterSetElements; i++)
          {
          characterSetElementId = rscReader.ReadInt32();
          for (TInt j = 0; j < iCharacterSet->Count(); j++ )
               {
               elem = iCharacterSet->At(j);
               elemId = elem.Identifier();
               if ( elemId == characterSetElementId && !IsInTopCharacterSet(characterSetElementId) )
                    {
                    iTopCharacterSet->AppendL(elem);
                    }
               }
          }

     CleanupStack::PopAndDestroy(rscBuf);
     }

// -----------------------------------------------------------------------------
// CMPXPlaylistEngine::IsInTopCharacterSet()
// -----------------------------------------------------------------------------
//
TBool CMPXPlaylistEngine::IsInTopCharacterSet(TUint aCharacterSetId)
     {
     for (TInt i = 0; i < iTopCharacterSet->Count(); i++)
          {
          if ( iTopCharacterSet->At(i).Identifier() == aCharacterSetId )
               {
               return ETrue;
               }
          }
     return EFalse;
     }

// End of file
