# commom
common library only include header files.

It has only been tested on Windows.

---

## Tree

```cpp
│  code.snippet     //文件代码模板(vs->工具->代码片段管理中导入)
│  common.hpp       //主头文件(使用时include本文件)
│  common_all.hpp   //统一的主头文件(暂未生成)
├─common            //分头文件目录
│  │  cmdline.hpp      //命令行解析
│  │  codecvt.hpp      //字符编码转换
│  │  cuda.hpp         //cuda辅助(包含与opencv和directx的互操作)
│  │  debuglog.hpp     //windows调试日志
│  │  opencl.hpp       //opencl辅助
│  │  opencv.hpp       //opencv辅助
│  │  precomm.hpp      //公共辅助
│  │  windows.hpp      //windows辅助(包含directx辅助)
│  └─cuda           //cuda设备函数目录
│     │  texture_reference.cu    
│     └─ texture_reference.cuh
├─docs             //文档目录
├─samples          //使用样例目录
│  └─data             //测试数据 
└─tests            //单元测试目录
```

---

## Macro

默认关闭库/宏支持
- `HAVE_OPENCL`      //基于OpenCL 1.2
- `HAVE_OPENCV `     //基于OpenCV 4.0 with contrib
- `HAVE_DIRECTX`     //基于Microsoft DirectX SDK (June 2010)
- `HAVE_CUDA`        //基于CUDA 10.0
- `HAVE_CUDA_DEVICE` // 本项目cuda目录下的.cu文件添加到工程后可以开启本宏,宏详细说明见[common/cuda/README.md](common/cuda/README.md)

---

## Code style and code specification

**同一文件内应统一风格.**

### 命名

- 缩进应使用4空格而非制表符,语句后不应尾随空格.

- 简化匈牙利命名法:

   前缀|类型
   ---|---
   g_ | 全局变量
   m_ | 类成员变量
   s_ | 静态变量
   c_ | 常量
   p_ | 指针变量

- 所有前缀或后缀应该写在一起用一个下划线隔开:

   前后缀|类型
   ---|---
   mp_  | 成员指针变量
   mcp_ | 成员常量指针
   mpc_ | 成员指针常量
   _t   | 结构体类型
   _fn  | 函数指针类型
   _e   | 枚举类型

- 临时或局部变量可以加个下划线前缀.

- 文件/变量/命名空间等命名全小写+下划线.

- 命名空间/宏结束需要注释.

- 接口类以I开头或者Interface结尾.


### 文件
- 在`common.hpp`中进行分头文件的条件编译,而不是在头文件内部. 头文件在显式调用时不需要另外定义`HAVE_ `形式的宏来开启整个头文件的功能.

- 源文件内的头文件包含顺序应从最特殊到一般,如：
```cpp
#include "通用头文件"
#include "源文件同名头文件"
#include "本模块其他头文件"
#include "自定义工具头文件"
#include "第三方头文件"
#include "平台相关头文件"
#include "C++库头文件"
#include "C库头文件"
```
- 头文件中只用了指针/引用则使用前向声明而非引入头文件.
- 模块应使用命名空间`namespace{}`包含.
- 上下少空行,每屏代码越多越好.

- 如果需要设置编译警告,应将范围限制在本文件内
```cpp
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif
```

- 编译报错提示
```cpp
#if defined(_DEBUG) || defined(DEBUG)
#error error 错误 123
#endif
```

#### HPP文件要注意的问题

所有HPP文件使用宏避免重复包含.
```cpp
#ifndef _COMMON_PRECOMM_HPP_
#define _COMMON_PRECOMM_HPP_
#endif
```

HPP文件中可以使用using引用依赖,不应该使用`using namespace`污染命名空间.

函数的重定义问题:
- 将全局函数封装为类的静态方法.
- 通过冗余的模板参数变成模板函数.
```cpp
template<bool flag=false>
```
- static修饰.

### Doxygen文档

#### 注释

```
/**
 *  多行注释
 */
 
/**单行注释*/ 或 ///

/**<同行注释 */ 或 ///< (Doxygen认为注释是修饰接下来的程序代码的)
```

#### 文件信息

```
@file      文件名
@author    作者名
@version   版本号
@todo      待办事项
@date      日期时间
@section   章节标题 e.g. [@section LICENSE 版权许可] [@section DESCRIPTION 描述]
```
#### 模块信息

```
@defgroup   定义模块                         模块名(英文) 显示名    @{ 类/函数/变量/宏/... @}
@ingroup    作为指定名的模块的子模块          模块名(英文) [显示名]
@addtogroup 作为指定名的模块的成员            模块名(英文) [显示名]
@name       按用途分,以便理解全局变量/宏的用途 显示名(中文)           @{ 变量/宏 @}
```

#### 函数信息

```
@brief     摘要
@param     参数说明
@param[in]      输入参数
@param[out]     输出参数
@param[in,out]  输入输出参数
@return    返回值说明
@retval    特定返回值说明 [eg:@retval NULL 空字符串][@retval !NULL 非空字符串]
@exception 可能产生的异常描述
@enum      引用了某个枚举,Doxygen会在引用处生成链接
@var       引用了某个变量
@class     引用某个类 [eg: @class CTest "inc/class.h"]
@see       参考链接,函数重载的情况下,要带上参数列表以及返回值
@todo      todo注解
@pre       前置条件说明
@par       [段落标题] 开创新段落,一般与示例代码联用 
@code      示例代码开始 e.g. [code{.cpp}]
@ endcode  示例代码结束
```

#### 提醒信息

```
@note      注解
@attention 注意
@warning   警告
@bug       问题
@def       宏定义说明
```
 

#### 生成

文档目录下执行`doxygen Doxyfile`

### 类/函数

- 应尽可能多的使用模板函数.

- 构造函数只进行没有实际意义的初始化,通过`Init()`,`Setup()`等函数进行具体构造.

- `Switch`中若default永不执行可以使用`assert(false)`.

- 不使用class类型全局对象,除非单例模式或者使用`unique_ptr`.

- `explicit`修饰单参数构造函数,防止隐式类型转换误用

- 尽可能使用`const`

- 函数缺省参数,声明处缺省,定义处应该在形参列表使用注释标明缺省值`/**/`

- 使用自动字符数组`autobuffer` 
   ```cpp
   std::vector<char> buffer(10);
   std::unique_ptr<char[]> buffer(new char[10]);
   ```

## Summary of experience

- 编译期可以确定的常量应该使用`constexpr`修饰.
- `assert` 断言
   
   - 调试阶段函数开始前进行参数检查.发布版可关闭断言.
      ```cpp
      #define NDEBUG /  /关闭断言
      #include <assert.h> // #include <cassert>
      assert(p!=0) //若错误则终止程序执行
      ```

- `volatile` 易变修饰 
   - 声明变量可能被系统,硬件或其他线程更改,而编译器不应该优化.**(不可优化的)**
   - 每次取值必须从内存中取 (防止编译器优化从寄存器中取值).**(易变的)**
   - `C/C++ Volatile`变量间的操作,编译器不会进行乱序优化.**(顺序的)**
   - `const` 可以是`volatile`(如只读状态的寄存器).
   - 指针可以是`volatile`.

- `inline` 内联修饰
   - 相当于宏,比宏多了类型检查,运行时可调试(宏定义不能).
   - 一般代码行数不多的函数(5行以内)可以建议编译器内联.
   - 类内自动隐式内联.类外可以显式内联.
   - 如果关闭优化,编译器可能会无视内联修饰符.

- `explicit` 显式调用
   - 修饰构造函数 防止隐式转换和复制初始化.
   - 修饰转换函数 防止隐式转换(按语境转换除外).

- 后置返回类型(`trailing return type`)
   - 将返回类型移到参数声明后面. `->double`被称为后置返回类型.
   `auto`相当于占位符.后置返回类型有利于进行模板返回类型推导.
      ```cpp
      double h(int x, float y); 
      // ==
      auto h(int x, float y) -> double;
      ```

   - `decltype`关键字用于检查实体的声明类型或表达式的类型及值分类.
      ```cpp
      template <typename T>
      auto fun(T beg)->decltype(*beg){
          return *beg;//返回引用
      }
   
      template <typename T>
      auto fun2(T beg)->typename std::remove_reference<decltype(*beg)>::type//移除引用,为了使用模板 参  数成员必须使用typename
      {
       return *beg;//返回拷贝
      }
      ```

- `sizeof()`
   - 对数组可得数组大小.
   - 对指针可得指针本身所占空间大小.
   - 对有静态变量的结构体.静态变量的存放位置与结构体实例的存储地址无关,是单独存放在静态数据区的,因此用`siezof`计算其大小时没有将静态成员所占的空间计算进来.
   - 对于可变长参数包可得参数的数目,`sizeof…(Args)`.
   - 基本数据类型所占内存大小

      类型|32bit|64bit
      ---|---|---
      char , bool|1|1
      short|2|2
      int , unsigned int , float|4|4
      long , void*|4|8
      double , long long|8|8

- `using` 声明
   - 引用命名空间
      ```cpp
      using namespace name::name
      ```
   - 引用命名空间的一个成员
      ```cpp
      using std::cin;
      using std::cout;
      using std::endl;
      ```
   - 构造函数的using声明
      ```cpp
      //c++11中派生类能重用其直接基类定义的构造函数
      class B:A{
      public:
          using A::A;
      };
      //如上using声明
      //对于基类的每个构造函数,编译器都生成一个与之对应(形参列表完全相同)的派生类构造函数
      B(param):A(args){}
      ```

- `extern "C"{}`
   - 按照C语言方式编译链接,避免因C++符号修饰导致代码不能和C语言库中的符号进行链接.如果都是cpp项目就不必要了.
      ```cpp
      #ifdef __cplusplus
      extern"C"{
      #endif
   
      #ifdef __cplusplus
      }
      #endif
      ```

- `std::initializer_list` 列表初始化
   ```cpp
   std::vector<T> V;
   S(std::initializer_list<T> v):V(v){}

   S({1,2,3});
   ```

- `std::move`
   - 将对象的状态或者所有权从一个对象转移到另一个对象,只是转移,没有内存的搬迁或者内存拷贝.
   - `string`类在赋值或者拷贝构造函数中会声明`char`数组来存放数据,然后原`string`中的`char`数组会被析构函数释放,如果`x`是一个临时变量,则上面的拷贝,析构就是多余的,完全可以把临时变量`x`中的数据直接"转移"到新的变量下面.
      ```cpp
      std::string str = "Hello";
      std::vector<std::string> v;
      //调用常规的拷贝构造函数,新建字符数组,拷贝数据
      v.push_back(str);
      //调用移动构造函数,移动str,移动后不能使用str,因为转移后的值是未定义的.
      v.push_back(std::move(str));
      ```
   - 对外接口谨慎使用,需确保入参不会被再次调用.内部自己可控的可以使用`move`,因为`move`会移交控制权,外部可能会不知情.
   - 编译器存在返回值优化,大部分时候直接`return`就行.除非`return`表达式是左值并且没有资格进行复制省略.
      ```cpp
      T bar(bool k)
      {
          T a, b;
          return k ? a : b;            // 左值表达式,复制
          return std::move(k ? a : b); // 移动
          if (k)
              return a;                // 移动,可能复制省略
          else
              return b;                // 移动,可能复制省略
      }
      ```
      当`x`是局部变量时,返回`x`更好,因为编译器会将`x`视为返回值中的右值,
      因为它知道`x`是本地变量.当`x`是引用时,编译器不会给它特殊处理.
    - `std::unique_ptr`不能进行普通的拷贝和赋值操作但是可以使用`std::move`.
       ```cpp
       unique_ptr<int> pInt(new int(5));
       unique_ptr<int> pInt2 = std::move(pInt);//转移所有权
       ```

- `std::forward`
   - 完美转发实现了参数在传递过程中保持其值属性的功能,即若是左值,则传递之后仍然是左值,若是右值,则传递之后仍然是右值.
   - `std::move`和`std::forward`本质都是转换.`std::move`执行到右值的无条件转换.`std::forward`只有在它的参数绑定到一个右值上的时候,才转换它的参数到一个右值.
   - `std::move`没有`move`任何东西,`std::forward`没有转发任何东西.在运行期,它们没有做任何事情.它们没有产生1byte需要执行的代码.
   - `std::forward<T>()`不仅可以保持左值或者右值不变,同时还可以保持`const`、`Lreference`、`Rreference`、`validate`等属性不变.

- `deprecated`  弃用
   - 声明时未被弃用的名字可被重声明为`deprecated`,而声明为`deprecated`的名字不能通过重声明变为未弃用.
   - 下列名字或实体的声明中允许使用这个属性:
      ```cpp
      class/struct/union：struct [[deprecated("Replaced by bar, which has an improved interface")]] S;

      typedef名,也包括别名声明：[[deprecated]] typedef S* PS;using PS [[deprecated]] = S*;

      变量,包括静态数据成员：[[deprecated]] int x;

      非静态数据成员：union U { [[deprecated]] int n; };

      函数：[[deprecated]] void f();

      命名空间：namespace [[deprecated]] NS { int x; }

      枚举：enum [[deprecated]] E {};

      枚举项：enum { A [[deprecated]], B [[deprecated]] = 42 };

      模板特化：template<> struct [[deprecated]] X<int> {};
      ```

- `noexcept`
   - 编译器编译期间不会检查`noexcept`.
   - 声明了`noexcept`却抛出异常时会直接调用`terminal`终止程序(外部将无法捕获异常).
   - 就算没修饰`noexcept`,编译器也会自动推断有些函数不会抛出异常,除非编译器不能确定的情况,
	 例如重载移动赋值构造函数时(需要声明不会抛出异常).
   - 编译器会对`noexcept`修饰的函数做优化.
   - 若函数内部处理了异常,可以声明`noexcept`.
   - 会抛出异常的函数应该声明为`noexcept(false)`.
   - `noexcept(noexcept(!x.empty()))`可以进行条件判断(e.g.对形参进行断言),如果满足条件则承诺不会抛出异常.
   - 编译器会自动为析构函数隐式加上`noexcept`.
   - `noexcept`支持推导,而容器元素在支持右值引用的情况下这类推导是很重要的.右值引用可以减少深拷贝的需求,但是在某些情况下会破坏强异常安全保证.利用`noexcept`推导来决定一个复杂类型作为容器元素的时候到底适合用移动还是适合用普通的拷贝策略来保证强异常安全.比如一个类型如果不支持无异常的移动(自身或任一基类或者数据成员的移动构造函数声明了`noexcept(false)`),则推导这个类型不适合移动而只能使用拷贝实现异常安全.

- 2次幂枚举量
   ```cpp
   enum Days
   {
       None = 0,
       Sunday = 1,
       Monday = 2,
       Tuesday = 4,
       Wednesday = 8,
       Thursday = 16,
       Friday = 32,
       Saturday = 64
   };

   Days flag = Days::Monday;   
   //添加条件: |
   flag = Days(flag | Days::Wednesday);   
   //删除条件: &~
   flag = Days(flag & ~ Wednesday);   
   // "flag" 为 "Monday"
   if ((flag & Monday) == Monday) {cout << "Monday" << endl;}   
   // "flag"  为"Monday 与 Wednesday"
   if ((flag & (Monday | Wednesday)) == (Monday | Wednesday)){cout << "Monday & Wednesday" << endl;}   
   // "flag" 为 "Monday 或者 Wednesday"
   if ((flag & (Monday | Wednesday)) != 0) {cout << "Monday | Wednesday" << endl;}
   ```

- 不定长参数模板
   ```cpp
   //用于结束递归的同名模板函数
   template<typename T>
   inline void delete_s(T& p)
   {
       if (p != nullptr) { delete(p); p = nullptr; }
   }
   
   //用于递归不定长参数的同名模板函数
   template<typename T, typename...Args>
   inline void delete_s(T& p, Args&... args)
   {
       if (p != nullptr) { delete(p); p = nullptr; }
       delete_s(args...);
   }
   ```

- 函数缓存

   为了优化程序性能我们经常使用缓存,比如某个函数非常耗时,频繁调用的时候性能会很低,这时我们可以通过缓存来提高性能.
   ```cpp
    namespace detail {
        //函数入参及结果缓存,缓存入参和函数的执行结果,若入参存在则从缓存返回结果
        template <typename R, typename... Args>
        std::function<R(Args...)> cache_fn(R(*func)(Args...))
        {
            auto result_map = std::make_shared<std::map<std::tuple<Args...>, R>>();
            return ([=](Args... args) {//延迟执行
                std::tuple<Args...> _args(args...);
                if (result_map->find(_args) == result_map->end()) {
                    (*result_map)[_args] = func(args...);//未找到相同入参,执行函数刷新缓存
                }
                return (*result_map)[_args];//返回缓存
            });
        }
    }

    //函数对象缓存,若存在相同类型函数指针,则调用相应缓存函数获取缓存结果
    template <typename R, typename...  Args>
    std::function<R(Args...)> cache_fn(R(*func)(Args...), bool flush = false)
    {
        using function_type = std::function<R(Args...)>;
        static std::unordered_map<decltype(func), function_type> functor_map;
        if (flush) {//明确要求刷新缓存
            return functor_map[func] = detail::cache_fn(func);
        }
        if (functor_map.find(func) == functor_map.end()) {
            functor_map[func] = detail::cache_fn(func);//未找到相同函数,执行函数刷新缓存
        }
        return functor_map[func];//返回缓存
    }
   ```
   函数缓存可以大幅提高递归类函数的性能
   ```cpp
   size_t fibonacci_1(size_t n) {
       return (n < 2) ? n : fibonacci_1(n - 1) + fibonacci_1(n - 2);
   }
   size_t fibonacci_2(size_t n) {
       return (n < 2) ? n : cache_fn(fibonacci_2)(n - 1) + cache_fn(fibonacci_2)(n - 2);
   }

   auto t1 = getFnDuration(fibonacci_1)(35);//47ms (为45时,为5000ms)
   auto t2 = getFnDuration(fibonacci_2)(35);//0ms  (为1000时,为2ms)
   ```

- 自动推断数组大小
   ```cpp
   //(unsigned需要放在前面)
   template<unsigned n,typename T>
   void arr(const T (&m)[n]) {
       std::cout << m;
   }
   ```

- 最简洁也是效率最高的单例模式的C++11实现
   ```cpp
   widget& widget::get_instance() {
       static widget instance;
       return instance;
   }
   ```

- 浮点数精度比较
   ```cpp
   #include<float.h>
   
   < DBL_EPSILON
   > FLT_EPSILON
   ```

- 关于`directx`
   - `CopyResource`之后需要刷新否则容易导致黑屏(未成功获取数据)
      ```cpp
      ctx->CopyResource();
      ctx->Flush();
      ```
   - 不同`device`之间的`texture`通过`SharedHandle`进行共享/拷贝.

- 关于`opencv`
   - `cv::UMat` 有些时候需要手动`release`.

- 关于`cuda`
   - `__syncthreads`等提示未定义标识符,并不会影响编译.原因可能是先创建的cpp工程而后添加的cuda生成自定义文件,而非直接创建cuda工程.可以通过在依赖的头文件前`#define __CUDACC__`(.cu源文件编译时会定义这个宏).


