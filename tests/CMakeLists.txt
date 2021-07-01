include(${CMAKE_SOURCE_DIR}/cmake/AddGoogleTest.cmake)

set(TESTLIB_NAME "testlib")
set(TEST_HEADER)
set(TEST_SOURCE)
file(GLOB_RECURSE TEST_HEADER ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE TEST_SOURCE ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(${TESTLIB_NAME} SHARED ${TEST_HEADER} ${TEST_SOURCE})
target_include_directories(${TESTLIB_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        )
target_link_libraries(${TESTLIB_NAME} cocos2d project_dependency)
target_include_directories(${TESTLIB_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
        )
target_compile_features(${TESTLIB_NAME} PRIVATE cxx_std_17)
target_compile_options(${TESTLIB_NAME} PRIVATE -Wall)

function(INIT_TEST ARG_TARGET)
    set(GAME_RES_FOLDER
            "${CMAKE_SOURCE_DIR}/Resources"
            )
    if (APPLE OR WINDOWS)
        cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
    endif ()
    add_executable(${ARG_TARGET} ${ARGN} ${common_res_files})
    target_compile_features(${ARG_TARGET} PRIVATE cxx_std_17)
    target_compile_options(${ARG_TARGET} PRIVATE -Wall)
    add_gtest(${ARG_TARGET})
    add_deps_to_test(${ARG_TARGET} ${TESTLIB_NAME})

    # mark app resources
    setup_cocos_app_config(${ARG_TARGET})
    if (APPLE)
        set_target_properties(${ARG_TARGET} PROPERTIES RESOURCE "${APP_UI_RES}")
        if (MACOSX)
            set_xcode_property(${ARG_TARGET} INFOPLIST_FILE "${CMAKE_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
        endif ()
    elseif (WINDOWS)
        cocos_copy_target_dll(${ARG_TARGET})
    endif ()

    if (LINUX OR WINDOWS)
        cocos_get_resource_path(APP_RES_DIR ${ARG_TARGET})
        cocos_copy_target_res(${ARG_TARGET} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
    endif ()
endfunction()

function(ADD_DEPS_TO_TEST ARG_TARGET)
    target_link_libraries(
            ${ARG_TARGET} PUBLIC ${ARGN})
endfunction()

################### All tests ###################
init_test(TestIntegration ./TestIntegration.cpp)

