SET PATH=D:\MDK5\ARM\ARMCC\Bin;C:\Python27\;C:\Python27\Scripts;C:\ProgramData\Oracle\Java\javapath;C:\Perl\site\bin;C:\Perl\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;D:\Git\cmd;D:\PuTTY\;D:\Microsoft VS Code\bin;C:\Users\jason\AppData\Local\Microsoft\WindowsApps;C:\Python27\Scripts;
SET CPU_TYPE=STM32F429IGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=rt-thread_stm32f4xx
SET CPU_CLOCK=0x00B71B00
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\app_uart.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\main.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\board.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\stm32f4xx_it.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmCC" --Via ".\build\drv_gpio.__i"
"D:\MDK5\ARM\ARMCC\Bin\ArmLink" --Via ".\build\rtthread-stm32f4xx.lnp"
"D:\MDK5\ARM\ARMCC\Bin\fromelf.exe" ".\build\rtthread-stm32f4xx.axf" --i32combined --output ".\build\rtthread-stm32f4xx.hex"
fromelf --bin .\build\rtthread-stm32f4xx.axf --output rtthread-stm32f4xx.bin
