file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/zephyr_property_target.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
