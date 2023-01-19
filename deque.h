#pragma once
#include <iterator>
#include <limits>
#include <memory>

namespace lab {
    template <typename T>
    class Allocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        Allocator() noexcept {}

        Allocator(const Allocator& other) noexcept {}

        template <class U>
        Allocator(const Allocator<U>& other) noexcept {}

        ~Allocator() {}

        pointer allocate(size_type n) {
            return static_cast<pointer>(::operator new (sizeof(T) *n));
        }

        void deallocate(pointer p, size_type n) noexcept {
            delete[] p;
        }

        template<typename Other>
        struct rebind {
            typedef Allocator<Other> other;
        };

        //[[nodiscard]] std::allocation_result<T*> allocate_at_least(
        // std::size_t n ); // TODO For extra points
    };

    template <typename ValueType, typename Reference, typename Pointer>
    class Deque_iterator  {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = Pointer;
        using reference = Reference;
        using chunk_ptr = std::__ptr_rebind<pointer, pointer>;
        using iter_type = Deque_iterator<value_type, reference, pointer>;

        const static std::size_t CHUNK_SIZE = 512 / sizeof(ValueType) == 0 ? 1 : 512 / sizeof(ValueType);
        pointer _el, _first, _last;
        chunk_ptr _chunkID; //chunk - указатель на ячейку в главном массиве

    private:
        void switchForwardChunk(difference_type n = 1) {
            if (n == 0)
                return;
            _chunkID = _chunkID + n;
            _first = *_chunkID;
            _el = _first;
            _last = _first + CHUNK_SIZE;
        }

        void switchBackChunk(difference_type n = 1) {
            if (n == 0)
                return;
            _chunkID = _chunkID - n;
            _first = *_chunkID;
            _last = _first + CHUNK_SIZE;
            _el = _last;
        }

    public:
        Deque_iterator() noexcept;

        Deque_iterator(const Deque_iterator& other) noexcept = default;

        Deque_iterator& operator=(const Deque_iterator& other) = default;

        ~Deque_iterator();

        friend void swap(Deque_iterator<value_type, reference, pointer>&, Deque_iterator<value_type, reference, pointer>&);

        friend bool operator==(const iter_type&, const iter_type&);
        friend bool operator!=(const iter_type&, const iter_type&);

        reference operator*() const {
            return *_el;
        }

        pointer operator->() const {
            return _el;
        }

        Deque_iterator& operator++() {
            if (_el == _last)
                switchForwardChunk();
            else
                _el++;
            return *this;
        }

        Deque_iterator operator++(int) {
            iter_type copy = *this;
            ++(*this);
            return copy;
        }

        Deque_iterator& operator--() {
            if (_el == _first)
                switchBackChunk();
            else
                _el--;
            return *this;
        }

        Deque_iterator operator--(int) {
            iter_type copy = *this;
            --(*this);
            return copy;
        }

        Deque_iterator operator+(const difference_type& n) const { // + 1
            iter_type r = *this;
            switchForwardChunk( ( (r._el - r._first) + n) / CHUNK_SIZE);
            r._el +=  ( (r._el + r._first) + n ) % CHUNK_SIZE;
            return r;
        }

        Deque_iterator& operator+=(const difference_type& n) {
            return (*this) + n;
        }

        Deque_iterator operator-(const difference_type& n) const {
            iter_type res;
            if (n - (res._el - res._first + 1) <= 0)
                res._el -= n;
            else {
                n -= (res._el - res._first + 1);
                switchBackChunk();
                switchBackChunk(n / CHUNK_SIZE);
                res._el -= (n % CHUNK_SIZE);
            }
            return res;
        }

        Deque_iterator& operator-=(const difference_type& n) {
            *this = *this - n;
            return *this;
        }

        difference_type operator-(const iter_type& r) const {
            if (_chunkID < r._chunkID)
                return -(r - *this);
            if (_chunkID == r._chunkID)
                return _el - r._el;
            difference_type s = (_last - _el) + CHUNK_SIZE * (r._chunkID - _chunkID - 1) + (r._el - r._first);
            return s;

        }

        reference operator[](const difference_type& n) {
            iter_type res = (*this) + n;
            return *res._el;
        }

        friend bool operator<(const iter_type&, const iter_type&);
        friend bool operator<=(const iter_type&, const iter_type&);
        friend bool operator>(const iter_type&, const iter_type&);
        friend bool operator>=(const iter_type&, const iter_type&);
        // operator<=> will be handy
    };

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator==(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return l._chunkID == r._chunkID && l._el == r._el;
    }

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator!=(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return !(l == r);
    }

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator<(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return (l._chunkID < r._chunkID) || (l._chunkID == r._chunkID && l._el < r._el);
    }

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator<=(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return (l._chunkID < r._chunkID) || (l._chunkID == r._chunkID && l._el <= r._el);
    }

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator>(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return (l._chunkID > r._chunkID) || (l._chunkID == r._chunkID && l._el > r._el);
    }

    template <typename ValueType, typename Reference, typename Pointer>
    bool operator>=(const  Deque_iterator<ValueType, Reference, Pointer>& l, const  Deque_iterator<ValueType, Reference, Pointer>& r) {
        return (l._chunkID > r._chunkID) || (l._chunkID == r._chunkID && l._el >= r._el);
    }



        // operator <=> will be handy

        // friend constexpr std::iter_rvalue_reference_t<Iter> iter_move( const
        // std::reverse_iterator& i ); // For extra points

        // template<std::indirectly_swappable<Iter> Iter2>
        // friend constexpr void iter_swap(const reverse_iterator& x, const
        // std::reverse_iterator<Iter2>& y); // For extra points

   //  template <class Iter>
  //  Deque_reverse_iterator<Iter> make_reverse_iterator(Iter i);






    template <typename T, typename Allocator = Allocator<T>>
    class Deque {
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = Deque_iterator<value_type, reference, pointer>;
        using const_iterator = Deque_iterator<value_type, const_reference, const_pointer>;
        using reverse_iterator = std::reverse_iterator<Deque_iterator<iterator, reference, pointer>>;
        using const_reverse_iterator = std::reverse_iterator<Deque_iterator<iterator, const_reference, const_pointer>>;

    private:
        using alloc_traits = std::allocator_traits<allocator_type>;
        using allocator_pointer = std::__alloc_rebind<allocator_type, pointer>;
        using chunk_ptr = std::__ptr_rebind<pointer, pointer>;
        allocator_pointer _alloc_p;
        allocator_type _alloc_t;
        const static size_type CHUNK_SIZE = 512 / sizeof(T) == 0 ? 1 : 512 / sizeof(T);
        chunk_ptr _map;
        std::size_t _map_capacity, _el_size;
        const_iterator _begin, _end;

        void reallocate(std::size_t new_size) {
            if (_map_capacity == 0)
                _map_capacity = 8;
            while (_map_capacity < new_size)
                _map_capacity << 2;
            if (_map_capacity > max_size()) ;
                //throw some error
            chunk_ptr _copy_map = _alloc_p.allocate(_map_capacity);
            std::size_t ind = (_map_capacity - new_size) >> 1;
            for (auto chunkID = _begin._chunkID; chunkID <= _end.chunkID; chunkID++)
                _copy_map[ind++] = chunkID;
            _begin._chunkID = &_map[ind];
            _end._chunkID = &_map[ind + (_end.chunkID - _begin._chunkID)];
            _alloc_p.deallocate(_map);
            _map = _copy_map;
        }

    public:
        /// @brief Default constructor. Constructs an empty container with a
        /// default-constructed allocator.
        Deque() {
            _map = _alloc_p.allocate(8);
            _map_capacity = 8;
            _el_size = 0;
            _map[4] = _alloc_t.allocate(CHUNK_SIZE);
            _begin._el = _end._el = &_map[4][CHUNK_SIZE / 2];
            _begin._first = _end._first = _map[4];
            _begin._last = _end._last = _map[4] + CHUNK_SIZE;
            _begin._chunkID = _end._chunkID = _map[4] + CHUNK_SIZE;
        }

        /// @brief Constructs an empty container with the given allocator
        /// @param alloc allocator to use for all memory allocations of this container
        explicit Deque(const Allocator& alloc) : Deque() {
            _alloc_t = alloc;
            _alloc_p = static_cast<allocator_pointer>(alloc);
        }

        /// @brief Constructs the container with count copies of elements with value
        /// and with the given allocator
        /// @param count the size of the container
        /// @param value the value to initialize elements of the container with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(size_type count, const T& value, const Allocator& alloc = Allocator()) : Deque(alloc) {
            reallocate(count);
            for (auto iter = _begin; iter != _begin + count; iter++)
                *iter = value;
            _end = _begin + count;
            _el_size = count;
        }

        /// @brief Constructs the container with count default-inserted instances of
        /// T. No copies are made.
        /// @param count the size of the container
        /// @param alloc allocator to use for all memory allocations of this container
        explicit Deque(size_type count, const Allocator& alloc = Allocator()) : Deque(count, 0, alloc) { };

        /// @brief Constructs the container with the contents of the range [first,
        /// last).
        /// @tparam InputIt Input Iterator
        /// @param first, last 	the range to copy the elements from
        /// @param alloc allocator to use for all memory allocations of this container
        template <class InputIt>
        Deque(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : Deque(alloc) {
            reallocate(last - first);
            auto cur = first;
            _end = _begin + (last - first);
            for (auto iter = _begin; iter != _end; iter++) {
                *iter = *cur;
                cur++;
            }
            _el_size = last - first;
        }

        /// @brief Copy constructor. Constructs the container with the copy of the
        /// contents of other.
        /// @param other another container to be used as source to initialize the
        /// elements of the container with
        Deque(const Deque& other) : Deque(other._begin, other._end) { }

        /// @brief Constructs the container with the copy of the contents of other,
        /// using alloc as the allocator.
        /// @param other another container to be used as source to initialize the
        /// elements of the container with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(const Deque& other, const Allocator& alloc) : Deque(other._begin, other._end, alloc) { }

        /**
         * @brief Move constructor.
         *
         * Constructs the container with the contents of other using move semantics.
         * Allocator is obtained by move-construction from the allocator belonging to
         * other.
         *
         * @param other another container to be used as source to initialize the
         * elements of the container with
         */
        Deque(Deque&& other) {
            _map = other._map;
            other._map = nullptr;
            _alloc_p = other._alloc_p;
            _alloc_t = other._alloc_t;
            _map_capacity = other._map_capacity;
            _el_size = other._el_size;
            _begin = other._begin;
            _end = other._end;
        }

        /**
         * @brief Allocator-extended move constructor.
         * Using alloc as the allocator for the new container, moving the contents
         * from other; if alloc != other.get_allocator(), this results in an
         * element-wise move.
         *
         * @param other another container to be used as source to initialize the
         * elements of the container with
         * @param alloc allocator to use for all memory allocations of this container
         */
        Deque(Deque&& other, const Allocator& alloc) : Deque(other) {
            _alloc_p = static_cast<allocator_pointer>(alloc);
            _alloc_t = alloc;
        }

        /// @brief Constructs the container with the contents of the initializer list
        /// init.
        /// @param init initializer list to initialize the elements of the container
        /// with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : Deque(init.begin(), init.end(), alloc) { }

        /// @brief Destructs the deque.
        ~Deque() {
            clear();
        }

        /// @brief Copy assignment operator. Replaces the contents with a copy of the
        /// contents of other.
        /// @param other another container to use as data source
        /// @return *this
        Deque& operator=(const Deque& other) {
            *this = Deque(other.begin(), other.end()); //move???
            return *this;
        }

        /**
         * Move assignment operator.
         *
         * Replaces the contents with those of other using move semantics
         * (i.e. the data in other is moved from other into this container).
         * other is in a valid but unspecified state afterwards.
         *
         * @param other another container to use as data source
         * @return *this
         */
        Deque& operator=(Deque&& other) {
            clear();
            _map = other._map;
            other._map = nullptr;
            _alloc_p = other._alloc_p;
            _alloc_t = other._alloc_t;
            _map_capacity = other._map_capacity;
            _el_size = other._el_size;
            _begin = other._begin;
            _end = other._end;

            return *this;
        }

        /// @brief Replaces the contents with those identified by initializer list
        /// ilist.
        /// @param ilist
        /// @return this
        Deque& operator=(std::initializer_list<T> ilist) {
            *this = Deque(ilist.begin(), ilist.end());
            return *this;
        }

        /// @brief Replaces the contents with count copies of value
        /// @param count
        /// @param value
        void assign(size_type count, const T& value) {
            *this = Deque(count, value);
            return *this;
        }

        /// @brief Replaces the contents with copies of those in the range [first,
        /// last).
        /// @tparam InputIt
        /// @param first
        /// @param last
        template <class InputIt>
        void assign(InputIt first, InputIt last) {
            *this = Deque(first, last);
            return *this;
        }

        /// @brief Replaces the contents with the elements from the initializer list
        /// ilis
        /// @param ilist
        void assign(std::initializer_list<T> ilist) {
            *this = Deque(ilist);
            return *this;
        }

        /// @brief Returns the allocator associated with the container.
        /// @return The associated allocator.
        allocator_type get_allocator() const noexcept {
            return _alloc_t;
        }

        /// ELEMENT ACCESS

        /// @brief Returns a reference to the element at specified location pos, with
        /// bounds checking. If pos is not within the range of the container, an
        /// exception of type std::out_of_range is thrown.
        /// @param pos position of the element to return
        /// @return Reference to the requested element.
        /// @throw std::out_of_range
        reference at(size_type pos) {
            if (pos >= _el_size)
                throw std::out_of_range("The position is out of range of deque");
            else
                return (*this)[pos];
        }

        /// @brief Returns a const reference to the element at specified location pos,
        /// with bounds checking. If pos is not within the range of the container, an
        /// exception of type std::out_of_range is thrown.
        /// @param pos position of the element to return
        /// @return Const Reference to the requested element.
        /// @throw std::out_of_range
        const_reference at(size_type pos) const {
            if (pos >= _el_size)
                throw std::out_of_range("The position is out of range of deque");
            else
                return (*this)[pos];
        }

        /// @brief Returns a reference to the element at specified location pos. No
        /// bounds checking is performed.
        /// @param pos position of the element to return
        /// @return Reference to the requested element.
        reference operator[](size_type pos) {
            return *(_begin + pos);
        }

        /// @brief Returns a const reference to the element at specified location pos.
        /// No bounds checking is performed.
        /// @param pos position of the element to return
        /// @return Const Reference to the requested element.
        const_reference operator[](size_type pos) const {
            return *static_cast<const_iterator>(_begin + pos);
        }

        /// @brief Returns a reference to the first element in the container.
        /// Calling front on an empty container is undefined.
        /// @return Reference to the first element
        reference front() {
            return *_begin;
        }

        /// @brief Returns a const reference to the first element in the container.
        /// Calling front on an empty container is undefined.
        /// @return Const reference to the first element
        const_reference front() const {
            return *static_cast<const_iterator>(_begin); //как вернуть const, если итератор обычный
        }

        /// @brief Returns a reference to the last element in the container.
        /// Calling back on an empty container causes undefined behavior.
        /// @return Reference to the last element.
        reference back() {
            return *(_end - 1);
        }

        /// @brief Returns a const reference to the last element in the container.
        /// Calling back on an empty container causes undefined behavior.
        /// @return Const Reference to the last element.
        const_reference back() const {
            return *static_cast<const_iterator>(_end - 1);
        }

        /// ITERATORS

        /// @brief Returns an iterator to the first element of the deque.
        /// If the deque is empty, the returned iterator will be equal to end().
        /// @return Iterator to the first element.
        iterator begin() noexcept {
            return _begin;
        }

        /// @brief Returns an iterator to the first element of the deque.
        /// If the deque is empty, the returned iterator will be equal to end().
        /// @return Iterator to the first element.
        const_iterator begin() const noexcept {
            return static_cast<const_iterator>(_begin);
        }

        /// @brief Same to begin()
        const_iterator cbegin() const noexcept {
            return static_cast<const_iterator>(_begin);
        }

        /// @brief Returns an iterator to the element following the last element of
        /// the deque. This element acts as a placeholder; attempting to access it
        /// results in undefined behavior.
        /// @return Iterator to the element following the last element.
        iterator end() noexcept {
            return _end;
        }

        /// @brief Returns an constant iterator to the element following the last
        /// element of the deque. This element acts as a placeholder; attempting to
        /// access it results in undefined behavior.
        /// @return Constant Iterator to the element following the last element.
        const_iterator end() const noexcept {
            return static_cast<const_iterator>(_end);
        }

        /// @brief Same to end()
        const_iterator cend() const noexcept {
            return static_cast<const_iterator>(_end);
        }

        /// @brief Returns a reverse iterator to the first element of the reversed
        /// deque. It corresponds to the last element of the non-reversed deque. If
        /// the deque is empty, the returned iterator is equal to rend().
        /// @return Reverse iterator to the first element.
        reverse_iterator rbegin() noexcept {
            return static_cast<reverse_iterator>(_begin);
        }

        /// @brief Returns a const reverse iterator to the first element of the
        /// reversed deque. It corresponds to the last element of the non-reversed
        /// deque. If the deque is empty, the returned iterator is equal to rend().
        /// @return Const Reverse iterator to the first element.
        const_reverse_iterator rbegin() const noexcept {
            return static_cast<const_reverse_iterator>(_begin - 1);
        }

        /// @brief Same to rbegin()
        const_reverse_iterator crbegin() const noexcept {
            return static_cast<const_reverse_iterator>(_begin - 1);
        }

        /// @brief Returns a reverse iterator to the element following the last
        /// element of the reversed deque. It corresponds to the element preceding the
        /// first element of the non-reversed deque. This element acts as a
        /// placeholder, attempting to access it results in undefined behavior.
        /// @return Reverse iterator to the element following the last element.
        reverse_iterator rend() noexcept {
            return static_cast<reverse_iterator>(_end - 1);
        }

        /// @brief Returns a const reverse iterator to the element following the last
        /// element of the reversed deque. It corresponds to the element preceding the
        /// first element of the non-reversed deque. This element acts as a
        /// placeholder, attempting to access it results in undefined behavior.
        /// @return Const Reverse iterator to the element following the last element.
        const_reverse_iterator rend() const noexcept {
            return static_cast<const_reverse_iterator>(_end - 1);
        }

        /// @brief Same to rend()
        const_reverse_iterator crend() const noexcept {
            return static_cast<const_reverse_iterator>(_end - 1);
        }

        /// CAPACITY

        /// @brief Checks if the container has no elements
        /// @return true if the container is empty, false otherwise
        bool empty() const noexcept {
            return !_el_size;
        }

        /// @brief Returns the number of elements in the container
        /// @return The number of elements in the container.
        size_type size() const noexcept {
            return _el_size;
        }

        /// @brief Returns the maximum number of elements the container is able to
        /// hold due to system or library implementation limitations
        /// @return Maximum number of elements.
        size_type max_size() const noexcept {
            return std::min(alloc_traits::max_size(), std::numeric_limits<difference_type>::max());
        }

        /// @brief Requests the removal of unused capacity.
        /// It is a non-binding request to reduce the memory usage without changing
        /// the size of the sequence. All iterators and references are invalidated.
        /// Past-the-end iterator is also invalidated.
        void shrink_to_fit() {
            chunk_ptr _new_map = _alloc_p.allocate(_end._chunkID - _begin._chunkID + 1);
            _new_map[0] = _alloc_t(_begin._last - _begin._el);
            size_type _size = _el_size;
            {
                size_type i = 0;
                for(auto ptr = _begin._first; ptr != _begin._last; ptr++)
                    _new_map[0][i++] = *ptr;
            }

            if (_end._chunkID != _begin._chunkID) {
                _new_map[_end._chunkID - _begin._chunkID] = _alloc_t(_end._el - _end._first + 1);
                {
                    size_type i = 0;
                    for (auto ptr = _end._first; ptr != (_end._el + 1); ptr++)
                        _new_map[_end._chunkID - _begin._chunkID][i++] = *ptr;
                }

                {
                    size_type chunkID = 1;
                    for(auto chunk_ptr = _begin._chunkID + 1; chunk_ptr != _end._chunkID; chunk_ptr++)
                        _new_map[chunkID++] = chunk_ptr;
                }
            }

            _map.clear();
            _map = _new_map;
            _el_size = _size;
            _map_capacity = _size; //проверить как capacity в других функциях
        }

        /// MODIFIERS

        /// @brief Erases all elements from the container.
        /// nvalidates any references, pointers, or iterators referring to contained
        /// elements. Any past-the-end iterators are also invalidated.
        void clear() noexcept {
            for(auto iter = _begin; iter != _end; iter++)
                alloc_traits::destroy(_alloc_t, &front());
            for(auto chunkID = _begin._chunkID; chunkID <= _end._chunkID; chunkID++)
                _alloc_t.deallocate(chunkID);
            _alloc_p.deallocate(_map);
            _el_size = 0;
            _map_capacity = 0;
        }

        /// @brief Inserts value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param value element value to insert
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, const T& value) {
            //реализовать отдельную функцию сдвига элементов
        }

        /// @brief Inserts value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param value element value to insert
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, T&& value);

        /// @brief Inserts count copies of the value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param count number of elements to insert
        /// @param value element value to insert
        /// @return Iterator pointing to the first element inserted, or pos if count
        /// == 0.
        iterator insert(const_iterator pos, size_type count, const T& value);

        /// @brief Inserts elements from range [first, last) before pos.
        /// @tparam InputIt Input Iterator
        /// @param pos iterator before which the content will be inserted.
        /// @param first,last the range of elements to insert, can't be iterators into
        /// container for which insert is called
        /// @return Iterator pointing to the first element inserted, or pos if first
        /// == last.
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last);

        /// @brief Inserts elements from initializer list before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param ilist initializer list to insert the values from
        /// @return Iterator pointing to the first element inserted, or pos if ilist
        /// is empty.
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);

        /// @brief Inserts a new element into the container directly before pos.
        /// @param pos iterator before which the new element will be constructed
        /// @param ...args arguments to forward to the constructor of the element
        /// @return terator pointing to the emplaced element.
        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        /// @brief Removes the element at pos.
        /// @param pos iterator to the element to remove
        /// @return Iterator following the last removed element.
        iterator erase(const_iterator pos);

        /// @brief Removes the elements in the range [first, last).
        /// @param first,last range of elements to remove
        /// @return Iterator following the last removed element.
        iterator erase(const_iterator first, const_iterator last);

        /// @brief Appends the given element value to the end of the container.
        /// The new element is initialized as a copy of value.
        /// @param value the value of the element to append
        void push_back(const T& value) {
            _el_size++;
            if (_end._el == _end._last) {
                if (_end._chunkID == &_map[_map_capacity - 1])
                    reallocate(_el_size);
                else
                    _end++; //сюда мы входим если _end находится на ячейке сразу после НЕ ПОСЛЕДНЕГО чанка
                *_end = value;
                _end++;
            }
        }

        /// @brief Appends the given element value to the end of the container.
        /// Value is moved into the new element.
        /// @param value the value of the element to append
        void push_back(T&& value) {
            _el_size++;
            if (_end._el == _end._last) {
                if (_end._chunkID == &_map[_map_capacity - 1])
                    reallocate(_el_size);
                else
                    _end++; //сюда мы входим если _end находится на ячейке сразу после НЕ ПОСЛЕДНЕГО чанка
                *_end = std::move(value);
                _end++;
            }
        }

        /// @brief Appends a new element to the end of the container.
        /// @param ...args arguments to forward to the constructor of the element
        /// @return A reference to the inserted element.
        template <class... Args>
        reference emplace_back(T&& first, Args&&... args) {
            this->push_back(first);
            this->emplace_back(args...); //нужно смотреть, чтоб не зациклилось
            return back();
        }

        /// @brief Removes the last element of the container.
        void pop_back() {
            if (_begin == _end)
                return ;
            alloc_traits::destroy(_alloc_t, &back());
            _el_size--;
            _end--;
        }

        /// @brief Prepends the given element value to the beginning of the container.
        /// @param value the value of the element to prepend
        void push_front(const T& value) {
            _el_size++;
            if(_begin._el == _begin._first) {
                if (_begin._chunkID == &_map[0])
                    reallocate(_el_size);
                else
                    _begin--;
               *_begin = value;
               _begin--;
            }
        }

        /// @brief Prepends the given element value to the beginning of the container.
        /// @param value moved value of the element to prepend
        void push_front(T&& value) {
            _el_size++;
            if (_begin._el == _begin._first) {
                if (_begin._chunkID == &_map[0])
                    reallocate(_el_size);
                else
                    _begin--;
                *_begin = std::move(value);
                _begin--;
            }
        }

        /// @brief Inserts a new element to the beginning of the container.
        /// @param ...args arguments to forward to the constructor of the element
        /// @return A reference to the inserted element.
        template <class... Args>
        reference emplace_front(T&& first, Args&&... args) {
            this->push_front(first);
            this->template emplace_front(args...);
            return front();
        }

        /// @brief Removes the first element of the container.
        void pop_front() {
            if (_begin == _end)
                return;
            delete *_begin;
            _el_size--;
            _begin++;
        }

        /// @brief Resizes the container to contain count elements.
        /// If the current size is greater than count, the container is reduced to its
        /// first count elements. If the current size is less than count, additional
        /// default-inserted elements are appended
        /// @param count new size of the container
        void resize(size_type count) {
            resize(count, 0);
        }

        /// @brief Resizes the container to contain count elements.
        /// If the current size is greater than count, the container is reduced to its
        /// first count elements. If the current size is less than count, additional
        /// copies of value are appended.
        /// @param count new size of the container
        /// @param value the value to initialize the new elements with
        void resize(size_type count, const value_type& value) {
            if (count > _el_size)
                while (count > _el_size)
                    push_back(value);
            else if (count < _el_size)
                while (count < _el_size)
                    pop_back();
        }

        /// @brief Exchanges the contents of the container with those of other.
        /// Does not invoke any move, copy, or swap operations on individual elements.
        /// All iterators and references remain valid. The past-the-end iterator is
        /// invalidated.
        /// @param other container to exchange the contents with
        void swap(Deque& other);

        /// COMPARISIONS

        /// @brief Checks if the contents of lhs and rhs are equal
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator==(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
            if (lhs.size() != rhs.size())
                return false;

            auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
            while (lhs_iter != lhs.end()) {
                if (*lhs_iter != *rhs_iter)
                    return false;
                lhs_iter++;
                rhs_iter++;
            }

            return true;
        }

        /// @brief Checks if the contents of lhs and rhs are not equal
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator!=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs) {
            return !(lhs == rhs);
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator>(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
            auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
            while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
                if (*lhs_iter < *rhs_iter)
                    return false;
                else if (*lhs_iter > *rhs_iter)
                    return true;
                lhs_iter++;
                rhs_iter++;
            }
            return lhs.size() != rhs.size();
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator<(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
            auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
            while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
                if (*lhs_iter < *rhs_iter)
                    return true;
                else if (*lhs_iter > *rhs_iter)
                    return false;
                lhs_iter++;
                rhs_iter++;
            }
            return lhs.size() != rhs.size();
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator>=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs) {
            auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
            while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
                if (*lhs_iter < *rhs_iter)
                    return false;
                else if (*lhs_iter > *rhs_iter)
                    return true;
                lhs_iter++;
                rhs_iter++;
            }
            return lhs.size() == rhs.size();
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator<=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs) {
            auto lhs_iter = lhs.begin(), rhs_iter = rhs.begin();
            while (lhs_iter != lhs.end() && rhs_iter != rhs.end()) {
                if (*lhs_iter < *rhs_iter)
                    return true;
                else if (*lhs_iter > *rhs_iter)
                    return false;
                lhs_iter++;
                rhs_iter++;
            }
            return lhs.size() == rhs.size();
        }

        // operator <=> will be handy
    };

/// NON-MEMBER FUNCTIONS

/// @brief  Swaps the contents of lhs and rhs.
/// @param lhs,rhs containers whose contents to swap
    template <class T, class Alloc>
    void swap(Deque<T, Alloc>& lhs, Deque<T, Alloc>& rhs) {

    }

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param value value to be removed
/// @return The number of erased elements.
    template <class T, class Alloc, class U>
    typename Deque<T, Alloc>::size_type erase(Deque<T, Alloc>& c, const U& value);

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param pred unary predicate which returns ​true if the element should be
/// erased.
/// @return The number of erased elements.
    template <class T, class Alloc, class Pred>
    typename Deque<T, Alloc>::size_type erase_if(Deque<T, Alloc>& c, Pred pred);
}  // namespace fefu_laboratory_two