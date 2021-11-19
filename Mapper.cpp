#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <utility>

template<typename Key, typename Value>
class Pair{
    public:
    Key key_{};
    Value value_{};
};

template<typename Key, typename Value>
class Map{
    Pair<Key, Value> *map_ = nullptr;
    int size_ = 0, capacity_ = 0;
    
    void adjust_capacity(){
        if (size_ == capacity_){
            capacity_ = capacity_ ? capacity_ * 2 : 1;
            auto map_copy = new Pair<Key, Value>[capacity_]{};
            std::copy(map_, map_ + size_, map_copy);
            std::swap(map_, map_copy);
            delete [] map_copy;
        }
    }

    auto key_pos(Key key) const{
        return std::find_if(map_, map_ + size_, [&](auto const &p) {
                return p.key_ == key;
            });
    }   

    public:

    Map() = default;

    Map(std::initializer_list<Pair<Key, Value>> il){
        for(auto p : il)
            insert(p);
    }

    void insert(Pair<Key, Value> const & p_add){
        if (auto f = key_pos(p_add.key_); f == map_ + size_){
            adjust_capacity();
            map_[size_++] = p_add; 
        }    
    }

    int size() const{
        return size_;
    }

    Value& operator[](Key key)
    {
        if (auto f = key_pos(key); f != map_ + size_){
            return f->value_;
        }
        else{
            insert({key, {}});
            return key_pos(key)->value_;
        }
    }

    void erase(Key key){
        if (auto f = key_pos(key); f != map_ + size_){
            std::rotate(f, f + 1, map_ + size_);
            size_--;
        }
    }

    Map(Map const &m){
        for(int i = 0; i < m.size_; ++i)
            insert(m.map_[i]);
    }

    Map& operator=(Map m){
        std::swap(size_, m.size_);
        std::swap(capacity_, m.capacity_);
        std::swap(map_, m.map_);

        return *this;
    }

    ~Map(){
        delete [] map_;
    }
};

