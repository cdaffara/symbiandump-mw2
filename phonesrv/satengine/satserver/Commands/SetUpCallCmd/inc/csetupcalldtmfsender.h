/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Create the call and emergency call to ETelMM
*
*/


#ifndef CSETUPCALLDTMFSENDER_H
#define CSETUPCALLDTMFSENDER_H

#include <e32base.h>

class MSatMultiModeApi;

NONSHARABLE_CLASS ( CSetupCallDtmfSender ) : public CActive
{
public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the MSatMultiModeApi.
     * @return a pointer to the newly created object.
     */
    static CSetupCallDtmfSender* NewL( MSatMultiModeApi& aPhone );

    /**
     * Destructor.
     */
    virtual ~CSetupCallDtmfSender();

    /**
     * Send Dtmf string after creating a call successfully 
     */
    void SendDtmfString();    

    /**
     * Separate Dtmf string from whole string.
     * Purl tel number will return by aString.
     * Dtmf string will store in data member for furture sending dtmf. 
     */
    void SeparateDtmfFromTelNumber( TDes& aString );
    
protected:

    /**
     * From CActive, handles the request completion.
     */
    void RunL();    
    
    /**
     * From CActive, handle the request cancel
     */
    void DoCancel();
    
private:

    /**
     * C++ default constructor.
     * @param aPhone A reference to MSatMultiModeApi.
     */
    CSetupCallDtmfSender( MSatMultiModeApi& aPhone );
        
private: // Data

    /**
     * Reference to the MSatMultiModeApi
     */
    MSatMultiModeApi& iPhone;
    
    /**
     * Own. Dtmf string
     */    
    HBufC* iDtmfString;    

};

#endif // CSETUPCALLDTMFSENDER_H
