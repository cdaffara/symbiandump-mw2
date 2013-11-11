/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   COMASuplDMParam Class definition
*
*/




#ifndef C_COMASUPLDMPARAM_H
#define C_COMASUPLDMPARAM_H


#include <e32base.h>


/**
 * COMASuplDMParam represents OMA SUPL DM Settings
 * @since S60 v3.1
 */
class COMASuplDMParam : public CBase
    {
public: // enums

    typedef enum
        {
        EGet,
        EAdd,
        EDelete,
        EReplace,
        EExec,
        ECopy
        } TCommand;

public:

    /**
     * Factory function that instantiates a new object of COMASuplDMParam
     *
     * @since S60 v3.0
     */
    static COMASuplDMParam* NewL(const TDesC8& aURI,
                                const TDesC8& aLUID, 
                                const TDesC8* aObject, 
                                const TDesC8& aType, 
                                const TInt aStatusRef,
                                const TInt aResultRef,
                                const TCommand aCommand);

    /**
     * Destructor
     *
     */
    ~COMASuplDMParam();

    /**
     * Retrieves SUPL DM parameter values
     *
     * @since S60 v3.1
     */
    void Get(HBufC8*& aURI,
                HBufC8*& aLUID,
                HBufC8*& aObject,
                HBufC8*& aType, 
                TInt& aStatusRef,
                TInt& aResultRef,
                TCommand& aCommand);

    /**
     * Initializes SUPL DM parameter values
     *
     * @since S60 v3.1
     */
    void Set(HBufC8* aURI,
                HBufC8* aLUID,
                HBufC8* aObject,
                HBufC8* aType, 
                TInt aStatusRef,
                TInt aResultRef,
                TCommand aCommand);

private:

    /**
     * Constructor
     * 
     */
    COMASuplDMParam();

    /**
     * 2nd phase constuctor for instantiating member variables
     *
     * @since S60 v3.0
     */
    void ConstructL(const TDesC8& aURI,
                        const TDesC8& aLUID, 
                        const TDesC8* aObject, 
                        const TDesC8& aType, 
                        const TInt aStatusRef,
                        const TInt aResultRef,
                        const TCommand aCommand);

private:

    /**
     * URI of the SUPL setting parameter
     */
    HBufC8*     iURI;

    /**
     * UID
     */
    HBufC8*     iLUID;

    /**
     * Data of the object
     */
    HBufC8*     iObject;

    /**
     * MIME type
     */    
    HBufC8*     iType;

    /**
     * Command reference
     */
    TInt        iStatusRef;

    /**
     * Result code
     */    
    TInt        iResultRef;

    /**
     * Type of command
     */
    TCommand    iCommand;

    };

#endif  // C_COMASUPLDMPARAM_H
