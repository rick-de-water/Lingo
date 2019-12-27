# Lingo
Lingo is an encoding and code page aware string library for C++11 and up. It attempts to be a drop in replacement for the standard library strings by mirroring its interface as much as possible, while also adding a bunch of new functionally made possible by knowing what the data inside of the string actually represents.

# Some of the features
* Encoding and code page aware `lingo::string` and `lingo::string_view`, almost fully compatible with `std::string` and `std::string_view`.
* Null terminator aware `lingo::string_view`.
* Automatic conversion between `lingo::string`s of different encodings and/or character sets.
* `lingo::encoding::*` for low level encoding and decoding of code points.
* `lingo::page::*` for additional code point information and conversion between different code pages.
* `lingo::error::*` for complete control over error handling behaviour.

# How it works
The string class from the standard library is defined like this:
```c++
namespace std
{
    template<class CharT, class Traits, class Allocator>
    class basic_string;
}
```

All code point information is stored in a `Traits` type. This could work if it was implemented well, but it has several problems:
 * It assumes that every `CharT` is a code point, making it very difficult to work with multibyte/multiunit code points.
 * It generally has no information about the code page used. `char` could be ascii, utf8, iso 8859-7, or anything really. And while the standard is adding `char8_t`, `char16_t` and `char32_t` for unicode, we can't expect the standard to add a strong typedef for every code page out there.

To solve this problem, the library adds a new string type that is defined as such:
```c++
namespace lingo
{
    template <typename Encoding, typename Page, typename Allocator>
	class basic_string;
}
```

The library splits the responsibility of managing the code points of a string between an `Encoding` type and a `Page` type.
`Encoding` defines how a code point can be encoded to and decoded from one or more code units. `Page` on the other hand knows what every decoded code point actually means, and know how to convert it to other `Page`s.

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
    lingo::encoding::none<char32_t, char32_t>,
    lingo::page::unicode>;

using iso_8895_1_string = lingo::basic_string<
    lingo::encoding::none<unsigned char, unsigned char>,
    lingo::page::iso_8895_1>;
```

You may have noticed that there is no `lingo::encoding::utf32`. This is because of that split of responsibility. There is no difference between UTF-32 and decoded Unicode, so we can simply use `lingo::encoding::none`. It is only necessary to define the code page as `lingo::page::unicode`.


# Currently implemented

## Encodings
* `lingo::encoding::none`
* `lingo::encoding::utf8`
* `lingo::encoding::utf16`

## Code pages
* `lingo::page::ascii`
* `lingo::page::unicode`
* `lingo::page::iso_8859_n` with n = [1, 16] except 12

## Error handlers
TODO for 0.1.0

## Algorithms
Will be added in a future version.

