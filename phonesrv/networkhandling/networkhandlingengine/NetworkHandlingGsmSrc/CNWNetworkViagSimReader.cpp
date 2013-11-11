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
* Description:  This module contains the implementation of CNWNetworkViagSimReader
*                class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkViagSimReader.h"
#include     "CNWNetworkViagBaseEngine.h"
#include    "NWLogger.h"

// Constants
const TInt KSubOneSize( 700 );// LAC/CI size are 700 bytes 
const TInt KSubTwoSize( 150 );// LAC/CI size are 150 bytes
const TInt KTagsSize( 96 ); // Tags size is 96 bytes
const TInt KSettingsSize( 2 ); // Settings size is 2 bytes.     
const TInt KViagOffset( 0 ); // The files are readed from begin.
const TInt KNoneElements( 0 );
        
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagSimReader::CNWNetworkViagSimReader
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkViagSimReader::CNWNetworkViagSimReader(
        CNWNetworkViagBaseEngine& aOwner,
        RMmCustomAPI& aCustomAPI ) 
        :  CActive( EPriorityStandard ), 
           iOwner ( aOwner ),                               
           iCustomAPI( aCustomAPI ),
           iSimFileInfoPckg( iSimFileInfo )
    {
    CActiveScheduler::Add( this );
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::CNWNetworkViagSimReader complete." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagSimReader::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagSimReader::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::~CNWNetworkViagSimReader() Called." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagSimReader::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagSimReader* CNWNetworkViagSimReader::NewL(
                CNWNetworkViagBaseEngine& aOwner,
                RMmCustomAPI& aCustomAPI
                )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::NewL() Begin." );
    
    CNWNetworkViagSimReader* self = new (ELeave) CNWNetworkViagSimReader( 
            aOwner, aCustomAPI);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::NewL() End." );
    return self;
    }

    
// Destructor
CNWNetworkViagSimReader::~CNWNetworkViagSimReader()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::~CNWNetworkViagSimReader() Begin." );
    
    Cancel();
    iElementFiles.Reset();
    iElementFiles.Close();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::~CNWNetworkViagSimReader() End." );
    }


// ---------------------------------------------------------------------------
// CNWNetworkViagSimReader::DoCancel
// Cancels the pending async. command. 
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CNWNetworkViagSimReader::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW CNWNetworkViagSimReader::DoCancel: \
        RMobilePhone::CancelAsyncRequest( EMobilePhoneNotifyModeChange )" );
    
    iCustomAPI.CancelAsyncRequest( ECustomReadSimFileIPC  );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagSimReader::DoCancel() End." );
    }

// ---------------------------------------------------------------------------
// CNWNetworkViagSimReader::RunL
// Receives the completion of the pending async. command, saves possible values 
// from async. function call and sends notification to client object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagSimReader::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkViagSimReader::RunL() Begin, \
        iStatus = %d", iStatus.Int());

    if ( iStatus == KErrNone )
        {
         if (iElementFiles[0] == EFileSettings )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagSimReader::RunL() * LC-Supported * ");
            iOwner.SetLcSupported( ETrue ); 
            }
       
        // Update the readed values to base.
        TBool activation = iOwner.UpdateCache( iElementFiles[0], iResponse );
        // Remove readed and updated elemement.
        NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkViagSimReader::RunL, activation = %d",activation );
        iElementFiles.Remove( 0 ); // Remove fist element.
        
        // EF files to read
        if ( iElementFiles.Count() > 0 && activation )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagSimReader::RunL() Read...Next EF");
            // Read next elemet file.
            ReadSimFile();           
               }
           else
               {
               if ( !activation )
                   {
                   iElementFiles.Reset();
                   }
               // When all files are readed. 
               iOwner.UpdateEFReadingState( EViagEFRead, ETrue );
               }
        }
    else
        {
        if (iElementFiles[0] == EFileSettings )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagSimReader::RunL() * LC-NOT supported * ");
            iOwner.SetLcSupported( EFalse ); 
            }        
        iElementFiles.Reset();
        iOwner.UpdateEFReadingState( EViagEFRead, ETrue );
        NWLOGSTRING2( KNWERROR,
           "NW: CNWNetworkViagSimReader::RunL, Unknown error situation, \
           iStatus = %d", iStatus.Int());
        }
    
    NWLOGSTRING( KNWREQEND,
        "NW: CNWNetworkViagSimReader::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagSimReader::ReadSimFile
// Reads sim file based on a given elementary file. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagSimReader::ReadSimFile()
    {
     NWLOGSTRING( KNWOBJECT, 
         "NW: CNWNetworkViagSimReader::ReadSimFile() Begin ");
     
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagSimReader::ReadSimFile()-- Not Active --- ");
        if ( iElementFiles.Count() > KNoneElements )
            { 
             // Clear old EF file path.
            iSimFileInfo.iPath.Zero();
            iSimFileInfo.iOffSet = KViagOffset;
            // Set MF / DF of O2  ( 3F00/7F43 )
            iSimFileInfo.iPath.Append( 0x3F );
            iSimFileInfo.iPath.Append( 0x00 );
            iSimFileInfo.iPath.Append( 0x7F );
            iSimFileInfo.iPath.Append( 0x43 );

            switch ( iElementFiles[0] )
                {        
                case EFileSettings:
                    {
                     NWLOGSTRING( KNWOBJECT, 
                         "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                         EF-SETTINGS ");
                    // Viag setting path.  0x6F87"
                    iSimFileInfo.iPath.Append( 0x6F );            
                    iSimFileInfo.iPath.Append( 0x87 );
                    iSimFileInfo.iSize = KSettingsSize;
                    break;
                    }
                case EFileTags:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                        EF-TAGS ");
                    // Viag Tags path 0x6F80
                    iSimFileInfo.iPath.Append( 0x6F );
                    iSimFileInfo.iPath.Append( 0x80 );
                    iSimFileInfo.iSize = KTagsSize;
                    break;
                    }
                case EFileSubscribedOne:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                        EF-SUB-ONE ");
                    // Viag LAC/CI file path 0x6f81
                    iSimFileInfo.iPath.Append( 0x6f );            
                    iSimFileInfo.iPath.Append( 0x81 );
                    iSimFileInfo.iSize = KSubOneSize;
                    break;
                    }
                case EFileSubscribedTwo:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                        EF-SUB-TWO ");
                    // Viag LAC/CI file path 0x6f82
                    iSimFileInfo.iPath.Append( 0x6F );            
                    iSimFileInfo.iPath.Append( 0x82 );
                    iSimFileInfo.iSize = KSubOneSize;
                    break;
                    }
                case EFileSubscribedThree:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                        EF-SUB-THREE ");
                    // Viag LAC/CI file path 0x6f83
                    iSimFileInfo.iPath.Append( 0x6F );            
                    iSimFileInfo.iPath.Append( 0x83 );
                    iSimFileInfo.iSize = KSubTwoSize;
                    break;
                    }
                case EFileSubscribedFour:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile() ::\
                        EF-SUB-FOUR ");
                    // Viag LAC/CI file path 0x6f84
                    iSimFileInfo.iPath.Append( 0x6F );            
                    iSimFileInfo.iPath.Append( 0x84 );
                    iSimFileInfo.iSize = KSubTwoSize;
                    break;
                    }
                case EFileHZ:
                case EFileUnknown:
                default:
                    {
                    NWLOGSTRING2( KNWOBJECT, 
                        "NW: CNWNetworkViagSimReader::ReadSimFile NOT \
                        SUPPORTED VALUE %d", iElementFiles[0] );
                    }
                }
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagSimReader::ReadSimFile()-- From Custom \
                API --- ");
            iCustomAPI.ReadSimFile( iStatus, iSimFileInfoPckg, iResponse );
            SetActive();
            }          
        } 
    else 
        {
        NWLOGSTRING( KNWERROR,
            "NW: CNWNetworkViagSimReader::ReadSimFile: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagSimReader::ReadSimFile() End." );
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagSimReader::SetElementFile
// Set element file to array.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagSimReader::SetElementFile( 
        TNWNetworkElementFile aElementFile )
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWNetworkViagSimReader::SetElementFile() Begin,\
        aElementFile = %d.", aElementFile );
    
    iElementFiles.Append( aElementFile );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagSimReader::SetElementFile() End." );
    }

//  End of File  
