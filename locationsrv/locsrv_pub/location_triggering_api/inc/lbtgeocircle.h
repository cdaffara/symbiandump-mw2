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
* Description:  Geographical circular area class definition.
*
*/


#ifndef LBTGEOCIRCLE_H
#define LBTGEOCIRCLE_H

#include <lbtgeoareabase.h>
#include <lbsposition.h>

/**
 * Geographical circular area.
 *
 * A geographical circular area is defined by the center of the 
 * circle and the radius. The radius is in meters. The altitude in 
 * the center's coordinate is always ignored. It is optionally also possible 
 * to provide additional geographical area information of CLbtGeoCell 
 * type. Providing additional cell information would enable further 
 * optimization by reducing the number of GPS fixes required to
 * supervise the trigger, there by, reducing the impact on battery
 * consumption. Triggers would, initially, be monitored using the cell 
 * information provided. GPS would be used to check the coordinates 
 * only when the mobile terminal reaches the cell / cells specified. 
 * If a particular location is served by multiple cells, then it is 
 * important to mention all available cell information. If not, there 
 * is a possibility of missing triggers even when the mobile terminal 
 * reaches the location specified by the coordinates.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtGeoCircle : public CLbtGeoAreaBase
    {
public:
    /**
     * Allocates and constructs a new geographical circular area object.
     * In the returned object, the latitude, longitude and altitude 
     * are set to NaN, and the radius is set to NaN.
     *
     * @return Pointer to the new geographical circular area object.
     */
    IMPORT_C static CLbtGeoCircle* NewL();

    /**
     * Constructs a new geographical circular area object and pushes
     * it onto cleanup stack. 
     * In the returned object, the latitude, longitude and altitude 
     * are set to NaN, and the radius is set to NaN.
     *
     * @return Pointer to the new geographical circular area object.
     */
    IMPORT_C static CLbtGeoCircle* NewLC();

    /**
     * Allocates and constructs a new geographical circular area object
     * with specified center coordinate and radius.
     *
     * @panic ELbtErrArgument If the input radius is negative.
     *
     * @param[in] aCenter The coordinate of the center.
     * @param[in] aRadiusInMeters The radius of the circle in meters.
     * @return Pointer to the new geographical circle area object.
     */
    IMPORT_C static CLbtGeoCircle* NewL( 
        const TCoordinate& aCenter,
        TReal aRadiusInMeters );

    /**
     * Allocates and constructs a new geographical circular area object
     * with specified center coordinate and radius. The constructed
     * object is pushed onto cleanup stack.
     *
     * @panic ELbtErrArgument If the input radius is negative.
     *
     * @param[in] aCenter The coordinate of the center.
     * @param[in] aRadiusInMeters The radius of the circle in meters.
     * @return Pointer to the new geographical circle area object.
     */
    IMPORT_C static CLbtGeoCircle* NewLC( 
        const TCoordinate& aCenter,
        TReal aRadiusInMeters );

    /**
     * Destructor.
     */
    IMPORT_C ~CLbtGeoCircle();

    /**
     * Returns the type of geographical area, CLbtGeoAreaBase::ECircle
     *
     * @return CLbtGeoAreaBase::ECircle.
     */
    virtual TGeoAreaType Type() const;

    /**
     * Gets the center of the area. If the center has not been set
     * before, the returned coordinate has latitude, longitude and
     * altitude set to NaN, 
     *
     * @return The center of the area.
     */
    virtual TCoordinate Center() const;

    /**
     * Sets the center of the circle.
     *
     * @param[in] aCenter The coordinate of the center.
     */
    IMPORT_C void SetCenter(const TCoordinate& aCenter);

    /**
     * Gets the radius of the circle. If the radius has not been
     * set before, the returned value is NaN.
     *
     * @return The radius of the circle in meters.
     */
    IMPORT_C TReal Radius() const;

    /**
     * Sets the radius of the circle.
     *
     * @panic ELbtErrArgument If the input radius is negative.
     *
     * @param[in] aRadiusInMeters The radius of the circle in meters.
     */
    IMPORT_C void SetRadius(TReal aRadiusInMeters);
    
    /**
     * Gets geographical area information. The following method is currently not 
     * supported. This method will only return empty array when invoked.
     *
     * @return An array of geographical area instances
     */
    IMPORT_C RPointerArray< CLbtGeoAreaBase >& GetAdditionalGeoAreaInfo();
    
    /**
     * Sets additional geographical area information. The geographical 
     * information provided can only be of CLbtGeoCell type. This is optional
     * and clients may specify this information when available.The following method 
     * is currently not supported.
     *
     * @panic ELbtErrArgument if argument passed is NULL or if the argument
     * passed is of any other type other than CLbtGeoCell.
     *
     * @param[in] aGeoArea A pointer representing a geographical area.
     */
    IMPORT_C void SetAdditionalGeoAreaInfo(CLbtGeoAreaBase* aGeoArea);
    
    /**
     * Validates cell information.
     *
     * @leave KErrArgument if any of the mandatory parameters have not
     * been specified.
     */
    void ValidateCircleInformationL();  
    
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
     * Default constructor
     */
    CLbtGeoCircle();

    /**
     * By default, prohibit copy constructor
     */
    CLbtGeoCircle(const CLbtGeoCircle&);
   
    /**
     * Prohibit assigment operator
     */
    CLbtGeoCircle& operator= (const CLbtGeoCircle&);
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL(TCoordinate aCenter,TReal aRadius);
    
        
private:
    /**
     * The center of the circle.
     */
    TCoordinate iCenter;

    /**
     * Radius of the circle in meters.
     */
    TReal iRadius;
    
    /**
     * An array of instances representing additional geographical area 
     * information
     */
    RPointerArray< CLbtGeoAreaBase > iAdditionalGeoAreaInfo;
    };

#endif // LBTGEOCIRCLE_H
