file( GLOB TEST_SOURCES *.c )

foreach( testsrc ${TEST_SOURCES} )
        get_filename_component (test ${testsrc} NAME_WE)
        string(CONCAT test "test_" ${test})
        message ("adding test: ${test}")
	add_executable( ${test} ${testsrc} )
	target_link_libraries( ${test} hll m)
	add_test( ${test} ${test} )
endforeach( testsrc ${TEST_SOURCES} )


