# Lingo

||Github Actions|Codecov|Coverals|
|:-:|:-:|:-:|:-:|
| Master | [![ga-build][ga-master]][ga-master-link] | [![ccov-coverage][ccov-master]][ccov-master-link] | [![cvrl-coverage][cvrl-master]][cvrl-master-link] |
| Latest | [![ga-build][ga-latest]][ga-latest-link] | [![ccov-coverage][ccov-latest]][ccov-latest-link] | [![cvrl-coverage][cvrl-latest]][cvrl-latest-link]

[ga-master]: https://img.shields.io/github/workflow/status/rick-de-water/Lingo/lingo/master
[ga-master-link]: https://github.com/rick-de-water/Lingo
[ga-latest]: https://img.shields.io/github/workflow/status/rick-de-water/Lingo/lingo
[ga-latest-link]: https://github.com/rick-de-water/Lingo

[ccov-master]: https://img.shields.io/codecov/c/gh/rick-de-water/Lingo/master
[ccov-master-link]: https://codecov.io/gh/rick-de-water/Lingo/branch/master
[ccov-latest]: https://img.shields.io/codecov/c/gh/rick-de-water/Lingo
[ccov-latest-link]: https://codecov.io/gh/rick-de-water/Lingo

[cvrl-master]: https://img.shields.io/coveralls/github/rick-de-water/Lingo
[cvrl-master-link]: https://coveralls.io/github/rick-de-water/Lingo
[cvrl-latest]: https://img.shields.io/coveralls/github/rick-de-water/Lingo/master
[cvrl-latest-link]: https://coveralls.io/github/rick-de-water/Lingo

[![latest-release](https://img.shields.io/github/v/release/rick-de-water/Lingo)](https://github.com/rick-de-water/Lingo/releases)

Lingo is an encoding aware string library for C++11 and up. It tries to be a drop in replacement for the standard library strings by mirroring its interface as much as possible, while also adding new functionally by utilizing the encoding definitions.

# Features
* Encoding and code page aware `lingo::string` and `lingo::string_view`, almost fully compatible with `std::string` and `std::string_view`.
* Null terminator aware `lingo::string_view`.
* `lingo::make_null_terminated` helper function for APIs that only support C strings, which ensures that a string is null terminated with minimal copying.
* Explicit conversion constructors between `lingo::string`s of different encodings and code pages.
* Support for endianness conversion.
* `lingo::encoding::*` for low level encoding and decoding of code points.
* `lingo::page::*` for additional code point information and conversion between different code pages.
* `lingo::error::*` for different error handling behaviours.
* `lingo::encoding::point_iterator` and `lingo::page::point_mapper` helpers to manually iterate or convert points individually.
* `lingo::string_converter` to manually convert entire strings.

# How it works
The string class from the standard library is defined like this:
```c++
namespace std
{
    template <class CharT, class Traits, class Allocator>
    class basic_string;
}
```

All code unit information is stored in a `Traits` type. This works well for ascii strings, but starts to run into issues when dealing with encodings that go beyond ascii.
 * It assumes that every `CharT` is a code point, making it very difficult to work with multibyte/multiunit code points.
 * It generally has no information about the code page used. `char` could be ascii, utf8, iso 8859-7, or anything really. And while the standard is adding `char8_t`, `char16_t` and `char32_t` for unicode, it really only knows that it is a form of UTF, but has no idea how actually encode, decode or transform the data.

To solve this problem, Lingo adds a new string type:
```c++
namespace lingo
{
    template <typename Encoding, typename Page, typename Allocator>
    class basic_string;
}
```

Lingo splits the responsibility of managing the code points of a string between an `Encoding` type and a `Page` type.
The `Encoding` type defines how a code point can be encoded to and decoded from one or more code units. The `Page` type defines what every decoded code point actually means, and knows how to convert it to other `Page`s.

Here are some examples of what that actually looks like:
```c++
using ascii_string = lingo::basic_string<
    lingo::encoding::none<char, char>,
    lingo::page::ascii>;

using utf8_string = lingo::basic_string<
    lingo::encoding::utf8<char8_t, char32_t>,
    lingo::page::unicode>;

using utf16_string = lingo::basic_string<
    lingo::encoding::utf16<char16_t, char32_t>,
    lingo::page::unicode>;

using utf32_string = lingo::basic_string<
    lingo::encoding::utf32<char32_t, char32_t>,
    lingo::page::unicode>;

using iso_8895_1_string = lingo::basic_string<
    lingo::encoding::none<unsigned char, unsigned char>,
    lingo::page::iso_8895_1>;
```

You might wonder why there is a `lingo::encoding::utf32` encoding, since there is no difference between UTF-32 and decoded Unicode.

It is indeed possible to use `lingo::encoding::none` instead, and still have a fully functional UTF-32 string. However, `lingo::encoding::utf32` does add some extra validation, such as detecting surrogate code units, making it better at dealing with invalid inputs.


# Currently implemented

## Encodings
* `lingo::encoding::none`
* `lingo::encoding::utf8`
* `lingo::encoding::utf16`
* `lingo::encoding::utf32`
* `lingo::encoding::base64`

### Meta encodings
* `lingo::encoding::swap_endian`: Swaps the endianness of the code units.
* `lingo::encoding::join`: Chains multiple encodings together (e.g. `join<swap_edian, utf16>` to create `utf16_be`).

## Code pages
* `lingo::page::ascii`
* `lingo::page::unicode`
* `lingo::page::iso_8859_n` with n = [1, 16] except 12.

## Error handlers
* `lingo::error::strict` Throws an exception on error.

## Algorithms
#### Will be added in a future version.

## How to build

Lingo is a header only library, but some of the header files do have to be generated first. You can check the latest releases for a package that has all headers generated for you.

If you want the library yourself, you will have to build the CMake project. All you need is CMake 3.12 or higher and a C++11 compatible compiler. The tests are written using [Catch](https://github.com/catchorg/Catch2) and can be run with `ctest`.

## How to include in your project
Since Lingo is a header only library, all you need to do is copy the header files and add it as an include directory. 

There is one thing that you do need to look out for, which is the execution character set. This library assumes by default that `char` is UTF-8, and that `wchar_t` is UTF-16 or UTF-32, depending on the size of `wchar_t`.

This matches the default settings of GCC and Clang, but not of Visual Studio. If your compiler's execution set does not match the defaults, you have two options:

### Configure your compiler
 * [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=vs-2019)
 * [GCC](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#index-fexec-charset)
 * [Clang](https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fexec-charset)

### Configure the library

The following macros can be defined to overwrite the default encodings for `char` and `wchar_t`:
 * `LINGO_CHAR_ENCODING`
 * `LINGO_WCHAR_ENCODING`
 * `LINGO_CHAR_PAGE`
 * `LINGO_WCHAR_PAGE`

So for example, if you want to use ISO/IEC 8859-1 for `char`s, you will have to define the follow macros:
* `-DLINGO_CHAR_ENCODING=none`
* `-DLINGO_CHAR_PAGE=iso_8859_1`

This method is not recommended.
Compiler flags are a much more reliable way to set the correct execution encoding.

## More documentation
 * [Glossary](doc/glossary.md)
 * [Interfaces](doc/interfaces.md)
 * [TODO](doc/TODO.md) (A very poorly written list of features to come)