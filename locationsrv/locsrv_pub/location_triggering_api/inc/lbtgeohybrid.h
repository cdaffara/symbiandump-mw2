/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Geographical hybrid area declaration.
*
*/

#ifndef LBTGEOHYBRID_H
#define LBTGEOHYBRID_H

#include <lbtgeoareabase.h>


/**
 * Geographical hybrid area class.
 *  
 * Geographical hybrid area is defined by collection of similar or different 
 * geographical area objects.
 * These triggers are fired when any of the geographical area info matches the 
 * information obtained by LBT server.
 *  
 * @lib lbt.lib
 *
 * @since S60 5.2
 */
class CLbtGeoHybrid : public CLbtGeoAreaBase
    {
public:
    /**
     * Allocates and constructs a new geographical hybrid area object.
     *
     * @return Pointer to the new geographical hybrid area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoHybrid* NewL();
    
    /**
     * Allocates and constructs a new geographical hybrid area  object and
     * pushes it onto cleanup stack. 
     *
     * @return Pointer to the new geographical hybrid area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoHybrid* NewLC();
    
    /**
     * Allocates and constructs a new geographical hybrid area object.
     * 
     * @panic ELbtErrArgument If array count is less than two or if one  
     * the input trigger area is other than cellular area.
     * 
     * @param[in] aHybridArea An array of pointers to geographical 
     * area.
     * @return Pointer to the new geographical hybrid area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoHybrid* NewL( RPointerArray<CLbtGeoAreaBase>& 
                                         aHybridArea );
    
    /**
     * Allocates and constructs a new geographical hybrid area object and
     * pushes it onto cleanup stack. 
     *
     * @panic ELbtErrArgument If array count is less than two or if one  
     * the input trigger area is other than cellular area.
     * 
     * @param[in] aHybridArea An array of pointers to geographical 
     * area.
     * @return Pointer to the new geographical hybrid area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */                                      
    IMPORT_C static CLbtGeoHybrid* NewLC( RPointerArray<CLbtGeoAreaBase>& 
                                          aHybridArea );
   
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtGeoHybrid();
    
    /**
     * Returns the type of geographical area, CLbtGeoAreaBase::ECellArray
     *
     * @return CLbtGeoAreaBase::EHybrid.
     */
    virtual TGeoAreaType Type() const;
    
    /**
     * Sets array of pointers to geographical area.
     * 
     * @panic ELbtErrArgument If array count is less than two or if one  
     * the input trigger area is other than cellular area.
     * 
     * @param[in] aHybridArea An array of pointers to geographical 
     * area.
     */
    IMPORT_C void SetHybridArea( const RPointerArray<CLbtGeoAreaBase>& 
                                 aHybridArea );
    
    /**
     * Returns An array of pointers to geographical.
     * 
     * @return An array of pointers to geographical 
     * area.
     */
    IMPORT_C const RPointerArray<CLbtGeoAreaBase>& HybridArea();
    
    /**
     * Validates hybrid area information
     *
     * @leave KErrArgument if If array count is less than two or
     * any of the mandatory parameters have not been specified.
     */
    void ValidateHybridInformationL();
    
private:
    /**
     * default constructor
     */
    CLbtGeoHybrid();
    
    /**
     * By default, prohibit copy constructor
     */ 
    CLbtGeoHybrid( const CLbtGeoHybrid& );
   
    /**
     * Prohibit assigment operator
     */ 
    CLbtGeoHybrid& operator= ( const CLbtGeoHybrid& );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL( RPointerArray<CLbtGeoAreaBase>& 
                     aHybridArea );
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL(RWriteStream& aStream) const;
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL(RReadStream& aStream);


private://data
   /**
    * Array of cell global identifier
    */
    RPointerArray<CLbtGeoAreaBase>  iHybridArea;
    };

#endif /*LBTGEOHYBRID_H*/
