// UpdateCallbackProxy.h

#ifndef __UPDATE_CALLBACK_PROXY_H
#define __UPDATE_CALLBACK_PROXY_H

#include "UpdateCallbackConsole.h"

class CUpdateCallbackProxy:
  public CUpdateCallbackConsole
{
  UInt64 savedTotal = 0;
public:
  STDMETHOD(SetTotal)(UInt64 total);
  STDMETHOD(SetCompleted)(const UInt64 *completeValue);

  void (*PorgessCallback) (const UInt64 completed, const UInt64 total) = nullptr;
};

#endif