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
* Description: Configuration Components Factory
*
*/


#ifndef CONFIGURATIONCOMPONENTSFACTORY_H
#define CONFIGURATIONCOMPONENTSFACTORY_H

#include <e32base.h>

class CConfigurationComponentsFactoryImpl;
class CMMFDevSound;
class CRestrictedAudioOutput;
class CAudioOutputControlUtility;

/**
 * CConfigurationComponentsFactory class documentation.
 *
 * @lib ConfigurationComponentsFactory.lib
 *
 * @since S60 5.0
 */
class CConfigurationComponentsFactory : public CBase
	{
public:


  	/**
     * Constructs, and returns a pointer to, a new CConfigurationComponentsFactory
     * object.
     * Leaves on failure..
     * @param
     * @return CConfigurationComponentsFactory* A pointer to newly created object.
     */
    IMPORT_C static TInt CreateFactoryL(CConfigurationComponentsFactory*& aFactory);


    /**
     * Destructor
     *
     */
    IMPORT_C virtual ~CConfigurationComponentsFactory();

    /**
     * Creates a CRestrictedAudioOutput obj
     *
     * @param aDevSound, aAudioOutput
     * @return KErrNone if successful.
     */
    IMPORT_C TInt CreateRestrictedAudioOutput(CMMFDevSound& aDevSound, CRestrictedAudioOutput*& aAudioOutput);

   /**
     * Creates a CAudioOutputControlUtility obj.
     *
     * @param aAudioOutputControlUtility
     * @return KErrNone if successful.
     */
    IMPORT_C TInt CreateAudioOutputControlUtility(CAudioOutputControlUtility*& aAudioOutputControlUtility);

protected:

    /**
     * Constructor
     *
     */
    #ifdef __WINSCW__
    IMPORT_C CConfigurationComponentsFactory();
    IMPORT_C void ConstructL();
    #else 
    CConfigurationComponentsFactory();
    void ConstructL();
    #endif

    /**
     * Second phase constructor. The derived class should call this during
     * construction.
     *
     */
    protected:  // Data
        // Actual implementation class.
        class CBody;

        //DevSoundAdaptation body implementation
        CBody* iBody;

private:
    CConfigurationComponentsFactoryImpl* iImpl;
	};

#endif // CONFIGURATIONCOMPONENTSFACTORY_H

// End of file
