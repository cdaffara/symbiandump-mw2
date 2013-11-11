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
* Description:  This file contains the header file of the 
*                CNWProgrammableOperatorNameCommand class.
*
*/



#ifndef CNWPROGRAMMABLEOPERATORNAMECOMMAND_H
#define CNWPROGRAMMABLEOPERATORNAMECOMMAND_H

//  INCLUDES
#include <nwhandlingengine.h>
#include <etelmm.h>
#include <rmmcustomapi.h>


// CLASS DECLARATION

/**
*  Executes commands related to programmable operator logo.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWProgrammableOperatorNameCommand : public CActive
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CNWProgrammableOperatorNameCommand(
                CNWMessageHandler& aOwner,
                RMmCustomAPI&   aCustomAPI,
                TNWInfo& aNetworkInfo );

        /**
        * Destructor.
        */
        virtual ~CNWProgrammableOperatorNameCommand();

    public: // New functions
        
        /**
        * Starts async. Programmable Operator info query.
        */
        void IssueRequest();


    protected:  // Functions from CActive
        /**
        * Cancels asyncronous request(s).
        * 
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        void RunL();

    private: // New functions

        /**
        * Update network info to the client's TNWInfo struct
        */
        void UpdateNetworkInfo();

    private:
        // CNWMessageHandler object that owns this object
        CNWMessageHandler& iOwner;
        // Reference to the Custom API connection
        RMmCustomAPI&   iCustomAPI;
        // Reference to internal network info struct
        TNWInfo& iNetworkInfo;
        // Operator Name
        RMmCustomAPI::TOperatorNameInfo iOperatorName;
    };

#endif      // CNWPROGRAMMABLEOPERATORNAMECOMMAND_H
            
// End of File
