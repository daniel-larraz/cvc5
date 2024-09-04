execute_process(
  COMMAND otool -L ${DYLIB_PATH}
  OUTPUT_VARIABLE OTOOL_OUTPUT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" ";" OTOOL_LINES "${OTOOL_OUTPUT}")
if(RPATH)
  execute_process(
    COMMAND ${INSTALL_NAME_TOOL} -add_rpath ${RPATH} ${DYLIB_PATH}
  )
endif()
foreach(LINE ${OTOOL_LINES})
  if(LINE MATCHES "${DEPS_BASE}/lib")
    string(REGEX REPLACE "^[ \t]*([^ \t]+).*" "\\1" LIB_PATH "${LINE}")
    string(REPLACE "${DEPS_BASE}/lib" "@rpath" LIB_RPATH "${LIB_PATH}")
    message("COMMAND ${INSTALL_NAME_TOOL} -change ${LIB_PATH} ${LIB_RPATH} ${DYLIB_PATH}")

    execute_process(
      COMMAND ${INSTALL_NAME_TOOL} -change ${LIB_PATH} ${LIB_RPATH} ${DYLIB_PATH}
    )
  endif()
endforeach()