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
* Description:  Representing filter based on the geographical area
*
*/


#ifndef LBTTRIGGERFILTERBYAREA_H
#define LBTTRIGGERFILTERBYAREA_H

#include <lbttriggerfilterbase.h>

class CLbtGeoAreaBase;

/**
 * Class representing filter based on the geographical area.
 *
 * This class defines the geographical area filter used in retrieving triggers 
 * from Location Triggering Server. When this filter is used, only triggers that 
 * are inside the defined area will be retrieved. The area can be a geographical
 * circle or rectangle. 
 * 
 * If the area is not set and the filter is used in listing triggers, no trigger 
 * will be returned.
 * 
 * If the center of the geographical circular area is not specified(NaN), 
 * latest acquired location of the Location Triggering Server is 
 * used as the center. If the radius of the geographical circular area is zero, 
 * no triggers will be retrieved.
 *
 * @lib lbt.lib
 * @since S60 5.1
 */
class CLbtTriggerFilterByArea : public CLbtTriggerFilterBase
    {
public:
    /**
     * Returns CLbtTriggerFilterBase::EFilterByArea.
     *
     * @return CLbtTriggerFilterBase::EFilterByArea.
     */
    IMPORT_C virtual TFilterType Type() const;

    /**
     * Constructs a new instance of CLbtTriggerFilterByArea. 
     *
     * @param[in] aArea Pointer to the geographical area object. 
     * Ownership of aArea is transferred to this object. 
     * If the center of the geographical circular area is omitted, 
     * latest acquired location is used when retrieving triggers.
     * @return New instance of CLbtTriggerFilterByArea.
     */
    IMPORT_C static CLbtTriggerFilterByArea* NewL( 
        CLbtGeoAreaBase* aArea );

    /**
     * Constructs a new instance of CLbtTriggerFilterByArea. 
     * The returned object has no geographical area set. 
     *
     * @return New instance of CLbtTriggerFilterByArea.
     */
    IMPORT_C static CLbtTriggerFilterByArea* NewL();

    /**
     * Constructs a new instance of CLbtTriggerFilterByArea
     * and pushes it onto cleanup stack.
     * The returned object has no geographical area set. 
     *
     * @return New instance of CLbtTriggerFilterByArea.
     */
    IMPORT_C static CLbtTriggerFilterByArea* NewLC();

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtTriggerFilterByArea();
    
    /**
     * Gets the geographical area, inside which the triggers are 
     * retrieved.
     *
     * If the area has not been set, this function returns NULL.
     *
     * @return The pointer to the geographical area object. Ownership is not
     * transferred to the client. 
     */
    IMPORT_C CLbtGeoAreaBase* Area();

    /**
     * Sets the geographical area in the filter. Only triggers that have 
     * triggering area inside the defined area are retrieved.
     *
     * @param[in] aArea The pointer to the geographical area object. 
     * Ownership of aArea is transferred to this object.
     * If the area is a geographical circular area and the center of the
     * area is not set, latest acquired location is used by Location 
     * Triggering Server when retrieving triggers. 
     */
    IMPORT_C void SetArea( CLbtGeoAreaBase* aArea );

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
     * Default constructor.
     */
    void ConstructL(CLbtGeoAreaBase* aArea);
    
    /**
     * By default, prohibit copy constructor
     */ 
    CLbtTriggerFilterByArea( const CLbtTriggerFilterByArea& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerFilterByArea& operator= ( const CLbtTriggerFilterByArea& );
    
    /**
     * Constructor.
     */
    CLbtTriggerFilterByArea();

private:
    /**
     * The geographical area that the trigger lies in
     */
    CLbtGeoAreaBase* iGeoArea;

    };


#endif // LBTTRIGGERFILTERBYAREA_H
