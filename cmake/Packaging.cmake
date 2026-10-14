# Linux releases use the system package manager for GTK and other shared
# libraries. Avoid bundling libc or claiming a distribution-independent ABI.
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "pe2randomizer")
set(CPACK_PACKAGE_VENDOR "Strog and TheVozhd")
set(CPACK_PACKAGE_CONTACT "TheVozhd")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Randomized Eve II: Parasite Eve II randomizer")
set(CPACK_PACKAGE_DESCRIPTION "A desktop randomizer for Parasite Eve II (USA / NTSC-U). Users supply original BIN/CUE images of both discs; game disc images are not included.")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS OFF)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# The package manager resolves the precise wxWidgets/qrencode/GTK SONAMEs
# for the distribution where this package was built.
include(CPack)
