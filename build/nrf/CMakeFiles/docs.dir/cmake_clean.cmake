file(REMOVE_RECURSE
  "html"
  "latex"
  "userlib"
  "CMakeFiles/docs"
  "Zephyr-Kernel.map"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/docs.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
