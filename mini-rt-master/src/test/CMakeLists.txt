add_executable(minirt-test minirt_test.cpp)

target_link_libraries(minirt-test PUBLIC minirt)

#install(TARGETS minirt-test DESTINATION bin/minirt)
