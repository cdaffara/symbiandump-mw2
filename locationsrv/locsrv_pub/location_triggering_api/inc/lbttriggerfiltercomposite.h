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
* Description:  Class representing a composite filter.
*
*/


#ifndef LBTTRIGGERFILTERCOMPOSITE_H
#define LBTTRIGGERFILTERCOMPOSITE_H

#include <lbttriggerfilterbase.h>

/**
 * Class representing a composite filter. This class is used to combine 
 * multiple filters. 
 * 
 * All filters combined in this class will be applied when listing 
 * triggers. Currently only ECompositionTypeAnd is 
 * supported to be the composition type. In this case, only 
 * those triggers that fulfill the criteria specified by all the filters
 * will be retrieved. 
 * 
 *
 * @lib lbt.lib 
 * @since S60 5.1
 */
class CLbtTriggerFilterComposite : public CLbtTriggerFilterBase
    {
public:
    enum TCompositeType 
        {
        /**
         * Criteria in all composited filters 
         * must be fulfiled.
         */
        ECompositionTypeAnd
        };

    /**
     * Returns CLbtTriggerFilterBase::EFilterComposite.
     *
     * @return CLbtTriggerFilterBase::EFilterComposite.
     */
    IMPORT_C virtual TFilterType Type() const;

    /**
     * Consructs a new instance of CLbtTriggerFilterComposite. 
     *
     * @return New instance of CLbtTriggerFilterComposite.
     */
    IMPORT_C static CLbtTriggerFilterComposite* NewL( 
        TCompositeType aType = ECompositionTypeAnd );

    /**
     * Consructs a new instance of CLbtTriggerFilterComposite
     * and pushes it onto cleanup stack. 
     *
     * @return New instance of CLbtTriggerFilterComposite.
     */
    IMPORT_C static CLbtTriggerFilterComposite* NewLC( 
        TCompositeType aType = ECompositionTypeAnd );

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtTriggerFilterComposite();

    /**
     * Adds a filter.
     *
     * @param[in] aFilter Pointer to the filter to be added. 
     * If the pointer is NULL, this function does nothing.
     * Ownership of aFilter is transferred to this object.
     */
    IMPORT_C void AddFilterL( 
        CLbtTriggerFilterBase* aFilter );

    /**
     * Gets the number of filters contained in this object.
     *
     * @return The number of filters contained in this object.
     */
    IMPORT_C TInt NumberOfFilters() const;

    /**
     * Gets pointer to the filter at the specific position.
     *
     * @panic USER 130 if aIndex is negative, or greater than
     * the total number of filters contained in this object.
     *
     * @param[in] aIndex The position of the filter. The value 
     * shall be in the range of 0 to NumberOfFilters()- 1.
     * @return The pointer to the filter. Ownership of the 
     * returned object is not transferred to the client.
     */
    IMPORT_C CLbtTriggerFilterBase* GetFilter( 
        TInt aIndex );

    /**
     * Remove the specific trigger filter from this object
     *
     * @panic USER 130 if aIndex is negative, or greater than
     * the total number of filters contained in this object.
     *
     * @param[in] aIndex The position of the filter to be removed. 
     */
    IMPORT_C void RemoveFilter( 
        TInt aIndex );

    /**
     * Remove all trigger filters contained in this object.
     */
    IMPORT_C void Reset();

protected:    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL( RReadStream& aStream ) ;
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL( RWriteStream& aStream ) const ;
    
    
private:
    /**
     * Constructor.
     */
    CLbtTriggerFilterComposite();

    /**
     * By default, prohibit copy constructor
     */
    CLbtTriggerFilterComposite( const CLbtTriggerFilterComposite& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerFilterComposite& operator= ( const CLbtTriggerFilterComposite& );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL(); 

private:
    /**
    * Trigger filters
    */
    RPointerArray < CLbtTriggerFilterBase > iFilters;
    };


#endif // LBTTRIGGERFILTERCOMPOSITE_H
