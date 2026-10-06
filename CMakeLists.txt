cmake_minimum_required(VERSION 3.20)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_STANDARD 23)
project(os LANGUAGES C)

add_executable(bootx64 src/main.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_target_properties(bootx64 PROPERTIES SUFFIX ".efi" CXX_STANDARD 20)

target_compile_options(bootx64 PRIVATE
  --target=x86_64-unknown-windows
  -ffreestanding -fshort-wchar
  -fno-exceptions -fno-rtti
  -fno-stack-protector -mno-stack-arg-probe
  -Wall -Wextra)

target_link_options(bootx64 PRIVATE
  --target=x86_64-unknown-windows
  -nostdlib -fuse-ld=lld-link
  -Wl,-entry:efi_main
  -Wl,-subsystem:efi_application)

set(ESP ${CMAKE_BINARY_DIR}/esp)
add_custom_command(TARGET bootx64 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${ESP}/EFI/BOOT
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bootx64> ${ESP}/EFI/BOOT/BOOTX64.EFI)

add_custom_target(run DEPENDS bootx64 USES_TERMINAL
  COMMAND qemu-system-x86_64 -machine q35 -m 512M
          -bios /usr/share/ovmf/OVMF.fd
          -drive format=raw,file=fat:rw:${ESP})

target_include_directories(bootx64 PRIVATE
    /usr/include/efi
)