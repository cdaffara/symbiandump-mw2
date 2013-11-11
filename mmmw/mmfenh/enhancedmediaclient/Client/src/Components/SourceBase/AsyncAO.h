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
* Description:  This file contains the definition of asynchronous AO object.
*
*/


#ifndef ASYNCAO_H
#define ASYNCAO_H

// INCLUDES
#include <e32base.h>

namespace multimedia
    {
    class MAsyncAOObserver
        {
        public:
            virtual void Event( TInt aError ) = 0;
        };

    // CLASS DECLARATION
    class CAsyncAO : public CActive
        {
        public: // Constructors and destructor
            static CAsyncAO* NewL( MAsyncAOObserver& aObserver );
            ~CAsyncAO();
            TInt SetActive();

            void RunL();
            void DoCancel();
            TInt Error( TInt aError );

        private:
            CAsyncAO( MAsyncAOObserver& aObserver );
            void ConstructL();

        private:
            MAsyncAOObserver& iObserver;
        };
    } // namespace multimedia

#endif // ASYNCAO_H

//  End of File
