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
* Description:  This is the implementation of the ConfigurationComponentsFactoryImpl class.
*
*/


#ifndef CONFIGURATIONCOMPONENTSFACTORYIMPL_H
#define CONFIGURATIONCOMPONENTSFACTORYIMPL_H

#include <e32base.h>
#include <ConfigurationComponentsFactory.h>


class CRestrictedAudioOutput;
class CAudioOutputControlUtilityProxy;


/**
 * CConfigurationComponentsFactoryImpl class documentation.
 *
 * @lib ConfigurationComponentsFactory.lib
 *
 * @since S60 5.0 
 */
class CConfigurationComponentsFactory::CBody : public CBase
	{

public:
        
	// Constructors and destructor

    /**
     * Constructs, and returns a pointer to, a new CBody object.
     * Leaves on failure..
     * @return CBody* A pointer to newly created object.
     */
     static CBody* NewL();


    /**
     * Destructor
     *
     */
    ~CBody();

    /**
     * Creates RestrictedAudioOutputProxy
     *
     * @param aOutput to be appended.
     * @return KErrNone if successful. KErrAlreadyExists if aOutput is already
     *  appended before. KErrIncompleteInitialization if ConstructL is not called
     *  by the deriving class. Otherwise any systemwide error code.
     */
    TInt CreateRestrictedAudioOutput(CMMFDevSound& aDevSound, CRestrictedAudioOutput*& aAudioOutput);
    
    /**
     * Creates AudioOutputControlUtility
     *
     * @param aOutput to be appended.
     * @return KErrNone if successful. KErrAlreadyExists if aOutput is already
     *  appended before. KErrIncompleteInitialization if ConstructL is not called
     *  by the deriving class. Otherwise any systemwide error code.
     */
    TInt CreateAudioOutputControlUtility(CAudioOutputControlUtility*& aAudioOutputControlUtility);    

    
    private:

        /**
        * C++ default constructor.
        */
        CBody();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
	// Data:
	

    private:    // Friend classes
        friend class CConfigurationComponentsFactory;
        CRestrictedAudioOutput* iAudioOutput;
       	CAudioOutputControlUtility* iAudioOutputControlUtility;
	        
    

	};

#endif // CONFIGURATIONCOMPONENTSFACTORYIMPL_H

// End of file
