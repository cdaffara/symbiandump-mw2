/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for supl report message
*
*/

#ifndef EPOS_COMASUPLREPORT_H
#define EPOS_COMASUPLREPORT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplposmethod.h"
#include "lbs/epos_comasuplposition.h"
#include "epos_comasuplsetcapabilitiesver2.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplresponsever2.h"

#include "epos_tomasuplecellinfo.h"
#include "lbs/epos_comasuplposition.h"
//Forward declaration
class COMASuplMultipleLocIds;


// CLASS DECLARATION


struct TOMASuplTimeStamp
    {
//public:
    enum TOMASuplTimeStampType
        {
        ETOMASuplTimeStampUnknown = -1,
        ETOMASuplTimeStampAbsolute = 0,
        ETOMASuplTimeStampRelative
        };
    
//private:
    TOMASuplTimeStampType iType;
    TOMASuplUtcTime iAbsoluteTime;
    TInt32 iRelativeTime; 
    };
/**
 *  Class for providing position data
 *  @since S60 S60_5.2
 */
class COMASuplPositionData : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~COMASuplPositionData();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplPositionData* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplPositionData* NewLC();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    COMASuplPositionData();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

public:
    /**
     * Setter method for position
     * @since S60 S60_5.2
     * @param aPosition position to be set in the position data
     *          ownership is transferred to the position data object
     * @return none
     */
    IMPORT_C void SetPosition(COMASuplPosition* aPosition);
    /**
     * Setter method for position
     * @since S60 S60_5.2
     * @param aPosition position method to be set in the position data
     * @return none
     */
    IMPORT_C void SetPosMethod(const TOMASuplPosMethod aPosMethod);
    
public :
    /**
     * Getter method for position
     * @since S60 S60_5.2
     * @param aPosition a reference to the position is returned in this 
     *          ownership is still with the position data object
     * @return none
     */
    IMPORT_C void GetPosition(COMASuplPosition*& aPosition)const;
    /**
     * Getter method for position
     * @since S60 S60_5.2
     * @param aPosition position method to be set in the position data
     * @return none
     */
    IMPORT_C void GetPosMethod(TOMASuplPosMethod& aPosMethod)const;

public:
    /**
     * Clone Method for COMASuplReportData
     */
    IMPORT_C COMASuplPositionData* CloneL();
private:
    COMASuplPosition*          iPosition;
    TOMASuplPosMethod       iPosMethod;
        
    };

/**
 *  COMASuplReport
 * 
 */
class COMASuplReportData : public CBase
    {
public:
    enum TOMASuplReportDataResultCode
        {
        ETOMASuplReportDataResultCodeUnknown = 0,
        ETOMASuplReportOutOfRadioCoverage = 1,
        ETOMASuplReportNoPosition,
        ETOMASuplReportNoMeasurement,
        ETOMASuplReportNoPositionNoMeasurement,
        ETOMASuplReportOutOfMemory,
        ETOMASuplReportOutOfMemoryIntermediateReporting,
        ETOMASuplReportOther
        };
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~COMASuplReportData();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplReportData* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplReportData* NewLC();
public:
    /**
     * Setter method for position data
     * @since S60 S60_5.2
     * @param aPositionData position data to be set in the report data
     *          ownership is transferred to the report data class  
     */
    IMPORT_C void SetPosData(COMASuplPositionData* aPositionData);
    
    /**
     * Setter method for time stamp 
     */
    IMPORT_C void SetTimeStamp(const TOMASuplTimeStamp aTimeStamp);
    /**
     * Setter method result code
     */
    IMPORT_C void SetResultCode(const TOMASuplReportDataResultCode aResultCode);
    
    /**
     * Setter method for multiple location ids
     * @since  S60 S60_5.2
     * @param aMultipleLocIds multiple location id to be set 
     *                  ownership is transferred to the report data class
     */
    IMPORT_C void SetMutipleLocId(COMASuplMultipleLocIds* aMultipleLocIds);

public:
    /**
     * Getter method for position data
     * @since S60 S60_5.2
     * @param aPositionData a reference to the position data is returned in this
     *         ownership is still with the report data class 
     */
    IMPORT_C void GetPosData(COMASuplPositionData*& aPositionData)const;
    
    /**
     * Getter method for time stamp
     * @since  S60 S60_5.2
     * @param aTimeStamp time stamp value is returned in this 
     */
    IMPORT_C void GetTimeStamp(TOMASuplTimeStamp& aTimeStamp)const;
    /**
     * Getter method result code
     * @since
     * @param aResultCode result code is returned in this
     * 
     */
    IMPORT_C void GetResultCode(TOMASuplReportDataResultCode& aResultCode)const;
    
    /**
     * Getter method for multiple location ids
     * @since S60 S60_5.2
     * @param aMultipleLocIds a ref to the multiple location id is returned in this
     *          ownership is still with the report data class 
     */
    IMPORT_C void GetMutipleLocId(COMASuplMultipleLocIds*& aMultipleLocIds)const;
        
   
public :
    /**
     * Clone Method for COMASuplReportData
     */
    IMPORT_C COMASuplReportData* CloneL();
private:

    /**
     * Constructor for performing 1st stage construction
     */
    COMASuplReportData();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private:
    COMASuplPositionData* iPositionData;
    TOMASuplTimeStamp     iTimeStamp;
    TOMASuplReportDataResultCode iResultCode;
    COMASuplMultipleLocIds* iMultipleLocIds;
    };



/**
 *  COMASuplReport
 * 
 */
class COMASuplReport : public COMASuplAsnMessageBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~COMASuplReport();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplReport* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static COMASuplReport* NewLC();
    
public: // Functions from base classes
    
    /**
    * Cloning of SuplReport
    * @since S60 S60_5.2
    * @param aStart Base pointer
    * @param aErrorCode error code
    * @return Returns None
    */
     IMPORT_C void Clone(const COMASuplAsnMessageBase* aReport,TInt& aErrorCode);

public:
    /**
     * Getter method for Session List
     * @since S60 S60_5.2
     * @param aSessionList session list is retrieved in this.
     *          Ownership of the pointers in the returned array is retained by the Report object
     *          Ownership of the aSessionList array is not with the report object
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet otherwise
     * 
     */
    IMPORT_C TInt GetSessionList(RPointerArray<COMASuplSessionID>& aSessionList)const;

    /**
     * Getter method for Set caps
     * @since S60 S60_5.2
     * @param aSETCapabilities a reference to the set caps is returned in this
     *        Ownership of the set caps is still with the report object
     */
    IMPORT_C TInt GetSetCapabilities(COMASuplSETCapabilitiesVer2*& aSETCapabilities)const;

    /**
     * Getter method for Session List
     * @since S60 S60_5.2
     * @param aReportDataList reportdata list is retrieved in this.
     *          Ownership of the pointers in the returned array is retained by the Report object
     *          Ownership of the aReportDataList array is not with the report object
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet otherwise
     * 
     */
    IMPORT_C TInt GetReportDataList(RPointerArray<COMASuplReportData>& aReportDataList)const;

    /**
     * Getter method for version
     * @since S60 S60_5.2
     * @param aVer version is retrieved in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet otherwise
     * 
     */
    IMPORT_C TInt GetVer(TDes8& aVer)const;

public:
    
    /**
     * Setter method for Session List
     * @since S60 S60_5.2
     * @param aSessionList session list to be set in the report message
     * @return None
     */
    IMPORT_C void SetSessionList(const RPointerArray<COMASuplSessionID>& aSessionList);

    /**
     * Setter method for Set Capabilities
     * @since S60 S60_5.2
     * @param aSETCapabilities Set Caps to be set in report
     * @return None
     */
    IMPORT_C void SetSetCapabilities(COMASuplSETCapabilitiesVer2* aSETCapabilities);

    /**
     * Setter method for Report Data List
     * @since S60 S60_5.2
     * @param aReportDataList reportdata list to be set in the report object
     * @return None
     */
    IMPORT_C void SetReportDataList(const RPointerArray<COMASuplReportData>& aReportDataList);

    /**
     * Setter method for Version
     * @since S60 S60_5.2
     * @param aVer version to be set in the report 
     * @return none
     */
    IMPORT_C void SetVer(const TDesC8& aVer);


    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C COMASuplReport();
    
protected:
    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void ConstructL();
    

protected:
    RPointerArray<COMASuplSessionID> iSessionList;
    /* SetCapabilities*/
    COMASuplSETCapabilitiesVer2* iSETCapabilities; 
    RPointerArray<COMASuplReportData> iReportDataList;
    //ver field/ of Supl_end
    TBuf8<KVer> iVer;
    
    //iSessionList|iSetCaps|iReportData|iVer
    TInt iOptionalMask;
    };

#endif // EPOS_COMASUPLREPORT_H
