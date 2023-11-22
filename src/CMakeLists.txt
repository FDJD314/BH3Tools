include(CTest)
enable_testing()

# include_directories(${CMAKE_SOURCE_DIR}/inc/BH3Tool ${CMAKE_SOURCE_DIR}/inc/keyBoardHook ${CMAKE_SOURCE_DIR}/inc/PiontTool)

set(HEADER_LIST_1 ${CMAKE_SOURCE_DIR}/inc/BH3Tool)
set(HEADER_LIST_2 ${CMAKE_SOURCE_DIR}/inc/keyBoardHook)
set(HEADER_LIST_3 ${CMAKE_SOURCE_DIR}/inc/PiontTool)

add_library(driver SHARED Driver_FDJD.c ${HEADER_LIST_1})
add_library(taskgenerate SHARED TaskGenerate_FDJD.c ${HEADER_LIST_1})
add_executable(BH3Tool Control_FDJD.c)

target_include_directories(driver PUBLIC ${CMAKE_SOURCE_DIR}/inc/BH3Tool)

target_link_libraries(taskgenerate driver)
target_link_libraries(BH3Tool driver taskgenerate)

add_executable(keyBoardHook keyBoardHook.c ${HEADER_LIST_2})

target_include_directories(keyBoardHook PUBLIC ${CMAKE_SOURCE_DIR}/inc/keyBoardHook)

add_executable(PiontTool Find_CheckPiont.c ${HEADER_LIST_3})

target_include_directories(PiontTool PUBLIC ${CMAKE_SOURCE_DIR}/inc/PiontTool ${CMAKE_SOURCE_DIR}/inc/BH3Tool)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
