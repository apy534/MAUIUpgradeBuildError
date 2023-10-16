// UpdateCallbackProxy.cpp

#include "StdAfx.h"

#include "UpdateCallbackProxy.h"

STDMETHODIMP CUpdateCallbackProxy::SetTotal(UInt64 total)
{
  savedTotal = total;
  return CUpdateCallbackConsole::SetTotal(total);
}

STDMETHODIMP CUpdateCallbackProxy::SetCompleted(const UInt64 * completed)
{
  if (PorgessCallback != NULL)
    PorgessCallback(*completed, savedTotal);
  return CUpdateCallbackConsole::SetCompleted(completed);
}