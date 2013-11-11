/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provider of the SAT Mediator Events.
*
*/



#ifndef M_MSATMEDIATOREVENTPROVIDER_H
#define M_MSATMEDIATOREVENTPROVIDER_H


#include <e32std.h>


/**
*  Mediator Event provider interface.
*
*  Responsible to notify Mediator Event Consumers about SAT Mediator Events.
*
*  @lib SatEngine
*  @since S60 v3.1
*/
class MSatMediatorEventProvider
    {
    
public:  

    MSatMediatorEventProvider() {};

    virtual ~MSatMediatorEventProvider() {};

    /**
     * Raises SAT Mediator Event.
     *
     * @since S60 v3.1
     * @param aData contains packaged event information for event consumer
     * @return KErrNone or possible error code
     */
    virtual TInt RaiseSatEvent( const TDesC8& aData ) = 0;

    /**
     * Checks Cover UI support for SAT Mediator Event.
     *
     * @since S60 v3.1
     * @return ETrue if cover Ui is supported
     */        
    virtual TBool CoverUiSupported() = 0;

private:

    /**
     * Prohibit copy constructor if not deriving from CBase.
     *
     * @since S60 v3.1
     * @param const reference to MSatMediatorEventProvider.
     */
    MSatMediatorEventProvider( const MSatMediatorEventProvider& );

    /**
     * Prohibit assigment operator if not deriving from CBase.
     *
     * @since S60 v3.1
     * @param const reference to MSatMediatorEventProvider.
     */
    MSatMediatorEventProvider& 
            operator=( const MSatMediatorEventProvider& );

    };

#endif // M_MSATMEDIATOREVENTPROVIDER_H

