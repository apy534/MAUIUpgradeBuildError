// List.h

#ifndef __LIST_H
#define __LIST_H

#include "Common/Wildcard.h"
#include "../Common/LoadCodecs.h"

#ifdef ENV_NDK
struct CArchiveInfo
{
  UInt64 totalPackSize;
  UInt64 totalUnpackSize;
};
#endif

HRESULT ListArchives(CCodecs *codecs, const CIntVector &formatIndices,
    bool stdInMode,
    UStringVector &archivePaths, UStringVector &archivePathsFull,
    const NWildcard::CCensorNode &wildcardCensor,
    bool enableHeaders, bool techMode,
    #ifndef _NO_CRYPTO
    bool &passwordEnabled, UString &password,
    #endif
    #ifdef ENV_NDK
    CArchiveInfo *outArchiveInfo,
    #endif
    UInt64 &errors);

#endif

