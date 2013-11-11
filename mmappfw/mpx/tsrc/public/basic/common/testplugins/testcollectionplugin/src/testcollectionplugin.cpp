//*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of collection test plugin interface
*
*/

// INCLUDE FILES
#include <e32cmn.h>
#include <mpxcmn.h>
#include <mpxcollectionpluginobserver.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmessagecontainerdefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <stringloader.h>
#include <flogger.h>
#include "testcommonpluginuids.h"
#include "testcollectionplugin.h"

// CONSTANTS
_LIT(KFolder, "mpxt");
_LIT(KFile,   "CFwCollection.txt");

#define LOG1(a)             RFileLogger::Write(KFolder, KFile, EFileLoggingModeAppend, a)
#define LOG2(a,b)           RFileLogger::WriteFormat(KFolder, KFile, EFileLoggingModeAppend, a, b)
#define LOG3(a,b,c)         RFileLogger::WriteFormat(KFolder, KFile, EFileLoggingModeAppend, a, b, c)
#define LOG4(a,b,c,d)       RFileLogger::WriteFormat(KFolder, KFile, EFileLoggingModeAppend, a, b, c,d)


// =========================================== LOCAL FUNCTIONS ==============================================
// print out (index, id) for each level and the first 15 ids at top level
LOCAL_C void LogPath( const CMPXCollectionPath& aPath )\
    {
    TInt levels = aPath.Levels();
    TInt count = aPath.Count();
    LOG3(_L("Collection Path debug: levels=  %d, count = %d"), levels, count);
    for (TInt i=0; i<levels;++i)
        {
        LOG4(_L("Collection Path debug: Level[%d] = (index %d, id 0x%08x)"), i, 
                                                                             aPath.Index(i), 
                                                                             aPath.Id(i) );
        }
    if (count > 0)
        {
        TInt index = aPath.Index();
        CMPXCollectionPath* dpath = const_cast<CMPXCollectionPath*>(&aPath);
        dpath->SetToFirst();
        TInt c = 0;
        do
            {
            LOG3(_L("Collection Path debug top level: item %d = id 0x%08x"), c, dpath->Id());
            } while (++(*dpath) && ++c < 15 );
        dpath->Set(index);
        }
    }


// =========================================== MEMBER FUNCTIONS =============================================
// ----------------------------------------------------------------------------------------------------------
// Two-phased constructor. 
// ----------------------------------------------------------------------------------------------------------
//
CTestCollectionPlugin* CTestCollectionPlugin::NewL (TAny* /*aInitParams*/)
    {
    CTestCollectionPlugin* self = new (ELeave) CTestCollectionPlugin();
    CleanupStack::PushL (self);
    self->ConstructL ();
    CleanupStack::Pop (self);
    return self;        
    }

// ----------------------------------------------------------------------------------------------------------
// Destructor. 
// ----------------------------------------------------------------------------------------------------------
//
CTestCollectionPlugin::~CTestCollectionPlugin ()
    {
    }

// ----------------------------------------------------------------------------------------------------------
// Constructor. 
// ----------------------------------------------------------------------------------------------------------
//
CTestCollectionPlugin::CTestCollectionPlugin ()
    {
    }

// ----------------------------------------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::ConstructL ()
    {
    }

// ----------------------------------------------------------------------------------------------------------
// Navigates to the given path
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::OpenL(const CMPXCollectionPath& aPath,
                                      const TArray<TMPXAttribute>& aAttrs,
                                      CMPXFilter* aFilter)
    {
    LOG1(_L("CTestCollectionPlugin::OpenL"));
    LogPath( aPath );
    TMPXOpenMode openMode = aPath.OpenNextMode();
    TInt idIndex = aPath.Levels() - 1;
    LOG2(_L("TMPXOpendMode=%d"), openMode);
    LOG2(_L("aFilter=%x"), aFilter);
    
    RArray<TInt> supportedIds;
    CleanupClosePushL(supportedIds);
    supportedIds.AppendL(KMPXMediaIdContainer);
    supportedIds.AppendL(KMPXMediaIdGeneral);
    
    CMPXMedia* entries = CMPXMedia::NewL(supportedIds.Array());
    CleanupStack::PopAndDestroy(&supportedIds);  
    CleanupStack::PushL(entries);
    entries->SetTObjectValueL<TMPXItemId>(TMPXAttribute(KMPXMediaIdGeneral,EMPXMediaGeneralId), aPath.Id(idIndex));
    
    if(aPath.Levels() == 1)
        {
        // Plugin's root level
        RArray<TInt> ids;
        CleanupClosePushL( ids );
        ids.Append( 11 );
        ids.Append( 12 );
        ids.Append( 13 );
        ids.Append( 990 );  // for Playback Framework Test
        RArray<TMPXAttribute> attrs;
        CleanupClosePushL( attrs );
        attrs.Append( KMPXMediaGeneralId );
        attrs.Append( KMPXMediaGeneralUri );
        attrs.Append( KMPXMediaGeneralTitle );
        
        CMPXMediaArray* array=CMPXMediaArray::NewL();
        CleanupStack::PushL( array );
        
        const TArray<TMPXAttribute>& tempRef = aAttrs.Count() == 0? attrs.Array() : aAttrs;
        AddMediaL(*array, ids, tempRef);
        FilterMediaArray(*array, aFilter);
        
        entries->SetCObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents),array);
        entries->SetTObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayCount),array->Count());
        
        iObs->HandleOpen (entries, KErrNone);  
        CleanupStack::PopAndDestroy(3, &ids);   // attrs, array, ids
        }
    else if(aPath.Levels() == 2)
        {
        // Opening plugin's root menu
        TInt topLevId = aPath.Id(1);
        RArray<TInt> ids;
        CleanupClosePushL( ids );
        RArray<TMPXAttribute> attrs;
        CleanupClosePushL( attrs );
        CMPXMediaArray* array=CMPXMediaArray::NewL();
        CleanupStack::PushL( array );
        
        attrs.Append( KMPXMediaGeneralId );
        attrs.Append( KMPXMediaGeneralUri );
        attrs.Append( KMPXMediaGeneralTitle );                    
        if(topLevId == 11)
            {
            ids.Append( 211 );
            ids.Append( 212 );
            ids.Append( 213 );
            }
        else if(topLevId == 12)
            {
            ids.Append( 221 );
            }
        else if(topLevId == 13)
            {
            ids.Append( 231 );
            ids.Append( 232 );
            }
        else if(topLevId == 990)
            {
            ids.Append( 9901 );
            ids.Append( 9902 );
            ids.Append( 9903 );
            ids.Append( 9904 );
            ids.Append( 9905 );
            ids.Append( 9906 );
            ids.Append( 9907 );
            }
        const TArray<TMPXAttribute>& tempRef = aAttrs.Count() == 0? attrs.Array() : aAttrs;
        AddMediaL(*array, ids, tempRef);
        FilterMediaArray(*array, aFilter);
        
        entries->SetCObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents),array);
        entries->SetTObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayCount),array->Count());
        
        iObs->HandleOpen (entries, KErrNone);  
        CleanupStack::PopAndDestroy(3, &ids);   // attrs, array, ids
        }
    else if(aPath.Levels() == 3)
        {
        // Opening plugin's 2nd 
        TInt secLevId = aPath.Id(2);
        
        RArray<TInt> ids;
        CleanupClosePushL( ids );
        RArray<TMPXAttribute> attrs;
        CleanupClosePushL( attrs );
        CMPXMediaArray* array=CMPXMediaArray::NewL();
        CleanupStack::PushL( array );
        
        attrs.Append( KMPXMediaGeneralId );
        attrs.Append( KMPXMediaGeneralUri );
        attrs.Append( KMPXMediaGeneralTitle );
        switch( secLevId )
            {
            case 211:
                ids.Append( 3111 );
                break;
            case 212:
                ids.Append( 3121 );
                break;
            case 213:
                ids.Append( 3131 );
                break;
            case 221:
                // Empty
                break;
            case 231:
                ids.Append( 3311 );
                break;
            case 232:
                ids.Append( 3321 );
                break;
            };
        const TArray<TMPXAttribute>& tempRef = aAttrs.Count() == 0? attrs.Array() : aAttrs;
        AddMediaL(*array, ids, tempRef);
        FilterMediaArray(*array, aFilter);
        
        entries->SetCObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents),array);
        entries->SetTObjectValueL (TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayCount),array->Count());
        
        iObs->HandleOpen (entries, KErrNone);  
        CleanupStack::PopAndDestroy(3, &ids);   // attrs, array, ids
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy( entries );
    }

// ----------------------------------------------------------------------------------------------------------
// Get the extended properties of the current file (async) 
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::MediaL (const CMPXCollectionPath& aPath, 
                                        const TArray<TMPXAttribute>& aAttrs,
                                        const TArray<TCapability>& aCaps,
                                        CMPXAttributeSpecs* aSpecs)
    {
    LOG1(_L("CTestCollectionPlugin::MediaL"));
    LogPath( aPath );
    TMPXOpenMode openMode = aPath.OpenNextMode();
    //TInt idIndex = aPath.Levels() - 1;
    LOG2(_L("TMPXOpendMode=%d"), openMode);
    LOG2(_L("aAttrs.Count()=%d"), aAttrs.Count());
    LOG2(_L("aCaps.Count()=%d"), aCaps.Count());
    LOG2(_L("aSpecs=0x%x"), aSpecs);
    
    switch( (TInt)aPath.Id(1) )
        {
        case 14:    // Test0146
            {
            iObs->HandleMedia(NULL, KErrNone);
            break;
            }
        case 15:    // Test0147
            {
            CMPXMedia* media = CMPXMedia::NewL();
            CleanupStack::PushL( media );
            iObs->HandleMedia(media, KErrNone);
            CleanupStack::PopAndDestroy( media );
            break;
            }
        case 16:    // Test0148
            {
            CMPXMedia* media = CMPXMedia::NewL();
            CleanupStack::PushL( media );
            iObs->HandleMedia(media, KErrNotFound);
            CleanupStack::PopAndDestroy( media );
            break;
            }
        case 17:    // Test0149
            {
            CMPXMedia* media = CMPXMedia::NewL();
            CleanupStack::PushL( media );
            (*media) = *aSpecs;
            iObs->HandleMedia(media, KErrNone);
            CleanupStack::PopAndDestroy( media );
            break;
            }
        case 990:   // Test data for playback framework teests
            {
            CMPXMedia* media = CMPXMedia::NewL();
            CleanupStack::PushL( media );
                
            switch( (TInt)aPath.Id(2) )
                {
                case 9901:
                    AddAttrbuteL(*media, 9901, _L("song9901"), _L("\\song9901.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9901, aAttrs);
                    break;
                case 9902:
                    AddAttrbuteL(*media, 9902, _L("song9902"), _L("\\song9902.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9902, aAttrs);
                    break;
                case 9903:
                    AddAttrbuteL(*media, 9903, _L("song9903"), _L("\\song9903.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9903, aAttrs);
                    break;
                case 9904:
                    AddAttrbuteL(*media, 9904, _L("song9904"), _L("\\song9904.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9904, aAttrs);
                    break;
                case 9905:
                    AddAttrbuteL(*media, 9905, _L("song9905"), _L("\\song9905.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9905, aAttrs);
                    break;
                case 9906:
                    AddAttrbuteL(*media, 9906, _L("song9906"), _L("\\song9906.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9906, aAttrs);
                    break;
                case 9907:
                    AddAttrbuteL(*media, 9907, _L("song9907"), _L("\\song9907.ts"), aAttrs);
                    AddAttributePlaybackFwTestL(*media, 9907, aAttrs);
                    break;
                };
            iObs->HandleMedia(media, KErrNone);
            CleanupStack::PopAndDestroy( media );
            break;
            }
        default:    
            User::Leave( KErrNotFound );
        };    
    }

// ----------------------------------------------------------------------------------------------------------
// Cancel outstanding request 
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::CancelRequest()
    {
    LOG1(_L("CTestCollectionPlugin::CancelRequest"));
    }

// ----------------------------------------------------------------------------------------------------------
// Executes the given command on the collection 
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::CommandL(TMPXCollectionCommand aCmd, TInt aArg )
    {
    LOG1(_L("CTestCollectionPlugin::CommandL"));
    LOG3(_L("TMPXCollectionCommand=%d; aArg=%d"), aCmd, aArg);
    switch( aCmd )
        {
        case EMcCmdClose:
            LOG1( _L("McCmdClose command received"));
            break;
        case EMcCmdRefresh:
            LOG1( _L("EMcCmdRefresh command received"));
            break;
        case EMcCmdRemoveAll:
            LOG1( _L("EMcCmdRemoveAll command received"));
            break;
        case EMcCloseCollection:
            LOG1( _L("EMcCloseCollection command received"));
            break;
        case EMcReOpenCollection:
            LOG1( _L("EMcReOpenCollection command received"));
            break;
        case EMcRefreshStarted:
            LOG1( _L("EMcRefreshStarted command received"));
            break;
        case EMcRefreshEnded:
            LOG1( _L("EMcRefreshEnded command received"));
            break;
        case EMcCmdReCreateDB:
            LOG1( _L("EMcCmdReCreateDB command received"));
            break;
        case EMcCmdSelect:
            LOG1( _L("EMcCmdSelect command received"));
            break;
        case EMcCmdDbCorrupted:
            LOG1( _L("EMcCmdDbCorrupted command received"));
            break;
        case EMcCmdCollectionInit:
            LOG1( _L("EMcCmdCollectionInit command received"));
            break;
        case EMcCmdCollectionResyn:
            LOG1( _L("EMcCmdCollectionResyn command received"));
            break;
        default:
            break;
        };
    }

// ----------------------------------------------------------------------------------------------------------
// Executes the given command on the collection 
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::CommandL(CMPXCommand& aCmd)
    {
    LOG1(_L("CTestCollectionPlugin::CommandL"));
    const TDesC& title = aCmd.ValueText( KMPXMediaGeneralTitle );
    TMPXCommandId cmdId = aCmd.ValueTObjectL<TMPXCommandId>(KMPXCommandGeneralId);
    LOG2(_L("CommandId = %d"), cmdId);

    if(title == _L("CollectionPluginTest0175") )
        {
        }
    else if(title == _L("CollectionPluginTest0176") )
        {
        iObs->HandleCommandComplete(NULL, KErrNone);
        }
    else if(title == _L("CollectionPluginTest0177") )
        {
        iObs->HandleCommandComplete(NULL, KErrGeneral);
        }
    else if(title == _L("CollectionPluginTest0183") )
        {
        }
    else if(title == _L("CollectionPluginTest0184") )
        {
        CMPXCommand* newCmd = CMPXCommand::NewL();
        CleanupStack::PushL( newCmd );
        *newCmd = aCmd;
        iObs->HandleCommandComplete(newCmd, KErrNone);
        CleanupStack::PopAndDestroy( newCmd );
        }
    else
        {
        TBool syncOp(EFalse);
        if( aCmd.IsSupported(KMPXCommandGeneralDoSync) )
            {
            syncOp = *aCmd.Value<TBool>(KMPXCommandGeneralDoSync);
            }

        switch( cmdId )
            {
            case KMPXCommandIdCollectionRetrieveUriForDeletion:
                { // TODO
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionRetrieveUriForDeletion"));
                //DoRetrieveUriForDeletionL(aCmd);
                break;
                }        
            case KMPXCommandIdCollectionRemove:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionRemove"));
                //DoRemovePathL(aCmd);
                break;    
                }
            case KMPXCommandIdCollectionRemoveMedia:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionRemoveMedia"));
                //DoRemoveMediaL(aCmd);
                break;
                }
            case KMPXCommandIdCollectionCleanupDeletedMedias:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionCleanupDeletedMedias"));
                //DoCleanupDeletedMediasL(aCmd);
                break;
                }
            case KMPXCommandIdCollectionAdd:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectioAdd"));
                
                CMPXMedia& media = *aCmd.Value<CMPXMedia>(KMPXCommandColAddMedia);
                AddL(media);
                break;
                }
            case KMPXCommandIdCollectionSet:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionSet"));
                
                CMPXMedia& media = *aCmd.Value<CMPXMedia>(KMPXCommandColSetMedia);
                SetL( media );
                break;
                }
            case KMPXCommandIdCollectionCompleteDelete:
                {
                LOG1(_L("CTestCollectionPlugin::CommandL - KMPXCommandIdCollectionCompleteDelete"));
                //DoHandleDeleteCompleteL( aCmd );
                break;    
                }
            default:
                {
                User::Panic(_L("CTestCollectionPlugin::CommandL panic"), 1); // magic number
                }
            }
        if (!syncOp)
            {    
            iObs->HandleCommandComplete( NULL, KErrNone );
            }
        }
    }

// ----------------------------------------------------------------------------------------------------------
// Adds an item (song or playlist) to the collection
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddL (const CMPXMedia& aMedia)
    {
    LOG1(_L("CTestCollectionPlugin::AddL"));
    CMPXMessage* message = CMPXMessage::NewL();
    CleanupStack::PushL( message );
    
    const TDesC& title = aMedia.ValueText( KMPXMediaGeneralTitle );
    
    if(title == _L("CollectionPluginTest0078") )
        {
        FillItemChangeMessageL(*message, 78, EMPXItemInserted, EMPXOther);
        }
    else if(title == _L("CollectionPluginTest0079"))
        {
        FillItemChangeMessageL(*message, 79, EMPXItemInserted, EMPXSong);
        }
    else if(title == _L("CollectionPluginTest0080"))
        {
        FillItemChangeMessageL(*message, 80, EMPXItemInserted, EMPXPlaylist);
        }
    else
        {
        User::Panic(_L("CTestCollectionPlugin::AddL panic"), 1); // magic number
        }
    iObs->HandleMessage( *message );
    CleanupStack::PopAndDestroy( message );
    }

// ----------------------------------------------------------------------------------------------------------
// Remove an item from the collection database using the given path
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::RemoveL (const CMPXCollectionPath& aPath)
    {
    LOG1(_L("CTestCollectionPlugin::RemoveL"));
    LogPath( aPath );
    CDesCArrayFlat* fp = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL( fp );
    switch( aPath.Levels() )
        {
        case 2:
            if(aPath.Id() == 14)
                {   // Test0091
                iObs->HandleRemove( *fp, KErrNotFound );
                }
            break;
        case 3:
            if(aPath.Id() == 212)
                {   // Test0090
                fp->AppendL( _L("\\song3121.ts") );
                fp->AppendL( _L("\\song3122.ts") );
                fp->AppendL( _L("\\song3123.tp") );
                iObs->HandleRemove( *fp, KErrNone );
                }
            break;
        case 4:
            if(aPath.Id() == 3111)
                {   // Test0088
                fp->AppendL( _L("\\song3111.ts") );
                iObs->HandleRemove( *fp, KErrNone );
                }
            break;
        default:
            User::Panic(_L("CTestCollectionPlugin::RemoveL panic"), 1); // magic number
        }
    CleanupStack::PopAndDestroy( fp );
    }
    
// ----------------------------------------------------------------------------------------------------------
// Remove an item from the collection database using the given media properties
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::RemoveL (const CMPXMedia& aMedia)
    {
    LOG1(_L("CTestCollectionPlugin::RemoveL"));
    const TDesC& title = aMedia.ValueText( KMPXMediaGeneralTitle );
    
    // a list of change event messages a result of the item being removed
    CMPXMessageArray* itemChangedMessages = CMPXMessageArray::NewL();
    CleanupStack::PushL(itemChangedMessages);
    
    if(title == _L("CollectionPluginTest0099") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 99, EMPXItemDeleted, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else if(title == _L("CollectionPluginTest0100") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 100, EMPXItemDeleted, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        message = NULL;
        message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 100, EMPXItemDeleted, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else
        {
        User::Panic(_L("CTestCollectionPlugin::RemoveL panic"), 1); // magic number
        }
    
    CMPXMessage* message = CMPXMessage::NewL();
    CleanupStack::PushL(message);
    
    message->SetTObjectValueL<TMPXMessageId>(
        TMPXAttribute(KMPXMessageContentIdGeneral, EMPXMessageGeneralId), KMPXMessageIdItemChanged);    
    message->SetCObjectValueL(
        TMPXAttribute(KMPXMessageIdContainer, EMPXMessageArrayContents), itemChangedMessages);
    message->SetTObjectValueL(
        TMPXAttribute(KMPXMessageIdContainer, EMPXMessageArrayCount), itemChangedMessages->Count());   
    
    iObs->HandleMessage( *message );
    CleanupStack::PopAndDestroy(2, itemChangedMessages);    // message, itemChangedMessages
    }

// ----------------------------------------------------------------------------------------------------------
// Sets/updates the media for an item in the collection
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::SetL (const CMPXMedia& aMedia)
    {
    LOG1(_L("CTestCollectionPlugin::SetL"));
    const TDesC& title = aMedia.ValueText( KMPXMediaGeneralTitle );
    
    // a list of change event messages a result of the item being removed
    CMPXMessageArray* itemChangedMessages = CMPXMessageArray::NewL();
    CleanupStack::PushL(itemChangedMessages);
    
    if(title == _L("CollectionPluginTest0108") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 108, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else if(title == _L("CollectionPluginTest0109") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 109, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        message = NULL;
        message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 109, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else if(title == _L("CollectionPluginTest0119") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 119, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else if(title == _L("CollectionPluginTest0120") )
        {
        CMPXMessage* message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 120, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        message = NULL;
        message = CMPXMessage::NewL();
        CleanupStack::PushL(message);
        FillItemChangeMessageL(*message, 120, EMPXItemModified, EMPXSong);
        itemChangedMessages->AppendL( *message );
        CleanupStack::PopAndDestroy(message); 
        }
    else
        {
        // Playback Framework may call this function 
        //User::Panic(_L("CTestCollectionPlugin::SetL panic"), 1); // magic number
        }
    
    CMPXMessage* message = CMPXMessage::NewL();
    CleanupStack::PushL(message);
    
    message->SetTObjectValueL<TMPXMessageId>(
        TMPXAttribute(KMPXMessageContentIdGeneral, EMPXMessageGeneralId), KMPXMessageIdItemChanged);    
    message->SetCObjectValueL(
        TMPXAttribute(KMPXMessageIdContainer, EMPXMessageArrayContents), itemChangedMessages);
    message->SetTObjectValueL(
        TMPXAttribute(KMPXMessageIdContainer, EMPXMessageArrayCount), itemChangedMessages->Count());   
    
    iObs->HandleMessage( *message );
    CleanupStack::PopAndDestroy(2, itemChangedMessages);    // message, itemChangedMessages
    }
    
// ----------------------------------------------------------------------------------------------------------
// Find the items matching the media specifications
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::FindAllL(const CMPXMedia& aCriteria, const TArray<TMPXAttribute>& aAttrs)
    {
    LOG1(_L("CTestCollectionPlugin::FindAllL"));
    const TDesC& title = aCriteria.ValueText( KMPXMediaGeneralTitle );
    if(title == _L("CollectionPluginTest0128") ||
       title == _L("CollectionPluginTest0129") )
        {
        // Return exact copy
        CMPXMedia* media = CMPXMedia::NewL();
        (*media) = aCriteria;
        CleanupStack::PushL( media );
        media->SetTObjectValueL<TInt>(KMPXMediaGeneralSize, aAttrs.Count());
        iObs->HandleFindAll(media, KErrNone);
        CleanupStack::PopAndDestroy( media );
        }
    else if(title == _L("CollectionPluginTest0130") )
        {
        // Return error
        CMPXMedia* media = CMPXMedia::NewL();
        (*media) = aCriteria;
        CleanupStack::PushL( media );
        media->SetTObjectValueL<TInt>(KMPXMediaGeneralSize, aAttrs.Count());
        iObs->HandleFindAll(media, KErrArgument);
        CleanupStack::PopAndDestroy( media );
        }
    else        
        {
        User::Panic(_L("CTestCollectionPlugin::FindAllL panic"), 1); // magic number
        }
    }

// ----------------------------------------------------------------------------------------------------------
// Find the items matching the media specifications
// ----------------------------------------------------------------------------------------------------------
//
CMPXMedia* CTestCollectionPlugin::FindAllSyncL(const CMPXMedia& aCriteria, const TArray<TMPXAttribute>& aAttrs)
    {
    LOG1(_L("CTestCollectionPlugin::FindAllSyncL"));
    const TDesC& title = aCriteria.ValueText( KMPXMediaGeneralTitle );
    CMPXMedia* media = NULL;
    if(title == _L("CollectionPluginTest0138") ||
       title == _L("CollectionPluginTest0139"))
        {
        // Return exact copy
        media = CMPXMedia::NewL();
        CleanupStack::PushL( media );
        (*media) = aCriteria;
        media->SetTObjectValueL<TInt>(KMPXMediaGeneralSize, aAttrs.Count());
        CleanupStack::Pop( media );
        }
    else if(title == _L("CollectionPluginTest0140") )
        {
        media = NULL;
        }
    else        
        {
        User::Panic(_L("CTestCollectionPlugin::FindAllL panic"), 1); // magic number
        }
    return media;
    }
                                
// ----------------------------------------------------------------------------------------------------------
// Get the list of supported capabilities
// ----------------------------------------------------------------------------------------------------------
//
TCollectionCapability CTestCollectionPlugin::GetCapabilities()
    {
    LOG1(_L("CTestCollectionPlugin::GetCapabilities - EMcNotModifiable"));
    return EMcNotModifiable;
    }

// ----------------------------------------------------------------------------------------------------------
// Add specify attribute to Media
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddMediaL(CMPXMediaArray& aMediaArray, 
                                          const RArray<TInt> aIdArray, 
                                          const TArray<TMPXAttribute>& aAttrs)
    {
    TInt idCnt = aIdArray.Count();
    for(TInt i = 0; i < idCnt; i++)
        {
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL( media );
        switch( aIdArray[i] )
            {
            case 11:
                AddAttrbuteL(*media, 11, _L("level11"), KNullDesC, aAttrs);
                break;
            case 12:
                AddAttrbuteL(*media, 12, _L("level12"), KNullDesC, aAttrs);
                break;
            case 13:
                AddAttrbuteL(*media, 13, _L("level13"), KNullDesC, aAttrs);
                break;
            case 211:
                AddAttrbuteL(*media, 211, _L("level211"), KNullDesC, aAttrs);
                break;
            case 212:
                AddAttrbuteL(*media, 212, _L("level212"), KNullDesC, aAttrs);
                break;
            case 213:
                AddAttrbuteL(*media, 213, _L("level213"), KNullDesC, aAttrs);
                break;
            case 221:
                AddAttrbuteL(*media, 221, _L("level221"), KNullDesC, aAttrs);
                break;
            case 231:
                AddAttrbuteL(*media, 231, _L("level231"), KNullDesC, aAttrs);
                break;
            case 232:
                AddAttrbuteL(*media, 232, _L("level232"), KNullDesC, aAttrs);
                break;
            case 3111:
                AddAttrbuteL(*media, 3111, _L("song3111"), _L("\\song3111.ts"), aAttrs);
                break;
            case 3121:
                AddAttrbuteL(*media, 3121, _L("song3121"), _L("\\song3121.ts"), aAttrs);
                break;
            case 3131:
                AddAttrbuteL(*media, 3131, _L("playlist3131"), _L("\\playlist3131.tp"), aAttrs);
                break;
            case 3311:
                AddAttrbuteL(*media, 3311, _L("song3311"), _L("\\song3311.ts"), aAttrs);
                break;
            case 3321:
                AddAttrbuteL(*media, 3321, _L("playlist3321"), _L("\\playlist3321.tp"), aAttrs);
                break;
            case 990:
                AddAttrbuteL(*media, 990, _L("level990"), KNullDesC, aAttrs);
                break;
            case 9901:
                AddAttrbuteL(*media, 9901, _L("song9901"), _L("\\song9901.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9901, aAttrs);
                break;
            case 9902:
                AddAttrbuteL(*media, 9902, _L("song9902"), _L("\\song9902.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9902, aAttrs);
                break;
            case 9903:
                AddAttrbuteL(*media, 9903, _L("song9903"), _L("\\song9903.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9903, aAttrs);
                break;
            case 9904:
                AddAttrbuteL(*media, 9904, _L("song9904"), _L("\\song9904.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9904, aAttrs);
                break;
            case 9905:
                AddAttrbuteL(*media, 9905, _L("song9905"), _L("\\song9905.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9905, aAttrs);
                break;
            case 9906:
                AddAttrbuteL(*media, 9906, _L("song9906"), _L("\\song9906.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9906, aAttrs);
                break;
            case 9907:
                AddAttrbuteL(*media, 9907, _L("song9907"), _L("\\song9907.ts"), aAttrs);
                AddAttributePlaybackFwTestL(*media, 9907, aAttrs);
                break;
            };
        aMediaArray.AppendL( media );
        CleanupStack::Pop( media );
        }
    }


// ----------------------------------------------------------------------------------------------------------
// Add specify attribute to Media
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttrbuteL(CMPXMedia& aMedia, 
                                             const TInt aId,
                                             const TDesC& aTitle,
                                             const TDesC& aUri,
                                             const TArray<TMPXAttribute>& aAttrs)
    {
    TInt attrCnt = aAttrs.Count();
    for(TInt i = 0; i < attrCnt; i++)
        {
        if(aAttrs[i].ContentId() == KMPXMediaIdGeneral)
            {
            TInt attrId = aAttrs[i].AttributeId();
            if(attrId &  EMPXMediaGeneralId)
                {
                aMedia.SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, aId);
                }
            if(attrId &  EMPXMediaGeneralTitle)
                {
                aMedia.SetTextValueL(KMPXMediaGeneralTitle, aTitle);
                
                }
            if(attrId &  EMPXMediaGeneralUri)
                {
                aMedia.SetTextValueL(KMPXMediaGeneralUri, aUri);
                }
            }
        }
    }

// ----------------------------------------------------------------------------------------------------------
// Add specify attribute to Media specific for Playback framework
// test cases
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributePlaybackFwTestL(CMPXMedia& aMedia, 
                                                            const TInt aId,
                                                            const TArray<TMPXAttribute>& aAttrs)
    {
    TInt attrCnt = aAttrs.Count();
    for(TInt i = 0; i < attrCnt; i++)
        {
        TInt attrId = aAttrs[i].AttributeId();
        if(aAttrs[i].ContentId() == KMPXMediaIdGeneral)
            {
            if(attrId & EMPXMediaGeneralType)
                {
                AddAttributeGeneralTypeL(aMedia, aId);
                }
            if(attrId & EMPXMediaGeneralCategory)
                {
                AddAttributeGeneralCategoryL(aMedia, aId);
                }
            if(attrId & EMPXMediaGeneralDuration)
                {
                AddAttributeGeneralDurationL(aMedia, aId);
                }
            if(attrId & EMPXMediaGeneralLastPlaybackPosition)
                {
                AddAttributeGeneralLastPlaybackPositionL(aMedia, aId);
                }
            }
        else if(aAttrs[i].ContentId() == KMPXMediaIdMusic)
            {
            if(attrId & EMPXMediaMusicArtist)
                {
                AddAttributeMusicArtistL(aMedia, aId);
                }
            }
        else if(aAttrs[i].ContentId() == KMPXMediaIdDrm)
            {
            if(attrId & EMPXMediaDrmType)
                {
                AddAttributeDrmTypeL(aMedia, aId);
                }
            if(attrId & EMPXMediaDrmProtected)
                {
                AddAttributeDrmProtectedL(aMedia, aId);
                }
            if(attrId & EMPXMediaDrmRightsStatus)
                {
                AddAttributeDrmRightsStatusL(aMedia, aId);
                }
            }
        }
    }
        
// ----------------------------------------------------------------------------------------------------------
// Filter out media in aMediaArray which match aFilter
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::FilterMediaArray(CMPXMediaArray& aMediaArray, CMPXFilter* aFilter)
    {
    if(aFilter )
        {
        TArray<TMPXAttribute> filterAttr = aFilter->Attributes();
        TInt arrCnt = aMediaArray.Count();
        for(TInt i = arrCnt-1; i >= 0; i--) // Remove from the back
            {
            CMPXMedia* media = aMediaArray[i];
            for(TInt ii = 0; ii < filterAttr.Count(); ii++)
                {
                TMPXAttribute attr = filterAttr[ii];
                if( media->IsSupported( attr ) )
                    {
                    TBool match = EFalse;
                    if(attr == KMPXMediaGeneralId)
                        {
                        TInt filterId = *aFilter->Value<TInt>( attr );
                        TInt mediaId = *media->Value<TInt>( attr );
                        if(filterId == mediaId)
                            match = ETrue;
                        }
                    else if(attr == KMPXMediaGeneralTitle || attr == KMPXMediaGeneralUri)
                        {
                        const TDesC& filterText = aFilter->ValueText( attr );
                        const TDesC& mediaText = media->ValueText( attr );
                        if(filterText == mediaText)
                            match = ETrue;
                        }
                    if( match )
                        {
                        aMediaArray.Remove( i );
                        break;
                        }
                    }
                }
            }
        }
    }

// ----------------------------------------------------------------------------------------------------------
// Fill up media with change message
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::FillItemChangeMessageL(CMPXMessage& aMessage,
                                                       TMPXItemId aId,
                                                       TMPXChangeEventType aChangeType,
                                                       TMPXGeneralCategory aCategory)
    {
    LOG1( _L("Preparing HandleMessage callback") );
    aMessage.SetTObjectValueL<TMPXMessageId>(
        TMPXAttribute(KMPXMessageContentIdGeneral, EMPXMessageGeneralId), KMPXMessageIdItemChanged);

    aMessage.SetTObjectValueL<TUid>(
        TMPXAttribute(KMPXMessageIdItemChanged, EMPXMessageCollectionId), TUid::Uid(KCollectionTestPluginImpId));
        
    aMessage.SetTObjectValueL<TMPXChangeEventType>(
        TMPXAttribute(KMPXMessageIdItemChanged, EMPXMessageChangeEventType), aChangeType);
        
    aMessage.SetTObjectValueL<TMPXGeneralCategory>(
        TMPXAttribute(KMPXMessageIdItemChanged, EMPXMessageMediaGeneralCategory), aCategory);
        
    aMessage.SetTObjectValueL<TMPXItemId>(
        TMPXAttribute(KMPXMessageIdItemChanged, EMPXMessageMediaGeneralId), aId);
    }


// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeGeneralTypeL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
        case 9902:
        case 9903:
        case 9904:
        case 9905:
        case 9906:
        case 9907:
            aMedia.SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
            break;
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeGeneralCategoryL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
        case 9902:
        case 9903:
        case 9904:
        case 9905:
        case 9906:
        case 9907:
            aMedia.SetTObjectValueL<TMPXGeneralCategory>(KMPXMediaGeneralCategory, EMPXSong);
            break;
        }
    }

// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeGeneralDurationL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 0);         // 0sec
            break;
        case 9902:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 1000);   // 1sec
            break;
        case 9903:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 2000);   // 2sec
            break;
        case 9904:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 5000);   // 5sec
            break;
        case 9905:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 10000);   // 10sec
            break;
        case 9906:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 20000);   // 20sec
            break;
        case 9907:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralDuration, 15000);   // 15sec
            break;
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeGeneralLastPlaybackPositionL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 0);         // 0sec
            break;
        case 9902:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 0);         // 0sec
            break;
        case 9903:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 1000);   // 1sec
            break;
        case 9904:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 2500);   // 2.5sec
            break;
        case 9905:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 0);   // 0sec
            break;
        case 9906:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 0);   // 0sec
            break;
        case 9907:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaGeneralLastPlaybackPosition, 0);   // 0sec
            break;
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeMusicArtistL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9901"));
            break;
        case 9902:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9902"));
            break;
        case 9903:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9903"));
            break;
        case 9904:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9904"));
            break;
        case 9905:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9905"));
            break;
        case 9906:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9906"));
            break;
        case 9907:
            aMedia.SetTextValueL(KMPXMediaMusicArtist, _L("artist9907"));
            break;
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeDrmTypeL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            // Specify nothing
            break;
        case 9902:
            // Specify nothing
            break;
        case 9903:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaDrmType, EMPXDrmTypeOMA);
            break;
        case 9904:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaDrmType, EMPXDrmTypeWMA);
            break;
        case 9905:
            // Specify nothing
            break;
        case 9906:
            // Specify nothing
            break;
        case 9907:
            // Specify nothing
            break;    
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeDrmProtectedL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, EFalse);
            break;
        case 9902:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, ETrue);
            break;
        case 9903:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, EFalse);
            break;
        case 9904:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, ETrue);
            break;
        case 9905:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, EFalse);
            break;
        case 9906:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, EFalse);
            break;
        case 9907:
            aMedia.SetTObjectValueL<TBool>(KMPXMediaDrmProtected, EFalse);
            break;
        }
    }
    
// ----------------------------------------------------------------------------------------------------------
// Adding specific attribute
// ----------------------------------------------------------------------------------------------------------
//
void CTestCollectionPlugin::AddAttributeDrmRightsStatusL(CMPXMedia& aMedia, const TInt aId)
    {
    switch( aId )
        {
        case 9901:
            // Specify nothing
            break;
        case 9902:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaDrmRightsStatus, EMPXDrmRightsFull);
            break;
        case 9903:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaDrmRightsStatus, EMPXDrmRightsExpired);
            break;
        case 9904:
            aMedia.SetTObjectValueL<TInt>(KMPXMediaDrmRightsStatus, EMPXDrmRightsPreview);
            break;
        case 9905:
            // Specify nothing
            break;
        case 9906:
            // Specify nothing
            break;
        case 9907:
            // Specify nothing
            break;
        }
    }
    
// End of file
