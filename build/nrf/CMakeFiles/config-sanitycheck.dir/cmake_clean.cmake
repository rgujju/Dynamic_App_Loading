file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "CMakeFiles/config-sanitycheck"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/config-sanitycheck.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
