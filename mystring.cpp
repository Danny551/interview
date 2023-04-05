#include <iostream>
#include <ostream>
#include "cstring"

class MyString
{
public:
    MyString(char *pData = nullptr)
    {
        if (pData == nullptr) {
            // 只声明，不赋值
            m_pData = new char[1];
            m_pData[0] = '\0';
        } else {
            // +1 是因为字符串最后一位时 '\0'
            m_pData = new char[strlen(pData) + 1];
            strcpy(m_pData, pData);
        }
    }

    MyString(const MyString &str)
    {
        auto len = strlen(str.m_pData);
        m_pData = new char[len + 1];
        strcpy(m_pData, str.m_pData);
    };

    ~MyString(void) { delete[] m_pData; }

    //    初级写法
    //    MyString &operator=(const MyString &str)
    //    {
    //        if (m_pData == str.m_pData) {
    //            return *this;
    //        }
    //        // 先delete是为了防止原来就有值，先把原来的内存释放掉
    //        delete[] m_pData;
    //        m_pData = nullptr;
    //        m_pData = new char[strlen(str.m_pData) + 1];
    //        strcpy(m_pData, str.m_pData);
    //        return *this;
    //    }
    MyString &operator=(const MyString &str)
    {
        if (this != &str) {
            // 在没有析构自己前先分配内存，防止内存安全异常
            MyString tmp(str);
            char *pTmp = m_pData;
            m_pData = tmp.m_pData;
            tmp.m_pData = pTmp;
        }

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const MyString &str);

private:
    char *m_pData;
};

// operator重载必须是全局函数，如果是类的成员函数，就变成data<<cout,与使用习惯相反
std::ostream &operator<<(std::ostream &os, const MyString &str)
{
    os << str.m_pData;
    return os;
}

int main(int argc, char *argv[])
{
    char *c = "string";
    MyString str(c);
    MyString str2 = str;
    std::cout << str << "  " << str2 << std::endl;
    return 0;
}
