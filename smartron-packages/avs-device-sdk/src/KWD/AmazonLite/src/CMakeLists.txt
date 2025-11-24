add_definitions("-DACSDK_LOG_MODULE=pryonLiteKeywordDetector")

if(AMAZONLITE_KEY_WORD_DETECTOR_EMBEDDED_MODEL_CPP_PATH)
	add_library(AMAZONLITE SHARED
	PryonLiteKeywordDetector.cpp "${AMAZONLITE_KEY_WORD_DETECTOR_EMBEDDED_MODEL_CPP_PATH}")
else()
	add_library(AMAZONLITE SHARED
		PryonLiteKeywordDetector.cpp)
endif()

target_include_directories(AMAZONLITE PUBLIC
	"${AMAZONLITE_KEY_WORD_DETECTOR_INCLUDE_DIR}"
	"${KWD_SOURCE_DIR}/include" 
	"${AMAZONLITE_SOURCE_DIR}/include")

target_link_libraries(AMAZONLITE KWD AVSCommon "${AMAZONLITE_KEY_WORD_DETECTOR_LIB_PATH}")

# install target
asdk_install()