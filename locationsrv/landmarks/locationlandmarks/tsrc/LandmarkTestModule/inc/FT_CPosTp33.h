/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*   ?description_line
*
*/


#ifndef CPOSTP33_H
#define CPOSTP33_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"


// CLASS DECLARATION


//constants 
/**
*  Test procedure 33
*/
class CPosTp33 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp33() : CLandmarkTestProcedureBase(33) {};
        
        /**
        * Destructor.
        */
        ~CPosTp33() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();

        static void RunTestL(TAny* aData);

    private:

        /**
        * Opening and closing the landmark database serveral times fast
        */        
        void StressTestL();

        /**
        * TBD
        */        
        void ThreadTestL();

        void StartMultipleClientsL(TUint aNoClients);

        void CreateThreadsL(const TUint aNoThreads);

        void KillTestL();
        
        		void ListAllServers(const TDesC& aServerName);
		
		void ListAllProcesses(const TDesC& aProcessName);
		
		void ListAllThreads(const TDesC& aThreadName);

        /**
        * Prints the Landmark heap to the logfile.
        */
        void MonitorHeapL(TBool aSetMaxHeap = EFalse);

        /**
        * Get heap size for specified thread
        * using the RThread::GetRamSizes() method on TARGET
        */
        TInt GetHeapSizeL(const TDesC& aThreadName, TInt& aSize, TInt& aUsed);

    private:

        RArray<RThread>	    iThreads; 
        TInt                iThreadIndex;
        CPosLandmarkDatabase* iLmd1;
        CPosLandmarkDatabase* iLmd2;
        TInt                  iMaxHeap;
    };



#endif      // CPOSTP33_H
            
// End of File
