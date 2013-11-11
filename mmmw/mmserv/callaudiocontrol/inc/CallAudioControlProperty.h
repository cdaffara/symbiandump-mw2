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
 * Description:  Defines the CallAudioControlProperty class used by CallAudioControl
 *
 */

#ifndef CallAudioControlProperty_H
#define CallAudioControlProperty_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h> 
#include "CallAudioControlUtility.h"

// FORWARD DECLARATIONS
class CCallAudioControlImpl;

// CLASS DECLARATION
class CCallAudioControlProperty : public CActive
    {

public:

    static CCallAudioControlProperty* NewL(
            CCallAudioControlImpl* aImplParent, const TUid aUid,
            const TUint32 aKey, TAction aAction);

    ~CCallAudioControlProperty();

    void Subscribe();
    void Unsubscribe();
    TInt Get(TInt& aValue);
    TInt Set(TInt aValue);
private:

    // For CActive:
    void DoCancel();
    void RunL();
    TInt RunError();

    void ConstructL(CCallAudioControlImpl* aImplParent, const TUid aUid,
            const TUint32 aKey, TAction aAction);
    CCallAudioControlProperty();

    // Data:
    CCallAudioControlImpl* iImplParent;
    RProperty iProperty;
    //TRequestStatus iStatus; -MB- REMOVED
    TAction iAction;
    // TBool iSubscribed;	

    };

#endif      // CallAudioControlProperty_H
// End of File
