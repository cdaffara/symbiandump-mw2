/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Singleton
*
*/



#ifndef C_SENDUISINGLETON_H
#define C_SENDUISINGLETON_H

#include <e32base.h>
#include <msvapi.h>

class CMsvSession;
class CClientMtmRegistry;
class CMtmUiRegistry;
class CMtmUiDataRegistry;

/**
* Singelton
*
* @lib sendui
* @since S60 v3.2
*/ 
NONSHARABLE_CLASS( CSendUiSingleton ) : public CBase,
                                        public MMsvSessionObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @return Pointer to object of CSendUiSingleton.
        */
        IMPORT_C static CSendUiSingleton* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSendUiSingleton();

    public: // New functions

        /**
        * Returns message server session.
        *
        * @return message server session.
        */
        IMPORT_C CMsvSession& MsvSessionL();
        
        /**
        * Returns client MTM registry.
        *
        * @return client MTM registry.
        */
        IMPORT_C CClientMtmRegistry& ClientMtmRegistryL();

        /**
        * Returns MTM UI registry.
        *
        * @return MTM UI registry.
        */
        IMPORT_C CMtmUiRegistry& MtmUiRegistryL();
        
        /**
        * Returns MTM UI data registry.
        *
        * @return MTM UI registry.
        */
        IMPORT_C CMtmUiDataRegistry& MtmUiDataRegistryL();
        
    private: // From base classes

        /**
        * From MMsvSessionObserver
        */    
    	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

    private:

        /**
        * C++ default constructor.
        */
        CSendUiSingleton();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data

        /**
         * Own.
         */
        CMsvSession* iMsvSession;
        
        /**
         * Own.
         */
         CClientMtmRegistry* iClientMtmRegistry;
        
        /**
         * Own.
         */        
         CMtmUiRegistry* iMtmUiRegistry;
        
        /**
         * Own.
         */
        CMtmUiDataRegistry* iMtmUiDataRegistry;
        
    };

#endif      // C_SENDUISINGLETON_H

// End of File

