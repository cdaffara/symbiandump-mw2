/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the implementation of the CAudioOutputControlUtilityProxy class.
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include "AudioOutputControlUtilityProxyImpl.h"
#include "DRMConfigTypes.h"
#include "DRMConfigIntfc.h"
#include <RestrictedAudioOutput.h>
#include <platform/mw/drmagents.h>
#include <mmf/server/mmffile.h>
#include <MultimediaDataSource.h>

using namespace ContentAccess;
using namespace DRM;
#ifdef _DEBUG
#define DEBPRN0(str)                RDebug::Print(str, this)
#define DEBPRN1(str)    RDebug::Printf( "%s %s", __PRETTY_FUNCTION__, str );
#else
#define DEBPRN0
#define DEBPRN1(str)
#endif


// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::CAudioOutputControlUtilityProxyImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAudioOutputControlUtilityProxyImpl::CAudioOutputControlUtilityProxyImpl()
    {
	DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl[0x%x]::CAudioOutputControlUtilityProxyImpl\n"));
	iRestrictedAudioOutput = NULL;
	iDRMConfigIntfc = NULL;
	iAudioOutput = NULL;
	iFactory = NULL;
    iMMDataSource = NULL;
    iDataSource = NULL;
    }


// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// assumes that iParent has already been set up properly
// -----------------------------------------------------------------------------
//
void CAudioOutputControlUtilityProxyImpl::ConstructL()
    {
    }


// Two-phased constructor.
CAudioOutputControlUtilityProxyImpl* CAudioOutputControlUtilityProxyImpl::NewL()
	{	    
	CAudioOutputControlUtilityProxyImpl* self = new(ELeave) CAudioOutputControlUtilityProxyImpl();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;  

    }
    
// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::~CAudioOutputControlUtilityProxyImpl
// Destructor
// -----------------------------------------------------------------------------
//
CAudioOutputControlUtilityProxyImpl::~CAudioOutputControlUtilityProxyImpl()
    {
	DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl[0x%x]::~CAudioOutputControlUtilityProxyImpl\n"));
	
	if (iRestrictedAudioOutput)
		delete iRestrictedAudioOutput;
	if (iDRMConfigIntfc)
		delete iDRMConfigIntfc;
	if (iAudioOutput)
		delete iAudioOutput;
	if (iFactory)
		delete iFactory;
	if (iDataSource)
	    iDataSource = NULL;		// dont own iDataSource
	if (iMMDataSource)
	    iMMDataSource = NULL;	// dont own iMMDataSource
    } 
       
// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::Configure
// configures the adapatation to use allowed outputs through CRestrictedAudioOutput 
// or CAudioOutput in case the former doesn't work
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::Configure(CMMFDevSound& aDevSound)
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CAudioOutputControlUtilityProxyImpl::Configure\n")); 
#endif      
	TInt err = KErrNotSupported;
  
    if (iFactory)
        {
        delete iFactory;
        iFactory = NULL;
        }
    if (iRestrictedAudioOutput)
        {
        delete iRestrictedAudioOutput;
        iRestrictedAudioOutput = NULL;
        }
              
    TRAP(err,CConfigurationComponentsFactory::CreateFactoryL(iFactory));
	if (err == KErrNone) 
    	{
    	//iFactory constructed
    	CleanupStack::PushL(iFactory);
    	
	    err = iFactory->CreateRestrictedAudioOutput(aDevSound, iRestrictedAudioOutput);
	    if(err == KErrNone)
	        {
	        //iRestrictedAudioOutput constructed
            CleanupStack::PushL(iRestrictedAudioOutput);
            DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::Configure - after creating iRestrictedAudioOutput"));
	        }
	    else
	        {
            //cleanup iFactory
            CleanupStack::PopAndDestroy(iFactory);
            iFactory = NULL;
	        }
	    }

    // If error, resort to using audio output and CAudioOutput  
    if (err != KErrNone)
        {
        if (iAudioOutput)
            {
            delete iAudioOutput;
            iAudioOutput = NULL;
            }
        TRAP(err, iAudioOutput = CAudioOutput::NewL(aDevSound));
        if (err == KErrNone)
            {
            //iAudioOutput constructed
            CleanupStack::PushL(iAudioOutput);
            
            DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::Configure - iAudioOutput created")); 
            TRAP(err, iAudioOutput->SetSecureOutputL(ETrue));
            if(err == KErrNone)
                CleanupStack::Pop(iAudioOutput);
            else
                CleanupStack::PopAndDestroy(iAudioOutput);
                iAudioOutput = NULL;
            }
        return err;
        }

    //RestrictedAudioOutput created successfully
    //EMC sources
    if (iMMDataSource)
        {
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::Configure - EMC sources"));
        //Get DRM config interface
        if(iDRMConfigIntfc)
			{
          	delete  iDRMConfigIntfc;
          	iDRMConfigIntfc = NULL;
         	}
        TVersion version(KDRMConfigIntfcMajorVer1,KDRMConfigIntfcMinorVer1,KDRMConfigIntfcBuildVer1);
        err = iMMDataSource->GetInterface(KDRMConfigIntfc, version, (TAny*&)iDRMConfigIntfc);
        if (err == KErrNone)
            {
            //iDRMConfigIntfc constructed
            CleanupStack::PushL(iDRMConfigIntfc);
                    
            //Append allowed outputs in DRMConfigIntfc:
            TInt count(0);
            CRestrictedAudioOutput::TAllowedOutputPreference pref;  
            iDRMConfigIntfc->GetAllowedOutputDeviceCount(count);
            TDRMAllowedOutputDevice allowedDevice;
            for ( TInt index(0); index < count; index++ )
                {
                iDRMConfigIntfc->GetAllowedOutputDevice(index, allowedDevice);
                err = CalcPref(allowedDevice, pref); 
                if ( err == KErrNone )
                    {
                    err = iRestrictedAudioOutput->AppendAllowedOutput(pref);
                    if (err != KErrNone)
                        break;
                    }
                else
                    {
                    break;
                    }
                }//end for
            }
        }
     
    //MMF sources
    if (iDataSource)
        {
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::Configure - MMF sources"));
        ContentAccess::CData* data(NULL);
        static_cast<CMMFFile*>(iDataSource)->Data(data);
        
        //Obtain the bitmask for allowed outputs
        TInt value(0);
        err = data->GetAttribute(EDrmAllowedOutputs,value);
        
        //Append allowed outputs in value bitmask
        if(err == KErrNone)
            {
            err = SetCalculatedPreference(value);
            }
        }

    //Commit the outputs appended to RestrictedAudioOutput
    if(err == KErrNone)
        {
        err = iRestrictedAudioOutput->Commit();
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::Configure - iRestrictedAudioOutput->Commit"));
        }
    
    if(err == KErrNone)
        {
        if(iDRMConfigIntfc) 
            {
            //in case of EMC sources
            CleanupStack::Pop(iDRMConfigIntfc);
            }
        CleanupStack::Pop(iRestrictedAudioOutput);
        CleanupStack::Pop(iFactory);
        }
    else
        {
        if(iDRMConfigIntfc) 
            {
            //in case of EMC sources
            CleanupStack::PopAndDestroy(iDRMConfigIntfc);
            iDRMConfigIntfc = NULL;
            }
        CleanupStack::PopAndDestroy(iRestrictedAudioOutput);
        CleanupStack::PopAndDestroy(iFactory);
        iFactory = NULL;
        iRestrictedAudioOutput = NULL;
        }
        
    return err;
    }    
         
        
// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::CalcPref
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*
CRestrictedAudioOutput::TAllowedOutputPreference CAudioOutputControlUtilityProxyImpl::CalcPref(TDRMAllowedOutputDevice aOutputDevice)
    {
#ifdef _DEBUG    
 	RDebug::Print(_L("CalcPref: %d\n"), aOutputDevice); 
#endif
 	   
 	CRestrictedAudioOutput::TAllowedOutputPreference pref = CRestrictedAudioOutput::EAllowAllAudio; //default
 	switch (aOutputDevice)
 	{
    	case EAudioAllowAll:
    	pref = CRestrictedAudioOutput::EAllowAllAudio;
    	break;
    	
    	case EAudioAllowAnalog:
    	pref = CRestrictedAudioOutput::EAllowAudioAnalog;
    	break;
    	
       	case EAudioAllowFMTx:
    	pref = CRestrictedAudioOutput::EAllowAudioFMTransmitter;
    	break;
    	
       	case EAudioAllowBTA2DP:
    	pref = CRestrictedAudioOutput::EAllowAudioBtA2DP;
    	break;
    	
     	case EAudioAllowBTHFP:
    	pref = CRestrictedAudioOutput::EAllowAudioStereoBtHFP;
    	break;
    	
    	case EAudioAllowUplink:
    	pref = CRestrictedAudioOutput::EAllowAudioUplink;
    	break;
    	
     	case EAudioAllowUSB:
    	pref = CRestrictedAudioOutput::EAllowAudioUsb;
    	break;
    	
    	case EAudioAllowRecording:
    	pref = CRestrictedAudioOutput::EAllowAudioRecording;
    	break;
    	
    	case EAudioAllowVisualization:
    	pref = CRestrictedAudioOutput::EAllowVisualization;
    	break; 
    
    	case EAudioAllowBTHSP:
    	//pref = CRestrictedAudioOutput::EAllowAudioBTHSP;
    	break;     	
    	default:
    	break;
 		
 	} // End switch

    	
    return pref;
    }  
*/
TInt CAudioOutputControlUtilityProxyImpl::CalcPref(TDRMAllowedOutputDevice aOutputDevice, CRestrictedAudioOutput::TAllowedOutputPreference& aAllowedOutput)
    {
#ifdef _DEBUG    
 	RDebug::Print(_L("CalcPref: %d\n"), aOutputDevice); 
#endif
    TInt status(KErrNone);
 	   
 	switch (aOutputDevice)
 	    {
    	case EAudioAllowAll:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAllAudio;
        	break;
    	
    	case EAudioAllowAnalog:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioAnalog;
        	break;
    	
       	case EAudioAllowFMTx:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioFMTransmitter;
        	break;
    	
       	case EAudioAllowBTA2DP:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioBtA2DP;
        	break;
    	
     	case EAudioAllowBTHFPHSP:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioBtHFPHSP;
        	break;
    	
    	case EAudioAllowUplink:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioUplink;
        	break;
    	
     	case EAudioAllowUSB:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioUsb;
        	break;
    	
    	case EAudioAllowRecording:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioRecording;
        	break;
    	
    	case EAudioAllowVisualization:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowVisualization;
        	break;
        /**
		* RIM CR 417-7642: HDMI with HDCP to Resctricted Audio Output API
		* Due to addition of new ENUMs to CRestrictedAudioOutput::TAllowedOutputPreference for HDMI and HDCP
		* EAllowAudioHDMI and EAllowAudioHdmiHdcpRequired,the same is matched by adding
		* EAudioAllowHDMI and EAudioAllowHdmiHdcpRequired
		*/
        case EAudioAllowHDMI:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioHDMI;
        	break;

        case EAudioAllowHdmiHdcpRequired:
        	aAllowedOutput = CRestrictedAudioOutput::EAllowAudioHdmiHdcpRequired;
        	break;

/*
    	case EAudioAllowBTHSP:
    	// For now CRestrictedAudioOutput maps EAudioAllowBTHSP and
    	// EAudioAllowBTHFP to EAllowAudioStereoBtHFP.
    	// SCB-CR VRAH-77THZ7. Once approved, uncomment the following line
    	// So until then, eat the enum and return KErrNone.
    	// WMDRM defines BT which is mapped by MW to A2DP, HFP, HSP
    	// MW maps A2DP, HFP, HSP to A2DB, HFP in CRestrictedAudioOutput
    	// So we don't loose any original restriction.
    	//aAllowedOutput = CRestrictedAudioOutput::EAllowAudioBTHSP;
    	break;     	
*/
    	default:
    	    status = KErrNotSupported;
    	    break;
 		
    	} // End switch
    return status;
    }  

// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::CalcDrmAllowedOutputDevice
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*****
TDRMAllowedOutputDevice CAudioOutputControlUtilityProxyImpl::CalcDrmAllowedOutputDevice(CRestrictedAudioOutput::TAllowedOutputPreference aPref)
    {
#ifdef _DEBUG    
 	RDebug::Print(_L("CalcDrmAllowedOutputDevice for: %d\n"), aPref); 
#endif
 	   
 	TDRMAllowedOutputDevice device = EAudioAllowAll; //default
 	
 	switch (aPref)
 	{
    	case CRestrictedAudioOutput::EAllowAllAudio:
    	device = EAudioAllowAll;
    	break;
    	
    	case CRestrictedAudioOutput::EAllowAudioAnalog:
    	device = EAudioAllowAnalog;
    	break;
    	
       	case CRestrictedAudioOutput::EAllowAudioFMTransmitter:
       	device = EAudioAllowFMTx;
    	break;
    	
       	case CRestrictedAudioOutput::EAllowAudioBTA2DP:
       	device = EAudioAllowBTA2DP;
    	break;
    	
     	case CRestrictedAudioOutput::EAllowAudioBtHFPHSP:
     	device = EAudioAllowBTHFPHSP;
    	break;
    	
    	case CRestrictedAudioOutput::EAllowAudioUplink:
    	device = EAudioAllowUplink;
    	break;
    	
     	case CRestrictedAudioOutput::EAllowAudioUsb:
     	device = EAudioAllowUSB;
    	break;
    	
    	case CRestrictedAudioOutput::EAllowAudioRecording:
    	device = EAudioAllowRecording;
	   	break;
    	
    	case CRestrictedAudioOutput::EAllowVisualization:
    	device = EAudioAllowVisualization;
       	break;   
       	
 		case CRestrictedAudioOutput::EAllowAudioBTHSP:
 		device = EAudioAllowBTHSP;
 		break;
    	
    	default:
    	break;
 		
 	} // End switch

    	
    return device;
    }    
*****/
    
// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::SetDRMConfigIntfc
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::SetDRMConfigIntfc(CDRMConfigIntfc* aDRMConfigIntfc)
    {
   	RDebug::Print(_L("CAudioOutputControlUtilityProxyImpl::SetDRMConfigIntfc"));    
    
    if(iDRMConfigIntfc)
		{
		delete 	iDRMConfigIntfc;
		iDRMConfigIntfc = NULL;
		}    
    
    iDRMConfigIntfc = aDRMConfigIntfc;
    return KErrNone;
    }   
    
// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::ConfigureRestrictedAudioOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::ConfigureRestrictedAudioOutputL(CMMFDevSound& aDevSound) 
	{
#ifdef _DEBUG 
  	RDebug::Print(_L("CAudioOutputControlUtilityProxyImpl::ConfigureRestrictedAudioOutput")); 
#endif
  	
	TInt err = CConfigurationComponentsFactory::CreateFactoryL(iFactory);
	err = iFactory->CreateRestrictedAudioOutput(aDevSound, iRestrictedAudioOutput); 
	
	// If error, resort to using audio output and SetSecureOutput:  
 	if (err != KErrNone)
 	{
		TRAP(err, iAudioOutput = CAudioOutput::NewL(aDevSound));
 		if (err == KErrNone) 
 		{
			TRAP(err, iAudioOutput->SetSecureOutputL(ETrue)); 
 		}
 		else 
 			return err;
 	}
 	else
 	{
 			
		// Append allowed outputs in DRMConfigIntfc:
  		TInt count(0);
  		CRestrictedAudioOutput::TAllowedOutputPreference pref;	
   		iDRMConfigIntfc->GetAllowedOutputDeviceCount(count);
  		TDRMAllowedOutputDevice allowedDevice;
  		for ( TInt index(0); index < count; index++ )
   		{
   			iDRMConfigIntfc->GetAllowedOutputDevice(index, allowedDevice);
#ifdef _DEBUG    			
RDebug::Print(_L("ConfigureRestrictedAudioOutput:DRMAllowedOutput[%d]=[%d]"), index, allowedDevice);
#endif   			
   			//pref = CalcPref(allowedDevice);         
   			if ( CalcPref(allowedDevice, pref) == KErrNone )
   			{
     			err = iRestrictedAudioOutput->AppendAllowedOutput(pref);
     			if (err != KErrNone)
     				return err;
 			}
 			else
 			{
#ifdef _DEBUG  
RDebug::Print(_L("ConfigureRestrictedAudioOutput:DRMAllowedOutput err = %D"), err);
#endif 
 			    return err;
    	    }
    	}
    	
    // Commit
  	err = iRestrictedAudioOutput->Commit();    	
 	}
    
    return err;
              	  	
	}

// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::SetDataSource
// sets a MMF source to the utility
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::SetDataSource(MDataSource* aDataSource)
    {
    DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetDataSource for MMF"));    
    
    if(iDataSource)
        {
        iDataSource = NULL;
        }    
    
    iDataSource = aDataSource;
    return KErrNone;
    }   

// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::SetDataSource
// sets an EMC source to the utility
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::SetDataSource(CMultimediaDataSource* aDataSource)
    {
    DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetDataSource for EMC"));    
    
    if(iMMDataSource)
        {
        iMMDataSource = NULL;
        }    
    
    iMMDataSource = aDataSource;
    return KErrNone;
    }  

// ---------------------------------------------------------
// CAudioOutputControlUtilityProxyImpl::SetCalculatedPreference
// obtains the allowed output from the passed bitmask and appends to RestrictedAudioOutput
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAudioOutputControlUtilityProxyImpl::SetCalculatedPreference(const TInt& aValue)
    {
    DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref")); 
    TInt err = KErrNone;
    if(aValue == EDrmAllowAll)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAllAudio);
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAll"));
        return err;
        }
    if(aValue & EDrmAllowAudioAnalog)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioAnalog);
        if (err != KErrNone)
			{
			return err;
			}
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioAnalog"));
        }
    if(aValue & EDrmAllowAudioFmTransmitter)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioFMTransmitter);
        if (err != KErrNone)    
			{
			return err;
			}
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioFmTransmitter"));
        }
    if(aValue & EDrmAllowAudioBluetooth)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioBtA2DP);
        if (err != KErrNone)    
			{
			return err;
			}
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioBtHFPHSP);
        if (err != KErrNone)    
			{
			return err;
			}
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioBluetooth"));
        }
    if(aValue & EDrmAllowAudioUplink)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioUplink);
        if (err != KErrNone)    
			{
			return err;
			}
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioUplink"));
        }
    if(aValue & EDrmAllowAudioUsb)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioUsb);
        if (err != KErrNone)    
			{
			return err;
			}
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioUsb"));
        }
    if(aValue & EDrmAllowAudioHdmi)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioHDMI);
        if (err != KErrNone)    
            {
            return err;
            }
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioHdmi"));
        }
    if(aValue & EDrmAllowAudioHdmiHdcpRequired)
        {
        err = iRestrictedAudioOutput->AppendAllowedOutput(CRestrictedAudioOutput::EAllowAudioHdmiHdcpRequired);
        if (err != KErrNone)    
            {
            return err;
            }
        DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref - EDrmAllowAudioHdmiHdcpRequired"));
        }
    DEBPRN0(_L("CAudioOutputControlUtilityProxyImpl::SetCalcPref"));
    return err;
    }
// End of file

