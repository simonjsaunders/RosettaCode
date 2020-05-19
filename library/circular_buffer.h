#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>

// See https://en.wikipedia.org/wiki/Circular_buffer

template<typename T>
class circular_buffer {
public:
    explicit circular_buffer(size_t n) : start_(0), end_(0), buffer_(n + 1) {}
    void push_back(T element) {
        buffer_[end_] = element;
        end_ = next(end_);
        if (end_ == start_)
            start_ = next(start_);
    }
    class const_iterator;
    class iterator {
    public:
        iterator(circular_buffer* buf, size_t pos) : buf_(buf), pos_(pos) {}
        T& operator*() const  {
            return buf_->buffer_.at(pos_);
        }
        iterator& operator++()  {
            pos_ = buf_->next(pos_);
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            pos_ = buf_->next(pos_);
            return tmp;
        }
        bool operator==(const iterator& i) const {
            return buf_ == i.buf_ && pos_ == i.pos_;
        }
        bool operator!=(const iterator& i) const {
            return !(*this == i);
        }
    private:
        friend class const_iterator;
        circular_buffer* buf_;
        size_t pos_;
    };
    class const_iterator
    {
    public:
        const_iterator(const iterator& i) : buf_(i.buf_), pos_(i.pos_) {}
        const_iterator(const circular_buffer* buf, size_t pos) : buf_(buf), pos_(pos) {}
        const T& operator*() const {
            return buf_->buffer_.at(pos_);
        }
        const_iterator& operator++() {
            pos_ = buf_->next(pos_);
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            pos_ = buf_->next(pos_);
            return tmp;
        }
        bool operator==(const const_iterator& i) const {
            return buf_ == i.buf_ && pos_ == i.pos_;
        }
        bool operator!=(const const_iterator& i) const {
            return !(*this == i);
        }
    private:
        const circular_buffer* buf_;
        size_t pos_;
    };
    iterator begin() {
        return iterator(this, start_);
    }
    iterator end() {
        return iterator(this, end_);
    }
    const_iterator begin() const {
        return const_iterator(this, start_);
    }
    const_iterator end() const {
        return const_iterator(this, end_);
    }
    const T& at(size_t index) const {
        return buffer_.at(index_of(index));
    }
    T& at(size_t index) {
        return buffer_.at(index_of(index));
    }
    size_t size() const {
        return (buffer_.size() + end_ - start_) % buffer_.size();
    }
private:
    size_t next(size_t i) const {
        return (i + 1) % buffer_.size();
    }
    size_t index_of(size_t i) const {
        return (i + start_) % buffer_.size();
    }
    size_t start_;
    size_t end_;
    std::vector<T> buffer_;
};

#endif
