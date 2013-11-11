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
* Description:  Data class for trigger information
*
*/


#ifndef LBTTRIGGERINFO_H
#define LBTTRIGGERINFO_H

#include <e32base.h>
#include <lbtcommon.h>
#include <s32strm.h>

class CLbtTriggerEntry;
struct TLbtTriggerDynamicInfo;


/**
 * Data class for trigger information. The trigger information contains trigger 
 * entry and trigger dynamic information.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtTriggerInfo : public CBase
    {
public:
    /**
     * Constructs a new instance of trigger information object.
     *
     * @return The new instance of trigger information object.
     */
    IMPORT_C static CLbtTriggerInfo* NewL();

    /**
     * Constructs a new instance of trigger information object
     * and pushes it onto cleanup stack.
     *
     * @return The new instance of trigger information object.
     */
    IMPORT_C static CLbtTriggerInfo* NewLC();

    /**
     * Constructs a new instance of trigger information object.
     *
     * @param[in] aEntry The pointer to the trigger entry. 
     * Ownership is transferred to this object.
     * @param[in] aDynInfo The pointer to the trigger dynamic 
     * information object. Ownership is transferred to this object.
     * @return The new instance of trigger information object.
     */
    IMPORT_C static CLbtTriggerInfo* NewL( 
        CLbtTriggerEntry* aEntry,
        TLbtTriggerDynamicInfo* aDynInfo );

    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtTriggerInfo();

    /**
     * Gets trigger entry object.
     *
     * @return Pointer to the trigger entry object. Ownership is
     * not transferred to the client.
     */
    IMPORT_C CLbtTriggerEntry* TriggerEntry();

    /**
     * Sets trigger entry.
     *
     * @param[in] aEntry The trigger entry object. Ownership is 
     * transferred to this object.
     */
    IMPORT_C void SetTriggerEntry( CLbtTriggerEntry* aEntry );

    /**
     * Gets trigger dynamic information object.
     *
     * @return Pointer to the trigger dynamic information object.
     * Ownership is not transferred to the client.
     */
    IMPORT_C TLbtTriggerDynamicInfo* DynInfo();

    /**
     * Sets trigger dynamic information object.
     *
     * @param[in] aDynInfo Pointer to the trigger dynamic 
     * information object. Ownership is transferred to this object.
     */
    IMPORT_C void SetDynInfo( TLbtTriggerDynamicInfo* aDynInfo );
    
    
	/**
     * Internalizes the trigger object's details and attributes 
     * from stream.
     *
     * The presence of this function means that the standard template 
     * operator>>() ( defined in s32strm.h ) is available to internalize objects 
     * of this class.
     *
     * @param[in] aStream Stream from which the object should be internalized.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
    /**
     * Externalizes the trigger object's details and attributes
     * to stream.
     *
     * The presence of this function means that the standard template 
     * operator<<() ( defined in s32strm.h ) is available to externalize objects 
     * of this class.
     *
     * @param[in] aStream Stream to which the object should be externalized.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

protected:
    /**
     * Default constructor.
     */
    IMPORT_C CLbtTriggerInfo();

private:    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL(CLbtTriggerEntry* aEntry,
        TLbtTriggerDynamicInfo* aDynInfo);
    
    /**
     * Symbian 2nd phase constructor.
     */    
    void ConstructL();    
	
	/**
     * By default, prohibit copy constructor
     */
    CLbtTriggerInfo( const CLbtTriggerInfo& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerInfo& operator= ( const CLbtTriggerInfo& );


private:// data
    /**
     * Trigger entry
     */
    CLbtTriggerEntry* iEntry;

    /**
     * Trigger dynamic info
     */
    TLbtTriggerDynamicInfo* iDynInfo;
    };

#endif // LBTTRIGGERINFO_H
