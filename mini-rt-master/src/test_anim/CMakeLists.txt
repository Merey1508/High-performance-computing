add_executable(minirt-test-anim minirt_test_anim.cpp)

target_link_libraries(minirt-test-anim PUBLIC minirt)

#install(TARGETS minirt-test-anim DESTINATION bin/minirt)
