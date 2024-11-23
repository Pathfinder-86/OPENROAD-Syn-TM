# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/mpl2/test
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/mpl2/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(mpl2.macro_only "/usr/bin/bash" "/home/r12943092/OPENROAD-Syn-TM/src/mpl2/test/regression" "macro_only")
set_tests_properties(mpl2.macro_only PROPERTIES  ENVIRONMENT "TEST_TYPE=compare_logfile;CTEST_TESTNAME=macro_only;DIFF_LOCATION=/home/r12943092/OPENROAD-Syn-TM/src/mpl2/test/results/macro_only.diff" LABELS "IntegrationTest" WORKING_DIRECTORY "/home/r12943092/OPENROAD-Syn-TM/src/mpl2/test" _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/cmake/testing.cmake;2;add_test;/home/r12943092/OPENROAD-Syn-TM/src/mpl2/test/CMakeLists.txt;8;or_integration_test;/home/r12943092/OPENROAD-Syn-TM/src/mpl2/test/CMakeLists.txt;0;")
subdirs("cpp")
