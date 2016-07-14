Husky Programming Language
==========================

**alpha**

[![Build Status](https://travis-ci.org/mishavetl/huskylang.svg?branch=master)](https://travis-ci.org/mishavetl/huskylang)

### Why husky?
 * fast like a husky
 * friendly like a husky

### Features

 * pure functional programming
 * nelisp minimal syntax
 * no statements

### Examples

```
# foo.hsk
fn even?, ([]
    ($number), ->
        (($ number) rem 2) == 0
    .
)

io:puts (even? 4)
```

```bash
$ husky foo.hsk
true
```
