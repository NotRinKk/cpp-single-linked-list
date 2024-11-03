#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <iterator>
#include <utility>

using namespace std;
#include <iostream>

template <typename Type>
class SingleLinkedList {
        // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор»
    // определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node)
        : node_(node) {

        }
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept
        : node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;        


        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {    
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {    
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            this->node_ = this->node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            
            BasicIterator old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            
            return node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            
            return &node_->value;
        }
    
    private:
        Node* node_ = nullptr;
    };

public:

    template <typename Iterarator>
    void Initialize(Iterarator it_begin, Iterarator it_end) {
        if (it_begin == it_end) return;

        SingleLinkedList tmp;
        // Указатель на текущую ноду в новом списке
        Node* current = &tmp.head_;
        while (it_begin != it_end) {
            // Создаем новую ноду
            Node* new_node = new Node(*it_begin, nullptr); 
             // Если это первая нода
            if (!head_.next_node) {
                // Устанавливаем голову списка
                head_.next_node = new_node;
            } else {
                // Присоединяем новую ноду к текущей
                current->next_node = new_node;
            }
            // Обновляем указатель на текущую ноду
            current = new_node;
            // Передвигаем итератор к следующей ноде
            ++it_begin;
            this->size_ += 1u;
        }
        // Завершаем новый список
        current->next_node = nullptr;
    }

    SingleLinkedList(std::initializer_list<Type> values)
    : size_(0) {
        Initialize(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other)
    : size_(0) {
        
        Initialize(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        
        if (this != &rhs) {
            // Копируем другой список во временный
            SingleLinkedList temp(rhs);
            // Меняем содержимое
            swap(temp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        // Обмен указателями на узлы
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_); // Обмен размерами
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;  

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
         
         return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return begin();
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return end();
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        
        return ConstIterator(const_cast<Node*>(&head_));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        
        return ConstIterator(const_cast<Node*>(&head_));
    }

    SingleLinkedList() {
        head_.next_node = nullptr;
        size_ = 0u;
    }

    ~SingleLinkedList() {
        Clear();
    }

    // Возвращает количество элементов в списке
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список
    [[nodiscard]] bool IsEmpty() const noexcept {
       if (GetSize() == 0u) {
        return true;
       }
       return false;
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        // Если вставляем новый узел не на первое место в списке
        if (pos != this->before_begin()) {
            auto start_it = this->begin(); // Первый элемент списка итератор
            Node* current = this->head_.next_node;
            // Перемещяем итератор на pos
            while (start_it != pos) {
                current = current->next_node;
                ++start_it;
            }
            // current укаывает на узел pos
            // Получаем указатель на узел после pos
            Node* node_after_pos = current->next_node;
            // Создаём новый узел
            Node* new_node = new Node(value, nullptr);
            // С нового узла ссылаемся на current.next_node
            new_node->next_node = node_after_pos;
            // С узла pos ссылаемся на созданный узел
            current->next_node = new_node;
            ++size_;
            return Iterator(new_node);
        }
        else {
            // Создаём новый узел
            Node* new_node = new Node(value, nullptr);
            // Запоминаем указатель на узел на который указывает head_
            Node* node_after_head = head_.next_node;
            // Задаём новый узел как начало списка
            head_.next_node = new_node;
            // С нового первого элемента списка ссылаемся на предыдущий первый элемент
            new_node->next_node = node_after_head;
            ++size_;
            return Iterator(new_node);
        }

    }


    void PopFront() noexcept {
        
        Node* to_delete = head_.next_node;
        head_.next_node = to_delete->next_node;
        delete to_delete;
        --size_;
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        
        // Если удаляем узел не на первом место в списке
        if (pos != this->before_begin()) {
            // Первый элемент списка итератор
            auto start_it = this->begin();
            Node* current = this->head_.next_node;
            // Перемещяем итератор на pos
            while (start_it != pos) {
                current = current->next_node;
                ++start_it;
            }
            // current укаывает на узел pos
            // Получаем указатель на узел после pos
            Node* node_to_delete = current->next_node;
            // Запоминаем указатель на узел после pos
            Node* after_delete = node_to_delete->next_node;
            // Изменяем указатель следующего узла pos на узел после удалённого
            current->next_node = after_delete;
            delete node_to_delete;  
            --size_; 
            return Iterator(after_delete);
        }
        else {
            // Запоминаем какой узел удалить (первый)
            Node* node_to_delete = head_.next_node;
            // Меняем указатель головый на второй элемент списка
            head_.next_node = node_to_delete->next_node;
            // Указатель на узел после удалённого
            Node* after_delete =  head_.next_node;
            delete node_to_delete;
            --size_;
            return Iterator(after_delete);
        }
    }


    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        // Процесс очистки продолжается, пока список непустой,
        // указатель head_.next_node на первый элемент списка ненулевой. 
        while (head_.next_node != nullptr) {
            auto next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next;
        }
        size_ = 0u;
    }


private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0u;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    // Реализуйте обмен самостоятельно
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs==rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {  
    return !(lhs < rhs);
}