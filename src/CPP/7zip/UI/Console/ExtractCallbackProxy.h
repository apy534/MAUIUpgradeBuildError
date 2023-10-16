// ExtractCallbackProxy.h

#ifndef __EXTRACTCALLBACKPROXY_H
#define __EXTRACTCALLBACKPROXY_H

#include "ExtractCallbackConsole.h"

class CExtractCallbackProxy:
  public CExtractCallbackConsole
{
  UInt64 savedTotal = 0;
public:
  STDMETHOD(SetTotal)(UInt64 total);
  STDMETHOD(SetCompleted)(const UInt64 *completeValue);

  void (*PorgessCallback) (const UInt64 completed, const UInt64 total) = nullptr;
};

#endif