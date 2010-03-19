# - Find the Coriolis includes and libraries.
# The following variables are set if Coriolis is found.
# If Coriolis is not found, CRLCORE_FOUND is set to false.
#  CRLCORE_FOUND       - True when the Coriolis include directory is found.
#  CRLCORE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  CRLCORE_LIBRARIES   - The path to where the Coriolis library files are.

SET(CORIOLIS_INCLUDE_PATH_DESCRIPTION "directory containing the Coriolis include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")
SET(CORIOLIS_DIR_MESSAGE "Set the CORIOLIS_INCLUDE_DIR cmake cache entry to the ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  # Setup the DIR_SEARCH_PATH.
  MACRO(SETUP_SEARCH_DIR project)
    IF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
      MESSAGE("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
      LIST(FIND ${project}_DIR_SEARCH "${${project}_DIR_SEARCH}" DIR_INDEX)
      IF( DIR_INDEX LESS 0)
        LIST(INSERT ${project}_DIR_SEARCH 0 "$ENV{${project}_USER_TOP}")
      ENDIF( DIR_INDEX LESS 0)
    ENDIF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
    
    IF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
      MESSAGE("-- ${project}_TOP is set to $ENV{${project}_TOP}")
      LIST(FIND ${project}_DIR_SEARCH "${${project}_DIR_SEARCH}" DIR_INDEX)
      IF( DIR_INDEX LESS 0)
        LIST(INSERT ${project}_DIR_SEARCH 0 "$ENV{${project}_TOP}")
      ENDIF( DIR_INDEX LESS 0)
    ENDIF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
  ENDMACRO(SETUP_SEARCH_DIR project)
  
  SETUP_SEARCH_DIR(CORIOLIS)
  #
  # Look for an installation.
  #
  FIND_PATH(CRLCORE_INCLUDE_PATH NAMES crlcore/ToolEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CRLCORE_LIBRARY_PATH
    NAMES crlcore
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CRLCORE_STATIC_LIBRARY_PATH
    NAMES crlcore-static
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(CORIOLIS CRLCORE)
  HURRICANE_CHECK_LIBRARIES(CORIOLIS)

ENDIF(UNIX)
