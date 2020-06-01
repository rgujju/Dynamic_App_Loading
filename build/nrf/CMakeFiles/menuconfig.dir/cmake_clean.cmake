file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "CMakeFiles/menuconfig"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/menuconfig.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
