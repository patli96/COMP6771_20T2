//
//  Copyright UNSW Sydney School of Computer Science and Engineering
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file contains some test cases I created.
//
#include "comp6771/word_ladder.hpp"

#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "comp6771/testing/range/contain.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/is_sorted.hpp"
#include "range/v3/range/primitives.hpp"


TEST_CASE("work -> play") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate("work", "play", english_lexicon);

	CHECK(ranges::size(ladders) == 12);
	CHECK(ranges::is_sorted(ladders));

	CHECK(ranges::any_of(ladders, testing::contain({"work", "fork", "form", "foam", "flam", "flay", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "pork", "perk", "peak", "pean", "plan", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "pork", "perk", "peak", "peat", "plat", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "pork", "perk", "pert", "peat", "plat", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "pork", "porn", "pirn", "pian", "plan", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "pork", "port", "pert", "peat", "plat", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "word", "wood", "pood", "plod", "ploy", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "worm", "form", "foam", "flam", "flay", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "worn", "porn", "pirn", "pian", "plan", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "wort", "bort", "boat", "blat", "plat", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "wort", "port", "pert", "peat", "plat", "play"})));
	CHECK(ranges::any_of(ladders, testing::contain({"work", "wort", "wert", "pert", "peat", "plat", "play"})));
}

TEST_CASE("awake -> sleep") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate("awake", "sleep", english_lexicon);

	CHECK(ranges::size(ladders) == 2);
	CHECK(ranges::is_sorted(ladders));

	CHECK(ranges::any_of(ladders, testing::contain({"awake","aware","sware","share","sharn","shawn","shewn","sheen","sheep","sleep"})));
	CHECK(ranges::any_of(ladders, testing::contain({"awake","aware","sware","share","shire","shirr","shier","sheer","sheep","sleep"})));
}

TEST_CASE("at -> it") {
	auto const english_lexicon = word_ladder::read_lexicon("./word_list1.txt");
	auto const ladders = word_ladder::generate("at", "it", english_lexicon);

	CHECK(ranges::size(ladders) == 1);
	CHECK(ranges::is_sorted(ladders));

	CHECK(ranges::any_of(ladders, testing::contain({"at", "it"})));
}

TEST_CASE("apple -> appaa") {
	auto const english_lexicon = word_ladder::read_lexicon("./word_list2.txt");
	auto const ladders = word_ladder::generate("apple", "appaa", english_lexicon);

	CHECK(ranges::size(ladders) == 0);
}