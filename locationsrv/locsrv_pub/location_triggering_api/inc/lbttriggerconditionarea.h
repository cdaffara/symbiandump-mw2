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
* Description:  Data class for the area trigger condition.
*
*/




#ifndef LBTTRIGGERCONDITIONAREA_H
#define LBTTRIGGERCONDITIONAREA_H

#include <e32base.h>
#include <lbttriggerconditionbase.h>

class CLbtGeoAreaBase;

/**
 *  Data class for the trigger condition based on the geographical area and movement
 *  of the terminal.
 *
 *  The trigger condition specifies where and when a location trigger shall be 
 *  fired.
 *
 *  Trigger area defines where the trigger shall be fired. It is a 
 *  geographical area. Currently, only geographical circular area
 *  is supported by the system. 
 *
 *  Direction of movement defines when the trigger shall be fired. It specifies
 *  if the trigger shall be fired wether the terminal enters or exits the trigger area.
 * 
 *  When a trigger has been fired, it will not be ready to be fired again until 
 *  the terminal has moved a certain distance ( "hysteresis distance" ) outside 
 *  of the trigger area. 
 *
 *  Hysteresis distance is defined by system and can't be accessed
 *  by client applications.
 *
 *  @lib lbt.lib
 *  @since S60 5.1
 */
class CLbtTriggerConditionArea : public CLbtTriggerConditionBase
    {
public:  // Data types
    /**
     *  Specifies in which direction the location trigger is fired.
     */
    enum TDirection
        {
        /** The trigger is fired when the terminal is entering the 
         * trigger area. 
         */
        EFireOnEnter = 1,
        /** The trigger is fired when the terminal is exiting the 
         * trigger area.
         */
        EFireOnExit  = 2 
        };

public:  
    /**
     * Constructs a new instance of trigger condition class based
     * on the geographical area and movement of the terminal.
     *
     * @panic ELbtErrArgument If the input trigger area is a 
     * class of CLbtGeoRect.
     *
     * @param[in] aArea The trigger area. Only pointer to 
     * CLbtGeoCircle object is supported. Ownership is transferred
     * to the returned CLbtTriggerConditionArea object.
     * @param[in] aDirection The direction( enter/exit ) of when a 
     * trigger shall be fired. 
     * @return A new instance of trigger condition class.
     */
    IMPORT_C static CLbtTriggerConditionArea* NewL( 
        CLbtGeoAreaBase* aArea,
        TDirection aDirection );

    /**
     * Constructs a new instance of trigger condition class.
     *
     * If construction is successful, it returns s a pointer to the 
     * trigger condition object. The returned object has no 
     * trigger area set, and the direction is @p EFireOnEnter.
     *
     * @return A new instance of this class.
     */
    IMPORT_C static CLbtTriggerConditionArea* NewL();

    /**
     * Constructs a new instance of trigger condition class
     * and pushes it onto cleanup stack.
     *
     * If construction is successful, it returns s a pointer to the 
     * trigger condition object. The returned object has no 
     * trigger area set, and the direction is @p EFireOnEnter.
     *
     * @return A new instance of this class.
     */
    IMPORT_C static CLbtTriggerConditionArea* NewLC();

    /**
     * Returns CLbtTriggerConditionBase::ETriggerConditionArea.
     *
     * @return CLbtTriggerConditionBase::ETriggerConditionArea
     */
    virtual TType Type() const;

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtTriggerConditionArea();

public: // Member functions

    /**
     * Gets the direction that defines when a trigger shall be fired.
     *
     * If no direction has been set for the trigger, it will return
     * @p EFireOnEnter.
     *
     * @return The direction that specified when a trigger shall be fired.
     */
    IMPORT_C TDirection Direction() const;

    /**
     * Sets the direction that defines when a trigger shall be fired.
     *
     * @param[in] aDirection The trigger direction. It defines when a trigger
     * shall be fired.
     */
    IMPORT_C void SetDirection( 
        TDirection aDirection );

    /**
     * Gets trigger area. 
     *
     * If the trigger area is not set before, NULL is returned.
     * Ownership of the returned trigger area is not transferred
     * to the client.
     *
     * @return Pointer to the trigger area. Ownership of returned
     * object is not transfered to the client.
     */
    IMPORT_C CLbtGeoAreaBase* TriggerArea() const;

    /**
     * Sets trigger area. 
     *
     * @panic ELbtErrArgument If the input trigger area is a 
     * class of CLbtGeoRect.
     * @param[in] aArea The trigger area. Only pointer to 
     * CLbtGeoCircle class is supported by current system. 
     * Ownership is transferred to this object.
     */
    IMPORT_C void SetTriggerArea( 
        CLbtGeoAreaBase* aArea );
        
    
protected:
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
	virtual void DoExternalizeL(RWriteStream& aStream)const;
	
	/**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
	virtual void DoInternalizeL(RReadStream& aStream);
	
	
private:
    /**
     * Default constructor.
     *
     * Trigger area is set as NULL and default direction 
     * is @p EFireOnEnter.
     */
    CLbtTriggerConditionArea();

    /**
     *  Symbian 2nd phase constructor.
     */
    void ConstructL();
    
    /**
     *  Symbian 2nd phase constructor.
     */
    void ConstructL(CLbtGeoAreaBase* aArea,
        TDirection aDirection);  
    
    /**
     * By default, prohibit copy constructor
     */
    CLbtTriggerConditionArea( const CLbtTriggerConditionArea& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerConditionArea& operator= ( const CLbtTriggerConditionArea& );


private:// Data
    /**
     * Area
     */
    CLbtGeoAreaBase* iArea;
    
    /**
     * Direction
     */
    TDirection iDirection;
    
    /**
     * Reserved pointer for future extension
     */
    TAny* iReserved;
    };
#endif // LBTTRIGGERCONDITIONAREA_H
