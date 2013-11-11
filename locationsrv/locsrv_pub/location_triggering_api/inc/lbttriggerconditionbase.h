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
* Description:  Abstract base class for trigger condition classes.
*
*/




#ifndef LBTTRIGGERCONDITIONBASE_H
#define LBTTRIGGERCONDITIONBASE_H

#include <e32base.h>
#include <s32strm.h>

/**
 * Abstract base class for trigger condition classes.
 * 
 * This class is not meant to be derived nor instantiated by client application.
 * It defines methods that concrete trigger condition class must implement.
 *
 * @lib lbt.lib
 * @since S60 5.1
 */
class CLbtTriggerConditionBase : public CBase
    {
public:
    /**
     * Enumeration for trigger condition types.
     */
    enum TType
        {
        /** 
         * Trigger condition based on geographical area and movement of 
         * the terminal. A trigger is fired when the 
         * terminal enters or leaves the specified trigger area.
         */
        ETriggerConditionArea = 1
        };
        
    /**
     * Destructor
     */
    virtual ~CLbtTriggerConditionBase();
    
    /**
     * Gets the type of trigger condition class.
     * @return The type of trigger condition class.
     */
    virtual TType Type() const = 0;
    
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
     * Constructor
     */
    CLbtTriggerConditionBase();
    
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
    CLbtTriggerConditionBase( const CLbtTriggerConditionBase& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerConditionBase& operator= ( const CLbtTriggerConditionBase& );

    };
    
 
#endif // LBTTRIGGERCONDITIONBASE_H
