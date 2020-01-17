/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_ILIBRARY__
#define OCTINC_ILIBRARY__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

/*
Junior Libraries
*/

#include "Junior/auto_ptr.hpp"
#include "Junior/bitblock.hpp"
#include "Junior/bitstream.hpp"
#include "Junior/chtholly_tree.hpp" 
#include "Junior/complex.hpp" 
#include "Junior/dancing_links.hpp" 
#include "Junior/FFT.hpp"
#include "Junior/game_theory.hpp"
#include "Junior/group.hpp" 
#include "Junior/heap.hpp" 
#include "Junior/huffman_tree.hpp" 
#include "Junior/leftist_tree.hpp" 
#include "Junior/linear_algebra.hpp"
#include "Junior/linear_base.hpp"
#include "Junior/linear_random.hpp"
#include "Junior/link_cut_tree.hpp" 
#include "Junior/memory_pool.hpp"
#include "Junior/pairing_heap.hpp"
#include "Junior/partition.hpp"
#include "Junior/peak.hpp" 
#include "Junior/persistence_tree.hpp"
#include "Junior/pointer_vector.hpp"
#include "Junior/random_access_set.hpp" 
#include "Junior/RMQ.hpp" 
#include "Junior/scapegoat_tree.hpp"
#include "Junior/sequence.hpp"
#include "Junior/skip_list.hpp"
#include "Junior/sorted_queue.hpp"
#include "Junior/splay_tree.hpp"
#include "Junior/statistics.hpp" 
#include "Junior/thread_pool.hpp"
#include "Junior/treap.hpp"
#include "Junior/Trie.hpp"
#include "Junior/tuple.hpp" 

#ifndef __EXPORT_SENIOR // This macro is used to cancel the bulk import of the Senior library.

/*
Senior Libraries
*/

#include "Senior/arithmetic_coding.hpp" 
#include "Senior/B_star.hpp"
#include "Senior/BigInteger.hpp"
#include "Senior/BigDecimal.hpp"
#include "Senior/ex_string.hpp"
#include "Senior/fibonacci_heap.hpp"
#include "Senior/geometry.hpp"
#include "Senior/hash_table.hpp" 
#include "Senior/mathmetic.hpp"
#include "Senior/polynomial.hpp"
#include "Senior/red_black_tree.hpp"
#include "Senior/regexp.hpp"

#endif

#ifndef __EXPORT_TOOLS // This macro is used to cancel the bulk import of the Tools library.

/*
Tools Libraries
*/

#include "Tools/Checker.hpp"
#include "Tools/chrono_meter.hpp"
#include "Tools/File.hpp"
#include "Tools/log.hpp"
#include "Tools/optimize.hpp"
#include "Tools/RGBA.hpp"
#include "Tools/Timer.hpp"

#endif

#endif 
