set(SRC_FILES
    main.cpp
    models/booking.cpp
    models/service.cpp
)

set(HEADER_DIR
    "includes/"
)

include_directories(${HEADER_DIR})

add_executable(${EXECUTABLE_NAME} ${SRC_FILES})
target_include_directories(${EXECUTABLE_NAME}
                           PRIVATE 
                           ${CROW_INCLUDE_DIR}
                           ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${EXECUTABLE_NAME}
    ${CROW_LIBRARY}
    ${Boost_LIBRARIES}
    mysqlclient
    pthread
)


install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)