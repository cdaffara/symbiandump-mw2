/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of TestData class.
*
*/


#ifndef TESTDATA_H
#define TESTDATA_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS


// CLASS DECLARATIONS

/**
 * Virtual Phonebook contact link array.
 * This interface can be used to handle a read-only array of contact links.
 */
class TestData
    {
public:

    /**
     * Constructor.
     */
    TestData(){};
    
    /**
     * Destructor.
     */
    virtual ~TestData(){};

public: // data
    
    static TBool iFeatureIdCsVideoTelephonySupported;
    static TInt iCallingServiceByCallType;
    static TBool iCheckSupportedCase;
    
    };

#endif // TESTDATA_H

// End of File
