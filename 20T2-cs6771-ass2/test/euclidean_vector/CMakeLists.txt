cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET constructor_test
   FILENAME "constructor_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET operation_test
   FILENAME "operation_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET member_function_test
   FILENAME "member_function_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET friends_test
   FILENAME "friends_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET utility_test
   FILENAME "utility_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)
