project(wolkanlin_tests)

find_package(Qt5Test 5.6.0 REQUIRED)

set(TESTS_SERVER_URL "" CACHE STRING "URL to a server user for API tests. Can also be set via WOLKANLIN_TESTS_SERVER_URL environment variable.")
set(TESTS_ADMIN_USERNAME "" CACHE STRING "User name of an admin account used for API tests. Can also be set via WOLKANLIN_TESTS_ADMIN_USERNAME environment variable.")
set(TESTS_ADMIN_PASSWORD "" CACHE STRING "Password of an admin account used for API tests. Can also be set via WOLKANLIN_TESTS_ADMIN_PASSWORD environment variable.")
set(TESTS_USER_USERNAME "" CACHE STRING "User name of a normal account used for API tests. Can also be set via WOLKANLIN_TESTS_USER_USERNAME environment variable.")
set(TESTS_USER_PASSWORD "" CACHE STRING "Password of a normal account used for API tests. Can also be set via WOLKANLIN_TESTS_USER_PASSWORD environment variable.")
option(TESTS_IGNORE_SSL_ERRORS "Ignore SSL errors when performing API tests. Can also be set via WOLKANLIN_TESTS_IGNORE_SSL_ERRORS environment variable.")

add_executable(testdefaultvalues_exec testdefaultvalues.cpp testconfig.h testconfig.cpp testnamfactory.h testnamfactory.cpp)
add_test(NAME testdefaultvalues COMMAND testdefaultvalues_exec)
target_link_libraries(testdefaultvalues_exec Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network WolkanlinQt${QT_VERSION_MAJOR})

function(wolkanlin_unit_test _testname)
    add_executable(${_testname}_exec ${_testname}.cpp testconfig.h testconfig.cpp)
    add_test(NAME ${_testname} COMMAND ${_testname}_exec)
    target_link_libraries(${_testname}_exec Qt${QT_VERSION_MAJOR}::Test WolkanlinQt${QT_VERSION_MAJOR})
endfunction(wolkanlin_unit_test)

wolkanlin_unit_test(testquotaobject)
wolkanlin_unit_test(testuserobject)
wolkanlin_unit_test(testserverstatusobject)
wolkanlin_unit_test(testjobs)

if(WITH_API_TESTS)
    find_package(SchauerQt5 REQUIRED)
    add_executable(testapicalls_exec testapicalls.cpp testconfig.h testconfig.cpp)
#    add_test(NAME testapicalls COMMAND testapicalls_exec)
    target_link_libraries(testapicalls_exec Qt${QT_VERSION_MAJOR}::Test SchauerQt${QT_VERSION_MAJOR} WolkanlinQt${QT_VERSION_MAJOR})
    target_compile_definitions(testapicalls_exec
        PRIVATE
            WOLKANLIN_TESTS_SERVER_URL="${TESTS_SERVER_URL}"
            WOLKANLIN_TESTS_ADMIN_USERNAME="${TESTS_ADMIN_USERNAME}"
            WOLKANLIN_TESTS_ADMIN_PASSWORD="${TESTS_ADMIN_PASSWORD}"
            WOLKANLIN_TESTS_USER_USERNAME="${TESTS_USER_USERNAME}"
            WOLKANLIN_TESTS_USER_PASSWORD="${TESTS_USER_PASSWORD}"
            WOLKANLIN_TESTS_IGNORE_SSL_ERRORS="${TESTS_IGNORE_SSL_ERRORS}"
    )
endif(WITH_API_TESTS)
