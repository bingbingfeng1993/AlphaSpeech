cmake_minimum_required(VERSION 2.4.6)
project(ASR)

# Just to avoid the warning
if (COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif (COMMAND cmake_policy)

set(CMAKE_BUILD_TYPE "Release")

# Add base directory for includes (global)
include_directories(../../src)

# OpenMP settings
FIND_PACKAGE(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()


set(SRC
	../../src/layer/batchnorm.c
	../../src/layer/conv1d.c
	../../src/layer/conv2d.c
	../../src/layer/depthwise_conv1d.c
	../../src/layer/glu.c
	../../src/layer/gru.c
	../../src/layer/layernorm.c
	../../src/layer/linear.c
	../../src/layer/lstm.c
	../../src/layer/parameter.c
	../../src/layer/prelu.c
	../../src/layer/relu.c
	../../src/layer/sigmoid.c
	../../src/layer/silu.c
	../../src/layer/softmax.c
	../../src/layer/tanh.c
	../../src/core/crop.c
	../../src/core/mat.c
	../../src/core/packing.c
	../../src/core/padding.c
	../../src/core/permute.c
	../../src/utils/ctc_endpoint.c
	../../src/utils/ctc_prefix_beam_search.c
	../../src/utils/ctc_prefix_beam_search_wrt_timestamp.c
	../../src/utils/fbank.c
	../../src/utils/fft.c
	../../src/utils/inverse_text_normalization.c
	../../src/utils/post_processor.c
	../../src/decoder/asr_decoder.c
)

add_library(asr SHARED ${SRC})

target_link_libraries(asr m)

add_executable(asr_api_test ../../test/asr_api_test.c)

target_link_libraries(asr_api_test asr)

add_executable(ctc_endpoint_test ../../test/ctc_endpoint_test.c)

target_link_libraries(ctc_endpoint_test asr)

add_executable(ctc_prefix_beam_search_test ../../test/ctc_prefix_beam_search_test.c)

target_link_libraries(ctc_prefix_beam_search_test asr)

add_executable(ctc_prefix_beam_search_wrt_timestamp_test ../../test/ctc_prefix_beam_search_wrt_timestamp_test.c)

target_link_libraries(ctc_prefix_beam_search_wrt_timestamp_test asr)

add_executable(fbank_test ../../test/fbank_test.c)

target_link_libraries(fbank_test asr)

add_executable(inverse_text_normalization_test ../../test/inverse_text_normalization_test.c)

target_link_libraries(inverse_text_normalization_test asr)

add_executable(post_processor_test ../../test/post_processor_test.c)

target_link_libraries(post_processor_test asr)

