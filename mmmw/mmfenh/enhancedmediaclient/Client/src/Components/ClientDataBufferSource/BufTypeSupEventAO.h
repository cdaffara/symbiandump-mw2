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
* Description:  Definition of the BufTypeSupEventAO class.
*
*/


#ifndef C_BUFTYPESUPEVENTAO_H
#define C_BUFTYPESUPEVENTAO_H

//  INCLUDES
#include <e32base.h>

namespace multimedia
    {

    /**
     *  Observer that monitors the Buffering types Support
     *  @lib EnhancedMediaClient.lib
     *  @since S60 v3.2
     */    
    class MBufferingTypesSupportedObserver
        {
        public:
            virtual void BufferingTypesSupportedChanged() = 0;
        };
    
    /**
     *  Active Object that monitors the Buffering types Support
     *  Event from the Source Side
     *  @lib EnhancedMediaClient.lib
     *  @since S60 v3.2
     */    

    class CBufTypeSupEventAO : public CActive
        {
        public:  // Constructors and destructor
            /**
             * Function to Create the Object.
             * @param aObserver Observer to callback the Event.
             */
            static CBufTypeSupEventAO* NewL(MBufferingTypesSupportedObserver& aObserver);

            /**
            * Destructor.
            */
            virtual ~CBufTypeSupEventAO();

            /**
             * From CActive.
             * Indicates that this active object has issued a
             * request and that it is now outstanding 
             *
             * @since S60 v3.2
             */            
            void SetActive();

            /**
             * From CBufTypeSupEventAO.
             * Returns the Error Stored on Completion of the Request
             *
             * @since S60 v3.2
             */            
            TInt Error();
            
        protected:
            // From CActive
            void RunL();
            void DoCancel();
            TInt RunError( TInt aError );
            
        private:
            CBufTypeSupEventAO( MBufferingTypesSupportedObserver& aObserver );
            void ConstructL();
            
        private:
            /**
            * Observer stored to Callback
            */
            MBufferingTypesSupportedObserver& iObserver;

            /**
            * Error Stored upon Request Completion
            */
            TInt iError;
        };
    
    } // namespace multimedia

#endif // C_BUFTYPESUPEVENTAO_H

// End of File
