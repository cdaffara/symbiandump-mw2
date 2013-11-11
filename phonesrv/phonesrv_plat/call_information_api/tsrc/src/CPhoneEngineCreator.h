/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/
#ifndef CPHONEENGINECREATOR_H
#define CPHONEENGINECREATOR_H

#include <e32base.h>
#include "T_TelephonyAudioRouting.h"

class CPEMessageWaiter;
class MPEPhoneModel;

/**
 * Synchronise the construction of phoneengine.
 */
NONSHARABLE_CLASS( CPhoneEngineCreator ) : 
    public CBase
    {
public:

    static CPhoneEngineCreator* NewL();
    
    ~CPhoneEngineCreator();
    
    /**
     * Creates phoneengine.
     * @param aReturnImmedialtely, specifies if PE construction ready message is waited or not.     
     */
    MPEPhoneModel* CreatePhoneEngineL(
        CPEMessageWaiter& aMessageWaiter,
        TBool aReturnImmedialtely = EFalse );   
    
    /** 
     *  Access to stub audio factory.
     */
    CStubAudioFactory* AudioFactory() const;
    
protected:
    
    CPhoneEngineCreator();
    
    void ConstructL();

    
protected:
    
    CStubAudioFactory* iStubAudioFactory;
    };

#endif // CPHONEENGINECREATOR_H
