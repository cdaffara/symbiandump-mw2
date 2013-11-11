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
* Description:  This file contains the header file of the CNWNetworkViagLcEngine class.
*
*/



#ifndef CNWNETWORKVIAGLCENGINE_H
#define CNWNETWORKVIAGLCENGINE_H

//  INCLUDES
#include "CNWNetworkViagBaseEngine.h"
#include <f32file.h>


// OLD INCLUDES
#include "MNWNetworkTimer.h"
#include <nwhandlingengine.h>
#include "CNWGsmMessageHandler.h"
#include <mcbsmcnobserver.h> //For McbsMcnObserver
#include <rmmcustomapi.h>
#include <e32property.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CNWGsmSessionImplementation;
class CNWNetworkViagTimer;
class CMcn;
class CNWGsmMessageHandler;

// struct TViagElementReocord;
// CLASS DECLARATION


enum TNWRatType
    {
    ERatUtran     = 0x00,
    ERatGsm       = 0x40,  // 1000000
    ERatAny       = 0x80, //  10000000
    ERatUnvalid   = 0xC0, //  11000000
    };    

enum TNWViagUnitType
    {
    EUnitCellId,
    EUnitLac,
    EUnitNone
    };

// Descripes the Prefix 
class TNWViagFieldContent
    {
    public:
    TNWViagFieldContent()
        {
        iWildCarded = EFalse;
        iFieldLAC   = KErrNotFound;
        iUnitType   = EUnitNone;
        iUnit.Zero();
        };

    TBool     iWildCarded; // is wild card valid in current Field.     
    TInt      iFieldLAC; // If defined 
    TNWViagUnitType iUnitType;
    TBuf<128>       iUnit; // Max 128 units    
    };

// Section descriptor
class TNWViagSectionContent
    {
    public:
    TNWViagSectionContent()
            {
            iCodingScheme = KErrNotFound;
            iZoneId = KErrNotFound;
            iLastSection = ETrue;
            iField.Reset();
            };
    
    TInt iCodingScheme;                //coding scheme.
    TInt iZoneId;                    // Current zone id
    TBool iLastSection;                // Is the last section.   
    RArray<TNWViagFieldContent> iField;
    };
            
// Network coding 
class TViagElementReocord
    {                
    public:
    TViagElementReocord()
                    {
                    iRatType = ERatUnvalid;
                    iMCC     = KNullDesC;
                    iMNC     = KNullDesC;
                    iCellSubscribed = EFalse;
                    iCache.Reset();
                    };
    
    TNWRatType iRatType;             // Gsm, WCdma, Any ( GSM or WCDMA)
    TBuf<KNWCountryCodeLength> iMCC; // Country code.
    TBuf<KNWIdentityLength>    iMNC; // Network code.
    TBool iCellSubscribed;            // Informs is there any subscribed cells.   
    RArray<TNWViagSectionContent> iCache;
    };


/**
*  Monitors current network change event from MMEtel.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkViagLcEngine :  CBase 
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagLcEngine* NewL(
                        CNWNetworkViagBaseEngine& aBase,
                        TNWInfo& aNetworkInfo,
                        RMmCustomAPI& aCustomAPI,            
                        TNWInterInfo& aInterNetworkInfo);
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagLcEngine();


    public: // Functions from MCbsMcnObserver
        
        /**
        * Is called by CNWNetworkCurrentNetworkMonitor in case of 
        * cell re-selection.
        * @param 
        * @return Error code.
        */
        void CellReselection();

        /**
        * Decides which parser is used. Desicion is made according to 
        * Element file. 
        * @param aResponse        Struct containing all neccessary information 
        *                         got from SIM.
        * @param aElementFile     Used element file. 
        * @return Error code.
        */
        void ParseResult( TNWNetworkElementFile aElementFile, 
                const TDesC8&  aResponse );
        
        /**
        * Parse setting value from aResponse parameter and update it if wanted. 
        *
        * @param  aResponse      A reasponse data from SIM.
        * @return Error code.
        */
        void ParseSettings( const TDesC8&  aResponse );
        
        /**
        * Checks if current LAC file contains any cubscribed cells.
        *
        * @param 
        * @return Error code.
        */
        void ParseSubscribed( TNWNetworkElementFile aElementFile, 
                const TDesC8&  aResponse );
        
        /**
        * Parses zone tags from response and writes them to iZoneTags struct. 
        *
        * @param  aResponse    A reasponse data from Sim including tag information.
        * @return Error code.
        */
        void ParseTags( const TDesC8&  aResponse );
        
        /**
        * Finds a section information from given string and saves it to iCurrentCache struct.
        *
        * @param aParse    String containging section information
        * @return Error code.
        */
        TBool ParseSection( TLex8& aParse );
        
        /**
        * Parses fields from given string to iField .
        *
        * @param aParse    String containging field information
        * @return Error code.
        */
        TBool ParseField( TLex8& aParse );             
        
        /**
        * Updates version number to SIM
        *
        * @param None.
        * @return Error code.
        */
        void UpdateVersion( );
        
        /**
        * Gets activation bits. 
        *
        * @param aActivation   General HomeZone activation. 
        * @param aCbActivation  Cell Broadcast activation bit. Tells wheter CB is active or not. 
        * @return Error code.
        */
        void GetActivation(TBool& aActivation, TBool& aCbActivation );
        
        /**
        * Returns a boolean to indicate if there is at least one subscribed GSM cell. 
        *
        * @param 
        * @return subscribed   A boolean, ETrue if there is at least one subscibed GSM Cell
        */
        TBool IsGsmCellSubscribed();     
     
    private:

        /**
        * C++ default constructor.
        */
        CNWNetworkViagLcEngine( CNWNetworkViagBaseEngine& aBase,
                              TNWInfo& aNetworkInfo,
                              RMmCustomAPI& aCustomAPI,
                              TNWInterInfo& aInterNetworkInfo);


        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /*
        * Resets a given Lc cache struct. 
        *
        * @param 
        * @return None.
        */
        void ResetLcCache( TViagElementReocord& aCurrentcache );

        /*
        * Updates founded homezone values 
        *
        * @param 
        * @return None.
        */    
        void WriteViagValues( TInt aSectionIndex );
        
        /*
        * Adds TUint in descriptor
        *
        * @param aBuffer    destination buffer
        * @param aData      data to be added
        * @return None.
        */    
        void AddInBufferAsTUint( TDes & aBuffer, const TUint aData );
    
        /*
        * Extends TUint to descriptor
        *
        * @param aDestData  destination buffer
        * @param aData      source data
        * @return None.
        */    
        void ExtendTUintIntoBuffer( TDes & aDestData, const TUint aData );
 
    private:
        /*
        * Handles network related matching stuff
        *
        * @param 
        * @return TBool
        */    
        TBool HandleNetworkCheck();
        
        /*
        * Makes the Cell id based matching
        * 
        * @param aCell Cell Id from network
        * @param aLac  Location Area code from Network
        * @param aSectionIndex  index of used Cache
        * @param aIndex field index to search 
        * @return TBool. ETrue if 
        */    
        TBool HandleCellMatch( const TDesC & aCell, 
                        const TDesC & aLac, 
                        TInt aSectionIndex, 
                        TInt aIndex );

        /*
        * Makes the LAC based matching
        *
        * @param aSectionIndex  index of used Cache
        * @param aIndex field index to search 
        * @param aLacBuf Location Area code from Network 
        * @return None.
        */    
        void HandleLACMatch( TInt aSectionIndex, TInt aIndex, 
                        const TDesC& aLacBuf );
        /*
        * converts cell id's to TInt and makes comprison
        *
        * @param aCell  Cell Id from network
        * @param aCmpCell Cell Id from SIM file.
        * @return KErrNone if given values matches.
        */                  
        TInt CompareTIntCellIdValues( const TDesC& aCell, 
                        const TDesC& aCmpCell );

    private:    // Data
        // owner..
        CNWNetworkViagBaseEngine& iBase;
        // Reference to the network info structure.
        TNWInfo&                iNWNetworkInfo;
        // Reference to the customAPI connection
        RMmCustomAPI&           iCustomAPI;   
        // Reference to the internal network info structure.
        TNWInterInfo&           iInterNetworkInfo;
        // boolean to tell wheter section is last in curren file. 
        TBool iLastSection;
        // Match is found.
        TBool iMatchFound;
        // General activation bit.
        TBool iActivation;
        // Cell Broadcast activation bit.
        TBool iCbActivation;
        // Current LC Cache
        TViagElementReocord*  iCurrentLcCache;    
        // version of HZ application in USIM card
        TInt iVersion;
        // Current coding scheme.
        TInt iCodingScheme;        
        // Number of Lac/Ci field in section.
        TInt iNumberOfFields;
        // Number of Cell id's.
        TInt iNumberOfUnits;            
        // Lc caches
        TViagElementReocord*  iLcCacheOne;
        TViagElementReocord*  iLcCacheTwo;
        TViagElementReocord*  iLcCacheThree;
        TViagElementReocord*  iLcCacheFour;
    };

#endif      // CNWNetworkViagLcEngine_H   
    
            
            
// End of File
