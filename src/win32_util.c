#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "win32_util.h"


uint32_t virtual_address_to_file_offset(const void* file_data, const uintptr_t virtual_address)
{
    const IMAGE_DOS_HEADER* const dos_header = (void*)file_data;
    const IMAGE_NT_HEADERS* const nt_headers = (void*)((uint8_t*)file_data + dos_header->e_lfanew);

    const uintptr_t image_base = nt_headers->OptionalHeader.ImageBase;

    if (virtual_address < image_base || virtual_address >(image_base + nt_headers->OptionalHeader.SizeOfImage))
    {
        return UINT32_MAX;
    }

    const uint32_t rva_find = (uint32_t)(virtual_address - image_base);

    const IMAGE_SECTION_HEADER* section_header = IMAGE_FIRST_SECTION(nt_headers);

    for (uint32_t i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i, ++section_header)
    {
        const uint32_t rva_seg_lo = section_header->VirtualAddress;
        const uint32_t rva_seg_hi = rva_seg_lo + section_header->SizeOfRawData;

        if (rva_find >= rva_seg_lo && rva_find <= rva_seg_hi)
        {
            return (rva_find - rva_seg_lo) + section_header->PointerToRawData;
        }
    }
    return UINT32_MAX;
}

uintptr_t file_offset_to_virtual_address(const void* file_data, const uint32_t file_offset)
{
    const IMAGE_DOS_HEADER* const dos_header = (void*)file_data;
    const IMAGE_NT_HEADERS* const nt_headers = (void*)((uint8_t*)file_data + dos_header->e_lfanew);

    const uintptr_t image_base = nt_headers->OptionalHeader.ImageBase;

    const IMAGE_SECTION_HEADER* section_header = IMAGE_FIRST_SECTION(nt_headers);

    for (uint32_t i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i, ++section_header)
    {
        const uint32_t file_off_seg_lo = section_header->PointerToRawData;
        const uint32_t file_off_seg_hi = file_off_seg_lo + section_header->SizeOfRawData;

        if ((file_offset >= file_off_seg_lo) && (file_offset <= file_off_seg_hi))
        {
            return image_base + section_header->VirtualAddress + (file_offset - file_off_seg_lo);
        }
    }
    return UINT32_MAX;
}