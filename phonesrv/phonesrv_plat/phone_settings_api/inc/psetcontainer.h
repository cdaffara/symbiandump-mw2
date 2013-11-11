/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*      CPsetContainer is the general access class for phonesettings.
*      Container must exists as long as other objects are used.          
*
*
*/


#ifndef     PSETCONTAINER_H
#define     PSETCONTAINER_H

//  INCLUDES
#include <psetcallwaiting.h> 
#include <psetcalldiverting.h> 
#include <psetcallbarring.h> 
#include <psetcli.h> 
#include <psetnetwork.h> 
#include <psetrefreshhandler.h> 


// FORWARD DECLARATIONS
class CPSetRefreshHandlerImpl;


// CLASS DECLARATION
/**
*  CPsetContainer class is the general access class for phonesettings.
*  @lib phonesettings.lib
*  @since 1.0
*/
class CPsetContainer : public CBase
    {
    public: //constructor & destructor

        /* Symbian OS 2-phase Constructor. */
        IMPORT_C static CPsetContainer* NewL();

        /* Destructor */
        IMPORT_C ~CPsetContainer();

    public:
        /**
        * Creates CPsetCallWaiting-object.
        *
        * @param aObserver Observer to CPsetCallWaiting class.
        * @return returns CPsetCallWaiting-pointer
        */
        IMPORT_C CPsetCallWaiting* CreateCWObjectL( 
            MPsetCallWaitingObserver& aObserver );
        
        /**
        * Creates CPsetCallDiverting-object.
        * 
        * @param  aObserver Observer to CPsetCallDiverting class.
        * @return returns CPsetCallDiverting-pointer
        */
        IMPORT_C CPsetCallDiverting* CreateCFObjectL( 
            MPsetDivertObserver& aObserver );

        /**
        * Creates CPsetClir-object.
        * 
        * @param aObserver Observer to CPsetCli class.
        * @return returns CPsetClir-pointer
        */
        IMPORT_C CPsetCli* CreateCliObjectL( MPsetCliObserver& aObserver );
        
        /**
        * Creates CPsetCallBarring-object.
        *
        * @param aObserver Observer to CPsetCallBarring class.
        * @return returns CPsetCallBarring-pointer
        */
        IMPORT_C CPsetCallBarring* CreateCBObjectL( 
            MPsetBarringObserver& aObserver);
         
        /**
        * Creates CPsetNetwork-object.
        * 
        * @param aObserver Observer to CPsetNetwork class.
        * @return Returns CPsetNetwork-pointer.
        */
        IMPORT_C CPsetNetwork* CreateNetworkObjectL(
            MPsetNetworkInfoObserver& aObserver );

        /**
        * Creates refresh handler object.
        * 
        * @since 2.8.
        * @return Return Refresh handler pointer.
        */
        IMPORT_C CPSetRefreshHandler* CreateRefreshHandlerL();

        /**
        * Creates CPsetNetwork-object.
        * 
        * @param aObserver Observer to CPsetNetwork class.
        * @return Returns CPsetNetwork-pointer.
        */
        IMPORT_C CPsetNetwork* CreateNetworkModeObjectL(
            MPsetNetworkModeObserver& aModeObserver );


    private: //constructors

        void ConstructL();

    private: //new

        TInt ConnectServer();

    private:
        //Provides client access to mobile phone functionality provided by TSY.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobilePhone iPhone;
        //ETEL telephony server.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RTelServer   iServer;
        //Provides client access to mobile line functionality provided by TSY.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        RMobileLine  iLine;
    };

#endif // PSETCONTAINER_H

// End of File
