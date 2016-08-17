Husky Programming Language
==========================

**alpha**

[![Build Status](https://travis-ci.org/mishavetl/huskylang.svg?branch=master)](https://travis-ci.org/mishavetl/huskylang)

### Why husky?

 * fast like a husky
 * friendly like a husky

### Features

 * purely functional programming
 * nelisp minimal syntax
 * no statements

### Examples

```
--| foo.hsk |--

var:tuple foo, ({} 1, 2, 3, 4)

io:puts (number:to_string ((($ foo) # 0) + (($ foo) # 1)))
```

```bash
$ husky foo.hsk
3
```
