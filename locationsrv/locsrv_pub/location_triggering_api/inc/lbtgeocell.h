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
* Description:  Geographical cellular area class declaration.
*
*/




#ifndef C_LBTGEOCELL_H
#define C_LBTGEOCELL_H


#include <etelmm.h>
#include "lbtgeoareabase.h"


/**
 * A typedef'd buffer to hold the network country code. 
 */ 
typedef TBuf<4> TNetworkCountryCode;

/**
 * A typedef'd buffer to hold the network identity.
 */
typedef TBuf<8> TNetworkIdentity;


/**
 * Geographical cellular area.
 *  
 * Geographical cellular area is defined by the area covered by a network
 * cell. Client applications must specify complete CGI information of the
 * serving cell while defining a geographical cellular area. This includes 
 * the Mobile Country Code (MCC), Mobile Network Code (MNC), 
 * Location Area Code (LAI) and the Cell Identity (CI). In addition client 
 * applications must also mention the the Network Type.
 *  
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtGeoCell: public CLbtGeoAreaBase
    {
public:
    /**
     * Allocates and constructs a new geographical cellular area object.
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoCell* NewL();
    
    /**
     * Allocates and constructs a new geographical cellular area object and
     * pushes it onto cleanup stack. 
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoCell* NewLC();
    
    /**
     * Allocates and constructs a new geographical cellular area object.
     * 
     * @panic ELbtErrArgument If inputs are negative or zero.
     * 
     * @param[in] aNetworkType The network type
     * @param[in] aCountryCode The network country code
     * @param[in] aNetworkId The network identity code
     * @param[in] aLocationAreaCode The location area code
     * @param[in] aCellId The cell id 
     * @return Pointer to the new geographical cellular area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */
    IMPORT_C static CLbtGeoCell* NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                      TNetworkCountryCode aCountryCode,
                                      TNetworkIdentity aNetworkId,
                                      TUint aLocationAreaCode,
                                      TUint aCellId);
    
    /**
     * Allocates and constructs a new geographical cellular area object and
     * pushes it onto cleanup stack. 
     *
     * @panic ELbtErrArgument If inputs are negative or zero.
     * 
     * @param[in] aNetworkType The network type
     * @param[in] aCountryCode The network country code
     * @param[in] aNetworkId The network identity code
     * @param[in] aLocationAreaCode The location area code
     * @param[in] aCellId The cell id 
     * @return Pointer to the new geographical cellular area object.
     * @leave Other standard Symbian error code, such as KErrNoMemory
     */                                      
    IMPORT_C static CLbtGeoCell* NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                       TNetworkCountryCode aCountryCode,
                                       TNetworkIdentity aNetworkId,
                                       TUint aLocationAreaCode,
                                       TUint aCellId);
   
     
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtGeoCell();
    
    /**
     * Returns the type of geographical area, CLbtGeoAreaBase::ECellular
     *
     * @return CLbtGeoAreaBase::ECellular.
     */
    virtual TGeoAreaType Type() const;
    
    /**
     * Sets the network type.
     *
     * @param[in] aNetworkType The type of network
     */ 
    IMPORT_C void SetNetworkType( RMobilePhone::TMobilePhoneNetworkMode aNetworkType );
     
    /**
     * Returns the type of network
     *
     * @return The type of network. If the Network Type is not set earlier, 
     * this method would return RMobilePhone::ENetworkModeUnknown.
     */
    IMPORT_C RMobilePhone::TMobilePhoneNetworkMode NetworkType() const;
    
    /**
     * Sets the network country code.
     * @panic ELbtErrArgument If the input location area code  is negative or zero.
     *
     * @param[in] aCountryCode The network country code.
     */   
    IMPORT_C void SetNetworkCountryCode( TNetworkCountryCode aCountryCode);
    
    /**
     * Gets the network country code. If the country code has not been
     * set before, the returned value is zero.
     *
     * @return The network country code.
     */
    IMPORT_C TNetworkCountryCode NetworkCountryCode() const;
    
    /**
     * Sets the network identity code.
     * @panic ELbtErrArgument If the input location area code  is negative or zero.
     *
     * @param[in] aNetworkId The network identity code.
     */ 
    IMPORT_C void SetNetworkIdentityCode( TNetworkIdentity aNetworkId );
    
    /**
     * Gets the network identity code. If the network identity code has not been
     * set before, the returned value is zero.
     *
     * @return The network identity code.
     */
    IMPORT_C TNetworkIdentity NetworkIdentityCode() const;
    
    /**
     * Sets the location area code.
     *
     * @param[in] aLocationAreaCode The location area code.
     */
    IMPORT_C void SetLocationAreaCode(TUint aLocationAreaCode);  
    
    /**
     * Gets the location area code. If the location area code has not been
     * set before, the returned value is zero.
     *
     * @return The location area code.
     */    
    IMPORT_C TUint LocationAreaCode() const;    
    
    /**
     * Sets the cell id.
     *
     * @param[in] aCellId The cell id.
     */
    IMPORT_C void SetCellId( TUint aCellId ); 
    
    /**
     * Gets the cell id. If the cell code has not been
     * set before, the returned value is zero.
     *
     * @return The cell id.
     */  
    IMPORT_C TUint CellId() const; 
    
    /**
     * This method is for furture use and is currently not supported.
     */
    virtual TBool CGISpecifiedDuringCreation() const;
    
    /**
     * Validates create cell information.
     *
     * @leave KErrArgument if any of the mandatory parameters have not
     * been specified.
     */
    void ValidateCreateCellInformationL();  
    
    /**
     * Validates cell information.
     *
     * @leave KErrArgument if any of the mandatory parameters have not
     * been specified.
     */
    void ValidateCellInformationL(); 
    
protected:
  
    /**
     * default constructor
     */
    CLbtGeoCell();
    
    /**
     * By default, prohibit copy constructor
     */ 
    CLbtGeoCell( const CLbtGeoCell& );
   
    /**
     * Prohibit assigment operator
     */ 
    CLbtGeoCell& operator= ( const CLbtGeoCell& );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                    TNetworkCountryCode aCountryCode,
                    TNetworkIdentity aNetworkId,
                    TUint aLocationAreaCode,
                    TUint aCellId);
    
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

           
private: // data
    /**
     * Network type
     */
    RMobilePhone::TMobilePhoneNetworkMode iNetworkType;
    /**
     * Network country code
     */
    TUint iCountryCode;
    /**
     * Network identity code
     */
    TUint iNetworkId;
    /**
     * Location area code
     */
    TUint	iLocationAreaCode;
    /**
     * Cell id
     */ 
    TUint	iCellId; 
    
   };



#endif // C_LBTGEOCELLBASE_H
