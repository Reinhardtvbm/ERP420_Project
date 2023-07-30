
if (NOT EXISTS "/home/reinhardt/personal/ERP420_Project/Practical1_Rust/target/release/build/glfw-sys-114129cec19a5345/out/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/reinhardt/personal/ERP420_Project/Practical1_Rust/target/release/build/glfw-sys-114129cec19a5345/out/build/install_manifest.txt\"")
endif()

file(READ "/home/reinhardt/personal/ERP420_Project/Practical1_Rust/target/release/build/glfw-sys-114129cec19a5345/out/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/usr/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/usr/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()
