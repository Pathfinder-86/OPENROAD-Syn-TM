# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/par/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/par/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(par.read_part "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/par/test/regression" "read_part")
set_tests_properties(par.read_part PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=read_part;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/par/test/results/read_part.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/par/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/par/test/CMakeLists.txt;9;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/par/test/CMakeLists.txt;0;")
add_test(par.partition_gcd "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/par/test/regression" "partition_gcd")
set_tests_properties(par.partition_gcd PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=partition_gcd;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/par/test/results/partition_gcd.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/par/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/par/test/CMakeLists.txt;9;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/par/test/CMakeLists.txt;0;")
