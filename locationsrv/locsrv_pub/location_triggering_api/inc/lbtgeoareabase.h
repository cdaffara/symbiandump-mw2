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
* Description:  Geographical base class definition.
*
*/


#ifndef LBTGEOAREABASE_H
#define LBTGEOAREABASE_H

#include <e32base.h>
#include <s32strm.h>

/**
 * Base class for geographical area.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtGeoAreaBase : public CBase
    {
public:
    /** 
     * Type of geographical area
     */
    enum TGeoAreaType
        {
        ECircle = 1,    ///<Circular area
        ERectangular = 2,     ///<Rectangular area
        ECellular = 3,        
        EHybrid = 4
        };
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtGeoAreaBase();

    /**
     * Gets the type of the geographical area
     * 
     * @return The type of geographical area.
     */
    virtual TGeoAreaType Type() const = 0;
    
          
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
     * Default constructor
     */  
    CLbtGeoAreaBase();
  
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
    CLbtGeoAreaBase( const CLbtGeoAreaBase& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtGeoAreaBase& operator= ( const CLbtGeoAreaBase& );

    };


#endif //LBTGEOAREABASE_H
