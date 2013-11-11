/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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



#ifndef CLFCONSTS_H
#define CLFCONSTS_H

//  INCLUDES
#include <e32def.h>
#include <e32cmn.h>

// CONSTANTS
// Server version number
const TInt KCLFServerMajor( 1 );
const TInt KCLFServerCollectionManagerMajor( 2 );
const TInt KCLFServerMinor( 1 );
const TInt KCLFServerBuild( 1 );

// Message slots
// RCLFDatabase::PrepareItemIds
const TInt KCLFPrepareItemIdsMimeTypeSlot( 0 );
const TInt KCLFPrepareItemIdsMediaTypeSlot( 1 );
const TInt KCLFPrepareItemIdsDataSizeSlot( 2 );

// RCLFDatabase::FetchItemIds
// RCLFDatabase::FetchItemData
const TInt KCLFFetchDataSlot( 0 );

// RCLFDatabase::PrepareItems
const TInt KCLFPrepareItemsIdDataSlot( 0 );
const TInt KCLFPrepareItemsDataSizeSlot( 1 );

// RCLFEventHandler::GetUpdateEndEvent
const TInt KCLFGetUpdateEventDataSizeSlot( 0 );

// RCLFEventHandler::FetchItemListData
const TInt KCLFFetchItemListIdArraySlot( 0 );

// RCLFEventHandler::UpdateItems (id list)
const TInt KCLFUpdateItemsIdArraySlot( 0 );

// RCLFEventHandler::UpdateItems (opaque data)
const TInt KCLFUpdateItemsSemanticIdSlot( 0 );
const TInt KCLFUpdateItemsOpaqueDataSlot( 1 );

// Collection manager message slots
// RCLFCollectionManagerServer::PrepareCollectionInfoArray
const TInt KCLFPrepareCollectionInfoArrayDataSizePckg( 0 );

// RCLFCollectionManagerServer::FetchCollectionInfoArray
const TInt KCLFFetchCollectionInfoArraySlot( 0 );

// RCLFCollectionManagerServer::PrepareCollectionInfoById
const TInt KCLFPrepareCollectionInfoByIdSlot( 0 );
const TInt KCLFPrepareCollectionInfoByIdDataSizePckgSlot( 1 );

// RCLFCollectionManagerServer::FetchCollectionInfo
const TInt KCLFFetchCollectionInfoSlot( 0 );

// RCLFCollectionManagerServer::CreateCollection
const TInt KCLFCreateCollectionNameSlot( 0 );
const TInt KCLFCreateCollectionTypesSlot( 1 );
const TInt KCLFCreateCollectionPropertiesSlot( 2 );
const TInt KCLFCreateCollectionIdPckgSlot( 3 );

// RCLFCollectionManagerServer::DeleteCollection
const TInt KCLFDeleteCollectionIdSlot( 0 );

// RCLFCollectionManagerServer::AddToCollection
const TInt KCLFAddToCollectionIdSlot( 0 );
const TInt KCLFAddToCollectionItemArraySlot( 1 );
const TInt KCLFAddToCollectionRemoveFromOtherCollectionsSlot( 2 );

// RCLFCollectionManagerServer::RemoveFromCollection
const TInt KCLFRemoveFromCollectionIdSlot( 0 );
const TInt KCLFRemoveFromCollectionItemArraySlot( 1 );

// RCLFCollectionManagerServer::PrepareCollectionItemArray
const TInt KCLFPrepareCollectionItemArrayIdSlot( 0 );
const TInt KCLFPrepareCollectionDataSizePckgSlot( 1 );

// RCLFCollectionManagerServer::FetchCollectionItemArray
const TInt KCLFFetchCollectionItemArrayItemsSlot( 0 );

// RCLFCollectionManagerServer::PrepareCollectionIdByItem
const TInt KCLFPrepareCollectionIdByItemNameSlot( 0 );
const TInt KCLFPrepareCollectionIdByItemDataSizePckgSlot( 1 );

// RCLFCollectionManagerServer::FetchCollectionIdByItem
const TInt KCLFFetchCollectionIdByItemCollectionIdsSlot( 0 );

// RCLFCollectionManagerServer::GetCollectionManagerChangeEvent
const TInt KCLFGetCollectionManagerChangeEventParameterPckgSlot( 0 );
const TInt KCLFGetCollectionManagerChangeEventChangeEventPckgSlot( 1 );

// RCLFCollectionManagerServer::FetchChangedIds
const TInt KCLFFetchChangedCollectinIdsSlot( 0 );

// RCLFCollectionManagerServer::RenameCollection
const TInt KCLFRenameCollectionIdSlot( 0 );
const TInt KCLFRenameCollectionNewNameSlot( 1 );

// RCLFCollectionManagerServer::RemoveFromAllCollections
const TInt KCLFRemoveFromAllCollectionsItemArraySlot( 0 );

// RCLFCollectionManagerServer::GetDriveById
const TInt KCLFGetDriveIdSlot( 0 );
const TInt KCLFGetDriveCollectionDriveSlot( 1 );

// DATA TYPES
enum TCLFServerOpCodes
    {
    ECLFCancelPrepare       = 0x0, // RCLFDatabase::CancelPrepareItems
    ECLFCancelEvent         = 0x1, // RCLFEventHandler::CancelGetEvent
    ECLFPrepareItems        = 0x2, // RCLFDatabase::PrepareItems
    ECLFFetchItems          = 0x3, // RCLFDatabase::FetchItemData
    ECLFFetchChangedItems   = 0x4, // RCLFEventHandler::FetchItemListData
    ECLFUpdateItems         = 0x5, // RCLFEventHandler::UpdateItems
    ECLFUpdateAllItems      = 0x6, // RCLFEventHandler::UpdateAllItems
    ECLFGetUpdateEvent      = 0x7, // RCLFEventHandler::GetUpdateEndEvent
    ECLFUpdateItemsOpaqueData   = 0x8, // RCLFEventHandler::UpdateItems
    ECLFProcessStartEvent   = 0x9, // RCLFEventHandler::GetUpdateStartEvent
    ECLFCancelStartEvent    = 0xA, // RCLFEventHandler::CancelGetUpdateStartEvent
    ECLFFetchItemIds        = 0xB, // RCLFDatabase::FetchItemIds
    ECLFPrepareItemIds      = 0xC, // RCLFDatabase::PrepareItemIds
    ECLFCancelPrepareIds    = 0xD, // RCLFDatabase::CancelPrepareItemIds

    //Collection manager op codes
    ECLFPrepareCollectionById   = 0xE,
    ECLFFetchPreparedCollection = 0xF,
    ECLFPrepareCollectionInfoArray = 0x10,
    ECLFFetchCollectionInfoArray = 0x11,
    ECLFCreateCollection        = 0x12,
    ECLFDeleteCollection        = 0x13,
    ECLFRenameCollection        = 0x14,
    ECLFAddToCollection         = 0x15,
    ECLFRemoveFromCollection    = 0x16,
    ECLFPrepareCollectionItems  = 0x17,
    ECLFFetchCollectionItems    = 0x18,
    ECLFPrepareCollectionIdByItem = 0x19,
    ECLFFetchCollectionIdByItem = 0x1A,
    ECLFGetCollectionChangeEvent = 0x1B,
    ECLFFetchChangedCollectionIds = 0x1C,
    ECLFCancelCollectionEvent   = 0x1D,
    ECLFRemoveFromAllCollections = 0x1E,
    ECLFGetDriveById = 0x1F

    };

const TInt KCLFCheck( -43464 ); //Security check

enum TCMChangeEventParameters
    {
    ECLFCollectionDataChanged = 0x1,
    ECLFCollectionManagerError = 0x2
    };

// MACROS
#define IS_FLAG( flags, flag ) ( flags & flag )
#define SET_FLAG( flags, flag ) ( flags |= flag )
#define REMOVE_FLAG( flags, flag ) ( flags &= ~flag )

const TInt KCLFUpdateFoldersSemanticId = 0x1000;
const TInt KCLFUpdateCollectionsSemanticId = 0x1001;
const TInt KCLFItemsDeletedSemanticId = 0x1002;

#endif      // CLFCONSTS_H

// End of File
