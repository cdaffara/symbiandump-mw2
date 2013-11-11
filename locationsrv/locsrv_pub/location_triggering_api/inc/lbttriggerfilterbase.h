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
* Description:  Abstract base class for filter when listing triggers.
*
*/


#ifndef LBTTRIGGERFILTERBASE_H
#define LBTTRIGGERFILTERBASE_H

#include <e32base.h>
#include <s32strm.h>

/**
 * Abstract base class for filter when listing triggers from 
 * Location Triggering Server.
 *
 * @lib lbt.lib
 * @since S60 5.1
 */
class CLbtTriggerFilterBase : public CBase
    {
public:
    /**
     * Specifies the type of the filter
     */
    enum TFilterType
        {
        /**
         * Filter based on the trigger entry's attribute value.
         */
        EFilterByAttribute = 1,
        /**
         * Filter based on the geographical area.
         */
        EFilterByArea = 2,
        /**
         * Composite filter.
         */
        EFilterComposite = 3
        };

    /**
     * Returns the type of filter.
     *
     * @return The type of the filter.
     */
    virtual TFilterType Type() const = 0;

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtTriggerFilterBase();
    
    
    /**
     * Internalizes the trigger condition object's details and attributes 
     * from stream.
     *
     * The presence of this function means that the standard templated 
     * operator>>() ( defined in s32strm.h ) is available to internalize objects 
     * of this class.
     *
     * @param[in] aStream Stream from which the object should be internalized.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
    /**
     * Externalizes the trigger condition object's details and attributes
     * to stream.
     *
     * The presence of this function means that the standard templated 
     * operator<<() ( defined in s32strm.h ) is available to externalize objects 
     * of this class.
     *
     * @param[in] aStream Stream to which the object should be externalized.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;
    

protected:    
    /**
     * Constructor.
     */
    CLbtTriggerFilterBase();
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL( RReadStream& aStream ) = 0;
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL( RWriteStream& aStream ) const = 0;
    
private:
    /**
     * By default, prohibit copy constructor
     */
    CLbtTriggerFilterBase( const CLbtTriggerFilterBase& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerFilterBase& operator= ( const CLbtTriggerFilterBase& );

    };

#endif // LBTTRIGGERFILTERBASE_H
