/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_HASH_TABLE_HPP
#  define OCTINC_HASH_TABLE_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
typedef unsigned char u_char;

template <typename __Key, typename __Value>
class hash_table {
   public:
    enum {
        MaxKey = 10000000,
        /*
        The maximum value of keys.
        */
        MaxCharLength = 128
        /*
        The maximum length of strings.
        */
    };

    hash_table();
    hash_table(hash_table<__Key, __Value> &src);
    ~hash_table();

    inline size_t count()
    /*
    The number of elements actually filled in hash_table.
    */
    {
        return m_val_count;
    }

    inline size_t size() { return m_tb_size; };

    inline bool empty() { return size() == 0; }

    const __Value operator[](__Key key)
    /*
    Query the corresponding value of the key.
    */
    {
        long index;
        if ((index = __hash_keyindex_by_key(key, hash_keyindex(key))) > -1)
            return m_table[index];
        else
            return *m_empty;
    }

    const __Value value(__Key key)
    /*
    Query the key whose corresponding value is the key.
    */
    {
        long index;
        if ((index = __hash_keyindex_by_key(key, hash_keyindex(key))) > -1)
            return m_table[index];
        else
            return *m_empty;
    }

    bool contain(__Key key)
    /*
    Query whether the table contains the key.
    */
    {
        return __hash_keyindex_by_key(key, hash_keyindex(key)) > -1;
    }

    __Key key(__Value val);

    void insert(__Key key, __Value val);
    /*
    Insert the key and the corresponding value.
    */

    void insert(std::pair<__Key, __Value> &key_and_val);

    void remove(__Key key);
    /*
    Delete the key and the corresponding value.
    */

    hash_table<__Key, __Value> operator=(hash_table<__Key, __Value> &src);

    void clear();

    struct iterator;

    iterator begin() {
        int i = 0;
        for (; i < m_tb_size && !m_is_setted[i]; ++i)
            ;
        return iterator(this, i);
    }

    iterator end() { return iterator(this, m_tb_size); }

   private:
    __Value *m_table;
    __Key *m_keys;
    bool *m_is_setted;
    long m_tb_size;
    long m_val_count;
    __Value *m_oldtable;
    __Key *m_oldkeys;
    bool *m_old_issetted;
    long m_old_tbsize;
    __Value *m_empty;

    long hash_keyindex(int8_t);
    long hash_keyindex(int16_t);
    long hash_keyindex(int32_t);
    long hash_keyindex(int64_t);
    long hash_keyindex(uint8_t);
    long hash_keyindex(uint16_t);
    long hash_keyindex(uint32_t);
    long hash_keyindex(uint64_t);
    long hash_keyindex(float);
    long hash_keyindex(double);
    long hash_keyindex(std::string &);
    long __hash_keyindex(unsigned char *first, size_t size);
    long __hash_null_keyindex(__Key &key, long key_index);
    long __hash_keyindex_by_key(__Key &key, long key_index);
    void reallocate(long size);
    void need_rehash();
    void rehash();

    void put_error(std::string str) {
        std::cerr << "HASH_TABLE:" << str << std::endl;
    }
};

template <typename __Key, typename __Value>
hash_table<__Key, __Value>::hash_table()
    : m_keys(0),
      m_table(0),
      m_is_setted(0),
      m_tb_size(0),
      m_val_count(0),
      m_empty(0) {
    reallocate(10);
}

template <typename __Key, typename __Value>
hash_table<__Key, __Value>::hash_table(hash_table<__Key, __Value> &src)
    : m_keys(0),
      m_table(0),
      m_is_setted(0),
      m_tb_size(0),
      m_val_count(0),
      m_empty(0) {
    reallocate(src.m_tb_size);
    for (int i = 0; i < src.m_tb_size; ++i) {
        if (src.m_is_setted[i]) {
            insert(src.m_keys[i], src.m_table[i]);
        }
    }
}

template <typename __Key, typename __Value>
hash_table<__Key, __Value>::~hash_table() {
    clear();
}

template <typename __Key, typename __Value>
hash_table<__Key, __Value> hash_table<__Key, __Value>::operator=(
    hash_table<__Key, __Value> &src) {
    clear();

    reallocate(src.m_tb_size);
    for (int i = 0; i < src.m_tb_size; ++i) {
        if (src.m_is_setted[i]) {
            insert(src.m_keys[i], src.m_table[i]);
        }
    }

    return *this;
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::clear() {
    if (m_keys) delete[] m_keys;
    if (m_table) delete[] m_table;
    if (m_is_setted) delete[] m_is_setted;
    if (m_empty) delete m_empty;
    m_keys = 0;
    m_table = 0;
    m_is_setted = 0;
    m_empty = 0;
    m_tb_size = 0;
    m_val_count = 0;
    reallocate(10);
}

template <typename __Key, typename __Value>
__Key hash_table<__Key, __Value>::key(__Value val) {
    for (int i = 0; i < m_tb_size; ++i) {
        if (m_is_setted[i]) {
            if (val == m_table[i]) {
                return m_keys[i];
            }
        }
    }
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(int8_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(int8_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(int16_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(int16_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(int32_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(int32_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(int64_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(int64_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(uint8_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(uint8_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(uint16_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(uint16_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(uint32_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(uint32_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(uint64_t key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(uint64_t));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(float key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(float));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(double key) {
    u_char *p = reinterpret_cast<u_char *>(&key);
    return __hash_keyindex(p, sizeof(double));
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::hash_keyindex(std::string &key) {
    char tmp_data[MaxCharLength] = {0};
    ::memcpy(tmp_data, key.data(), key.size());
    return __hash_keyindex((u_char *)tmp_data, key.size());
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::__hash_keyindex(u_char *first, size_t size) {
    unsigned long result = 0;
    while (size--) {
        result = *first++ ^ result * 0x1579532586542ul;
        result *= 0x35715ul;
    }
    return result % m_tb_size;
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::__hash_null_keyindex(__Key &key,
                                                      long key_index) {
    while (1) {
        key_index = (key_index + 1) % m_tb_size;
        if (!m_is_setted[key_index] || m_keys[key_index] == key) {
            return key_index;
        }
    }
}

template <typename __Key, typename __Value>
long hash_table<__Key, __Value>::__hash_keyindex_by_key(__Key &key,
                                                        long key_index) {
    long end = key_index;
    do {
        key_index = (key_index + 1) % m_tb_size;
        if (m_is_setted[key_index] && m_keys[key_index] == key) {
            return key_index;
        } else if (!m_is_setted[key_index]) {
            break;
        }
    } while (key_index != end);
    return -1;
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::reallocate(long size) {
    if (size > MaxKey) {
        put_error("too many key");
        return;
    }

    if (m_empty == 0) {
        m_empty = new __Value;
    }

    m_oldkeys = m_keys;
    m_oldtable = m_table;
    m_old_issetted = m_is_setted;
    m_old_tbsize = m_tb_size;

    try {
        m_keys = new __Key[size];
        m_table = new __Value[size];
        m_is_setted = new bool[size];
        m_val_count = 0;

        if (!m_keys || !m_table || !m_is_setted) {
            throw "memory allocation error";
        }

        ::memset(m_is_setted, 0, size * sizeof(bool));
        m_tb_size = size;
    } catch (std::string err) {
        put_error(err);
    }

    rehash();
    delete[] m_oldkeys;
    delete[] m_oldtable;
    delete m_old_issetted;
    m_old_tbsize = 0;
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::need_rehash() {
    if (m_tb_size >= MaxKey) {
        return;
    }

    double ratio = (double)m_val_count / m_tb_size;
    if (ratio >= 0.7) {
        long size = m_tb_size * 2;
        size = size >= MaxKey ? MaxKey : size;
        reallocate(size);
    }
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::rehash() {
    for (int i = 0; i < m_old_tbsize; ++i) {
        if (m_old_issetted[i]) {
            insert(m_oldkeys[i], m_oldtable[i]);
        }
    }
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::insert(__Key key, __Value val) {
    if (m_val_count >= MaxKey) {
        put_error("too many key");
        return;
    }
    need_rehash();

    long new_key_index = __hash_null_keyindex(key, hash_keyindex(key));
    if (m_keys[new_key_index] == key && m_is_setted[new_key_index]) {
        m_table[new_key_index] = val;
    } else {
        m_table[new_key_index] = val;
        m_keys[new_key_index] = key;
        m_is_setted[new_key_index] = true;
        ++m_val_count;
    }
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::insert(
    std::pair<__Key, __Value> &key_and_val) {
    insert(key_and_val.first, key_and_val.second);
}

template <typename __Key, typename __Value>
void hash_table<__Key, __Value>::remove(__Key key) {
    long key_index = __hash_keyindex_by_key(key, hash_keyindex(key));
    if (key_index != -1) {
        m_is_setted[key_index] = false;
        --m_val_count;
    }
}

template <typename __Key, typename __Value>
struct hash_table<__Key, __Value>::iterator
    : public std::iterator<std::input_iterator_tag,
                           std::pair<__Key, __Value> > {
    hash_table<__Key, __Value> *phash;
    long hash_index;
    std::pair<__Key, __Value> hash_data;

   public:
    iterator() : phash(0), hash_index(0) {}
    iterator(const iterator &other) { *this = other; }
    iterator(hash_table<__Key, __Value> *p, long i) : phash(p), hash_index(i) {}
    const iterator operator++() {
        int i;
        for (i = hash_index + 1; i < phash->m_tb_size; ++i) {
            if (phash->m_is_setted[i]) {
                break;
            }
        }
        hash_index = i;
        return *this;
    }
    const iterator operator++(int) {
        iterator old(*this);
        int i;
        for (i = hash_index + 1; i < phash->m_tb_size; ++i) {
            if (phash->m_is_setted[i]) {
                break;
            }
        }
        hash_index = i;
        return old;
    }

    bool operator==(iterator iter) {
        return (iter.phash == phash) && (iter.hash_index == hash_index);
    }

    bool operator!=(iterator iter) {
        return (iter.phash != phash) || (hash_index != iter.hash_index);
    }
    const std::pair<__Key, __Value> operator*() {
        return std::pair<__Key &, __Value &>(phash->m_keys[hash_index],
                                             phash->m_table[hash_index]);
    }

    const std::pair<__Key, __Value> *operator->() {
        hash_data.first = phash->m_keys[hash_index];
        hash_data.second = phash->m_table[hash_index];
        return &hash_data;
    }
};
}  // namespace Octinc

#endif
