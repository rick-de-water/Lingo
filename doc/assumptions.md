# Assumptions

The following assumptions are made about all current and future character sets and encodings. They may lead to compatibility issues in the future when something breaks these assumptions, and may require refactoring.

## First unit defines point size
The library currently assumes that the size of a code point can be derived from the first code unit. Encodings which have to scan multiple units to determine the size of a code point are currently not supported.

## Single Unicode Codepoint
The library assumes that every code point in every character set can be represented by a single unicode code point. Code points that are not available in unicode (highly unlikely) or that require multiple code points in unicode (slightly more likely) are not supported.