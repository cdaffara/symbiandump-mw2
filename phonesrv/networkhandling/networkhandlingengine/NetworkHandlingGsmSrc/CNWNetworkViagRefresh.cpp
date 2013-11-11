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
* Description:  This module contains the implementation of CNWNetworkViagRefresh
*                class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkViagRefresh.h"
#include    "CNWNetworkViagBaseEngine.h"
#include    "NWLogger.h"
#include    <rsatrefresh.h>
#include    <rsatsession.h>
#include    "CNWNetworkViagSimReader.h"



// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagRefresh::CNWNetworkViagRefresh
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkViagRefresh::CNWNetworkViagRefresh(
        CNWNetworkViagBaseEngine& aBase,
        RMmCustomAPI& aCustomAPI,
        CNWNetworkViagSimReader& aViagSimReader, 
        TBool& aReadingOnGoing ):
           iBase ( aBase ),                               
           iCustomAPI( aCustomAPI ),
           iViagSimReader ( aViagSimReader ),        
           iSimFileInfoPckg( iSimFileInfo ),
           iReadingOnGoing( aReadingOnGoing )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::CNWNetworkViagRefresh complete." );
    }




// ----------------------------------------------------------------------------
// CNWNetworkViagEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagRefresh::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::ConstructL() begin " );
    
    iSatSession = new ( ELeave ) RSatSession;
    iSatSession->ConnectL();
    iSatRefresh = new ( ELeave ) RSatRefresh( *this );
    iSatRefresh->OpenL( *iSatSession );
    iSatRefresh->NotifyFileChangeL();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::ConstructL() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagEngine::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagRefresh* CNWNetworkViagRefresh::NewL(
                CNWNetworkViagBaseEngine& aBase,
                RMmCustomAPI& aCustomAPI,
                CNWNetworkViagSimReader& aViagSimReader,
                TBool& aReadingOnGoing
                )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::NewL() Begin " );
    
    CNWNetworkViagRefresh* self = new (ELeave) CNWNetworkViagRefresh( 
                                            aBase, 
                                            aCustomAPI, 
                                            aViagSimReader, 
                                            aReadingOnGoing );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::NewL() End " );
    
    return self;
    }

    
// Destructor
CNWNetworkViagRefresh::~CNWNetworkViagRefresh()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::~CNWNetworkViagRefresh() Begin." );
    
    if ( iSatRefresh )
        {
        // Cancel notifications.
        iSatRefresh->Cancel();    
        // Close SubSession.
        iSatRefresh->Close();
        }
    delete iSatRefresh;
    
    if ( iSatSession )
        {
        iSatSession->Close();
        }
    delete iSatSession;

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::~CNWNetworkViagRefresh() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagRefresh::AllowRefresh
// Refresh query. Client should determine whether it allow the refresh to happen. 
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagRefresh::AllowRefresh( 
        TSatRefreshType aType, const TSatRefreshFiles& /*aFiles*/ )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::AllowRefresh(),\
        aType = %d ", aType );
    
    // Sat Server does not send refresh notification if it is not allowed.
    if ( aType == ESimReset )
        {
        iBase.SendMessage( MNWMessageObserver::ENWMessageAllowRefresh );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::AllowRefresh() End " );
    return ETrue;
    }
 
// ----------------------------------------------------------------------------
// CNWNetworkViagRefresh::Refresh
// Notification of refresh. In this method the client should not close 
// or cancel the RSatRefresh subsession. 
// ----------------------------------------------------------------------------
//     
void CNWNetworkViagRefresh::Refresh( 
        TSatRefreshType aType, const TSatRefreshFiles& aFiles )
    {             
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::Refresh() Begin " );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::Refresh Type: %d", aType );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::Refresh File: %S", &aFiles );
    
    TBool viagReadStatus( ETrue );
    // With these types the file list is filled
    if ( aType == EFileChangeNotification || 
            aType == ESimInitFileChangeNotification )
        {
        TInt fileCount( aFiles.Length() );

        for ( TInt fileIndex = 0; fileIndex < fileCount; fileIndex++ )
              {
            switch( aFiles[ fileIndex ] )
                {
                case  EFileSubscribedOne: 
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh::SUBCRIBE ONE REFRESH" );
                    iReadingOnGoing = ETrue;
                    viagReadStatus = EFalse;    
                    iViagSimReader.SetElementFile( EFileSubscribedOne );
                    break;    
                    }
                case EFileSubscribedTwo:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh::SUBCRIBE TWO REFRESH" );
                    iReadingOnGoing = ETrue;
                    viagReadStatus = EFalse;
                    iViagSimReader.SetElementFile( EFileSubscribedTwo );    
                    break;
                    }                           
                case EFileSubscribedThree:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh::SUBCRIBE THREE REFRESH" );
                    viagReadStatus = EFalse;
                    iReadingOnGoing = ETrue;
                    iViagSimReader.SetElementFile( EFileSubscribedThree );    
                    break;
                    }                
                case EFileSubscribedFour:
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh::SUBCRIBE FOUR REFRESH" );
                    viagReadStatus = EFalse;
                    iReadingOnGoing = ETrue;
                    iViagSimReader.SetElementFile( EFileSubscribedFour );    
                    break;
                    } 
                case EFileHZ:
                    {
                    iReadingOnGoing = ETrue;
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh::HZ REFRESH" );
                    iBase.SatRefreshCompleteNotification();
                    break;
                    } 
                case EFileProgrammableList:         // EFopl   // Programmable operator name
                case EFileProgrammableOpereator:    // EFpnn 
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh:: PROGRAMMABLE OPERATOR NAME REFRESH" );
                    iReadingOnGoing = ETrue;
                    iBase.UpdateEFReadingState( EProgEFRead, EFalse );
                    iBase.SendMessage( 
                        MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating );
                    break;
                    }
                case EFileNetworkProvider:        
                case EFileNetworkProvidersecond: 
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh:: NETWORK PROVIDER REFRESH" );
                    iReadingOnGoing = ETrue;
                    iBase.UpdateEFReadingState( ENPNEFRead, EFalse );
                    iBase.SendMessage( 
                        MNWMessageObserver::ENWMessageNetworkProviderNameUpdating );    
                      break;
                    }
                case EFileServiceProvider:             // EFspn     
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagRefresh:: SERVICE PROVIDER REFRESH" );
                    iReadingOnGoing = ETrue;
                    iBase.UpdateEFReadingState( ESPNEFRead, EFalse );
                    iBase.SendMessage( 
                        MNWMessageObserver::ENWMessageServiceProviderNameUpdating );
                    break;
                    }                  
                case EFileTags: 
                case EFileSettings:         
                case EFileUnknown:            
                default:
                    break;
                } // Switch end
            } // For
       
        } // if
    else if ( aType != ERefreshTypeNotSet )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagRefresh:: FULL FILE CHANGE OR SIMILAR REFRESH" );
        viagReadStatus = EFalse;
        iReadingOnGoing = ETrue;
        iBase.UpdateEFReadingState( EFullFileChange, EFalse );
        iViagSimReader.SetElementFile( EFileSubscribedOne );
        iViagSimReader.SetElementFile( EFileSubscribedTwo );    
        iViagSimReader.SetElementFile( EFileSubscribedThree );    
        iViagSimReader.SetElementFile( EFileSubscribedFour );
        iBase.SatRefreshCompleteNotification();    
        iBase.SendMessage( MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating );       
        iBase.SendMessage( MNWMessageObserver::ENWMessageNetworkProviderNameUpdating );        
        iBase.SendMessage( MNWMessageObserver::ENWMessageServiceProviderNameUpdating );
        }
    
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::Refresh(),\
        iReadingOnGoing = %d, viagReadStatus = %d",
        iReadingOnGoing, viagReadStatus );
    
    if ( iReadingOnGoing )
            {
            if ( !viagReadStatus )
                {
                if ( aType == EFileChangeNotification || 
                        aType == ESimInitFileChangeNotification  )
                    {   
                    iBase.UpdateEFReadingState( EViagEFRead, viagReadStatus );
                    }
                // some  homezone files are updated.
                iViagSimReader.ReadSimFile();   
                }       
            iBase.CompleteRefresh();
            }
    else if ( aType != ERefreshTypeNotSet )
        {
        RefreshEFRead();
        }
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagRefresh::Refresh() End " );
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagRefresh::RefreshEFRead
// Notifies SAT Server when all EF's has been read.
// ----------------------------------------------------------------------------
//     
void CNWNetworkViagRefresh::RefreshEFRead()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::RefreshEFRead() Begin " );
    
    if ( iSatRefresh )
        {
        iSatRefresh->RefreshEFRead( EFalse ); // 
        iReadingOnGoing = EFalse;
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagRefresh::RefreshEFRead() End " );
    }

//  End of File  
