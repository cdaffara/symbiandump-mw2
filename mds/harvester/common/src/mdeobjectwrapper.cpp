/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/

#include "mdeobjectwrapper.h"
#include "harvesterlog.h"
#include "mdeobjectdef.h"
#include "mdepropertydef.h"
#include "mdeobject.h"
#include "mdetextproperty.h"

CMdeObjectWrapper::CMdeObjectWrapper()
    {
	// No implementation required
    }


CMdeObjectWrapper::~CMdeObjectWrapper()
    {
    }

EXPORT_C CMdeObjectWrapper* CMdeObjectWrapper::NewLC()
    {
	CMdeObjectWrapper* self = new (ELeave)CMdeObjectWrapper();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
    }

EXPORT_C CMdeObjectWrapper* CMdeObjectWrapper::NewL()
    {
	CMdeObjectWrapper* self=CMdeObjectWrapper::NewLC();
	CleanupStack::Pop( self ); // self;
	return self;
    }

void CMdeObjectWrapper::ConstructL()
    {

    }

EXPORT_C void CMdeObjectWrapper::HandleObjectPropertyL(CMdEObject& aMdeObject, const TDesC& aPropertyType, TAny* aData, TBool aIsAdd )
	{
	CMdEObjectDef& mdoDef = aMdeObject.Def();
    CMdEPropertyDef& propertyDef = mdoDef.GetPropertyDefL( aPropertyType );
    
    HandleObjectPropertyL( aMdeObject, propertyDef, aData, aIsAdd );
	}

EXPORT_C void CMdeObjectWrapper::HandleObjectPropertyL(CMdEObject& aMdeObject, CMdEPropertyDef& aPropertyDef, TAny* aData, TBool aIsAdd )
	{
	if( ! aData )
		{
#ifdef _DEBUG
		WRITELOG1("CMdeObjectWrapper::HandleObjectPropertyL - aData is null for property: %S", &aPropertyDef.Name() );
#endif
		return;
		}
	
    TPropertyType pt = aPropertyDef.PropertyType();
    
    // check if edit and readonly property then do nothing
    if( ! aIsAdd )
    	{
    	if ( aPropertyDef.ReadOnly() != 0 )
    		{
#ifdef _DEBUG
    		WRITELOG1("CMdeObjectWrapper::HandleObjectPropertyL - property: %S is readonly", &aPropertyDef.Name() );
#endif
    		return;
    		}
 
	    CMdEProperty* property = NULL;
	    const TInt error = aMdeObject.Property( aPropertyDef, property );

	    if ( error >= KErrNone && property )
	        {
	        switch ( pt )
		        {
				case EPropertyBool:
					{
					TBool* value = static_cast<TBool*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetBoolValueL( *value );
					}
				break;
				
				case EPropertyInt8:
					{
					TInt8* value = static_cast<TInt8*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetInt8ValueL( *value );
					}
				break;
				
				case EPropertyUint8:
					{
					TUint8* value = static_cast<TUint8*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetUint8ValueL( *value );
					}
				break;
				
				case EPropertyInt16:
					{
					TInt16* value = static_cast<TInt16*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetInt16ValueL( *value );
					}
				break;
				
				case EPropertyUint16:
					{
					TUint16* value = static_cast<TUint16*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetUint16ValueL( *value );
					}
				break;
				
				case EPropertyInt32:
					{
					TInt32* value = static_cast<TInt32*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetInt32ValueL( *value );
					}
				break;
				
				case EPropertyUint32:
					{
					TUint32* value = static_cast<TUint32*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetUint32ValueL( *value );
					}
				break;
				
				case EPropertyInt64:
					{
					TInt64* value = static_cast<TInt64*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetInt64ValueL( *value );
					}
				break;
				
				case EPropertyReal32:
					{
					TReal32* value = static_cast<TReal32*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %f", &aPropertyDef.Name(), *value );
					property->SetReal32ValueL( *value );
					}
				break;
				
				case EPropertyReal64:
					{
					TReal64* value = static_cast<TReal64*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %d", &aPropertyDef.Name(), *value );
					property->SetReal64ValueL( *value );
					}
				break;
				
				case EPropertyTime:
					{
					TTime* value = static_cast<TTime*>( aData );
					WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %u", &aPropertyDef.Name(), value->Int64() );
					property->SetTimeValueL( *value );
					}
				break;
				
				case EPropertyText:
					{
					const TDesC16* value = static_cast<const TDesC16*>( aData );
					if( value->Length() > 0 )
						{
						WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Edit property: %S with value %S", &aPropertyDef.Name(), value );
						property->SetTextValueL( *value );
						}
					else
						{
#ifdef _DEBUG
						WRITELOG1("CMdeObjectWrapper::HandleObjectPropertyL - property: %S was empty and not edited", &aPropertyDef.Name() );
#endif
						}
					}
				break;
				
				default:
				break;
				}
	        
	        return;
	        }
    	}
   
    switch ( pt )
        {
		case EPropertyBool:
			{
			TBool* value = static_cast<TBool*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddBoolPropertyL( aPropertyDef, *value );
			}
		break;

		case EPropertyInt8:
			{
			TInt8* value = static_cast<TInt8*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddInt8PropertyL( aPropertyDef, *value );
			}
		break;

		case EPropertyUint8:
			{
			TUint8* value = static_cast<TUint8*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddUint8PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyInt16:
			{
			TInt16* value = static_cast<TInt16*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddInt16PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyUint16:
			{
			TUint16* value = static_cast<TUint16*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddUint16PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyInt32:
			{
			TInt32* value = static_cast<TInt32*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddInt32PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyUint32:
			{
			TUint32* value = static_cast<TUint32*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddUint32PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyInt64:
			{
			TInt64* value = static_cast<TInt64*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddInt64PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyReal32:
			{
			TReal32* value = static_cast<TReal32*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddReal32PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyReal64:
			{
			TReal64* value = static_cast<TReal64*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), *value );
			aMdeObject.AddReal64PropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyTime:
			{
			TTime* value = static_cast<TTime*>( aData );
			WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %d", &aPropertyDef.Name(), value->Int64() );
			aMdeObject.AddTimePropertyL( aPropertyDef, *value );
			}
		break;
		
		case EPropertyText:
			{
			const TDesC16* value = static_cast<const TDesC16*>( aData );
			if( value->Length() > 0 )
				{
				WRITELOG2("CMdeObjectWrapper::HandleObjectPropertyL - Add property: %S with value %S", &aPropertyDef.Name(), value );
				aMdeObject.AddTextPropertyL( aPropertyDef, *value );
				}
			else
				{
#ifdef _DEBUG
				WRITELOG1("CMdeObjectWrapper::HandleObjectPropertyL - property: %S was empty and not added", &aPropertyDef.Name() );
#endif
				}
			}
		break;
		
		default:
		break;
		}
	}
