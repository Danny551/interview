#include <iostream>
#include <memory> // C++11 shared_ptr头文件
#include <mutex>  // C++11 mutex头文件

/** 要点:
 1、构造函数私有化，防止从类外调用构造函数，保证在任何情况下只生成一个实例
 2、提供一个全局的静态方法getInstance（全局访问点）,便于提供从类外部获取单例类的唯一实例的方法
 3、在类中定义一个静态指针，指向本类的变量的静态变量指针
 4、禁止拷贝构造
 单例模式有许多种实现方法:
  a. 懒汉式：使用的时候才创建，多线程访问的时候线程不安全(双检锁)
  b. 饿汉式：类文件加载的时候创建对象，如果对象一直没有使用，则类对象浪费空间

  特点与选择：
  如果要进行线程同步，访问量比较大，或者可能访问的线程比较多时，采用饿汉实现，可以实现更好的性能。这是以空间换时间。
  在访问量较小时，采用懒汉实现。这是以时间换空间。

 */

/**
  存在问题：
  1、懒汉式只能在单线程下工作，如果两个线程同时判断instance ==
  nullptr，并且instance在此时确实没有创建，那么两个线程都会创造一个实例，
  避免上述问题的方法是在判断时加上同步锁即可。
  2、需要手动调用delete回收实例内存，如不手动调用delete，可改用智能指针
  */
// 懒汉式
class Singleton1
{
private:
    static Singleton1 *instance; //定义指针
    Singleton1() { std::cout << "Singleton1的构造" << std::endl; }
    // 销毁实例
    static void destoryInstance()
    {
        if (instance != NULL) {
            delete instance;
            instance = NULL;
        }
    }
    // 禁用拷贝构造和移动构造
    Singleton1(Singleton1 &) = delete;
    Singleton1 &operator=(const Singleton1 &) = delete;

public:
    //定义一个公有接口，返回本类对象的指针
    static Singleton1 *getInstance()
    {
        if (instance == nullptr) {
            instance = new Singleton1();
        }
        return instance;
    }
};

// 加锁的懒汉式
class Singleton2
{
private:
    static std::shared_ptr<Singleton2> instance; //定义指针
    static std::mutex mtx;

    Singleton2() { std::cout << "Singleton2的构造" << std::endl; }

    // 禁用拷贝构造和移动构造
    Singleton2(Singleton2 &) = delete;
    Singleton2 &operator=(const Singleton2 &) = delete;

public:
    // 定义一个公有接口，返回本类对象的指针
    static std::shared_ptr<Singleton2> getInstance()
    {
        // 双重检测锁，第一次判断是为了判断是否创建实例，我们只是在实例没有创建之前需要加锁操作，实例创建之后就不需要加锁了
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lck(mtx); // 离开作用域后自动解锁
            // 第二次判断是为了判断是否重复创建
            if (instance == nullptr) {
                instance = std::shared_ptr<Singleton2>(new Singleton2());
            }
        }
        return instance;
    }
};

// 饿汉式
class Singleton3
{
private:
    static Singleton3 *instance; //定义指针

    Singleton3() { std::cout << "Singleton3的构造" << std::endl; }

    // 禁用拷贝构造和移动构造
    Singleton3(Singleton3 &) = delete;
    Singleton3 &operator=(const Singleton3 &) = delete;

public:
    // 定义一个公有接口，返回本类对象的指针
    static Singleton3 *getInstance() { return instance; }
};

Singleton1 *Singleton1::instance = nullptr;
std::shared_ptr<Singleton2> Singleton2::instance = nullptr;
std::mutex Singleton2::mtx;
Singleton3 *Singleton3::instance = new Singleton3();

int main()
{
    {
        auto instance1 = Singleton1::getInstance();
        auto instance2 = Singleton1::getInstance();
        // instance1 与instance2 相同，构造函数只调用一次
        std::cout << "instance1: " << instance1 << std::endl;
        std::cout << "instance2: " << instance2 << std::endl;
    }

    {
        auto instance1 = Singleton2::getInstance();
        auto instance2 = Singleton2::getInstance();
        // instance1 与instance2 相同，构造函数只调用一次
        std::cout << "instance1: " << instance1 << std::endl;
        std::cout << "instance2: " << instance2 << std::endl;
    }

    {
        auto instance1 = Singleton3::getInstance();
        auto instance2 = Singleton3::getInstance();
        // instance1 与instance2 相同，构造函数只调用一次
        std::cout << "instance1: " << instance1 << std::endl;
        std::cout << "instance2: " << instance2 << std::endl;
    }
    return 0;
}
