/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This is the main interface to Radio Utilities API. This class acts
*                as factory for FM Tuner Utility, Radio Player Utility, and Radio
*                RDS Utility. All utility objects returned by this factory are owned
*                by CRadioUtility and should not be deleted by the client.
*
*
*/



#ifndef C_RADIOUTILITY_H
#define C_RADIOUTILITY_H

#include <e32base.h>

class CRadioFmTunerUtility;
class MRadioFmTunerObserver;
class CRadioPlayerUtility;
class MRadioPlayerObserver;
class CRadioRdsUtility;
class MRadioRdsObserver;

/**
 *  This class provides an interface for accessing other radio utility interfaces.
 *
 *  @lib RadioUtility.lib
 *  @since S60 3.2
 */
class CRadioUtility : public CBase
    {
public:

    /**
     * Factory function to create an instance of the radio utility.
     *
     * @since S60 3.2
     * @param aPrimaryClient Indicates whether the client is a primary client. Primary
     * clients are clients that can control the radio tuner such as FM Radio Application,
     * Visual Radio, or a Java Radio App.  Non-primary clients are observers of the tuner,
     * player, and RDS utilities and cannot exist without a primary client such as Active
     * Idle, Cover UI, or a smart accessory driver.
     */
    IMPORT_C static CRadioUtility* NewL( TBool aPrimaryClient = ETrue );

    IMPORT_C virtual ~CRadioUtility();

    /**
     * Returns a reference to FM Tuner Utility class for controlling the tuner settings.
     * NOTE: Object is owned by CRadioUtility and should not be deleted by the client.
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     * @return Reference to FM Tuner Utility
     */
    IMPORT_C CRadioFmTunerUtility& RadioFmTunerUtilityL( MRadioFmTunerObserver& aObserver );

    /**
     * Returns a reference to Radio Player Utility class for controlling the radio playback.
     * NOTE: Object is owned by CRadioUtility and should not be deleted by the client.
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     * @return Reference to Radio Player Utility
     */
    IMPORT_C CRadioPlayerUtility& RadioPlayerUtilityL( MRadioPlayerObserver& aObserver );

    /**
     * Returns a reference to Radio RDS Utility class for accessing the Radio Data System (RDS)
     * for FM sound broadcasting. CRadioRdsUtility can also be used with Radio Broadcast Data
     * System (RDBS).
     *
     * Client must first instantiate CRadioFmTunerUtility and check the tuner capabilities to
     * see if RDS is supported prior to calling this function.
     * NOTE: Object is owned by CRadioUtility and should not be deleted by the client.
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     * @return Reference to Radio RDS Utility
     */
    IMPORT_C CRadioRdsUtility& RadioRdsUtilityL( MRadioRdsObserver& aObserver );

private:

    CRadioUtility();

    void ConstructL();

public: 

	  class CBody;

private: // data
    /**
     * Radio utility implementation body
     * Own.
     */
    CBody* iBody;
    };

#endif // C_RADIOUTILITY_H
