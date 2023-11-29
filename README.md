Diese Repo soll als Beispiel dienen für Grundfunktionen von einem ARM Kerns für die Entwicklung eines ARM Kernels. Der Code bassiert drauf wie ich die ARM Dokumentation verstanden habe und wurde getestet. Jedoch gibt es keine Garantie das ich die Dokumentation zu 100% richtig verstanden habe.

Genutzt werden hier:
- Generic Timer
- Generic Intertupt Controller
- Context Switching
- SVC (SWI)
- UART PL011

# Compiler

Für das Compiling muss der ARM Compiler installiert sein und die Compiler Pfade in .vscode/cmake-kits.json müssen auf die installierten Compiler zeigen.

Entweder
arm-gnu-toolchain-11.3.rel1-mingw-w64-i686-aarch64-none-elf.exe
oder
arm-gnu-toolchain-11.3.rel1-x86_64-aarch64-none-elf.tar.xz

Beide runtergeladeden von https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

# Compiling

Voraussetzungen
- Es sollte CMake installiert sein.
- Eine Path Variable zu Ninja sollte vorhanden sein
- ARM Compiler sollten vorhanden sein

Mit VS Code 

oder manuell mit

1. mkdir build

2. cd build

3. cmake -D CMAKE_C_COMPILER=aarch64-none-elf-gcc -D CMAKE_CXX_COMPILER=aarch64-none-elf-g++ -D CMAKE_ASM_COMPILER=aarch64-none-elf-as -G Ninja ..

4. cmake --build .

# Betriebsystem starten ohne Debugging

Für Windows Path Variable zu qemu-system-aarch64 muss vorhanden sein (z.b C:\Program Files\qemu)

qemu-system-aarch64 -machine virt,virtualization=off,secure=off -m 1G -cpu cortex-a57 -kernel myos.elf

# Befehle für die Konsole

| Befehl | Description |
| ----------- | ----------- |
| start counter | starte Zähler Thread |
| start semaphore oder start sema | Start Consumer-Producer Problem |
| yield | Führe yield aus|
