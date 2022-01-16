#pragma once

template<typename T>
struct scoped_ptr {
    using self_type = scoped_ptr<T>;

    explicit scoped_ptr(T *p = nullptr) noexcept : ptr_(p) {}
    ~scoped_ptr() { delete ptr_; }

    scoped_ptr(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;

    void reset(T *p = nullptr) noexcept { self_type(p).swap(*this); }
    void drop() noexcept { ptr_ = nullptr; }

    T &operator*() const noexcept { return *ptr_; }
    T *operator->() const noexcept { return ptr_; }
    T *get() const noexcept { return ptr_; }

    void swap(self_type &other) {
        T *tmp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = tmp;
    }
  private:
    T *ptr_;
};