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
 * Description: VOIP Audio Services  Defines buffer type for VoIP data
 *              streaming.
 *
 */

#ifndef VOIPDATABUFFERIMPL_H
#define VOIPDATABUFFERIMPL_H

#include <e32base.h>
#include <voipdatabuffer.h>

/**
 *  CVoIPDataBuferImpl
 */
NONSHARABLE_CLASS(CVoIPDataBufferImpl) : public CBase,
                                         public CVoIPJBDataBuffer
    {
public:
    /**
     * 2-phased constructor
     *
     * @param TInt
     * @return none
     */
    static CVoIPDataBufferImpl* NewL(TInt aBufferLen);

    /**
     * Destructor
     *
     * @param none
     * @return none
     */
    virtual ~CVoIPDataBufferImpl();

    /**
     * GetPayloadPtr
     *
     * @param TPtr8&
     * @return none
     */
    void GetPayloadPtr(TPtr8& aPayloadPtr);

    /**
     * SetPayloadPtr
     *
     * @param TPtr8
     * @return none
     */
    void SetPayloadPtr(TPtr8 aPayloadPtr);

    /**
     * GetPayloadPtr
     *
     * @param CVoIPDataBuffer::TVoIPBufferType&
     * @return none
     */
    void GetBufferType(CVoIPDataBuffer::TVoIPBufferType& aType);

protected:
    /**
     * Constructor
     *
     * @param none
     * @return none
     */
    CVoIPDataBufferImpl();

    /**
     * Symbian constructor
     *
     * @param TInt
     * @return void
     */
    void ConstructL(TInt aBufferLen);

protected:
    CVoIPDataBuffer::TVoIPBufferType iType;
    TPtr8 iPayloadPtr;
    HBufC8* iBuf;
    TInt iBufferLen;
    };

#endif //VOIPDATABUFFERIMPL_H

//End of file
