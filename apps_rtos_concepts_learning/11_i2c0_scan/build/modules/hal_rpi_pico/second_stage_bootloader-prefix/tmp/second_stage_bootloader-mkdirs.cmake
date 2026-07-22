# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ezhil/embedded/zephyr_workspace/zephyr/modules/hal_rpi_pico/bootloader"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/bootloader"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/tmp"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/src/second_stage_bootloader-stamp"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/src"
  "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/src/second_stage_bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/src/second_stage_bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ezhil/embedded/zephyr_workspace/apps_learning/11_i2c0_scan/build/modules/hal_rpi_pico/second_stage_bootloader-prefix/src/second_stage_bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
