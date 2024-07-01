SET PATH=C:\Keil_v5\ARM\ARMCC\bin\;D:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX\;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Python27\;C:\Python27\Scripts;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\dotnet\;D:\MATLAB2020a\runtime\win64;D:\MATLAB2020a\bin;D:\MATLAB2020a\polyspace\bin;C:\Program Files\dotnet\;C:\build\17.0\arm\sysroots\i686-nilrtsdk-mingw32\usr\libexec\arm-nilrt-linux-gnueabi\gcc\arm-nilrt-linux-gnueabi\4.9.2;C:\build\17.0\x64\sysroots\i686-nilrtsdk-mingw32\usr\libexec\x86_64-nilrt-linux\gcc\x86_64-nilrt-linux\4.9.2;C:\Program Files (x86)\IVI Foundation\IVI\bin;C:\Program Files\IVI Foundation\IVI\bin;C:\Program Files (x86)\EasyShare\x86\;C:\Program Files (x86)\EasyShare\x64\;C:\Users\KingSei\AppData\Local\Microsoft\WindowsApps;C:\Users\KingSei\AppData\Roaming\ËÑºüÓ°Òô\6.5.12.0;C:\Users\KingSei\.dotnet\tools;D:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX\.\jre\\bin
SET CPU_TYPE=STM32F103RB
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=J_WaterfallLight
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\bin\ArmAsm" --Via "j_waterfalllight\startup_stm32f103xb._ia"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\main.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_msp.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_gpio_ex.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_tim.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_tim_ex.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_rcc_ex.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_dma.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_pwr.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_flash.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_flash_ex.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\stm32f1xx_hal_exti.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmCC" --Via "j_waterfalllight\system_stm32f1xx.__i"
"C:\Keil_v5\ARM\ARMCC\bin\ArmLink" --Via "J_WaterfallLight\J_WaterfallLight.lnp"
"C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe" "J_WaterfallLight\J_WaterfallLight.axf" --i32combined --output "J_WaterfallLight\J_WaterfallLight.hex"
