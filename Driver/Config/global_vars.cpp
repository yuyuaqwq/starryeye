#include "Config/global_vars.h"

UNICODE_STRING StarryEye::ProcessTypeString = UNICODE_STRING();
UNICODE_STRING StarryEye::ThreadTypeString = UNICODE_STRING();
PVOID StarryEye::PspCidTable = nullptr;
PULONG64 StarryEye::ObTypeIndexTable = nullptr;
UCHAR StarryEye::ObHeaderCookie = 0;