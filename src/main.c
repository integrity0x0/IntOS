#include <stdint.h>
#include <stdbool.h>
#include <efi.h>

#define U(s) L##s

extern EFI_STATUS EFIAPI efi_main(EFI_HANDLE, EFI_SYSTEM_TABLE *st) {
    st->ConOut->ClearScreen(st->ConOut);
    st->ConOut->OutputString(st->ConOut, U("Hello, world\n"));
    for (;;) {
        __asm__ volatile("hlt");
    }
}