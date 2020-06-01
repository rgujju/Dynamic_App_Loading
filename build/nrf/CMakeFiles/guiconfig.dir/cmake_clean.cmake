file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "CMakeFiles/guiconfig"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/guiconfig.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
