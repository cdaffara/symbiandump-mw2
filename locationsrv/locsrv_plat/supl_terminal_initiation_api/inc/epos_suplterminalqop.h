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
* Description:  Client-Server communication
*
*/

/*! \file epos_suplterminalqop.h
    \brief Quality of position parameters 
*/

#ifndef __EPOS_SUPLTERMINALQOP_H__
#define __EPOS_SUPLTERMINALQOP_H__

// Forward declarations

/**
 * This class is used to specify the desired quality of position.  The quality of 
 * position includes horizontal accuracy, vertical accuracy, maximum
 * location age and response time.  By default, all these will 
 * have value as zero.
 *
 * Horizontal accuracy is the radius of error estimate circle specified in meters.
 * Horizontal accuracy reflects the error for the latitude and longitude 
 * components of a position. Vertical accuracy is the error in the altitude 
 * component. Vertical accuracy specifies a linear value at a vertical distance 
 * from the origin of the error estimate circle within which the location of the point falls.
 * Both horizontal accuracy and vertical accuracy values are specified in meters.
 * Maximun location age is the maximum age of the position fix that the  
 * client is willing to accept.  Respone time is the desired
 * response time after issuing location request.  Maximum location age and response time are 
 * specified in seconds. Response time is also referred as delay.
 * Horizontal accuracy can take values from 0 to 1800000.
 * Vertical accuracy can take values from 0 to 65535.
 * Maximum location age can take values from 0 to 65535.
 * Respone time time can take value 1 to 65535.
 *
 */
class TSuplTerminalQop
	{
public:
    /**
     * Constructor for TSuplTerminalQop
     */
	IMPORT_C TSuplTerminalQop();

    /**
     * Sets QoP parameters.  Client can set only required parameters by specifying reset values
     * for other parameters.  Reset values are defined in epos_suplterminalconstants.h and
     * allowed values are:
     *      - For horizontal accuracy reset value is @ref KUndefinedHorizontalAccuracy
     *      - For vertical accuracy reset value is @ref KUndefinedVerticalAccuracy
     *      - For maximum location age reset value is @ref KUndefinedMaxLocationAge
     *      - For delay reset value is @ref KUndefinedDelay
     * 
     * @param [IN] aHorAccuracy Horizontal accuracy in meters and can take values from 0 to 1800000 or reset value
     * @param [IN] aVerAccuracy Vertical accuracy in meters and can take values from 0 to 65535 or reset value
     * @param [IN] aMaxLocationAge Maximum location age in seconds, can take values from 0 to 65535 or reset value
     * @param [IN] aDelay Response time in seconds, can take values from 1 to 65535 or reset value
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range or values specified are not reset value
     */
	IMPORT_C TInt Set(TReal32 aHorAccurary, TReal32 aVerAccuracy, TInt aMaxLocationAge, TInt aDelay);

    /**
     * Sets horizontal accuracy.  Client can reset value by using @ref KUndefinedHorizontalAccuracy as value
     * for the parameter.
     * 
     * @param [IN] aHorAccuracy Horizontal accuracy in meters and can take values from 0 to 1800000 or reset value
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if argument is not in the specified range or value specified is not the reset value
     */
	IMPORT_C TInt SetHorizontalAccuracy(TReal32 aHorAccurary);

    /**
     * Sets vertical accuracy.  Client can reset value by using @ref KUndefinedVerticalAccuracy as value
     * for the parameter.
     * 
     * @param [IN] aVerAccuracy Vertical accuracy in meters and can take values from 0 to 65535 or reset value
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if argument is not in the specified range or value specified is not the reset value
     */
	IMPORT_C TInt SetVerticalAccuracy(TReal32 aVerAccuracy);

    /**
     * Sets maximum location age.  Client can reset value by using @ref KUndefinedMaxLocationAge value
     * for the parameter.
     * 
     * @param [IN] aMaxLocationAge Maximum location age in seconds, can take values from 0 to 65535 or reset value
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if argument is not in the specified range or value specified is not the reset value
     */
	IMPORT_C TInt SetMaxLocationAge(TInt aMaxLocationAge);

    /**
     * Sets delay.  Client can reset value by using @ref KUndefinedDelay value
     * for the parameter.
     * 
     * @param [IN] aDelay Response time in seconds, can take values from 1 to 65535 or reset value
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if argument is not in the specified range or value specified is not the reset value
     */
	IMPORT_C TInt SetDelay(TInt aDelay);

    /**
     * Returns QoP parameters. 
     *
     * @param [OUT] aHorAccuracy Horizontal accuracy in meters and can have values from 0 to 1800000 or reset value
     * @param [OUT] aVerAccuracy Vertical accuracy in meters and can have values from 0 to 65535 or reset value
     * @param [OUT] aMaxLocationAge Maximum location age in seconds, can have values from 0 to 65535 or reset value
     * @param [OUT] aDelay Response time in seconds, can have values from 1 to 65535 or reset value
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters has reset value 
     */
	IMPORT_C TInt Get(TReal32& aHorAccurary, TReal32& aVerAccuracy, TInt& aMaxLocationAge, TInt& aDelay) const;

    /** 
     * Returns Horizontal Accuracy in meters
     *
     * @param [OUT] aHorAccuracy Horizontal accuracy in meters and can have values from 0 to 1800000
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if Horizontal accuracy has reset value 
     */ 
	IMPORT_C TInt GetHorizontalAccuracy(TReal32& aHorAccurary) const;

    /** 
     * Retrieves Vertical Accuracy
     *
     * @param [OUT] aVerAccuracy Vertical accuracy in meters and can have values from 0 to 65535
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if Vertical accuracy has reset value 
     */ 
	IMPORT_C TInt GetVerticalAccuracy(TReal32& aVerAccurary) const;

    /** 
     * Retrieves Maximum Location Age
     *
     * @param [OUT] aMaxLocationAge Maximum location age in seconds, can have values from 0 to 65535 
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if maximum location age has reset value 
     */ 
	IMPORT_C TInt GetMaxLocationAge(TInt& aMaxLocationAge) const;

    /** 
     * Retrieves delay 
     *
     * @param [OUT] aDelay Response time in seconds, can have values from 1 to 65535
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if delay has reset value 
     */ 
	IMPORT_C TInt GetDelay(TInt& aDelay) const;




private:
    TReal32 iHorizontalAccuracy;
    TReal32 iVerticalAccuracy;
    TInt    iMaxLocationAge;
    TInt    iResponseTime;
	};
#endif // __EPOS_SUPLTERMINALQOP_H__
