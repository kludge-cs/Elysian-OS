# Code modification and contribution guide

## Contribution
Follow the style guide, make sure it builds, and submit a pull request.

## Manners
Don't directly insult people, attack their work instead.

## Style
C89 is used, with the exception of the type `long long`.
Next-line bracing is used everywhere, if there is an excessively long line with parenthesis, it is split as follows:
```
if
	(
		(condition1 || condition2) &&
		condition3 &&
		condition4
	)
{
	/* Code */
}
```

`#pragma once` is used instead of include guards, it is supported by all major compilers, including TCC, GCC, and Clang.

#### What's with the infix typedefs?
`infix` acts more like a modifier such as `static` than an operator.  It may be placed infix or prefixed, and I prefer infixed since I don't get the types confused (`typedef mytype type` vs `typedef type mytype`, I consider the first to make more sense but it is wrong)