cmake_minimum_required(VERSION 3.17)
project(videochat)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/deps/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")
include_directories(src)
add_executable(videochat src/App.cpp src/AppComponent.hpp src/controller/RoomController.hpp src/dto/OfferDto.hpp src/db/DbClient.hpp src/service/RoomService.hpp)
find_package(oatpp CONFIG REQUIRED)
find_package(oatpp-sqlite CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(oatpp-swagger CONFIG REQUIRED)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/sql/database.sqlite"
        ## SQLite database test file
        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)
target_link_libraries(videochat PRIVATE oatpp::oatpp oatpp::oatpp-test unofficial::sqlite3::sqlite3 oatpp::oatpp-sqlite oatpp::oatpp-swagger)