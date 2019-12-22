# Glossary

A lot of the terms used in this document (and through the rest of this project) are commonly used in other contexts, but might have slightly different meanings. This document does not describe what the meaning of these terms is or should in those contexts. It only describes what these terms mean in the context of this project.

## Code page
A code page assigns meaning to each code point. For example, the code page may decide that the number `65` is the letter **A**. It may also decide that `0x0300` is the combining diacritic mark ◌̀  which must be combined with a previous code point. While the first 128 numbers are commonly based on ASCII, and have the same mapping, values over 128 can vary wildly between code pages.

The name is also commonly shortened to `page`.

Here are a few implementation examples:

* `lingo::page::ascii`, based on the [ASCII](https://en.wikipedia.org/wiki/ASCII) standard.
* `lingo::page::iso_8859_1`, based on the [ISO/IEC 8859-1](https://en.wikipedia.org/wiki/ISO/IEC_8859-1) standard.
* `lingo::page::unicode_12_1_0`, based on version 12.1.0 of the [Unicode](https://en.wikipedia.org/wiki/Unicode) standard.

## Encoding
An encoding decribes the rules of encoding and decoding code points into code units. For example, an encoding may decide to only use 1 byte for code points below 128, but 2 or more bytes for code points larger than that.

Here are a few implementation examples:

* `lingo::encoding::none`, no encoding, everything code point is stored directly.
* `lingo::encoding::utf8`, based on the [UTF-8](https://en.wikipedia.org/wiki/UTF-8) standard.
* `lingo::encoding::utf16`, based on the [UTF-16](https://en.wikipedia.org/wiki/UTF-16) standard.


## Code Unit
A code unit is the smallest unit of storage that can store part of or sometimes the entire code point, depending on the value of the code point and the encoding. These are usually represented by types such as `char`, `char16_t` and `char32_t`.

The name is also commonly shortened to `unit`.

## Code Point
A code point is a single number that is stored in a set of one or more code units, based on the encoding, and has its meaning defined by the character set.

A code point is **NOT** a character! It can be, but it can also be a modifier for the surrounding code points, such as accents (e + " = ë).

The name is also commonly shortened to `point`.

## TODO
* Character
* Grapheme
* Grapheme Cluster
