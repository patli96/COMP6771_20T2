cxx_test(
   TARGET graph_test1
   FILENAME "graph_test1.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET constructors_test
   FILENAME "constructors_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET accessors_test
   FILENAME "accessors_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET modifiers_test
   FILENAME "modifiers_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET iterator_test
   FILENAME "iterator_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET comparisons_test
   FILENAME "comparisons_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)

cxx_test(
   TARGET extractor_test
   FILENAME "extractor_test.cpp"
   LINK absl::flat_hash_set absl::flat_hash_map gsl::gsl-lite-v1 fmt::fmt-header-only range-v3
)