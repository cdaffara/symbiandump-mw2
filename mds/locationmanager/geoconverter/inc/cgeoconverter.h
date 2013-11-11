/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A class for converting CGI Info to lat, long.
*
*/

#ifndef C_GEOCONVERTER_H
#define C_GEOCONVERTER_H

/**
 *
 *  @since S60 9.2
 */
#ifdef LOC_GEOTAGGING_CELLID
#include <lbslocationinfoconverter.h>
#include <lbslocationinfo.h>
#endif
#include <lbsposition.h>
#include <etel3rdparty.h>

class MGeoConverterObserver
    {
public:    
    /**
     * This method is used for notifying completion of geotagging
     * @param aError error code
     * @param aPosition position (lat/lon) for the correcponding n/w info
     */
    virtual void ConversionCompletedL( const TInt aError, TLocality& aPosition ) = 0;

	/**
	* This method is used to handle any error during conversion
	* @param aError the error code
	*/
	virtual void HandleConversionError(TInt aError) = 0;
    };

// Class to handle conversion operations
class CGeoConverter : public CBase
#ifdef LOC_GEOTAGGING_CELLID    
               , public MLbsLocationInfoConverterObserver
#endif
    {     
public:
    /**
     * 2-phased constructor.
     */
    IMPORT_C static  CGeoConverter* NewL( MGeoConverterObserver& aObserver );
    /**
     * C++ destructor.
     */
    IMPORT_C virtual ~CGeoConverter();
    
    IMPORT_C void ConvertL( const CTelephony::TNetworkInfoV1& aNetworkInfo );

#ifdef GEOCONVERTER_UNIT_TESTCASE
    public:
#else
protected:
#endif
    // From MLbsLocationInfoConverterObserver
    void OnConversionComplete( TInt aStatusCode );
    
#ifdef GEOCONVERTER_UNIT_TESTCASE
    public:
#else    
    private:
#endif    

    /**
     * C++ constructor.
     */
    CGeoConverter( MGeoConverterObserver& aObserver );
    /**
     * 2nd phase constructor.
     */
    void ConstructL();    
    
#ifdef GEOCONVERTER_UNIT_TESTCASE
    public:
#else    
    private:
#endif    

    MGeoConverterObserver& iObserver;
#ifdef LOC_GEOTAGGING_CELLID    
    CLbsLocationInfoConverter*  iLocConverter;
    CLbsLocationInfo*   iLocInfo;
    CLbsGsmCellInfo* iGsmCellInfo;
    CLbsWcdmaCellInfo* iWcdmaCellInfo;
#endif
    };

#endif // C_GEOCONVERTER_H

// End of file.
