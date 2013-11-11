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
* Description:  Active object to extract metadata 
*  Version     : %version: da1mmcf#16.2.3.1.5.2.1 % 
*
*/


#include <e32base.h>
#include <f32file.h>
#include <badesca.h>
#include <apgcli.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectionpath.h>
#include <mpxcollectiontype.h>
#include <mpxdrmmediautility.h>
#include <mpxlog.h>
#include <mpxmetadataextractor.h>
#include "mpxmetadatascanner.h"
#include "mpxmetadatascanobserver.h"
#include "mpxfilescanstateobserver.h"

// CONSTANTS
const TInt KLoopCount = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CMPXMetadataScanner::CMPXMetadataScanner( MMPXMetadataScanObserver& aObs,
                                          MMPXFileScanStateObserver& aStateObs )
                                      : CActive( EPriorityNull ),
                                        iExtractType( EMaxFile ),
                                        iObserver( aObs ),
                                        iStateObserver( aStateObs )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXMetadataScanner::ConstructL( RFs& aFs, 
                                      RApaLsSession& aAppArc,
                                      RPointerArray<CMPXCollectionType>& aTypesAry )
    {
    iExtractor = CMPXMetadataExtractor::NewL( aFs, aAppArc, aTypesAry );
    iTargetProps = CMPXMediaArray::NewL();
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXMetadataScanner* CMPXMetadataScanner::NewL( RFs& aFs,
                                 RApaLsSession& aAppArc,
                                 RPointerArray<CMPXCollectionType>& aTypesAry,
                                 MMPXMetadataScanObserver& aObs,
                                 MMPXFileScanStateObserver& aStateObs )
    {
    CMPXMetadataScanner* self = CMPXMetadataScanner::NewLC( aFs,
                                                            aAppArc,
                                                            aTypesAry, 
                                                            aObs,
                                                            aStateObs );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXMetadataScanner* CMPXMetadataScanner::NewLC( RFs& aFs,
                                  RApaLsSession& aAppArc,
                                  RPointerArray<CMPXCollectionType>& aTypesAry,
                                  MMPXMetadataScanObserver& aObs,
                                  MMPXFileScanStateObserver& aStateObs )
    {
    CMPXMetadataScanner* self = new( ELeave ) CMPXMetadataScanner( aObs,
                                                                   aStateObs );
    CleanupStack::PushL( self );
    self->ConstructL( aFs, aAppArc, aTypesAry );
    return self;
    }


// ---------------------------------------------------------------------------
// Virtual Destructor
// ---------------------------------------------------------------------------
//
CMPXMetadataScanner::~CMPXMetadataScanner()
    {
    Cancel();
    Reset();
    iNewFiles.Close();
    iModifiedFiles.Close();
    
    delete iTargetProps;
   
    delete iExtractor;
    }
    
// ---------------------------------------------------------------------------
// Resets this object
// ---------------------------------------------------------------------------
//
void CMPXMetadataScanner::Reset()
    {
    iNewFiles.ResetAndDestroy();
    iModifiedFiles.ResetAndDestroy();
    iTargetProps->Reset();
    }

// ---------------------------------------------------------------------------
// Starts the metadata extraction process
// ---------------------------------------------------------------------------
//    
void CMPXMetadataScanner::Start()
    {
    MPX_FUNC("MPXMetadataScanner::StartL()");
    if( !IsActive() )
        {
        // Setup
        iAryPos = 0;
        iExtractType = ENewFiles;
        iExtracting = ETrue;        
        iTargetProps->Reset();
                
        // Set Active
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );    
        }
    }
// ---------------------------------------------------------------------------
// Stops the metadata extraction process
// ---------------------------------------------------------------------------
//
void CMPXMetadataScanner::Stop()
    {
    MPX_FUNC("MPXMetadataScanner::Stop()");
    DoCancel();
    }

// ---------------------------------------------------------------------------
// Add a file to the scanning list
// ---------------------------------------------------------------------------
//  
void CMPXMetadataScanner::AddNewFileToScanL( const TDesC& aFile )
    {
    MPX_FUNC("MPXMetadataScanner::AddNewFileToScanL()");
    HBufC* file = aFile.AllocLC();
    iNewFiles.AppendL( file );
    CleanupStack::Pop( file );
    }

// ---------------------------------------------------------------------------
// Add a file to the modified list
// ---------------------------------------------------------------------------
//      
void CMPXMetadataScanner::AddModifiedFileToScanL( const TDesC& aFile )
    {
    MPX_FUNC("MPXMetadataScanner::AddModifiedFileToScanL()");
    HBufC* file = aFile.AllocLC();
    iModifiedFiles.AppendL( file );
    CleanupStack::Pop( file );
    }

// ---------------------------------------------------------------------------
// Extract media properties for a file
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXMetadataScanner::ExtractFileL( const TDesC& aFile )
    {
    MPX_FUNC("MPXMetadataScanner::ExtractFileL()");
    CMPXMedia* media;
    iExtractor->CreateMediaL( aFile, media );
    return media;
    }
    
// ---------------------------------------------------------------------------
// Cancel the operation
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::DoCancel()
    {
    MPX_FUNC("MPXMetadataScanner::DoCancel()");
    if( iExtracting )
        {
        iExtractor->CancelRequest();
        Reset();
        // Callback to observer
        TRAP_IGNORE( iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanMetadata,
                                                              KErrCancel ) );
        iExtracting = EFalse;
        }    
    }
    
// ---------------------------------------------------------------------------
// RunL function
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::RunL()
    {
    MPX_FUNC("CMPXMetadataScanner::RunL()");
    if ( iExtracting )
        {
        TRAPD( err, DoExtractL() );
        if ( err )
            {
            MetadataScannerComplete( err );
            }
        }
    }

// ---------------------------------------------------------------------------
// Extract metadata
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::DoExtractL()
    {
    MPX_FUNC("CMPXMetadataScanner::DoExtractL()");
    
    RPointerArray<HBufC>* source = GetSource();
    if ( source->Count() )
        {
        // Call asynchronous CreateMedia to get metadata.
        iExtractor->CreateMediaAsyncL( *(*source)[iAryPos], this );
        }
    else
        {
        // Source array is empty, go to next array.
        MPX_DEBUG2("CMPXMetadataScanner::DoExtractL Source array is empty ExtractType = %d.", iExtractType);
        iAryPos = 0;
        iExtractType++;
        RunAgain();
        }
    }

// ---------------------------------------------------------------------------
// Callback for CreateMediaAsyncL
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::HandleCreateMediaComplete( CMPXMedia* aMedia, TInt aError )
    {
    MPX_FUNC("CMPXMetadataScanner::HandleCreateMediaComplete()");
    MPX_DEBUG2("CMPXMetadataScanner::HandleCreateMediaComplete error = %d", aError);
    TInt err = KErrNone;
    
    // Scanning cancelled
    if ( !iExtracting )
        {
        delete aMedia;
        return;
        }
    
    // Add media to target array.
    if ( ( aError == KErrNone ) && 
         ( aMedia != NULL ) )
        {
        TRAP( err, iTargetProps->AppendL( aMedia ) );
        if ( err )
            {
            delete aMedia;
            }
        }
    
    iAryPos++;
    if( iAryPos >= GetSource()->Count() )
        {
        // Finished with this array, go to the next array.
        iAryPos = 0;
        TRAP( err, AddToCollectionL() );
        if ( err )
            {
            MetadataScannerComplete( err );
            }
        iExtractType++;
        }
    else
        {
        // Batch update collection DBs.
        if ( iTargetProps->Count() >= KLoopCount )
            {
            TRAP( err, AddToCollectionL() );
            if ( err )
                {
                MetadataScannerComplete( err );
                }
            }
        }
    
    RunAgain();
    }    

// ---------------------------------------------------------------------------
// Get source array
// ---------------------------------------------------------------------------
//   
RPointerArray<HBufC>* CMPXMetadataScanner::GetSource()
    {
    MPX_FUNC("CMPXMetadataScanner::GetSource()");
    TExtractType curType = (TExtractType) iExtractType;

    if( curType == ENewFiles )
        {
        return &iNewFiles;
        }
    else if( curType == EModFiles )
        {
        return &iModifiedFiles;
        }
    else
        return NULL;
    }

// ---------------------------------------------------------------------------
// Is metadata scanner done
// ---------------------------------------------------------------------------
//   
TBool CMPXMetadataScanner::IsDone()
    {
    MPX_FUNC("CMPXMetadataScanner::IsDone()");
    TExtractType curType = (TExtractType) iExtractType;

    TBool done = EFalse;
    if ( curType >= EMaxFile )
        {
        done = ETrue;
        }
    return done;
    }

// ---------------------------------------------------------------------------
// Run Active Object again
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::RunAgain()
    {
    MPX_FUNC("CMPXMetadataScanner::RunAgain()");
    if ( IsDone() )
        {
        MetadataScannerComplete( KErrNone );
        }
    else
        {
        MPX_DEBUG1("CMPXMetadataScanner::RunAgain -- Run again");
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );    
        }
    }

// ---------------------------------------------------------------------------
// Add metadata to collection
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::AddToCollectionL()
    {
    MPX_FUNC("CMPXMetadataScanner::AddToCollectionL()");
    TExtractType curType = (TExtractType) iExtractType;
    
    switch( curType )
        {
        case ENewFiles:
            {
            if( iTargetProps->Count() )
                {
                iObserver.AddFilesToCollectionL( *iTargetProps );
                }
            break;
            }
        case EModFiles:
            {
            if( iTargetProps->Count() )
                {
                iObserver.UpdatesFilesInCollectionL( *iTargetProps );
                }
            break;
            }
        default:
            ASSERT(0); 
        }
    iTargetProps->Reset();
    }

// ---------------------------------------------------------------------------
// Complete metadata scanner
// ---------------------------------------------------------------------------
//   
void CMPXMetadataScanner::MetadataScannerComplete( TInt aError )
    {
    MPX_FUNC("CMPXMetadataScanner::MetadataScannerCompleteL()");
    MPX_DEBUG2("CMPXMetadataScanner::MetadataScannerCompleteL error = %d", aError);    

    // Callback to observer 
    TRAP_IGNORE( iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanMetadata, aError ) );
    iExtracting = EFalse;
    Reset();
    }
