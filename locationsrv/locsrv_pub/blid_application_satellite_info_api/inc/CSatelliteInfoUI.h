/*
* Copyright (c) 2005 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implements interface for SatelliteInfoUI.
*
*/


#ifndef __SATELLITE_INFO_UI_H__
#define __SATELLITE_INFO_UI_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATION
class CSatellite;

// CLASS DECLARATION
/**
 * Implements entry point class to Satellite Info UI
 */
 
class CSatelliteInfoUI : public CBase
    {
    public:
    	/**
    	* Enumeration to specify the default launch view of the 
    	* dialog. 
    	*/
    	enum TSatelliteView
    		{
    	    /** Launch option for firmament view. Firmament view
    		  * displays all the satellites in view with the satellite's
    		  * number on a firmament.
    		  */
    		ESatelliteFirmamentView 		= 0x0001,
    	    /** Launch option for signal strength view. Signal strength
    	      * view displays all the satellite with their correspoinding
    	      * signal strength represented by bars.
    		  */
			ESatelliteSignalStrengthView	= 0x0002,
    	    /** Launch option for compass view. Compass view
    	      * displays latitude, longitude, speed and direction along
    	      * with 2D/3D type of Fix.
    		  */
			ESatelliteCompassView	= 0x0003
			};
    		
    public:
        /**
         * Two-phased constructor.
         *
         * @return CSatelliteInfoUI object.
         */
        IMPORT_C static CSatelliteInfoUI* NewL();

        /**
         * Destructor.
         */
        IMPORT_C virtual ~CSatelliteInfoUI();

    private:
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

        /**
         * C++ default constructor.
         */
        CSatelliteInfoUI();

    public: // new functions
        /**
         * Display satellite's information dialog.
         * 
         * This library uses the services provided by Location Framework.
         * Once the dialog is launched satellite information is continuously 
         * requested via Location Acquisition API. The Location Acquisition 
         * API is offered by Location Framework. The user can switch between the 
         * two views once the dialog is launched.
         *
         * @param aNameOfRule is requestor data for Location FW which will be used 
         * for privacy verification in the future. Application name should be 
         * used to specify the requestor. The string should not be empty.
         * @return TInt value of the selected softkey, which is EEikCommandTypeExitOrBack,
         * because it is the only softkey of the dialog.
         *
         * @leave KErrArgument if requestor data (aNameOfRule argument) length exceeds 
         * 255 characters or if it is empty. This function may also leave with any 
         * one of the standard error codes such as out of memory (e.g. KErrNoMemory)
         */
        IMPORT_C TInt ExecuteLD( const TDesC& aNameOfRule );

        /**
         * Dialog switched to foreground or background
         * @param aForeground ETrue to switch to the foreground. 
         * EFalse to switch to background.
         */
        IMPORT_C void HandleForegroundEventL(TBool aForeground);
        
        /**
        * Used to set the dialog's launch view
        * 
        * This method is used to set the view in which the dialog should
        * be launched. The two available views are signal strength and
        * firmament view. Constants for settings default view specified 
        * in enum #TSatelliteView. This method should be called before the 
        * method #ExecuteLD is invoked. 
        * 
        * @param aLaunchView ESatelliteFirmamentView for firmament view 
        * and ESatelliteSignalStrengthView for signal strength view.
        * ESatelliteCompassView for compass ciew
        * ESatelliteCompassView Visibility will be variated depending on the product configuration/regional variation.
        * if it is disabled to show compass view then function will ignore the ESatelliteCompassView and 
        * show firmament view instead.
        *
        * @panic EAknPanicOutOfRange if the method is invoked with an invalid parameter.
        * Values provided apart from those specified in #TSatelliteView are
        * invalid and will cause the method to panic.
        */
        IMPORT_C void SetLaunchView(TSatelliteView aLaunchView);

    private: //Data

        /// Own: A pointer to CSatellite. Contains the engine and
        /// the dialog implementation.
        CSatellite* iSatellite;

        /// Own: A pointer to TBool
        TBool* iDestroyedPtr;

    };

#endif // __SATELLITE_INFO_UI_H__

// End of File
