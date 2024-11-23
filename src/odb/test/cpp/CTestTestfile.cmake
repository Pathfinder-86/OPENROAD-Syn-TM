# CMake generated Testfile for 
# Source directory: /home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp
# Build directory: /home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/OdbGTests[1]_include.cmake")
add_test(odb.TestCallBacks "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestCallBacks")
set_tests_properties(odb.TestCallBacks PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;60;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestGeom "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestGeom")
set_tests_properties(odb.TestGeom PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;61;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestModule "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestModule")
set_tests_properties(odb.TestModule PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;62;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestGroup "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestGroup")
set_tests_properties(odb.TestGroup PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;63;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestGCellGrid "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestGCellGrid")
set_tests_properties(odb.TestGCellGrid PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;64;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestGuide "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestGuide")
set_tests_properties(odb.TestGuide PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;65;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestNetTrack "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestNetTrack")
set_tests_properties(odb.TestNetTrack PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;66;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
add_test(odb.TestMaster "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/TestMaster")
set_tests_properties(odb.TestMaster PROPERTIES  _BACKTRACE_TRIPLES "/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;67;add_test;/home/r12943092/OPENROAD-Syn-TM/src/odb/test/cpp/CMakeLists.txt;0;")
subdirs("helper")
subdirs("scan")
