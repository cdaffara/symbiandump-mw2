rem Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of the "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem echo Description: script for sisx file creation for thumbnail engine STIF test
rem


rm *.SIS
rm *.sis*
makesis tnetest.pkg
signsis -v tnetest.sis tnetest.sisx C:\keys\RDTest_02.der C:\keys\RDTest_02.key

