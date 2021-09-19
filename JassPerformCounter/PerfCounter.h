/*
Module : PERFCOUNTER.H
Purpose: Defines the interface for a simple C++ class which encapsulates the Win32 Performance Counter APIs
Created: PJN / 30-03-2010
History: PJN / 30-03-2010 1. Initial Public release
         PJN / 05-04-2015 1. Updated copyright details.
         PJN / 02-01-2016 1. Updated copyright details.
                          2. Verified the code compiles cleanly on VC 2015.

Copyright (c) 2010 - 2016 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Includes ///////////////////////////////////

#pragma once

#ifndef __PERFCOUNTER_H__
#define __PERFCOUNTER_H__

#ifndef CPERFORMANCECOUNTER_EXT_CLASS
#define CPERFORMANCECOUNTER_EXT_CLASS
#endif //#ifndef CPERFORMANCECOUNTER_EXT_CLASS


//////////////////////// Classes //////////////////////////////////////////////

//A simple class to encapsulate the Win32 Performance Counter APIs
class CPERFORMANCECOUNTER_EXT_CLASS CPerformanceCounter
{
public:
//Constructors / Destructors
  CPerformanceCounter()
  {
    //Initialize our member variables
    m_lStart.QuadPart = 0;
    m_lStop.QuadPart = 0;
    m_lFrequency.QuadPart = 0;
  };
  
//Methods  
  BOOL Start()
  {
    return QueryPerformanceCounter(&m_lStart);
  }
  
  BOOL Stop()
  {
    return QueryPerformanceCounter(&m_lStop);
  }
  
  LONGLONG Difference()
  {
    return m_lStop.QuadPart - m_lStart.QuadPart;
  }

  LONGLONG AsNanoSeconds(LONGLONG lValue)
  {
    return static_cast<LONGLONG>(AsSeconds(lValue) * 1000000000);
  }

  LONGLONG AsMicroSeconds(LONGLONG lValue)
  {
    return static_cast<LONGLONG>(AsSeconds(lValue) * 1000000);
  }

  LONGLONG AsMilliSeconds(LONGLONG lValue)
  {
    return static_cast<LONGLONG>(AsSeconds(lValue) * 1000);
  }

  double AsSeconds(LONGLONG lValue)
  {
    return lValue / static_cast<double>(Frequency());
  }

  LONGLONG Frequency()
  {
    if (m_lFrequency.QuadPart == 0)
    {
      if (!QueryPerformanceFrequency(&m_lFrequency))
        m_lFrequency.QuadPart = -1;
    }

    return m_lFrequency.QuadPart;
  }
  
protected:
//Member variables
  LARGE_INTEGER m_lStart;
  LARGE_INTEGER m_lStop;  
  LARGE_INTEGER m_lFrequency;
};


#endif //#define __PERFCOUNTER_H__