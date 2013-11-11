/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  xadebugtraceutility functionality
*
*/

#include <e32debug.h>

extern "C"  {

    void xa_print(const char* formatString)
    {
        RDebug::Printf(formatString);
    }

    void xa_print_1(const char* formatString, int a1)
    {
        RDebug::Printf(formatString, a1);
    }

    void xa_print_1_str(const char* formatString, const char* a1)
    {
        RDebug::Printf(formatString, a1);
    }

    void xa_print_2(const char* formatString, int a1, int a2)
    {
        RDebug::Printf(formatString, a1, a2);
    }

    void xa_print_2_str(const char* formatString, const char* a1, const char* a2)
    {
        RDebug::Printf(formatString, a1, a2);
    }

    void xa_print_3(const char* formatString, int a1, int a2, int a3)
    {
        RDebug::Printf(formatString, a1, a2, a3);
    }

    void xa_print_4(const char* formatString, int a1, int a2, int a3, int a4)
    {
        RDebug::Printf(formatString, a1, a2, a3, a4);
    }

    void xa_heap_total_alloc_print(const char* formatString)
    {
        RThread thread;
        TInt cells = User::CountAllocCells();
        TInt threadId = thread.Id();
        RDebug::Printf(formatString);
        RDebug::Print(_L("ThreadId[%d] Cells[%d]"),threadId, cells);
    }
}

void xa_cppprint(const char* formatString)
{
    RDebug::Printf(formatString);
}

void xa_cppprint_1(const char* formatString, int a1)
{
    RDebug::Printf(formatString, a1);
}

void xa_cppprint_2(const char* formatString, int a1, int a2)
{
    RDebug::Printf(formatString, a1, a2);
}
