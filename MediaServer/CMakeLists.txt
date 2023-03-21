set(CMAKE_CXX_STANDARD 17)
set(DEST_BIN "PEMediaServer")

# 暂时保留
add_definitions(-DFMT_HEADER_ONLY)

# 添加编译选项
add_definitions(-Wall -g -Wpsabi -rdynamic -fPIC)

# 公共头文件路径
include_directories(../Common/include)
include_directories(../Common/include/glog)
include_directories(../Common/include/libevent)

# 本地工程头文件路径
include_directories(./Application/include)
include_directories(./glog/include)

# 公共链接库路径
link_directories(../Common/lib/glog)
link_directories(../Common/lib/libevent)

# 设置子目录名
set(module
        Application
        glog)

#编译子目录
foreach (module_name ${module})
    add_subdirectory(${module_name})
endforeach ()

# 打印子目录名称
message(STATUS "Add sub dirs as follows: ")
foreach (dir ${module})
    message(STATUS "Sub dir = '${dir}'")
endforeach ()
message(STATUS "\n")

add_executable(
        ${DEST_BIN}
        $<TARGET_OBJECTS:Application>
        $<TARGET_OBJECTS:glog>
)

target_link_libraries(
        ${DEST_BIN}
        event
        libglog.a
)