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
* Description:  Handles client side implementation.
*
*/


#ifndef CAUDIOOUTPUTCONTROLUTILITYPROXY_H
#define CAUDIOOUTPUTCONTROLUTILITYPROXY_H

#include <AudioOutputControlUtility.h>




class CAudioOutputControlUtilityProxyImpl;

/**
 * CAudioOutputControlUtilityProxy class documentation.
 *
 * @lib RestrictedAudioOutputProxy.lib
 *
 * @since S60 5.0 
 */
class CAudioOutputControlUtilityProxy: public CAudioOutputControlUtility
	{
    public:  
    

  	/**
     * Constructs, and returns a pointer to, a new 
     * CAudioOutputControlUtilityProxy object.
     * Leaves on failure..
     * @param 
     * @return CAudioOutputControlUtilityProxy* A pointer to newly created object.
     */    
    IMPORT_C static CAudioOutputControlUtilityProxy* NewL();
    
   	IMPORT_C virtual ~CAudioOutputControlUtilityProxy();
    
	private:

		/**
		* Private C++ constructor for this class.
        * @since	5.0
        * @param aMessageHandler reference to message handler
        * @param aCustomCommand reference to custom command utility
        * @return	-
        */
		CAudioOutputControlUtilityProxy();
		
        /**
         * Second phase constructor.
         *
         */
        void ConstructL();    
	};

#endif      // CAUDIOOUTPUTCONTROLUTILITYPROXY_H
            
// End of File
