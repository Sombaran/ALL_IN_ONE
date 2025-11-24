#
# Setup the auto test build.
#
# To enable auto test, include the following option on the cmake command line:
#     -ACSDK_AUTO_TEST_SUPPORTED=ON
#

option(ACSDK_AUTO_TEST_SUPPORTED "Enable Auto-Test." OFF)

if(ACSDK_AUTO_TEST_SUPPORTED)
    add_definitions(-DAUTO_TEST_SUPPORTED)
else()
    message(STATUS "Auto-Test is not supported.")
endif()
