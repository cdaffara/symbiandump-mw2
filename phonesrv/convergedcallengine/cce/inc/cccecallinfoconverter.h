/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts cce call information to mediator call information
*
*/


#ifndef CCCECALLINFORMATIONCONVERTER_H
#define CCCECALLINFORMATIONCONVERTER_H

#include <e32base.h>

class MCCECallInfo;

#include <callinformation.h>
#include <mcall.h>

#include "mccecallobserver.h"
#include "mccecallinfo.h"

/**
 *  Converts cce call information to mediator call information
 *  
 *  @lib CCE.lib
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCCECallInfoConverter ) : public CBase
    {
public:

    /**
     * Converts call info.
     *
     * @since S60 5.0
     * @param aCallInfo Info to be converted.
     * @return Mediator call info
     */
    virtual TCallInfo Convert( MCCECallInfo& aCallInfo ) const;
    
private:
     
   
    };

#endif // CCCECALLINFORMATIONCONVERTER_H
