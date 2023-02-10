#pragma once
#include <iterator>
#include <memory>
#include <limits>

namespace lol  {
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

        Allocator() noexcept = default;

        Allocator(const Allocator& other) noexcept = default;

        template <class U>
        Allocator(const Allocator<U>& other) noexcept {};

        ~Allocator() = default;

        pointer allocate(size_type n) {
            return static_cast<pointer>(operator new(sizeof(T) * n));
        };

        void deallocate(pointer p) noexcept {
            operator delete(p);
        }
    };

    template <typename Type>
    class Node{
    public:
        Type value = NULL;
        Node* _prev = nullptr;
        Node* _next = nullptr;
    };

    template <typename ValueType>
    class Deque_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        Node<value_type> *_cur;

        Deque_iterator() noexcept = default;
        Deque_iterator(const Deque_iterator& other) noexcept : _cur(other._cur){}

        Deque_iterator& operator=(const Deque_iterator& it){
            _cur = it._cur;
            return *this;
        }

        Deque_iterator(const pointer it) {
            _cur = it;
        }

        ~Deque_iterator()  = default;

        friend void swap(Deque_iterator<ValueType>& it1, Deque_iterator<ValueType>& it2) {
            Deque_iterator <ValueType> tmp = it1;
            it1 = it2;
            it2 = tmp;
        }

        friend bool operator==(const Deque_iterator<ValueType>& it1,
                               const Deque_iterator<ValueType>& it2) {
            return it1._cur == it2._cur;
        }

        friend bool operator!=(const Deque_iterator<ValueType>& it1,
                               const Deque_iterator<ValueType>& it2) {
            return it1._cur != it2._cur;
        }

        reference operator*() const {
            return _cur->value;
        }

        pointer operator->() const {
            return _cur + sizeof(ValueType);
        }

        Deque_iterator& operator++() {
            _cur = _cur->_next;
            return *this;
        }
        Deque_iterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        Deque_iterator& operator--() {
            _cur = _cur->_prev;
            return *this;
        }
        Deque_iterator operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        Deque_iterator operator+(const difference_type& n) const {
            if (n == 0) return *this;
            auto th = *this;
            for (auto p = 0; p < n; p++) {
                th++;
            }
            return th;
        }
        Deque_iterator& operator+=(const difference_type& n) {
            _cur = _cur + n;
            return *this;
        }

        Deque_iterator operator-(const difference_type& n) const {
            if (n == 0)
                return *this;
            auto th = *this;
            for (auto p = 0; p < n; p++) {
                th--;
            }
            return th;
        }

        Deque_iterator& operator-=(const difference_type& n) {
            _cur = _cur - n;
            return *this;
        }

        difference_type operator-(const Deque_iterator& it) const {
            return abs(_cur - it._cur);
        }

        reference operator[](const difference_type& n) {
            return *(_cur + n);
        }

        friend bool operator<(const Deque_iterator<ValueType>& it1,
                              const Deque_iterator<ValueType>& it2) {
            return it1._cur < it2._cur;
        }
        friend bool operator<=(const Deque_iterator<ValueType>& it1,
                               const Deque_iterator<ValueType>& it2) {
            return it1._cur <= it2._cur;
        }
        friend bool operator>(const Deque_iterator<ValueType>& it1,
                              const Deque_iterator<ValueType>& it2) {
            return it1._cur > it2._cur;
        }
        friend bool operator>=(const Deque_iterator<ValueType>& it1,
                               const Deque_iterator<ValueType>& it2) {
            return it1._cur >= it2._cur;
        }

    };

    template <typename ValueType>
    class Deque_const_iterator {
        // Shouldn't give non const references on value
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const ValueType*;
        using reference = const ValueType&;

        Node<value_type> *_cur;

        Deque_const_iterator() noexcept = default;
        Deque_const_iterator(const Deque_const_iterator& other) noexcept : _cur(other._cur){};

        Deque_const_iterator& operator=(const Deque_const_iterator& it) {
            _cur = it._cur;
            return *this;
        }
        Deque_const_iterator& operator=(const Deque_iterator<ValueType>& it) {
            _cur = it._cur;
            return *this;
        }

        ~Deque_const_iterator() = default;

        friend void swap(Deque_const_iterator<ValueType>& it1, Deque_const_iterator<ValueType>& it2) {
            Deque_const_iterator <ValueType> tmp = it1;
            it1 = it2;
            it2 = tmp;
        }

        friend bool operator==(const Deque_const_iterator<ValueType>& it1,
                               const Deque_const_iterator<ValueType>& it2) {
            return it1._cur == it2._cur;
        }

        friend bool operator!=(const Deque_const_iterator<ValueType>& it1,
                               const Deque_const_iterator<ValueType>& it2) {
            return it1._cur != it2._cur;
        }

        reference operator*() const {
            return *_cur->value;
        }

        pointer operator->() const {
            return _cur + sizeof(ValueType);
        }

        Deque_const_iterator& operator++() {
            _cur = _cur->_next;
            return *this;
        }

        Deque_const_iterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        Deque_const_iterator& operator--() {
            _cur = _cur->_prev;
            return *this;
        }
        Deque_const_iterator operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        Deque_const_iterator operator+(const difference_type& n) const {
            if (n == 0) return *this;
            auto th = *this;
            for (auto p = 0; p < n; p++) {
                th++;
            }
            return th;
        }
        Deque_const_iterator& operator+=(const difference_type& n) {
            _cur = _cur + n;
            return *this;
        }

        Deque_const_iterator operator-(const difference_type& n) const {
            if (n == 0) return *this;
            auto th = *this;
            for (auto p = 0; p < n; p++) {
                th--;
            }
            return th;
        }

        Deque_const_iterator& operator-=(const difference_type& n) {
            _cur = _cur - n;
            return *this;
        }

        difference_type operator-(const Deque_const_iterator& it) const {
            return abs(_cur - it._cur);
        }

        reference operator[](const difference_type& n) {
            return *(_cur + n);
        }

        friend bool operator<(const Deque_const_iterator<ValueType>& it1,
                              const Deque_const_iterator<ValueType>& it2) {
            return it1._cur < it2._cur;
        }
        friend bool operator<=(const Deque_const_iterator<ValueType>& it1,
                               const Deque_const_iterator<ValueType>& it2) {
            return it1._cur <= it2._cur;
        }
        friend bool operator>(const Deque_const_iterator<ValueType>& it1,
                              const Deque_const_iterator<ValueType>& it2) {
            return it1._cur > it2._cur;
        }
        friend bool operator>=(const Deque_const_iterator<ValueType>& it1,
                               const Deque_const_iterator<ValueType>& it2) {
            return it1._cur >= it2._cur;
        }
        // operator<=> will be handy

    };

    template <class Iter>
    class Deque_reverse_iterator {
    public:
        using iterator_type = Iter;
        using iterator_category =
                typename std::iterator_traits<Iter>::iterator_category;
        using value_type = typename std::iterator_traits<Iter>::value_type;
        using difference_type = typename std::iterator_traits<Iter>::difference_type;
        using pointer = typename std::iterator_traits<Iter>::pointer;
        using reference = typename std::iterator_traits<Iter>::reference;

        constexpr Deque_reverse_iterator() = default;

        constexpr explicit Deque_reverse_iterator(iterator_type x) : iter(x) {}


        template <class U>
        constexpr explicit Deque_reverse_iterator(const Deque_reverse_iterator<U>& other) : Iter(other.iter) {}

        template <class U>
        Deque_reverse_iterator& operator=(const Deque_reverse_iterator<U>& other) {
            iter = other.iter;
        }

        iterator_type base() const {
            return iter;
        }

        reference operator*() const {
            return *base();
        }

        pointer operator->() const {
            return base();
        }

        reference operator[](difference_type n) {
            return *(base() + n);
        }

        Deque_reverse_iterator& operator++() {
            --iter;
            return *this;
        }

        Deque_reverse_iterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        Deque_reverse_iterator& operator--() {
            ++iter;
            return *this;
        }

        Deque_reverse_iterator operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        Deque_reverse_iterator operator+(difference_type n) const {
            iterator_type  it = iter - n;
            return Deque_reverse_iterator<iterator_type>(it);
        }
        Deque_reverse_iterator& operator+=(difference_type n) {
            iter -= n;
            return reverse_iterator(iter);
        }

        Deque_reverse_iterator operator-(difference_type n) const {
            iterator_type  it = iter + n;
            return Deque_reverse_iterator<iterator_type>(it);
        }
        Deque_reverse_iterator& operator-=(difference_type n) {
            iter += n;
            return reverse_iterator(iter);
        }

        template <class Iterator1, class Iterator2>
        friend bool operator==(const Deque_reverse_iterator<Iterator1>& lhs,
                               const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() == rhs.base();
        }

        template <class Iterator1, class Iterator2>
        friend bool operator!=(const Deque_reverse_iterator<Iterator1>& lhs,
                               const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() != rhs.base();
        }

        template <class Iterator1, class Iterator2>
        friend bool operator>(const Deque_reverse_iterator<Iterator1>& lhs,
                              const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() > rhs.base();
        }

        template <class Iterator1, class Iterator2>
        friend bool operator<(const Deque_reverse_iterator<Iterator1>& lhs,
                              const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() < rhs.base();
        }

        template <class Iterator1, class Iterator2>
        friend bool operator<=(const Deque_reverse_iterator<Iterator1>& lhs,
                               const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() <= rhs.base();
        }

        template <class Iterator1, class Iterator2>
        friend bool operator>=(const Deque_reverse_iterator<Iterator1>& lhs,
                               const Deque_reverse_iterator<Iterator2>& rhs){
            return lhs.base() >= rhs.base();
        }

        template <class IterT>
        friend Deque_reverse_iterator<IterT> operator+(
                typename Deque_reverse_iterator<IterT>::difference_type n,
                const Deque_reverse_iterator<IterT>& it){
            return Deque_reverse_iterator<IterT>(it.base() - n);
        }

        template <class Iterator>
        friend auto operator-(const Deque_reverse_iterator<Iterator>& lhs,
                              const Deque_reverse_iterator<Iterator>& rhs){
            return Deque_reverse_iterator<Iterator>(lhs.base() - rhs.base());
        }

        pointer return__cur() const{
            return _cur;
        }

    private:
        iterator_type iter;
        value_type* _cur;
    };

    template <class Iter>
    Deque_reverse_iterator<Iter> make_reverse_iterator(Iter i) {
        return Deque_reverse_iterator <Iter>(i);
    }

    template <typename T, typename Allocator = Allocator<Node<T>>>
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
        using iterator = Deque_iterator<value_type>;
        using const_iterator = Deque_const_iterator<value_type>;
        using reverse_iterator = Deque_reverse_iterator<iterator>;
        using const_reverse_iterator = Deque_reverse_iterator<const_iterator>;

        /// @brief Default constructor. Constructs an empty container with a
        /// default-constructed allocator.
        Deque() :  size_node(0), first(nullptr), last(nullptr) {}

        /// @brief Constructs an empty container with the given allocator
        /// @param alloc allocator to use for all memory allocations of this container
        explicit Deque(const Allocator& alloc) {
            Node<value_type> *node = alloc.allocate(1);
            first = node;
            last = node;
        }

        /// @brief Constructs the container with count copies of elements with value
        /// and with the given allocator
        /// @param count the size of the container
        /// @param value the value to initialize elements of the container with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(size_type count, const T& value, const Allocator& alloc = Allocator()){
            for(auto p = 0; p < count; p++){
                Node<value_type> *node = alloc.allocate(1);
                push_back(0);
            }
        }

        /// @brief Constructs the container with count default-inserted instances of
        /// T. No copies are made.
        /// @param count the size of the container
        /// @param alloc allocator to use for all memory allocations of this container
        explicit Deque(size_type count, const Allocator& alloc = Allocator()){
            for(auto p= 0; p < count; p++){
                push_back(value_type());
            }
        }

        /// @brief Constructs the container with the contents of the range [first,
        /// last).
        /// @tparam InputIt Input Iterator
        /// @param first, last 	the range to copy the elements from
        /// @param alloc allocator to use for all memory allocations of this container
        template <class InputIt>
        Deque(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {
            for(auto p = first; p != last; p++){
                push_back(*p);
            }
        }

        /// @brief Copy constructor. Constructs the container with the copy of the
        /// contents of other.
        /// @param other another container to be used as source to initialize the
        /// elements of the container with
        Deque(const Deque& other) {
            size_node = 0;
            for(auto p = other.begin(); p < other.end(); p++){
                Node<value_type> *node = allocate.allocate(1);
                node->_next = nullptr;
                node->_prev = nullptr;
                node->value = *p;
                if(first == nullptr) first = node;
                if(last != nullptr) last->_next = node;
                last = std::move(node);
                size_node++;
            }
        }

        /// @brief Constructs the container with the copy of the contents of other,
        /// using alloc as the allocator.
        /// @param other another container to be used as source to initialize the
        /// elements of the container with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(const Deque& other, const Allocator& alloc) {
            size_node = 0;
            for(auto p = other.begin(); p < other.end(); p++){
                Node<value_type> *node = alloc.allocate(1);
                node->_next = nullptr;
                node->_prev = nullptr;
                node->value = *p;
                if(first == nullptr) first = node;
                if(last != nullptr) last->_next = node;
                last = std::move(node);
                size_node++;
            }
        }

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
            size_node = 0;
            allocate = std::move(other.get_allocator());
            for(auto p = other.begin(); p < other.end(); p++){
                Node<value_type> *node = allocate.allocate(1);
                node->_next = nullptr;
                node->_prev = last;
                node->value = std::move(*p);
                if(first == nullptr) first = node;
                if(last != nullptr) last->_next = node;
                last = std::move(node);
                size_node++;
            }
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
        Deque(Deque&& other, const Allocator& alloc) {
            this->allocate = alloc;
            for(auto p = 0; p < other.size(); p++)
                push_back(other[p]);
        }

        /// @brief Constructs the container with the contents of the initializer list
        /// init.
        /// @param init initializer list to initialize the elements of the container
        /// with
        /// @param alloc allocator to use for all memory allocations of this container
        Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
            for(auto p : init)
                push_back(p);
        }

        /// @brief Destructs the deque.
        ~Deque() {
            Node<value_type>* _curr = first;
            while(_curr != nullptr){
                Node<value_type>* _next = _curr->_next;
                allocate.deallocate(_curr);
                _curr = _next;
            }
        }

        /// @brief Copy assignment operator. Replaces the contents with a copy of the
        /// contents of other.
        /// @param other another container to use as data source
        /// @return *this
        Deque& operator=(const Deque& other) {
            clear();
            for(auto p = 0; p < other.size(); p++)
                push_back(other[p]);
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
            for(auto p = 0; p < other.size(); p++)
                push_back(std::move(other[p]));
        }

        /// @brief Replaces the contents with those identified by initializer list
        /// ilist.
        /// @param ilist
        /// @return this
        Deque& operator=(std::initializer_list<T> ilist) {
            clear();
            for(auto p: ilist)
                push_back(*p);
        }

        /// @brief Replaces the contents with count copies of value
        /// @param count
        /// @param value
        void assign(size_type count, const T& value) {
            resize(count, value);
        }

        /// @brief Replaces the contents with copies of those in the range [first,
        /// last).
        /// @tparam InputIt
        /// @param first
        /// @param last
        template <class InputIt>
        void assign(InputIt first, InputIt last) {
            clear();
            for(auto p = first; p != last; p++)
                push_back(*p);
        }

        /// @brief Replaces the contents with the elements from the initializer list
        /// ilis
        /// @param ilist
        void assign(std::initializer_list<T> ilist) {
            assign(ilist.begin(), ilist.end());
        }

        /// @brief Returns the allocator associated with the container.
        /// @return The associated allocator.
        allocator_type get_allocator() const noexcept{
            return allocate;
        }

        /// ELEMENT ACCESS

        /// @brief Returns a reference to the element at specified location pos, with
        /// bounds checking. If pos is not within the range of the container, an
        /// exception of type std::out_of_range is thrown.
        /// @param pos position of the element to return
        /// @return Reference to the requested element.
        /// @throw std::out_of_range
        reference at(size_type pos){
            ++pos;
            if(pos > size_node)
                throw std::out_of_range("out_of_range");
            else
                return operator[](pos);
        }

        /// @brief Returns a const reference to the element at specified location pos,
        /// with bounds checking. If pos is not within the range of the container, an
        /// exception of type std::out_of_range is thrown.
        /// @param pos position of the element to return
        /// @return Const Reference to the requested element.
        /// @throw std::out_of_range
        const_reference at(size_type pos) const{
            ++pos;
            if(pos > size_node)
                throw std::out_of_range("out_of_range");
            else
                return operator[](pos);

        }

        /// @brief Returns a reference to the element at specified location pos. No
        /// bounds checking is performed.
        /// @param pos position of the element to return
        /// @return Reference to the requested element.
        reference operator[](size_type pos){
            size_type t = 0;
            for(Node<value_type>*_curr = first; _curr != nullptr; _curr = _curr->_next){
                if(pos == t) return _curr->value;
                t++;
            }
        }

        /// @brief Returns a const reference to the element at specified location pos.
        /// No bounds checking is performed.
        /// @param pos position of the element to return
        /// @return Const Reference to the requested element.
        const_reference operator[](size_type pos) const{
            size_type t = 0;
            for(Node<value_type>*_curr = first; _curr != nullptr; _curr = _curr->_next){
                if(pos == t) return _curr->value;
                t++;
            }
        }

        /// @brief Returns a reference to the first element in the container.
        /// Calling front on an empty container is undefined.
        /// @return Reference to the first element
        reference front() {
            return first->value;
        }

        /// @brief Returns a const reference to the first element in the container.
        /// Calling front on an empty container is undefined.
        /// @return Const reference to the first element
        const_reference front() const{
            return first->value;
        }

        /// @brief Returns a reference to the last element in the container.
        /// Calling back on an empty container causes undefined behavior.
        /// @return Reference to the last element.
        reference back(){
            last->value;
        }

        /// @brief Returns a const reference to the last element in the container.
        /// Calling back on an empty container causes undefined behavior.
        /// @return Const Reference to the last element.
        const_reference back() const{
            last->value;
        }

        /// ITERATORS

        /// @brief Returns an iterator to the first element of the deque.
        /// If the deque is empty, the returned iterator will be equal to end().
        /// @return Iterator to the first element.
        iterator begin() noexcept {
            iterator it;
            it._cur = first;
            return it;
        }

        /// @brief Returns an iterator to the first element of the deque.
        /// If the deque is empty, the returned iterator will be equal to end().
        /// @return Iterator to the first element.
        const_iterator begin() const noexcept{
            const_iterator it;
            it._cur = first;
            return it;
        }

        /// @brief Same to begin()
        const_iterator cbegin() const noexcept{
            const_iterator it;
            it._cur = first;
            return it;
        }

        /// @brief Returns an iterator to the element following the last element of
        /// the deque. This element acts as a placeholder; attempting to access it
        /// results in undefined behavior.
        /// @return Iterator to the element following the last element.
        iterator end() noexcept {
            iterator it;
            it._cur = last;
            return it;
        }

        /// @brief Returns an constant iterator to the element following the last
        /// element of the deque. This element acts as a placeholder; attempting to
        /// access it results in undefined behavior.
        /// @return Constant Iterator to the element following the last element.
        const_iterator end() const noexcept{
            const_iterator it;
            it._cur = last;
            return it;
        }

        /// @brief Same to end()
        const_iterator cend() const noexcept{
            const_iterator it;
            it._cur = last;
            return it;
        }

        /// @brief Returns a reverse iterator to the first element of the reversed
        /// deque. It corresponds to the last element of the non-reversed deque. If
        /// the deque is empty, the returned iterator is equal to rend().
        /// @return Reverse iterator to the first element.
        reverse_iterator rbegin() noexcept{
            reverse_iterator it(begin());
            return it;
        }

        /// @brief Returns a const reverse iterator to the first element of the
        /// reversed deque. It corresponds to the last element of the non-reversed
        /// deque. If the deque is empty, the returned iterator is equal to rend().
        /// @return Const Reverse iterator to the first element.
        const_reverse_iterator rbegin() const noexcept{
            reverse_iterator it(begin());
            return it;
        }

        /// @brief Same to rbegin()
        const_reverse_iterator crbegin() const noexcept{
            reverse_iterator it(begin());
            return it;
        }

        /// @brief Returns a reverse iterator to the element following the last
        /// element of the reversed deque. It corresponds to the element preceding the
        /// first element of the non-reversed deque. This element acts as a
        /// placeholder, attempting to access it results in undefined behavior.
        /// @return Reverse iterator to the element following the last element.
        reverse_iterator rend() noexcept{
            reverse_iterator it(end());
            return it;
        }

        /// @brief Returns a const reverse iterator to the element following the last
        /// element of the reversed deque. It corresponds to the element preceding the
        /// first element of the non-reversed deque. This element acts as a
        /// placeholder, attempting to access it results in undefined behavior.
        /// @return Const Reverse iterator to the element following the last element.
        const_reverse_iterator rend() const noexcept{
            reverse_iterator it(begin());
            return it;
        }

        /// @brief Same to rend()
        const_reverse_iterator crend() const noexcept{
            reverse_iterator it(begin());
            return it;
        }

        /// CAPACITY

        /// @brief Checks if the container has no elements
        /// @return true if the container is empty, false otherwise
        bool empty() const noexcept{
            return (first == nullptr);
        }

        /// @brief Returns the number of elements in the container
        /// @return The number of elements in the container.
        size_type size() const noexcept{
            return size_node;
        }

        /// @brief Returns the maximum number of elements the container is able to
        /// hold due to system or library implementation limitations
        /// @return Maximum number of elements.
        size_type max_size() const noexcept{
            return std::numeric_limits<size_t>::max();
        }

        /// @brief Requests the removal of unused capacity.
        /// It is a non-binding request to reduce the memory usage without changing
        /// the size of the sequence. All iterators and references are invalidated.
        /// Past-the-end iterator is also invalidated.
        void shrink_to_fit(){
            for(auto p = this->first(); p != this->last; p++){
                if (*p == nullptr)
                    this->erase(p);
            }
        }

        /// MODIFIERS

        /// @brief Erases all elements from the container.
        /// nvalidates any references, pointers, or iterators referring to contained
        /// elements. Any past-the-end iterators are also invalidated.
        void clear() noexcept {
            while (size() != 0)
                pop_back();
        }

        /// @brief Inserts value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param value element value to insert
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, const T& value){
            Node<value_type>* _curr = allocate.allocate(1);
            _curr->_next = pos._cur;
            _curr->_prev = pos._cur->_prev;
            pos._cur->_prev->_next = _curr;
            pos._cur->_prev = _curr;
            _curr->value = value;
            size_node++;
            iterator it;
            it._cur = _curr;
            return it;
        }

        /// @brief Inserts value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param value element value to insert
        /// @return Iterator pointing to the inserted value.
        iterator insert(const_iterator pos, T&& value){
            Node<value_type>* _curr = allocate.allocate(1);
            _curr->_next = pos._cur;
            _curr->_prev = pos._cur->_prev;
            pos._cur->_prev->_next = _curr;
            pos._cur->_prev = _curr;
            _curr->value = value;
            size_node++;
            iterator it;
            it._cur = _curr;
            return it;
        }

        /// @brief Inserts count copies of the value before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param count number of elements to insert
        /// @param value element value to insert
        /// @return Iterator pointing to the first element inserted, or pos if count
        /// == 0.
        iterator insert(const_iterator pos, size_type count, const T& value){
            Node<value_type>* _current;
            for(auto p = 0; p < count; p++){
                Node<value_type>* _curr = allocate.allocate(1);
                _curr->_next = pos._cur;
                _curr->_prev = pos._cur->_prev;
                pos._cur->_prev->_next = _curr;
                pos._cur->_prev = _curr;
                _curr->value = value;
                size_node++;
                _current = _curr;
            }
            iterator it;
            it._cur = _current;
            return it;
        }

        /// @brief Inserts elements from range [first, last) before pos.
        /// @tparam InputIt Input Iterator
        /// @param pos iterator before which the content will be inserted.
        /// @param first,last the range of elements to insert, can't be iterators into
        /// container for which insert is called
        /// @return Iterator pointing to the first element inserted, or pos if first
        /// == last.
        template <class InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last){
            Node<value_type>* _current;
            for(auto p = first; p != last; p++){
                Node<value_type>* _curr = allocate.allocate(1);
                _curr->_next = pos._cur;
                _curr->_prev = pos._cur->_prev;
                pos._cur->_prev->_next = _curr;
                pos._cur->_prev = _curr;
                _curr->value = *p;
                size_node++;
                _current = _curr;
            }
            iterator it;
            it._cur = _current;
            return it;
        }

        /// @brief Inserts elements from initializer list before pos.
        /// @param pos iterator before which the content will be inserted.
        /// @param ilist initializer list to insert the values from
        /// @return Iterator pointing to the first element inserted, or pos if ilist
        /// is empty.
        iterator insert(const_iterator pos, std::initializer_list<T> ilist){
            Node<value_type>* _current;
            for(auto p: ilist){
                Node<value_type>* _curr = allocate.allocate(1);
                _curr->_next = pos._cur;
                _curr->_prev = pos._cur->_prev;
                pos._cur->_prev->_next = _curr;
                pos._cur->_prev = _curr;
                _curr->value = p;
                size_node++;
                _current = _curr;
            }
            iterator it;
            it._cur = _current;
            return it;
        }

        /// @brief Inserts a new element into the container directly before pos.
        /// @param pos iterator before which the new element will be constructed
        /// @param ...args arguments to forward to the constructor of the element
        /// @return terator pointing to the emplaced element.
        template <class... Args>
        iterator emplace(const_iterator pos, Args&&... args){
            auto new_value = value_type(args...);
            return this->insert(pos, new_value);
        }

        /// @brief Removes the element at pos.
        /// @param pos iterator to the element to remove
        /// @return Iterator following the last removed element.
        iterator erase(const_iterator pos){
            if(pos._cur == first){
                if(size_node != 1) {
                    Node<value_type> *_cur = first;
                    first = first->_next;
                    first->_prev = nullptr;
                    size_node--;
                    allocate.deallocate(_cur);
                }
                else clear();
                iterator it;
                it._cur = first;
                return it;
            }
            if(pos._cur == last){
                Node<value_type> *_cur = last;
                last = last->_prev;
                last->_next = nullptr;
                size_node--;
                iterator it;
                it._cur = last;
                return it;
            }
            Node<value_type> *_cur = pos._cur->_next;
            pos._cur->_prev->_next = pos._cur->_next;
            pos._cur->_next->_prev = pos._cur->_prev;
            size_node--;
            allocate.deallocate(pos._cur);
            iterator it;
            it._cur = _cur;
            return it;
        }

        /// @brief Removes the elements in the range [first, last).
        /// @param first,last range of elements to remove
        /// @return Iterator following the last removed element.
        iterator erase(const_iterator first, const_iterator last){
            if(last == cend() && first == cbegin()){
                clear();
                iterator it;
                it._cur = this->first;
                return it;
            }
            if(first == cbegin()) {
                this->first = last._cur;
                this->first->_prev = nullptr;
            }
            const_iterator _curr = first;
            while (_curr._cur != last._cur){
                erase(_curr);
                _curr++;
                size_node--;
            }
            iterator it;
            it._cur = last._cur;
            return it;
        }

        /// @brief Appends the given element value to the end of the container.
        /// The new element is initialized as a copy of value.
        /// @param value the value of the element to append
        void push_back(const T& value){
            Node<value_type> *node = get_allocator().allocate(1);
            node->_next = nullptr;
            node->_prev = last;
            node->value = value;
            if(!empty()){
                last -> _next = node;
                last = node;
            }
            else{
                first = node;
                last = node;
            }
            size_node++;
        }

        /// @brief Appends the given element value to the end of the container.
        /// Value is moved into the new element.
        /// @param value the value of the element to append
        void push_back(T&& value){
            Node<value_type> *node = get_allocator().allocate(1);
            node->_next = nullptr;
            node->_prev = last;
            node->value = std::move(value);
            if(!empty()){
                last -> _next = node;
                last = node;
            }
            else{
                first = node;
                last = node;
            }
            size_node++;
        }

        /// @brief Appends a new element to the end of the container.
        /// @param ...args arguments to forward to the constructor of the element
        /// @return A reference to the inserted element.
        template <class... Args>
        reference emplace_back(Args&&... args){
            push_back(value_type(args...));
            return last->value;
        }

        /// @brief Removes the last element of the container.
        void pop_back(){
            if(first == last){
                allocate.deallocate(first);
                first = nullptr;
                last = nullptr;
                size_node--;
                return;
            }
            Node<value_type>* d = last;
            last = last->_prev;
            last->_next = nullptr;
            get_allocator().deallocate(d);
            size_node--;
        }

        /// @brief Prepends the given element value to the beginning of the container.
        /// @param value the value of the element to prepend
        void push_front(const T& value){
            Node<value_type>* node = get_allocator().allocate(1);
            node->_next = first;
            node->_prev = nullptr;
            node->value = value;
            if (empty()){
                first = node;
                last = node;
                return;
            }
            first->_prev = node;
            first = node;
            size_node++;
        }

        /// @brief Prepends the given element value to the beginning of the container.
        /// @param value moved value of the element to prepend
        void push_front(T&& value){
            Node<value_type>* node = get_allocator().allocate(1);
            node->_next = first;
            node->_prev = nullptr;
            node->value = std::move(value);
            if (empty()){
                first = node;
                last = node;
                return;
            }
            first->_prev = node;
            first = node;
            size_node++;
        }

        /// @brief Inserts a new element to the beginning of the container.
        /// @param ...args arguments to forward to the constructor of the element
        /// @return A reference to the inserted element.
        template <class... Args>
        reference emplace_front(Args&&... args){
            auto new_value = value_type(args...);
            push_front(new_value);
            return front();
        }

        /// @brief Removes the first element of the container.
        void pop_front(){
            Node<value_type> *n = first->_next;
            Node<value_type> *c = first;
            n->_prev = nullptr;
            allocate.deallocate(c);
            size_node--;
        }

        /// @brief Resizes the container to contain count elements.
        /// If the current size is greater than count, the container is reduced to its
        /// first count elements. If the current size is less than count, additional
        /// default-inserted elements are appended
        /// @param count new size of the container
        void resize(size_type count){
            while(size_node > count)
                pop_back();
            while(count > size_node){
                push_back(value_type());
            }
        }

        /// @brief Resizes the container to contain count elements.
        /// If the current size is greater than count, the container is reduced to its
        /// first count elements. If the current size is less than count, additional
        /// copies of value are appended.
        /// @param count new size of the container
        /// @param value the value to initialize the new elements with
        void resize(size_type count, const value_type& value){
            while(size_node > count)
                pop_back();
            while(count > size_node){
                push_back(value);
            }
        }

        /// @brief Exchanges the contents of the container with those of other.
        /// Does not invoke any move, copy, or swap operations on individual elements.
        /// All iterators and references remain valid. The past-the-end iterator is
        /// invalidated.
        /// @param other container to exchange the contents with
        void swap(Deque& other){
            Node<value_type>* a = first;
            Node<value_type>* b = last;
            first = other.first;
            other.first = a;
            last = other.last;
            other.last = b;
        }

        /// COMPARISIONS

        /// @brief Checks if the contents of lhs and rhs are equal
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator==(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs){
            if(lhs.size() != rhs.size()) return false;
            for(auto p = 0; p < lhs.size(); p++){
                if(lhs[p] != rhs[p]) return false;
            }
            return true;
        }

        /// @brief Checks if the contents of lhs and rhs are not equal
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator!=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs){
            return !(lhs == rhs);
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator>(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs){
            if (lhs.size() < rhs.size()) return false;
            if (lhs.size() > rhs.size()) return true;
            for(auto p = 0; p < lhs.size(); p++){
                if(lhs[p] > rhs[p]) return true;
                if(lhs[p] < rhs[p]) return false;
            }
            return false;
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator<(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs){
            return !(lhs > rhs) && (lhs != rhs);
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator>=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs){
            return ((lhs > rhs) || (lhs == rhs));
        }

        /// @brief Compares the contents of lhs and rhs lexicographically.
        /// @param lhs,rhs deques whose contents to compare
        template <class U, class Alloc>
        friend bool operator<=(const Deque<U, Alloc>& lhs,
                               const Deque<U, Alloc>& rhs){
            return ((lhs < rhs) || (lhs == rhs));
        }

        // operator <=> will be handy

        const_iterator get_it(value_type v){
            size_type it = 0;
            for(Node<value_type>* _curr = first; _curr != nullptr; _curr = _curr->_next){
                if(_curr->value == v){
                    const_iterator i;
                    i._cur = _curr;
                    return i;
                }
                it++;
            }
        }

    private:
        value_type size_node = 0;
        Node<value_type>* first = nullptr;
        Node<value_type>* last = nullptr;
        Allocator allocate;
    };

    /// NON-MEMBER FUNCTIONS (not implemented)

    /// @brief  Swaps the contents of lhs and rhs.
    /// @param lhs,rhs containers whose contents to swap
    template <class T, class Alloc>
    void swap(Deque<T, Alloc>& lhs, Deque<T, Alloc>& rhs){
        lhs.swap(rhs);
    }

    /// @brief Erases all elements that compare equal to value from the container.
    /// @param c container from which to erase
    /// @param value value to be removed
    /// @return The number of erased elements.
    template <class T, class Alloc, class U>
    typename Deque<T, Alloc>::size_type erase(Deque<T, Alloc>& c, const U& value){
        for(auto p = 0; p < c.size(); p++){
            if(c[p] == value){
                c.erase(c.get_it(value));
                return p;
            }
        }
    }

    /// @brief Erases all elements that compare equal to value from the container.
    /// @param c container from which to erase
    /// @param pred unary predicate which returns ​true if the element should be
    /// erased.
    /// @return The number of erased elements.
    template <class T, class Alloc, class Pred>
    typename Deque<T, Alloc>::size_type erase_if(Deque<T, Alloc>& c, Pred pred){
        for(auto p = 0; p < c.size(); p++){
            if(pred[p]){
                c.erase(pred[p]);
                return p;
            }
        }
    }

}  // lol