#pragma once

#include "CosKmd.h"

class CosKmDevice;

class CosKmContext
{
    //
    // CosKmdCreateContext acts as the constructor so we list it as a friend
    // CosKmdEscape also acts as an initializer
    //
    friend NTSTATUS CosKmdCreateContext(IN_CONST_HANDLE, INOUT_PDXGKARG_CREATECONTEXT);
    friend NTSTATUS CosKmdEscape(IN_CONST_HANDLE, IN_CONST_PDXGKARG_ESCAPE);

public:

    static NTSTATUS
        __stdcall
        DdiRender(
            IN_CONST_HANDLE         hContext,
            INOUT_PDXGKARG_RENDER   pRender);

    static NTSTATUS
        __stdcall
        DdiCreateContext(
            IN_CONST_HANDLE                 hDevice,
            INOUT_PDXGKARG_CREATECONTEXT    pCreateContext);

    static NTSTATUS
        __stdcall
        DdiDestroyContext(
            IN_CONST_HANDLE     hContext);

protected:
    CosKmDevice           *m_pDevice;
    UINT                    m_Node;
    HANDLE                  m_hRTContext;

    DXGK_CREATECONTEXTFLAGS m_Flags;

#if COS_GPUVA_SUPPORT

    D3DGPU_PHYSICAL_ADDRESS m_rootPageTableAddress;
    UINT                    m_numRootPageTableEntries;

#endif

public:

    UINT
        GetNode()
    {
        return m_Node;
    }

    CosKmDevice *
        GetDevice()
    {
        return m_pDevice;
    }

    DXGK_CREATECONTEXTFLAGS
        GetFlags() const
    {
        return m_Flags;
    }

    static CosKmContext * Cast(IN_CONST_HANDLE hContext)
    {
        CosKmContext * rosKmContext = reinterpret_cast<CosKmContext *>(hContext);

        NT_ASSERT(rosKmContext->m_magic == CosKmContext::kMagic);

        return rosKmContext;
    }

    NTSTATUS
        RenderKm(
            INOUT_PDXGKARG_RENDER   pRender);

#if COS_GPUVA_SUPPORT

    VOID
        SetRootPageTable(
            IN_CONST_PDXGKARG_SETROOTPAGETABLE  pSetPageTable);

#if COS_GPUVA_USE_LOCAL_VIDMEM
    BYTE *
        EmulationGpuVaToCpuVa(
            D3DGPU_VIRTUAL_ADDRESS  gpuVa);
#endif

#endif

public: // PAGED

    _Check_return_
    _Function_class_DXGK_(DXGKDDI_PRESENT)
    _IRQL_requires_(PASSIVE_LEVEL)
    NTSTATUS Present (INOUT_PDXGKARG_PRESENT Args);

private:

    BYTE *
    EmulationGpuPaToCpuVa(
        UINT64  segmentOffset)
    {
        return ((BYTE *)CosKmdGlobal::s_pVideoMemory) + segmentOffset;
    }

    static const UINT32 kMagic = 'CTXT';
    UINT32 m_magic;

};
