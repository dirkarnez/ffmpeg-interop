cmake_minimum_required(VERSION 3.0)
project(app)

set(exename "ffmpeg-interop")

# ffmpeg lib
# find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
# find_library(AVCODEC_LIBRARY avcodec)

# find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
# find_library(AVFORMAT_LIBRARY avformat)

# find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
# find_library(AVUTIL_LIBRARY avutil)

# find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
# find_library(AVDEVICE_LIBRARY avdevice)

# find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
# find_library(SWSCALE_LIBRARY swscale)

include_directories(../ffmpeg/include)
link_directories(../ffmpeg/lib)
link_libraries(avcodec)
link_libraries(avdevice)
link_libraries(avfilter)
link_libraries(avformat)
link_libraries(avutil)
link_libraries(postproc)
link_libraries(swresample)
link_libraries(swscale)

add_executable(${exename}
    "src/main.cpp"
 )

# target_include_directories(${exename} PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})
target_link_libraries(${exename} PRIVATE ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SWSCALE_LIBRARY})