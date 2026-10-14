include(GNUInstallDirs)
set(PE2_RELEASE_DOCUMENTS "${PROJECT_SOURCE_DIR}/README.txt")
foreach(notice IN ITEMS LICENSE THIRD_PARTY_NOTICES.md)
    if(EXISTS "${PROJECT_SOURCE_DIR}/${notice}")
        list(APPEND PE2_RELEASE_DOCUMENTS "${PROJECT_SOURCE_DIR}/${notice}")
    endif()
endforeach()
if(WIN32)
    install(FILES ${PE2_RELEASE_DOCUMENTS} DESTINATION "${CMAKE_INSTALL_BINDIR}")
    # Inspect the built PE imports recursively: imported .dll.a/UNKNOWN
    # targets do not reliably populate TARGET_RUNTIME_DLLS (e.g. qrencode).
    set(PE2_WINDOWS_DLL_DIRS "" CACHE STRING "Directories containing Windows runtime DLLs")
    get_filename_component(PE2_COMPILER_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    install(TARGETS pe2randomizer
        RUNTIME_DEPENDENCIES
            DIRECTORIES "$<TARGET_FILE_DIR:pe2randomizer>" "${PE2_COMPILER_BIN_DIR}" ${PE2_WINDOWS_DLL_DIRS}
            PRE_EXCLUDE_REGEXES "^[Aa][Pp][Ii]-[Mm][Ss]-" "^[Ee][Xx][Tt]-[Mm][Ss]-"
            POST_EXCLUDE_REGEXES
                [=[[/\\][Ss][Yy][Ss][Tt][Ee][Mm]32[/\\]]=]
                [=[[/\\][Ss][Yy][Ss][Ww][Oo][Ww]64[/\\]]=]
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    if(MSVC)
        # Supply the redistributable compiler runtime even when the dependency
        # scan resolves a copy from System32 on the build machine.
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
        include(InstallRequiredSystemLibraries)
    endif()
else()
    install(TARGETS pe2randomizer RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" BUNDLE DESTINATION .)
endif()
if(APPLE)
    install(FILES ${PE2_RELEASE_DOCUMENTS} DESTINATION "pe2randomizer.app/Contents/Resources")
    if(PE2_BUNDLE_DEPENDENCIES)
        set(PE2_RUNTIME_LIBRARY_DIRS "" CACHE STRING "Extra library search directories for macOS bundle installation")
        set(PE2_FIXUP_DIRS ${PE2_RUNTIME_LIBRARY_DIRS} ${wxWidgets_LIBRARY_DIRS} ${QRENCODE_LIBRARY_DIRS})
        foreach(prefix IN LISTS CMAKE_PREFIX_PATH CMAKE_SYSTEM_PREFIX_PATH)
            list(APPEND PE2_FIXUP_DIRS "${prefix}/lib")
        endforeach()
        list(REMOVE_DUPLICATES PE2_FIXUP_DIRS)
        configure_file(cmake/FixupMacBundle.cmake.in FixupMacBundle.cmake @ONLY)
        install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/FixupMacBundle.cmake")
    endif()
else()
    if(WIN32)
        set(resource_install_dir "${CMAKE_INSTALL_BINDIR}/resources")
    else()
        set(resource_install_dir "${CMAKE_INSTALL_DATADIR}/pe2randomizer")
    endif()
    foreach(resource IN LISTS PE2_RUNTIME_FILES)
        get_filename_component(parent "${resource}" DIRECTORY)
        install(FILES "${PROJECT_SOURCE_DIR}/${resource}" DESTINATION "${resource_install_dir}/${parent}")
    endforeach()
    if(UNIX)
        install(FILES ${PE2_RELEASE_DOCUMENTS} DESTINATION "${CMAKE_INSTALL_DOCDIR}")
        install(FILES cmake/pe2randomizer.desktop DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
        install(FILES assets/aya.png DESTINATION "${CMAKE_INSTALL_DATADIR}/pixmaps" RENAME pe2randomizer.png)
        include("${PROJECT_SOURCE_DIR}/cmake/Packaging.cmake")
    endif()
endif()
