git submodule update --init --recursive



# GCC Reference:

The `__attribute((unused))` is a gcc attribute to indicate to the compiler that the argument args is unused, and it prevents warnings about it.

### Preventing compiler optimazation for debugging
Use this `__attribute__((optimize("O0")))` to disable optimizations for a function.
ex.
```
void __attribute__((optimize("O0"))) myFunc()
{
    // this will not be optimized
}
```

# DEV TOOLS:
- [Bitwise Operations Playground](https://bitwisecmd.com/)