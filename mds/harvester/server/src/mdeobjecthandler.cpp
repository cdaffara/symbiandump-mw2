/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*/

#include "mdeobjecthandler.h"
#include "mdesession.h"
#include "harvesterdata.h"
#include "harvesterlog.h"
#include "harvesterplugin.h"
#include "mdeconstants.h"
#include "mdsutils.h"
#include <harvesterclientdata.h>

using namespace MdeConstants;

CMdeObjectHandler::CMdeObjectHandler( CMdESession& aSession ) : iDefaultNamespace( NULL )
    {
	iMdeSession = &aSession;
    }


CMdeObjectHandler::~CMdeObjectHandler()
    {
	iFs.Close();
    }

CMdeObjectHandler* CMdeObjectHandler::NewLC( CMdESession& aSession )
    {
	CMdeObjectHandler* self = new (ELeave)CMdeObjectHandler( aSession );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }

CMdeObjectHandler* CMdeObjectHandler::NewL( CMdESession& aSession )
    {
	CMdeObjectHandler* self=CMdeObjectHandler::NewLC( aSession );
	CleanupStack::Pop(); // self;
	return self;
    }

void CMdeObjectHandler::ConstructL()
    {
	iDefaultNamespace = &iMdeSession->GetDefaultNamespaceDefL();
	User::LeaveIfError( iFs.Connect() );
    }

void CMdeObjectHandler::SetMetadataObjectL( CHarvesterData& aHD )
	{
	WRITELOG( "CMdeObjectHandler::SetMetadataObjectL()" );
    
    TTime time( 0 );
    time.UniversalTime();
    CMdEEventDef* eventDef = NULL;
    CMdEObject& mdeObject = aHD.MdeObject();
    
    const THarvesterEventType type( aHD.EventType() );
    if ( type == EHarvesterAdd )
        {
        if( aHD.IsBinary() && !MdsUtils::FileExists( iFs, mdeObject.Uri() )) 
        	{
            const TDesC16& uri = mdeObject.Uri();
            WRITELOG1( "CMdeObjectHandler::SetMetadataObjectL - File doesn't exist any more! Uri: %S", &uri );
            aHD.SetErrorCode( KMdEErrHarvestingFailedPermanent );
            return;
            }
        
        const TInt id( iMdeSession->AddObjectL( mdeObject ) );
        if( id == KNoId )
            {
            // If add failure occures, retry
            iMdeSession->AddObjectL( mdeObject );
            }
        
        if ( mdeObject.Id() == 0 )
            {
            const TDesC16& uri = mdeObject.Uri();
            WRITELOG1( "CMdeObjectHandler::SetMetadataObjectL - Adding to db failed! Uri: %S", &uri );
            aHD.SetErrorCode( KMdEErrHarvestingFailedPermanent );
            return;
            }
        
        // Since event was EHarvesterAdd
        eventDef = &iDefaultNamespace->GetEventDefL( Events::KCreated );
        }
    else if ( type== EHarvesterEdit )
        {    
        iMdeSession->CommitObjectL( mdeObject );
        eventDef = &iDefaultNamespace->GetEventDefL( Events::KEdited );        
        }    

    RPointerArray<CMdEInstanceItem> relationEventArray;
    TCleanupItem cleanupItem( MdsUtils::CleanupPtrArray<CMdEInstanceItem>, &relationEventArray );
    CleanupStack::PushL( cleanupItem );

    // If additional parameters are given, add relations between metadata objects and albums.
    if ( aHD.ClientData() )
        { 
        WRITELOG( "CMdeObjectHandler::SetMetadataObjectL() - Creating album relations." );
        
        CMdERelationDef& albumRelationDef = iDefaultNamespace->
        		GetRelationDefL( Relations::KContains );        
        
        TItemId objectId = mdeObject.Id();
        if ( objectId != 0 )
            {
            CHarvestClientData* clientData = STATIC_CAST( CHarvestClientData*, aHD.ClientData() );
            const TInt count = clientData->iAlbumIds.Count();
            for ( TInt i = 0 ; i < count; i++ )
                {
                TItemId albumId = clientData->iAlbumIds[i];

                WRITELOG1( "CMdeObjectHandler::SetMetadataObjectL() - AlbumID: %d", albumId );
                CMdERelation* relation =  
               		iMdeSession->NewRelationLC(
                		albumRelationDef, albumId, objectId, 0 );
                relationEventArray.AppendL( STATIC_CAST( CMdEInstanceItem*, relation ) );
                CleanupStack::Pop(); //relation
                }
            }
        }
    
    if ( eventDef )
        {
        WRITELOG( "CMdeObjectHandler::SetMetadataObjectL() - Creating event array" );
        const TItemId objectId = mdeObject.Id();
        if ( objectId != 0 )
            {
            CMdEEvent* event = iMdeSession->NewEventLC( 
            		*eventDef, objectId, time );
            relationEventArray.AppendL( STATIC_CAST( CMdEInstanceItem*, event ) );
            CleanupStack::Pop(); // event
            }
        }

    if ( relationEventArray.Count() > 0 )
        {
#ifdef _DEBUG
        WRITELOG1( "CMdeObjectHandler::SetMetadataObjectL() - Event and relation array count: %d", relationEventArray.Count() );
#endif
        iMdeSession->AddItemsL( relationEventArray );
        }        

    CleanupStack::PopAndDestroy( &relationEventArray );
	}

// ---------------------------------------------------------------------------
// GetMetadataObjectL
// ---------------------------------------------------------------------------
//
CMdEObject* CMdeObjectHandler::GetMetadataObjectL( CHarvesterData& aHD, const TDesC& aObjectDefStr )
    {
    WRITELOG( "CMdeObjectHandler::GetMetadataObjectL()" );
    
    CMdEObjectDef& mdeObjectDef = 
    	iDefaultNamespace->GetObjectDefL( aObjectDefStr );
    	
    if ( aHD.EventType() == EHarvesterEdit )
        {
        if( aHD.IsBinary() )
        	{
        	// Checking whether file is open
        	TBool isOpen( EFalse );
        	TInt error = iFs.IsFileOpen( aHD.Uri(), isOpen );
            if ( error != KErrNone )
                {
                WRITELOG1( "CMdeObjectHandler::GetMetadataObjectL() - file error %d", error );
                User::Leave( error );
                }
        	if ( isOpen )
        		{
        		// The file can be open, as long as it is not open for modification anywhere
                RFile64 tempFile;
                error = tempFile.Open( iFs, aHD.Uri(), EFileRead | EFileShareReadersOnly );
                if( error != KErrNone )
                    {
#ifdef _DEBUG
                    if( error == KErrInUse || error == KErrLocked )
                        {
                        TPtrC fileName( aHD.Uri().Mid(2) );
                        WRITELOG1( "CMdeObjectHandler :: Checking open file handles to %S", &fileName );

                        CFileList* fileList = 0;
                        TOpenFileScan fileScan( iFs );

                        fileScan.NextL( fileList );   
          
                        while ( fileList )   
                            {
                            const TInt count( fileList->Count() ); 
                            for (TInt i = 0; i < count; i++ )   
                                {   
                                if ( (*fileList)[i].iName == aHD.Uri().Mid(2) )
                                    {
                                    TFullName processName;
                                    TFindThread find(_L("*"));
                                    while( find.Next( processName ) == KErrNone )
                                        {
                                        RThread thread;
                                        TInt err = thread.Open( processName );
             
                                        if ( err == KErrNone )
                                            {
                                            if ( thread.Id().Id() ==  fileScan.ThreadId() )
                                                {
                                                processName = thread.Name();
                                                thread.Close();
                                                WRITELOG1( "CMdeObjectHandler:: %S has a file handle open", &processName );
                                                break;
                                                }
                                            thread.Close();
                                            }
                                        }
                                    }
                                }
                            fileScan.NextL( fileList );   
                            } 
                        }
#endif
                    WRITELOG( "CMdeObjectHandler::GetMetadataObjectL() - file handle is open! Returning." );
                    if( error == KErrInUse || error == KErrLocked )
                        {
                        // If file is locked, push it to reharvesting queue
                        User::Leave( error );
                        }
                    else
                        {
                        return NULL;
                        }
                    }
                tempFile.Close();
        		}
        	}

        WRITELOG( "CMdeObjectHandler::GetMetadataObjectL() - open object" );
        return iMdeSession->OpenObjectL( aHD.Uri(), mdeObjectDef );
        }

    WRITELOG( "CMdeObjectHandler::GetMetadataObjectL() - new object" );
    CMdEObject* mdeObject = iMdeSession->NewObjectL( mdeObjectDef, aHD.Uri() );

    return mdeObject;
    }
