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
* Description:  Geographical rectangular area class definition.
*
*/


#ifndef LBTGEORECT_H
#define LBTGEORECT_H

#include "lbtgeoareabase.h"

/**
 * Geographical rectangular area.
 *
 * A geographical rectangular area is defined as a spherical rectangle, 
 * limited by two longitude borders ( WestLongitude and EastLongitude ) and 
 * two latitude borders ( SouthLatitude and NorthLatitude ).
 *
 * It is required that - 90 =< SouthLatitude =< NorthLatitude =< 90. 
 * WestLongitude must be in the interval [- 180, 180 ), i.e. not including + 180.
 * EastLongitude must be in the interval [- 180, 180], i.e. including + 180.
 * It is allowed that EastLongitude < WestLongitude. This defines an area
 * which crosses the 180 meridian. 
 */
class CLbtGeoRect : public CLbtGeoAreaBase
    {
public:
    /**
     * Allocates and constructs a new geographical rectangular area object.
     * In returned object, the border values are set to NaN.
     *
     * @return Pointer to the new constructed geographical rectangular 
     * area object.
     */
    IMPORT_C static CLbtGeoRect* NewL();


    /**
     * Allocates and constructs a new geographical rectangular area object
     * and pushes it onto cleanup stack.
     * In returned object, the border values are set to NaN.
     *
     * @return Pointer to the new constructed geographical rectangular 
     * area object.
     */
    IMPORT_C static CLbtGeoRect* NewLC();

    /**
     * Allocates and constructs a new geographical rectangular area object.
     *
     * @param[in] aSouthLatitude The southern latitude border of the area.
     * @param[in] aNorthLatitude The northern latitude border of the area.
     * @param[in] aWestLongitude The western longitude border of the area.
     * @param[in] aEastLongitude The eastern longitude border of the area.
     * @return Pointer to the new constructed geographical rectangular 
     * area object.
     */
    IMPORT_C static CLbtGeoRect* NewL( 
        TReal  aSouthLatitude,
        TReal  aNorthLatitude,
        TReal  aWestLongitude,
        TReal  aEastLongitude );

    /**
     * Allocates and constructs a new geographical rectangular area object
     * and pushes it onto cleanup stack.
     *
     * @param[in] aSouthLatitude The southern latitude border of the area.
     * @param[in] aNorthLatitude The northern latitude border of the area.
     * @param[in] aWestLongitude The western longitude border of the area.
     * @param[in] aEastLongitude The eastern longitude border of the area.
     * @return Pointer to the new constructed geographical rectangular 
     * area object.
     */
    IMPORT_C static CLbtGeoRect* NewLC( 
        TReal  aSouthLatitude,
        TReal  aNorthLatitude,
        TReal  aWestLongitude,
        TReal  aEastLongitude );
    
    
    /**
     * Destructor
     */
    IMPORT_C ~CLbtGeoRect();

    /**
     * Returns the type of geographical area, CLbtGeoAreaBase::ERectangular.
     *
     * @return CLbtGeoAreaBase::ERectangular.
     */
    virtual TGeoAreaType Type() const;

    /**
     * Gets border values. If the border value has not been set
     * before, the returned border values are NaN.
     *
     * @param[out] aSouthLatitude The southern latitude border of the area.
     * @param[out] aNorthLatitude The northern latitude border of the area.
     * @param[out] aWestLongitude The western longitude border of the area.
     * @param[out] aEastLongitude The eastern longitude border of the area.
     */
    IMPORT_C void GetBorder( 
        TReal&  aSouthLatitude,
        TReal&  aNorthLatitude,
        TReal&  aWestLongitude,
        TReal&  aEastLongitude ) const;
    
    /**
     * Sets the border values.
     *
     * The input parameters specify the area as described in the
     * class description.
     *
     * If one of the parameters is out of range, this function fails with
     * panic with @p KLbtErrArgument. The correct ranges are:
     *   -# - 90 =< aSouthLatitude =< aNorthLatitude =< 90
     *   -# - 180 <= aWestLongitude <= 180
     *   -# - 180 <= aEastLongitude <= 180
     *
     * @param[in] aSouthLatitude The southern latitude border of the area.
     * @param[in] aNorthLatitude The northern latitude border of the area.
     * @param[in] aWestLongitude The western longitude border of the area.
     * @param[in] aEastLongitude The eastern longitude border of the area.
     * @panic LocTriggering ELbtErrArgument if the input
     *   parameters are invalid.
     */
    IMPORT_C void SetBorder( 
        TReal  aSouthLatitude,
        TReal  aNorthLatitude,
        TReal  aWestLongitude,
        TReal  aEastLongitude );

protected:
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL(RWriteStream& aStream) const ;
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL(RReadStream& aStream)  ;

private:
    /**
     * Constructor.
     */
    CLbtGeoRect();
    
    /**
     * By default, prohibit copy constructor
     */
    CLbtGeoRect( const CLbtGeoRect& );
    
    /**
     *  Prohibit assigment operator
     */
    CLbtGeoRect& operator= ( const CLbtGeoRect& );
    
    /**
     * Symbian 2nd phase constructor
     */    
    void ConstructL();
    
    /**
     * Symbian 2nd phase constructor
     *
     * @param[in] aSouthLatitude The southern latitude border of the area.
     * @param[in] aNorthLatitude The northern latitude border of the area.
     * @param[in] aWestLongitude The western longitude border of the area.
     * @param[in] aEastLongitude The eastern longitude border of the area.
     */
    void ConstructL( TReal  aSouthLatitude,
        TReal  aNorthLatitude,
        TReal  aWestLongitude,
        TReal  aEastLongitude );

private:
    /**
     * The southern latitude border of the area.
     */
    TReal iSouthLatitude;
    
    /**
     * The northern latitude border of the area. 
     */
    TReal iNorthLatitude;
    
    /**
     * The western longitude border of the area.
     */
    TReal iWestLongitude;
    
    /**
     * The eastern longitude border of the area.  
     */
    TReal iEastLongitude;
    };

#endif // LBTGEORECT_H
