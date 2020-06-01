file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "CMakeFiles/hardenconfig"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/hardenconfig.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
