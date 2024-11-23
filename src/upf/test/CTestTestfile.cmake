# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/upf/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/upf/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(upf.upf_test "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/upf/test/regression" "upf_test")
set_tests_properties(upf.upf_test PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=upf_test;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/upf/test/results/upf_test.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/upf/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/upf/test/CMakeLists.txt;9;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/upf/test/CMakeLists.txt;0;")
add_test(upf.levelshifter "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/upf/test/regression" "levelshifter")
set_tests_properties(upf.levelshifter PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=levelshifter;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/upf/test/results/levelshifter.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/upf/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/upf/test/CMakeLists.txt;9;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/upf/test/CMakeLists.txt;0;")
