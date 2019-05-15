﻿/*
@brief pre common header.
@author guobao.v@gmail.com
*/
#ifdef _MSC_VER
#pragma once
#endif

#ifndef _COMMON_PRECOMM_HPP_
#define _COMMON_PRECOMM_HPP_
#include <cstdlib>
#include <tchar.h>
#include <iostream>
#include <vector>

namespace common {

    ///----------资源初始化----------

    /**
    *@brief memset 0
    */
    template<typename T>
    inline void zeroset(T& p, size_t length)
    {
        std::memset(p, 0, sizeof(*p)*length);
    }
    template<unsigned N, typename T>
    inline void zeroset(T(&p)[N])
    {
        std::memset(p, 0, N);
    }

    /**
    *@brief wmemset 0
    */
    template<typename T>
    inline void wzeroset(T& p, size_t length)
    {
        std::wmemset(p, 0, sizeof(*p)*length);
    }
    template<unsigned N, typename T>
    inline void wzeroset(T(&p)[N])
    {
        std::wmemset(p, 0, N);
    }

    ///----------资源安全释放----------

    /**
    *@brief free_s 可接受不定长参数
    */
    template<typename T>
    inline void free_s(T& p)
    {
        if (p != nullptr) { std::free(static_cast<void*>(p)); p = nullptr; }
    }

    /**
    *@brief free_s 接受不定长参数
    */
    template<typename T, typename...Args>
    inline void free_s(T& p, Args&... args)
    {
        if (p != nullptr) { std::free(static_cast<void*>(p)); p = nullptr; }
        free_s(args...);
    }

    /**
    *@brief delete_s 可接受不定长参数
    */
    template<typename T>
    inline void delete_s(T& p)
    {
        if (p != nullptr) { delete(p); p = nullptr; }
    }

    /**
    *@brief delete_s 接受不定长参数
    */
    template<typename T, typename...Args>
    inline void delete_s(T& p, Args&... args)
    {
        if (p != nullptr) { delete(p); p = nullptr; }
        delete_s(args...);
    }

    /**
    *@brief delete[]_s 可接受不定长参数
    */
    template<typename T>
    inline void deleteA_s(T& p)
    {
        if (p != nullptr) { delete[](p); p = nullptr; }
    }

    /**
    *@brief delete[]_s 接受不定长参数
    */
    template<typename T, typename...Args>
    inline void deleteA_s(T& p, Args&... args)
    {
        if (p != nullptr) { delete[](p); p = nullptr; }
        deleteA_s(args...);
    }

    /**
    *@brief Release_s 可接受不定长参数
    */
    template<typename T>
    inline void Release_s(T& p)
    {
        if (p != nullptr) { p->Release(); p = nullptr; }
    }

    /**
    *@brief Release_s 接受不定长参数
    */
    template<typename T, typename...Args>
    inline void Release_s(T& p, Args&... args)
    {
        if (p != nullptr) { p->Release(); p = nullptr; }
        Release_s(args...);
    }

    /**
    *@brief release_s 可接受不定长参数
    */
    template<typename T>
    inline void release_s(T& p)
    {
        if (p != nullptr) { p->release(); p = nullptr; }
    }

    /**
    *@brief release_s 接受不定长参数
    */
    template<typename T, typename...Args>
    inline void release_s(T& p, Args&... args)
    {
        if (p != nullptr) { p->release(); p = nullptr; }
        release_s(args...);
    }

    ///----------字符串模板类型辅助----------

    template<typename _char_t, typename TA, typename TW>
    struct ttype;

    template<typename TA, typename TW>
    struct ttype<char, TA, TW>
    {
        typedef TA type;
    };

    template<typename TA, typename TW>
    struct ttype<wchar_t, TA, TW>
    {
        typedef TW type;
    };

    template<typename TA, typename TW>
    inline typename ttype<char, TA, TW>::type tvalue(char*, TA a, TW w)
    {
        return a;
    };

    template<typename TA, typename TW>
    inline typename ttype<wchar_t, TA, TW>::type tvalue(wchar_t*, TA a, TW w)
    {
        return w;
    }

    template<typename _char_t, typename TA, typename TW>
    inline typename ttype<_char_t, TA, TW>::type tvalue(TA a, TW w)
    {
        return tvalue<TA, TW>(static_cast<_char_t*>(0), a, w);
    }

    ///----------其他----------

    /**
    *@brief 目录检查,分隔符统一且末尾分隔符补全
    */
    template<typename T>
    auto fillDir(const T* dir, const T* separator = tvalue<T>("\\", L"\\"))
    {
        std::basic_string<T, std::char_traits<T>, std::allocator<T>> _dir = dir;
        std::vector<const T*> separators = { tvalue<T>("\\",L"\\"), tvalue<T>("/",L"/") };
        if (*separator == *separators[0]) {
            separators.erase(separators.begin());
        }
        size_t n = 0;
        while (true) {
            n = _dir.find_first_of(separators[0]);
            if (n == static_cast<size_t>(-1)) { break; }
            _dir.replace(n, 1, separator);
        }

        n = _dir.find_last_of(separator);
        if (n == static_cast<size_t>(-1) || n != _dir.size() - 1) { _dir += separator; }//无结尾分隔符
        return _dir;
    }

} // namespace common

#endif // _COMMON_PRECOMM_HPP_