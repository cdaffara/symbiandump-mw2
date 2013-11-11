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
* Description:  Implementation of the CMCustomCommand class.
*
*/


#include "CMCustomCommand.h"
#include <mmfcontroller.h>

using namespace multimedia;

CMCustomCommand::CMCustomCommand(RMMFController& aController)
: iController(aController)
    {
    // No impl
    }

CMCustomCommand::~CMCustomCommand()
    {
    // No impl
    }

TInt CMCustomCommand::CustomCommandSync(const TMMFMessageDestinationPckg&  aDestination,
                                        TInt                         aFunction,
                                        const TDesC8&                aDataTo1,
                                        const TDesC8&                aDataTo2,
                                        TDes8&                       aDataFrom)
    {
    return iController.CustomCommandSync( aDestination, aFunction, aDataTo1, aDataTo2, aDataFrom );
    }

TInt CMCustomCommand::CustomCommandSync(const TMMFMessageDestinationPckg&  aDestination,
                                        TInt                         aFunction,
                                        const TDesC8&                aDataTo1,
                                        const TDesC8&                aDataTo2)
    {
    return iController.CustomCommandSync( aDestination, aFunction, aDataTo1, aDataTo2 );
    }

void CMCustomCommand::CustomCommandAsync(const TMMFMessageDestinationPckg& aDestination,
                                         TInt                        aFunction,
                                         const TDesC8&               aDataTo1,
                                         const TDesC8&               aDataTo2,
                                         TDes8&                      aDataFrom,
                                         TRequestStatus&             aStatus)
    {
    iController.CustomCommandAsync( aDestination, aFunction, aDataTo1, aDataTo2, aDataFrom, aStatus );
    }

void CMCustomCommand::CustomCommandAsync(const TMMFMessageDestinationPckg& aDestination,
                                         TInt                        aFunction,
                                         const TDesC8&               aDataTo1,
                                         const TDesC8&               aDataTo2,
                                         TRequestStatus&             aStatus)
    {
    iController.CustomCommandAsync( aDestination, aFunction, aDataTo1, aDataTo2, aStatus );
    }

// End of file
