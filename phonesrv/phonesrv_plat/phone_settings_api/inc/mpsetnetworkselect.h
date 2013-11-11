/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       MPsetNetworkSelect defines interface to get network information and  
*       select network. MPsetNetworkInfoObserver is used to handle completion 
*       of function calls of this class. All the methods are asynchronous.    
*       Observer handles completion                                           
*
*
*/


#ifndef     MPSETNETWORKSELECT_H
#define     MPSETNETWORKSELECT_H

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS
class MPsetNetworkInfoObserver;

//  CLASS DEFINITIONS 
/**
*  MPsetNetworkSelect class is virtual class for network selection.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetNetworkSelect
    {
    public:

        //Length of codes (MNC and MCC)
        enum
            {
            EPSetNetMcc = 4,
            EPSetNetMnc = 8
            };

        //TNetworkId contains network ID and country ID
        struct TNetworkId
            {
            TBuf<EPSetNetMcc> iCountryCode;
            TBuf<EPSetNetMnc> iNetworkCode;
            };

        //Length of network names
        enum
            {
            ENetShortNameSize = 20,
            ENetLongNameSize = 30
            };

        //Network selection mode
        enum TSelectMode
            {
            ENetSelectModeAutomatic,
            ENetSelectModeManual
            };

        //Status of network
        enum TNetworkStatus
            {
            ENetStatUnknown,
            ENetStatAvailable,
            ENetStatCurrent,
            ENetStatForbidden
            };
        //Network access type
        enum TNetworkAccess
            {
            ENetNetworkGSM,
            ENetNetworkWCDMA,
            ENetNetworkGSMandWCDMA
            };

        //Information about network
        struct TNetworkInfo
            {
            TNetworkId iId;
            TNetworkStatus iStatus;
            TSelectMode iMode;
            TNetworkAccess iAccess;
            TBuf<ENetShortNameSize> iShortName;
            TBuf<ENetLongNameSize> iLongName;
            };        

        //Current status of the network
        enum TCurrentNetworkStatus
            {
            ENotRegisteredNotSearching,
            ERegisteredOnHomeNetwork,
            ENotRegisteredSearching,
            ERegistrationDenied,
            EUnknown,
            ERegisteredRoaming
            };

        //Current information about the network
        struct TCurrentNetworkInfo
            {
            TCurrentNetworkStatus iStatus;
            TUint iLocationAreaCode;
            TUint iCellId;
            };

    public:
  
        /**
        * Puts network retrieve results into array.
        *
        */
        virtual void GetAvailableNetworksL() = 0; 
          
        /**
        * Returns current network information.
        *
        * @param aInfo Current network information
        * @return Returns error code.
        */
        virtual TInt GetCurrentNetworkInfo( 
            MPsetNetworkSelect::TCurrentNetworkInfo& aInfo ) = 0;
          
        /**
        * Returns current network selection mode.
        *
        * @param aMode Network selection mode
        * @return Returns error code.
        */
        virtual TInt GetNetworkSelectMode( TSelectMode& aMode) = 0;
          
        /**
        * Sets network, also for selecting automatic/manual mode.
        *
        * @param aInfo Network information
        */
        virtual void SelectNetworkL( const TNetworkInfo& aInfo ) = 0;
          
        /**
        * CancelProcess-function, cancels the requests.
        */
        virtual void CancelProcess() = 0;
    };

#endif // MPSETNETWORKSELECT_H
// end of file
