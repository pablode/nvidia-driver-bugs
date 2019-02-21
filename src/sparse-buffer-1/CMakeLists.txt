set(SPARSE_BUFFER_1_SRCS
  main.c
)

add_executable(
  sparse-buffer-1
  ${SPARSE_BUFFER_1_SRCS}
)

target_link_libraries(
  sparse-buffer-1
  PRIVATE
  glad
  glfw
)
