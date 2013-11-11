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
* Description:  Definition of the CMCustomCommand class.
*
*/


#ifndef CMCUSTOMCOMMAND_H
#define CMCUSTOMCOMMAND_H

#include <e32base.h>
#include <MCustomCommand.h>

class RMMFController;

namespace multimedia
    {
    class CMCustomCommand : public MCustomCommand
        {
        public:
            CMCustomCommand(RMMFController& aController);
            ~CMCustomCommand();
            
            // From MCustomCommand
            TInt CustomCommandSync(const TMMFMessageDestinationPckg&  aDestination,
                TInt                        aFunction,
                const TDesC8&               aDataTo1,
                const TDesC8&               aDataTo2,
                TDes8&                      aDataFrom);
            
            TInt CustomCommandSync(const TMMFMessageDestinationPckg&  aDestination,
                TInt                        aFunction,
                const TDesC8&               aDataTo1,
                const TDesC8&               aDataTo2);
            
            void CustomCommandAsync(const TMMFMessageDestinationPckg& aDestination,
                TInt                        aFunction,
                const TDesC8&               aDataTo1,
                const TDesC8&               aDataTo2,
                TDes8&                      aDataFrom,
                TRequestStatus&             aStatus);
            
            void CustomCommandAsync(const TMMFMessageDestinationPckg& aDestination,
                TInt                        aFunction,
                const TDesC8&               aDataTo1,
                const TDesC8&               aDataTo2,
                TRequestStatus&             aStatus);
            
        private:
            RMMFController& iController;
        };
    }

#endif // CMCUSTOMCOMMAND_H

// End of file
