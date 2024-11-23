# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/gui/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/gui/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(gui.supported "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/gui/test/regression" "supported")
set_tests_properties(gui.supported PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=supported;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/gui/test/results/supported.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/gui/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/gui/test/CMakeLists.txt;8;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/gui/test/CMakeLists.txt;0;")
