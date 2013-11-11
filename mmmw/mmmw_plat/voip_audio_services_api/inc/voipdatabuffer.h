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
 * Description:  Defines buffer type for VoIP data streaming.
 *
 */

#ifndef VOIPDATABUFFER_H
#define VOIPDATABUFFER_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CVoIPDataBufferImpl;
class CVoIPJBDataBufferImpl;

/**
 *  CVoIPDataBuffer
 *  ?description
 *
 */
class CVoIPDataBuffer
    {
public:
    /**
     * Buffer types
     */
    enum TVoIPBufferType
        {
        EStandard,
        EJitterBuffer
        };

public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPDataBuffer();

    /**
     * SetPayloadPtr
     * @param TPtr8
     * @return none
     */
    IMPORT_C void SetPayloadPtr(TPtr8 aPayloadPtr);

    /**
     * GetPayloadPtr
     * @param TPtr8&
     * @return none
     */
    IMPORT_C void GetPayloadPtr(TPtr8& aPayloadPtr);

    /**
     * GetBufferType
     * @param TVoIPBufferType&
     * @return none
     */
    IMPORT_C void GetBufferType(TVoIPBufferType& aType);

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPDataBuffer();

    /**
     * 2nd phase constructor
     * @param CVoIPDataBufferImpl*
     * @return void
     */
    void ConstructL(CVoIPDataBufferImpl* aVoIPDataBufferImpl);

private:

    CVoIPDataBufferImpl* iVoIPDataBufferImpl;
    };

/**
 *  CVoIPJBDataBuffer
 *  ?description
 *
 */
class CVoIPJBDataBuffer : public CVoIPDataBuffer
    {
public:
    /**
     * Destructor
     * @param none
     * @return none
     */
    IMPORT_C virtual ~CVoIPJBDataBuffer();

    /**
     * SetBufferSequence
     * @param const TUint - between 0 and 0xFFFF
     * @return none
     */
    IMPORT_C void SetBufferSequence(const TUint aBufferSeq);

    /**
     * GetBufferSequence
     * @param TUint& - between 0 and 0xFFFF
     * @return none
     */
    IMPORT_C void GetBufferSequence(TUint& aBufferSeq);

protected:
    /**
     * Constructor
     * @param none
     * @return none
     */
    CVoIPJBDataBuffer();

    /**
     * 2nd phase constructor
     * @param CVoIPJBDataBufferImpl*
     * @return void
     */
    void ConstructL(CVoIPJBDataBufferImpl* aVoIPJBDataBufferImpl);

private:

    CVoIPJBDataBufferImpl* iVoIPJBDataBufferImpl;
    };

#endif //VOIPDATABUFFER_H

// End of file
