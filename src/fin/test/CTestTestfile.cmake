# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/fin/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/fin/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(fin.gcd_fill "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/fin/test/regression" "gcd_fill")
set_tests_properties(fin.gcd_fill PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=gcd_fill;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/fin/test/results/gcd_fill.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/fin/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/fin/test/CMakeLists.txt;8;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/fin/test/CMakeLists.txt;0;")
