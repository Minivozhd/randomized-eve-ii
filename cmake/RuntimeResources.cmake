# All game patch payloads are compiled into pe2_core; only GUI artwork is staged.
set(PE2_RUNTIME_FILES assets/aya.png assets/icon.ico)
foreach(resource IN LISTS PE2_RUNTIME_FILES)
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${resource}")
        message(FATAL_ERROR "Missing UI resource: ${resource}")
    endif()
endforeach()
set(PE2_RESOURCE_BASE "${CMAKE_CURRENT_BINARY_DIR}")
if(CMAKE_CONFIGURATION_TYPES)
    string(APPEND PE2_RESOURCE_BASE "/$<CONFIG>")
endif()
if(APPLE)
    set(PE2_RESOURCE_DESTINATION "${PE2_RESOURCE_BASE}/pe2randomizer.app/Contents/Resources")
else()
    set(PE2_RESOURCE_DESTINATION "${PE2_RESOURCE_BASE}/resources")
endif()
configure_file(cmake/ResourceManifest.cmake.in ResourceManifest.cmake @ONLY)
add_custom_target(pe2_resources
    COMMAND "${CMAKE_COMMAND}" "-DMANIFEST=${CMAKE_CURRENT_BINARY_DIR}/ResourceManifest.cmake"
            "-DDESTINATION=${PE2_RESOURCE_DESTINATION}" -P "${PROJECT_SOURCE_DIR}/cmake/StageResources.cmake"
    COMMENT "Staging UI artwork" VERBATIM)
add_dependencies(pe2randomizer pe2_resources)
