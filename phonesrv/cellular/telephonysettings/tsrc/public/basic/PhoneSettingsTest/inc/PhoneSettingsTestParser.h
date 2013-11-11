/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPhoneSettingsTestParser class.
*
*/



#ifndef PHONESETTINGSTESTPARSER_H
#define PHONESETTINGSTESTPARSER_H

//  INCLUDES
#include    <nwdefs.h> 
#include    <e32base.h>
#include    <etelmm.h>
#include    <RMmCustomAPI.h>
#include    <rsssettings.h> 
#include    <mpsetcallwaiting.h>
#include    <mpsetnetworkselect.h>
#include    <psetsaobserver.h>
#include    "PhoneSettingsTest.hrh"

// FORWARD DECLARATIONS
class MPhoneSettingsTestMessageHandler; 

// CLASS DECLARATION

/**
*  Class for parsing test result data to log/UI
*
*  @lib 
*  @since 
*/
class CPhoneSettingsTestParser : public CBase
    {        
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aMessageHandler Reference to class 
        * implementing abstract class MPhoneSettingsTestMessageHandler 
        */
        static CPhoneSettingsTestParser* NewL(MPhoneSettingsTestMessageHandler& aMessageHandler);
        
        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestParser();

    public: // New functions
        
        /**
        * Parser function for data type TServiceGroup
        *
        */ 
		void ParseTServiceGroup(TServiceGroup& aServiceGroup);

        /**
        * Parser function for data type TBarringProgram
        *
        */ 
		void ParseTBarringProgram(TBarringProgram& aBarringProgram);

        /**
        * Parser function for data type TBasicServiceGroups
        *
        */ 
		void ParseTBasicServiceGroups(TBasicServiceGroups& aBasicServiceGroups);

        /**
        * Parser function for data type TMobilePhonePasswordChangeV2
        *
        */ 
		void ParseTMobilePhonePasswordChangeV2( 
		    RMobilePhone::TMobilePhonePasswordChangeV2& aTMobilePhonePasswordChangeV2);

        /**
        * Parser function for data type TSSSettingsSetting
        *
        */ 
		void ParseTSSSettingsSetting(TSSSettingsSetting& aSsSetting);
				
        /**
        * Parser function for data type TCallDivertSetting
        *
        */ 
        void ParseTCallDivertSetting(TCallDivertSetting& aDivertSetting);		
		
        /**
        * Parser function for data type TCallDivertingCondition
        *
        */ 
        void ParseTCallDivertingCondition(TCallDivertingCondition& aDivertingCondition);
    
        /**
        * Parser function for data type TDivertingSetting
        *
        */ 
        void ParseTDivertingSetting(TDivertingSetting& aDivertingSetting); 

        /**
        * Parser function for data type TDivertingStatus
        *
        */ 
        void ParseTDivertingStatus(TDivertingStatus& aDivertingStatus); 
    
        /**
        * Parser function for data type TSetCallWaiting
        *
        */ 
        void ParseTSetCallWaiting(MPsetCallWaiting::TSetCallWaiting& aSetCallWaiting); 
    
        /**
        * Parser function for data type TCurrentNetworkInfo
        *
        */ 
        void ParseTCurrentNetworkInfo(
            MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentNetworkInfo); 
    
        /**
        * Parser function for data type TSelectMode
        *
        */ 
        void ParseTSelectMode(MPsetNetworkSelect::TSelectMode& aSelectMode); 

        /**
        * Parser function for data type TNetworkId
        *
        */ 
        void ParseTNetworkId(MPsetNetworkSelect::TNetworkId& aNetworkId);  

        /**
        * Parser function for data type TNetworkStatus
        *
        */ 
        void ParseTNetworkStatus(MPsetNetworkSelect::TNetworkStatus& aNetworkStatus); 

        /**
        * Parser function for data type TNetworkAccess
        *
        */ 
        void ParseTNetworkAccess(MPsetNetworkSelect::TNetworkAccess& aNetworkAccess);   
    
        /**
        * Parser function for data type TNetworkInfo
        *
        */ 
        void ParseTNetworkInfo(MPsetNetworkSelect::TNetworkInfo& aNetworkInfo); 

        /**
        * Parser function for data type TNetworkModeCaps
        *
        */ 
        void ParseTNetworkModeCaps(RMmCustomAPI::TNetworkModeCaps& aNetworkModeCaps); 

        /**
        * Parser function for data type TUnconditionalCFStatus
        *
        */ 
        void ParseTUnconditionalCFStatus(TUnconditionalCFStatus& aUnconditionalCFStatus); 

        /**
        * Parser function for data type TSelectedLine
        *
        */ 
        void ParseTSelectedLine(TSelectedLine& aSelectedLine); 

        /**
        * Parser function for data type TCallDivertNotifySetting
        *
        */ 
        void ParseTCallDivertNotifySetting(TCallDivertNotifySetting& aCallDivertNotifySetting);

        /**
        * Parser function for data type TBool
        *
        */ 		
		void ParseTBool(TBool aBool);		
		
    private: // Functions 

        /**
        * C++ default constructor.
        * @param aMessageHandler Reference to class 
        * implementing abstract class MPhoneSettingsTestMessageHandler 
        */
        CPhoneSettingsTestParser(MPhoneSettingsTestMessageHandler& aMessageHandler);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Reference to message handler
		MPhoneSettingsTestMessageHandler& iMessageHandler;  
    };

#endif      // PHONESETTINGSTESTPARSER_H
            
// End of File
