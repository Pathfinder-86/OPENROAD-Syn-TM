# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/utl/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/utl/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(utl.test_info "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/utl/test/regression" "test_info")
set_tests_properties(utl.test_info PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=test_info;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/utl/test/results/test_info.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/utl/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;11;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;0;")
add_test(utl.test_error "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/utl/test/regression" "test_error")
set_tests_properties(utl.test_error PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=test_error;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/utl/test/results/test_error.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/utl/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;11;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;0;")
add_test(utl.test_suppress_message "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/utl/test/regression" "test_suppress_message")
set_tests_properties(utl.test_suppress_message PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=test_suppress_message;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/utl/test/results/test_suppress_message.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/utl/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;11;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;0;")
add_test(utl.test_metrics "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/utl/test/regression" "test_metrics")
set_tests_properties(utl.test_metrics PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=test_metrics;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/utl/test/results/test_metrics.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/utl/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;11;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/utl/test/CMakeLists.txt;0;")
subdirs("cpp")
