#include "comp6771/word_ladder.hpp"
#include <absl/container/flat_hash_map.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

// Write your implementation here
void dfs (std::string const& from,
					std::string const& to,
                    absl::flat_hash_set<std::string> const& lexicon,
                    absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>>& map,
                    std::vector<std::vector<std::string>>& result,
                    std::vector<std::string> path,
                    unsigned long const& path_len);

namespace word_ladder {

    auto generate(std::string const& from,
	                std::string const& to,
	                absl::flat_hash_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>>
	{
	    absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>> map;
	    auto result = std::vector<std::vector<std::string>>();
        auto path = std::vector<std::string>();
        path.push_back(from);
        unsigned long shortest_path_len = 0;

        auto q_start = std::queue<std::string>();
        q_start.push(from);

        auto q_end = std::queue<std::string>();
        q_end.push(to);

        auto visited_start = absl::flat_hash_set<std::string>();
        auto visited_end = absl::flat_hash_set<std::string>();

        unsigned long path_len_start = 0;
        unsigned long path_len_end = 0;

		// Bidirectional BFS
		auto flag = 0;
        while (!q_start.empty() && !q_end.empty()) {
            // get the size of the queue
            auto queue_size_start = q_start.size();
            auto queue_size_end = q_end.size();

            // BFS from the start word
            for (unsigned long iter = 0; iter < queue_size_start; ++ iter){
                // pop
	            auto current_word = q_start.front();
				q_start.pop();
                visited_start.insert(current_word);
				// for loop the word to swap every letter from 'a' to 'z'
				for (unsigned long letter = 0; letter < current_word.length(); ++ letter){
					auto next_word = current_word;
					for (char swapped_letter = 'a'; swapped_letter <= 'z'; ++ swapped_letter){
						next_word[letter] = swapped_letter;
						if(next_word == current_word){
							continue;
						}
						if (!visited_end.contains(next_word)){
							if (!visited_start.contains(next_word) && lexicon.contains(next_word)){
								q_start.push(next_word);

								// insert the next word into map[current_word]
								if (!map.contains(current_word)){
									absl::flat_hash_set<std::string> temp;
									temp.insert(next_word);
									map.insert({current_word, temp});
								} else{
									map[current_word].insert(next_word);
								}
                            }

                        } else{
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1){
                        break;
                    }
                }
                if (flag == 1){
                    break;
                }
            }
            path_len_start ++;

            if (flag == 1){
                break;
            }

            // BFS from the end word
            for (unsigned long iter = 0; iter < queue_size_end; ++ iter){
                // pop
                auto current_word = q_end.front();
                q_end.pop();
                visited_end.insert(current_word);
                // for loop the word to swap every letter from 'a' to 'z'
                for (unsigned long letter = 0; letter < current_word.length(); ++ letter){
                    auto prev_word = current_word;
                    for (char swapped_letter = 'a'; swapped_letter <= 'z'; ++ swapped_letter){
                        prev_word[letter] = swapped_letter;
                        if(prev_word == current_word){
                            continue;
                        }
                        if (!visited_start.contains(prev_word)){
                            if (!visited_end.contains(prev_word) && lexicon.contains(prev_word)){
                                q_end.push(prev_word);

                                // insert the current word into map[prev_word]
                                if (!map.contains(prev_word)){
                                    absl::flat_hash_set<std::string> temp;
                                    temp.insert(current_word);
                                    map.insert({prev_word, temp});
                                } else{
                                    map[prev_word].insert(current_word);
                                }
                            }
                        } else{
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1){
                        break;
                    }
                }
                if (flag == 1){
                    break;
                }
            }
            path_len_end ++;

            if (flag == 1){
                break;
            }
        }

        // print an error message if no path is found
        if (flag == 0){
            std::cout << "No path is found between " << from << " and " << to << std::endl;
            return result;
        }

        shortest_path_len = path_len_start + path_len_end - 1;

        // search for all the shortest paths by DFS
        dfs(from, to, lexicon, map, result, path, shortest_path_len);

        // sort the ladders in alphabetical order
        std::sort(result.begin(), result.end());

        return result;
    }
} // namespace word_ladder

void dfs (std::string const& from,
					std::string const& to,
                    absl::flat_hash_set<std::string> const& lexicon,
                    absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>>& map,
                    std::vector<std::vector<std::string>>& result,
                    std::vector<std::string> path,
                    unsigned long const& path_len)
	{

        if (path.size() != path_len + 1 ){
            for (auto const& next_word: map[from]){
                auto next_path = path;
                next_path.push_back(next_word);
                dfs(next_word, to, lexicon, map, result, next_path, path_len);
            }
        } else{
            if (path.back() == to){
                result.push_back(path);
            }
        }
    }
