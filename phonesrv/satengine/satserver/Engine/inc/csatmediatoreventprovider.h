/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides events to mediator event consumer.
*
*/



#ifndef C_CSATMEDIATOREVENTPROVIDER_H
#define C_CSATMEDIATOREVENTPROVIDER_H


#include <e32base.h>

#include "msatmediatoreventprovider.h"

class CMediatorEventProvider;

/**
 *  Mediator event provider
 *
 *  Provides SAT mediator events to mediator event consumer ( CoverUi )
 *
 *  @lib SatEngine
 *  @since S60 v3.1
 */
class CSatMediatorEventProvider : public CBase,
                                  public MSatMediatorEventProvider
    {
public:  

    static CSatMediatorEventProvider* NewL();

    virtual ~CSatMediatorEventProvider();

// from base class MSatMediatorEventProvider

    /**
     * From MSatMediatorEventProvider.
     * Raises SAT mediator event 
     *
     * @since S60 v3.1
     * @param const TDesC8& ontains packaged data to mediator event consumer
     */
    TInt RaiseSatEvent( const TDesC8& aData );

    /**
     * From MSatMediatorEventProvider.
     * Checks if Cover UI is supported by SAT
     *
     * @since S60 v3.1
     * @return ETrue if Cover UI is supported
     */        
    TBool CoverUiSupported();

private:

    CSatMediatorEventProvider();

    void ConstructL();
        
    /**
     * Internal check for Cover Ui support
     * Checks if Cover UI is supported by SAT and stores the result 
     *
     * @since S60 v3.1
     */
    void CUiSupportedL();

private: // data
    
    /**
     * are mediator events registered
     */
    TBool iRegistered;
    
    /**
     * is cover UI supported
     */
    TBool iCoverUiSupported;
    
    /**
     * pointer to Mediator client
     * Own.
     */    
    CMediatorEventProvider* iEventProvider;
        
    };
#endif // C_CSATMEDIATOREVENTPROVIDER_H
