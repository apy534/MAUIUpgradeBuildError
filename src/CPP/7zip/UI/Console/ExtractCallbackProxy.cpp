// ExtractCallbackProxy.cpp

#include "StdAfx.h"

#include "ExtractCallbackProxy.h"

STDMETHODIMP CExtractCallbackProxy::SetTotal(UInt64 total)
{
  savedTotal = total;
  return CExtractCallbackConsole::SetTotal(total);
}

STDMETHODIMP CExtractCallbackProxy::SetCompleted(const UInt64 * completed)
{
  if (PorgessCallback != NULL)
    PorgessCallback(*completed, savedTotal);
  return CExtractCallbackConsole::SetCompleted(completed);
}