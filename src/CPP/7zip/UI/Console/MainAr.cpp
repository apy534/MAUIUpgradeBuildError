// MainAr.cpp

#include "StdAfx.h"

#include "Common/NewHandler.h" // FIXME

#include "Common/MyException.h"
#include "Common/StdOutStream.h"

#include "Windows/Error.h"
#include "Windows/NtCheck.h"

#include "../Common/ArchiveCommandLine.h"
#include "../Common/ExitCode.h"

#include "ConsoleClose.h"

using namespace NWindows;

#ifdef ENV_NDK
struct CArchiveInfo;
#endif

CStdOutStream *g_StdStream = 0;

extern int Main2(
  #ifdef ENV_NDK
  int numArgs, const char *args[], void (*porgessCallback) (const UInt64 completed, const UInt64 total), CArchiveInfo* outArchiveInfo
  #elif _WIN32
  #else
  int numArgs, const char *args[]
  #endif
);

static const char *kExceptionErrorMessage = "\n\nError:\n";
static const char *kUserBreak  = "\nBreak signaled\n";
static const char *kMemoryExceptionMessage = "\n\nERROR: Can't allocate required memory!\n";
static const char *kUnknownExceptionMessage = "\n\nUnknown Error\n";
static const char *kInternalExceptionMessage = "\n\nInternal Error #";

#define NT_CHECK_FAIL_ACTION (*g_StdStream) << "Unsupported Windows version"; return NExitCode::kFatalError;


#ifdef ENV_NDK
extern "C" int MY_CDECL external_main(
int numArgs, const char *args[], void (*porgessCallback) (const UInt64 completed, const UInt64 total), CArchiveInfo* outArchiveInfo
)
#else
int MY_CDECL main(
  #ifndef _WIN32
  int numArgs, const char *args[]
  #endif
)
#endif
{
  g_StdStream = &g_StdOut;

  NT_CHECK

  NConsoleClose::CCtrlHandlerSetter ctrlHandlerSetter;
  int res = 0;
  try
  {
    res = Main2(
    #ifdef ENV_NDK
    numArgs, args, porgessCallback, outArchiveInfo
    #elif _WIN32
    #else
    numArgs, args
    #endif
    );
  }
  catch(const CNewException &)
  {
    (*g_StdStream) << kMemoryExceptionMessage;
    return (NExitCode::kMemoryError);
  }
  catch(const NConsoleClose::CCtrlBreakException &)
  {
    (*g_StdStream) << endl << kUserBreak;
    return (NExitCode::kUserBreak);
  }
  catch(const CArchiveCommandLineException &e)
  {
    (*g_StdStream) << kExceptionErrorMessage << e << endl;
    return (NExitCode::kUserError);
  }
  catch(const CSystemException &systemError)
  {
    if (systemError.ErrorCode == E_OUTOFMEMORY)
    {
      (*g_StdStream) << kMemoryExceptionMessage;
      return (NExitCode::kMemoryError);
    }
    if (systemError.ErrorCode == E_ABORT)
    {
      (*g_StdStream) << endl << kUserBreak;
      return (NExitCode::kUserBreak);
    }
    UString message;
    NError::MyFormatMessage(systemError.ErrorCode, message);
    (*g_StdStream) << endl << endl << "System error:" << endl << message << endl;
    return (NExitCode::kFatalError);
  }
  catch(NExitCode::EEnum &exitCode)
  {
    (*g_StdStream) << kInternalExceptionMessage << exitCode << endl;
    return (exitCode);
  }
  /*
  catch(const NExitCode::CMultipleErrors &multipleErrors)
  {
    (*g_StdStream) << endl << multipleErrors.NumErrors << " errors" << endl;
    return (NExitCode::kFatalError);
  }
  */
  catch(const UString &s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(const AString &s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(const char *s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(int t)
  {
    (*g_StdStream) << kInternalExceptionMessage << t << endl;
    return (NExitCode::kFatalError);
  }
  catch(...)
  {
    (*g_StdStream) << kUnknownExceptionMessage;
    return (NExitCode::kFatalError);
  }
  return  res;
}

#if !defined(ENV_NDK)
void MY_CDECL BreakExecution
#else
extern "C" void MY_CDECL external_BreakExecution
#endif
()
{
  NConsoleClose::IncreaseBreakCounter();
}