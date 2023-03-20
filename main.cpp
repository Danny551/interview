#include <stdint.h>
#include <string.h>
#include <iostream>
#include <array>
template <class T, uint32_t SIZE>
class Circular_Queue
{
public:
    Circular_Queue() {}
    ~Circular_Queue() {}

    /**
     * @brief push 从队列尾部存入数据，如果队列已满，存入失败，返回false
     * @param data
     * @return
     */
    bool push(T data)
    {
        if (full()) {
            // 循环队列已满
            return false;
        } else {
            p_tail_ = (p_tail_ + 1) % p_max_;
            buffer_[p_tail_] = data;
        }
        return true;
    }

    /**
     * @brief pop 从队首取出数据，如果队列空则返回false
     * @return
     */
    bool pop()
    {
        if (empty()) {
            // 循环队列已空
            return false;
        } else {
            p_head_ = (p_head_ + 1) % p_max_;
            buffer_[p_head_] = T();
        }
    }

    /**
     * @brief front 获取队首数据
     * @return
     */
    T front() { return buffer_[(p_head_ + 1) % p_max_]; }

    /**
     * @brief back 获取队尾数据
     * @return
     */
    T back() { return buffer_[p_tail_]; }

    bool full()
    {
        if ((p_tail_ + 1) % p_max_ == p_head_) {
            return true;
        } else {
            return false;
        }
    }

    bool empty()
    {
        if (p_tail_ == p_head_) {
            return true;
        } else {
            return false;
        }
    }

private:
    uint32_t p_max_ = SIZE;
    uint32_t p_head_ = 0; // 指向当前头部元素的上一个内存
    uint32_t p_tail_ = 0; // 指向当前尾部元素
    std::array<T, SIZE> buffer_ = { T() };
};

int main(int argc, char *argv[])
{
    Circular_Queue<int, 15> queue;
    for (int i = 1; i < 100; i++) {
        queue.push(i);
        if (queue.full()) {
            queue.pop();
        }
    }

    return 0;
}
