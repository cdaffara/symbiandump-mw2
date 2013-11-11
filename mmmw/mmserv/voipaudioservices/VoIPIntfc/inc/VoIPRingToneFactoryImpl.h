/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VOIP Audio Services
 *
 */

#ifndef RINGTONEFACTORYIMPL_H
#define RINGTONEFACTORYIMPL_H

// FORWARD DECLARATIONS
class CRingTonePlayer;

/**
 *  CRingToneFactoryImpl class
 *
 *  @lib VoIPAudioIntfc.dll
 */
NONSHARABLE_CLASS(CRingToneFactoryImpl) : public CBase
    {
public:

    /**
     * 2-phase constructor
     *
     *
     * @param none
     * @return CRingToneFactoryImpl* -
     */
    static CRingToneFactoryImpl* NewL();

    /**
     * Destructor
     *
     *
     * @param none
     * @return none
     */
    virtual ~CRingToneFactoryImpl();

    /**
     *
     * @param CRingTonePlayer*&
     * @return TInt
     */
    TInt CreateRingTonePlayer(CRingTonePlayer*& aRingTonePlayer);

private:

    /**
     * Constructor
     *
     *
     * @param none
     * @return none
     */
    CRingToneFactoryImpl();

    /**
     * Symbian constructor
     *
     *
     * @param none
     * @return none
     */
    void ConstructL();
    };

#endif //RINGTONEFACTORYIMPL_H

// End of file
